#pragma once
#include <cstdint>
#include <vector>

namespace Engine {
namespace Graphics {

    const int CHUNK_SIZE = 32;

    class Chunk {
    public:
        Chunk(int chunkX, int chunkY, int chunkZ);
        ~Chunk();

        void setBlock(int x, int y, int z, uint8_t type);
        uint8_t getBlock(int x, int y, int z) const;
        
        void generateMesh();
        void render();

    private:
        int m_chunkX, m_chunkY, m_chunkZ;
        uint8_t m_blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];

        unsigned int m_vao, m_vbo;
        int m_vertexCount;
        bool m_isDirty;

        void setupGL();
        void addFace(std::vector<float>& vertices, int x, int y, int z, int faceId, uint8_t type);
    };

}
}
