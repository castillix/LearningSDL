#include "Engine/Graphics/VoxelMap.hpp"
#include <cmath>

namespace Engine {
namespace Graphics {

    VoxelMap::VoxelMap() : m_lastChunkPos(0x7FFFFFFF), m_lastChunk(nullptr) {}
    VoxelMap::~VoxelMap() {}

    void VoxelMap::worldToChunk(int wx, int wy, int wz, int& cx, int& cy, int& cz, int& lx, int& ly, int& lz) const {
        // Safe flooring for negative numbers
        cx = (int)std::floor((float)wx / CHUNK_SIZE);
        cy = (int)std::floor((float)wy / CHUNK_SIZE);
        cz = (int)std::floor((float)wz / CHUNK_SIZE);

        lx = wx - (cx * CHUNK_SIZE);
        ly = wy - (cy * CHUNK_SIZE);
        lz = wz - (cz * CHUNK_SIZE);
    }

    Chunk* VoxelMap::getChunk(int cx, int cy, int cz) const {
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

    Chunk* VoxelMap::getOrCreateChunk(int cx, int cy, int cz) {
        auto it = m_chunks.find(glm::ivec3(cx, cy, cz));
        if (it != m_chunks.end()) {
            return it->second.get();
        }
        m_chunks[glm::ivec3(cx, cy, cz)] = std::make_unique<Chunk>(cx, cy, cz);
        return m_chunks[glm::ivec3(cx, cy, cz)].get();
    }

    void VoxelMap::setBlock(int x, int y, int z, uint8_t type) {
        int cx, cy, cz, lx, ly, lz;
        worldToChunk(x, y, z, cx, cy, cz, lx, ly, lz);

        Chunk* chunk = getOrCreateChunk(cx, cy, cz);
        chunk->setBlock(lx, ly, lz, type);
    }

    uint8_t VoxelMap::getBlock(int x, int y, int z) const {
        int cx, cy, cz, lx, ly, lz;
        worldToChunk(x, y, z, cx, cy, cz, lx, ly, lz);

        Chunk* chunk = getChunk(cx, cy, cz);
        if (chunk) {
            return chunk->getBlock(lx, ly, lz);
        }
        return 0; // Air
    }

    void VoxelMap::render(Interfaces::IRenderer* renderer) {
        for (auto& pair : m_chunks) {
            pair.second->render();
        }
    }

}
}
