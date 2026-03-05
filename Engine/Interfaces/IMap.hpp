#pragma once
#include <cstdint>

namespace Engine {
namespace Interfaces {

    enum class BlockType : uint8_t {
        AIR = 0,
        FLOOR = 1,
        WALL = 2,
        PILLAR = 3,
        DOOR = 4
    };

    class IMap {
    public:
        virtual ~IMap() = default;

        virtual uint8_t getBlock(int x, int y, int z) const = 0;
        virtual void setBlock(int x, int y, int z, uint8_t type) = 0;
        virtual void render(class IRenderer* renderer) = 0;

        virtual bool isSolid(int x, int y, int z) const {
            uint8_t block = getBlock(x, y, z);
            return block != 0 && block != (uint8_t)BlockType::DOOR;
        }
    };

}
}
