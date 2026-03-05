#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <glm/glm.hpp>
#include "Engine/Graphics/Chunk.hpp"
#include "Engine/Physics/Raycaster.hpp"

// Hashing for ivec3 to use in unordered_map
struct IVec3Hash {
    std::size_t operator()(const glm::ivec3& k) const {
        return std::hash<int>()(k.x) ^ (std::hash<int>()(k.y) << 1) ^ (std::hash<int>()(k.z) << 2);
    }
};

namespace Game {
    class VoxelMap {
    public:
        VoxelMap();
        ~VoxelMap();

        void setBlock(int x, int y, int z, uint8_t type);
        uint8_t getBlock(int x, int y, int z) const;

        void render();
        void updateMeshes();

        bool saveToFile(const std::string& filepath);
        bool loadFromFile(const std::string& filepath);

        Graphics::Chunk* getChunk(int cx, int cy, int cz) const;
        Graphics::Chunk* getOrCreateChunk(int cx, int cy, int cz);

    private:
        std::unordered_map<glm::ivec3, std::unique_ptr<Graphics::Chunk>, IVec3Hash> m_chunks;
        
        // 1-element cache to speed up continuous chunk queries (e.g. from raycasts or 100 zombies walking)
        mutable glm::ivec3 m_lastChunkPos;
        mutable Graphics::Chunk* m_lastChunk;

        void worldToChunk(int wx, int wy, int wz, int& cx, int& cy, int& cz, int& lx, int& ly, int& lz) const;
    };
}
