#include "Engine/Graphics/Chunk.hpp"

namespace Graphics {
    // Face IDs: 0=Top, 1=Bottom, 2=Left, 3=Right, 4=Front, 5=Back
    const float FACE_VERTICES[6][18] = {
        // TOP
        { 0,1,0, 0,1,1, 1,1,1, 1,1,1, 1,1,0, 0,1,0 },
        // BOTTOM
        { 0,0,1, 0,0,0, 1,0,0, 1,0,0, 1,0,1, 0,0,1 },
        // LEFT
        { 0,1,1, 0,0,1, 0,0,0, 0,0,0, 0,1,0, 0,1,1 },
        // RIGHT
        { 1,1,0, 1,0,0, 1,0,1, 1,0,1, 1,1,1, 1,1,0 },
        // FRONT
        { 0,1,0, 0,0,0, 1,0,0, 1,0,0, 1,1,0, 0,1,0 },
        // BACK
        { 1,1,1, 1,0,1, 0,0,1, 0,0,1, 0,1,1, 1,1,1 }
    };

    Chunk::Chunk(int chunkX, int chunkY, int chunkZ) 
        : m_chunkX(chunkX), m_chunkY(chunkY), m_chunkZ(chunkZ), m_vao(0), m_vbo(0), m_vertexCount(0), m_isDirty(true) {
        
        for (int x = 0; x < CHUNK_SIZE; ++x) {
            for (int y = 0; y < CHUNK_SIZE; ++y) {
                for (int z = 0; z < CHUNK_SIZE; ++z) {
                    m_blocks[x][y][z] = 0; // 0 = AIR
                }
            }
        }
        setupGL();
    }

    Chunk::~Chunk() {
        if (m_vao) glDeleteVertexArrays(1, &m_vao);
        if (m_vbo) glDeleteBuffers(1, &m_vbo);
    }

    void Chunk::setupGL() {
        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);
    }

    void Chunk::setBlock(int x, int y, int z, uint8_t type) {
        if (x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_SIZE && z >= 0 && z < CHUNK_SIZE) {
            m_blocks[x][y][z] = type;
            m_isDirty = true;
        }
    }

    uint8_t Chunk::getBlock(int x, int y, int z) const {
        if (x >= 0 && x < CHUNK_SIZE && y >= 0 && y < CHUNK_SIZE && z >= 0 && z < CHUNK_SIZE) {
            return m_blocks[x][y][z];
        }
        return 0; // Treat out-of-bounds as AIR for meshing boundary logic temporarily
    }

    void Chunk::generateMesh() {
        if (!m_isDirty) return;

        std::vector<float> vertices;

        for (int x = 0; x < CHUNK_SIZE; ++x) {
            for (int y = 0; y < CHUNK_SIZE; ++y) {
                for (int z = 0; z < CHUNK_SIZE; ++z) {
                    uint8_t block = m_blocks[x][y][z];
                    if (block == 0) continue;

                    // Check neighbors to decide which faces to draw
                    if (getBlock(x, y + 1, z) == 0) addFace(vertices, x, y, z, 0, block); // Top
                    if (getBlock(x, y - 1, z) == 0) addFace(vertices, x, y, z, 1, block); // Bottom
                    if (getBlock(x - 1, y, z) == 0) addFace(vertices, x, y, z, 2, block); // Left
                    if (getBlock(x + 1, y, z) == 0) addFace(vertices, x, y, z, 3, block); // Right
                    if (getBlock(x, y, z - 1) == 0) addFace(vertices, x, y, z, 4, block); // Front
                    if (getBlock(x, y, z + 1) == 0) addFace(vertices, x, y, z, 5, block); // Back
                }
            }
        }

        m_vertexCount = vertices.size() / 5; // 3 pos + 2 uv

        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
        m_isDirty = false;
    }

    void Chunk::addFace(std::vector<float>& vertices, int x, int y, int z, int faceId, uint8_t type) {
        float cx = m_chunkX * CHUNK_SIZE + x;
        float cy = m_chunkY * CHUNK_SIZE + y;
        float cz = m_chunkZ * CHUNK_SIZE + z;

        for (int i = 0; i < 6; ++i) { // 6 vertices per face
            vertices.push_back(FACE_VERTICES[faceId][i * 3 + 0] + cx);
            vertices.push_back(FACE_VERTICES[faceId][i * 3 + 1] + cy);
            vertices.push_back(FACE_VERTICES[faceId][i * 3 + 2] + cz);
            
            // Pass block type as the X UV coordinate
            vertices.push_back((float)type);
            vertices.push_back(0.0f);
        }
    }

    void Chunk::render() {
        if (m_isDirty) generateMesh();
        
        if (m_vertexCount > 0) {
            glBindVertexArray(m_vao);
            glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
            glBindVertexArray(0);
        }
    }
}
