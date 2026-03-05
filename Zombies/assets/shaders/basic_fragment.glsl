#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in float BlockType;

void main()
{
    vec3 baseColor = vec3(0.5);
    if (BlockType > 0.5 && BlockType < 1.5) {
        baseColor = vec3(0.3, 0.4, 0.3); // Floor (Type 1)
    } else if (BlockType > 1.5 && BlockType < 2.5) {
        baseColor = vec3(0.4, 0.4, 0.45); // Wall (Type 2)
    } else if (BlockType > 2.5 && BlockType < 3.5) {
        baseColor = vec3(0.6, 0.3, 0.1); // Barrier (Type 3)
    } else if (BlockType > 3.5 && BlockType < 4.5) {
        baseColor = vec3(0.2, 0.8, 0.2); // Zombie (Type 4) - Bright Greenish
    }

    // Make blocks slightly shaded based on height
    float c = clamp(FragPos.y * 0.1 + 0.3, 0.2, 1.0);
    FragColor = vec4(baseColor * c, 1.0);
}
