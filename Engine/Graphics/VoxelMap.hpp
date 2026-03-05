#pragma once
#include "Engine/Interfaces/IMap.hpp"
#include "Engine/Graphics/Chunk.hpp"
#include <unordered_map>
#include <memory>
#include <glm/glm.hpp>

// Hashing for ivec3 to use in unordered_map
struct IVec3Hash {
    std::size_t operator()(const glm::ivec3& k) const {
        return std::hash<int>()(k.x) ^ (std::hash<int>()(k.y) << 1) ^ (std::hash<int>()(k.z) << 2);
    }
};

namespace Engine {
namespace Graphics {

    class VoxelMap : public Interfaces::IMap {
    public:
        VoxelMap();
        ~VoxelMap() override;

        uint8_t getBlock(int x, int y, int z) const override;
        void setBlock(int x, int y, int z, uint8_t type) override;
        void render(Interfaces::IRenderer* renderer) override;

    private:
        std::unordered_map<glm::ivec3, std::unique_ptr<Chunk>, IVec3Hash> m_chunks;
        
        mutable glm::ivec3 m_lastChunkPos;
        mutable Chunk* m_lastChunk;

        void worldToChunk(int wx, int wy, int wz, int& cx, int& cy, int& cz, int& lx, int& ly, int& lz) const;
        Chunk* getChunk(int cx, int cy, int cz) const;
        Chunk* getOrCreateChunk(int cx, int cy, int cz);
    };

}
}
