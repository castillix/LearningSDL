#include "Engine/Game/VoxelMap.hpp"
#include <fstream>
#include <SDL3/SDL.h>

namespace Game {
    VoxelMap::VoxelMap() : m_lastChunkPos(0x7FFFFFFF), m_lastChunk(nullptr) {}
    VoxelMap::~VoxelMap() {}

    void VoxelMap::worldToChunk(int wx, int wy, int wz, int& cx, int& cy, int& cz, int& lx, int& ly, int& lz) const {
        cx = (int)std::floor((float)wx / Graphics::CHUNK_SIZE);
        cy = (int)std::floor((float)wy / Graphics::CHUNK_SIZE);
        cz = (int)std::floor((float)wz / Graphics::CHUNK_SIZE);

        lx = wx - (cx * Graphics::CHUNK_SIZE);
        ly = wy - (cy * Graphics::CHUNK_SIZE);
        lz = wz - (cz * Graphics::CHUNK_SIZE);
    }

    Graphics::Chunk* VoxelMap::getChunk(int cx, int cy, int cz) const {
        if (m_lastChunk && m_lastChunkPos.x == cx && m_lastChunkPos.y == cy && m_lastChunkPos.z == cz) {
            return m_lastChunk;
        }
        
        auto it = m_chunks.find(glm::ivec3(cx, cy, cz));
        if (it != m_chunks.end()) {
            m_lastChunkPos = glm::ivec3(cx, cy, cz);
            m_lastChunk = it->second.get();
            return m_lastChunk;
        }
        return nullptr;
    }

    Graphics::Chunk* VoxelMap::getOrCreateChunk(int cx, int cy, int cz) {
        auto it = m_chunks.find(glm::ivec3(cx, cy, cz));
        if (it != m_chunks.end()) {
            return it->second.get();
        }
        m_chunks[glm::ivec3(cx, cy, cz)] = std::make_unique<Graphics::Chunk>(cx, cy, cz);
        return m_chunks[glm::ivec3(cx, cy, cz)].get();
    }

    void VoxelMap::setBlock(int x, int y, int z, uint8_t type) {
        int cx, cy, cz, lx, ly, lz;
        worldToChunk(x, y, z, cx, cy, cz, lx, ly, lz);

        Graphics::Chunk* chunk = getOrCreateChunk(cx, cy, cz);
        chunk->setBlock(lx, ly, lz, type);
    }

    uint8_t VoxelMap::getBlock(int x, int y, int z) const {
        int cx, cy, cz, lx, ly, lz;
        worldToChunk(x, y, z, cx, cy, cz, lx, ly, lz);

        Graphics::Chunk* chunk = getChunk(cx, cy, cz);
        if (chunk) {
            return chunk->getBlock(lx, ly, lz);
        }
        return 0; // Air
    }

    void VoxelMap::updateMeshes() {
        for (auto& pair : m_chunks) {
            pair.second->generateMesh(); // Only actualizes if dirty
        }
    }

    void VoxelMap::render() {
        updateMeshes();
        for (auto& pair : m_chunks) {
            pair.second->render();
        }
    }

    bool VoxelMap::saveToFile(const std::string& filepath) {
        std::ofstream file(filepath, std::ios::binary);
        if (!file.is_open()) return false;

        uint32_t chunkCount = m_chunks.size();
        file.write(reinterpret_cast<const char*>(&chunkCount), sizeof(chunkCount));

        for (const auto& pair : m_chunks) {
            glm::ivec3 pos = pair.first;
            file.write(reinterpret_cast<const char*>(&pos), sizeof(pos));

            // Write 32x32x32 array. Need to access it, so let's just loop over getBlock for simplicity 
            // even though it's slow, or add a raw data getter to Chunk. We'll do a simple iteration:
            for(int x=0; x<Graphics::CHUNK_SIZE; ++x) {
                for(int y=0; y<Graphics::CHUNK_SIZE; ++y) {
                    for(int z=0; z<Graphics::CHUNK_SIZE; ++z) {
                        uint8_t block = pair.second->getBlock(x,y,z);
                        file.write(reinterpret_cast<const char*>(&block), sizeof(block));
                    }
                }
            }
        }
        file.close();
        return true;
    }

    bool VoxelMap::loadFromFile(const std::string& filepath) {
        std::ifstream file(filepath, std::ios::binary);
        if (!file.is_open()) return false;

        m_chunks.clear();

        uint32_t chunkCount;
        file.read(reinterpret_cast<char*>(&chunkCount), sizeof(chunkCount));

        for (uint32_t i = 0; i < chunkCount; ++i) {
            glm::ivec3 pos;
            file.read(reinterpret_cast<char*>(&pos), sizeof(pos));

            Graphics::Chunk* chunk = getOrCreateChunk(pos.x, pos.y, pos.z);

            for(int x=0; x<Graphics::CHUNK_SIZE; ++x) {
                for(int y=0; y<Graphics::CHUNK_SIZE; ++y) {
                    for(int z=0; z<Graphics::CHUNK_SIZE; ++z) {
                        uint8_t block;
                        file.read(reinterpret_cast<char*>(&block), sizeof(block));
                        chunk->setBlock(x,y,z, block);
                    }
                }
            }
        }
        file.close();
        return true;
    }
}
