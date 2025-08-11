#include "block.hpp"

Block::Block(std::vector<int> position) {
    float xpos = position[0] * 1.0f;
    float ypos = position[1] * 1.0f;
    float zpos = position[2] * 1.0f;

    for (int i = 0; i < 24; ++i) {
        float vx = defaultVertices[i * 5 + 0];
        float vy = defaultVertices[i * 5 + 1];
        float vz = defaultVertices[i * 5 + 2];
        float u = defaultVertices[i * 5 + 3];
        float v = defaultVertices[i * 5 + 4];

        vertices.push_back(vx + xpos);
        vertices.push_back(vy + ypos);
        vertices.push_back(vz + zpos);
        vertices.push_back(u);
        vertices.push_back(v);
    }

    for (int i = 0; i < (int)(sizeof(defaultIndices) / sizeof(unsigned int)); ++i) {
        blockIndices.push_back(defaultIndices[i]);
    }
}

std::vector<float> Block::getVertices() {
    return this->vertices;
}
std::vector<unsigned int> Block::getIndices() {
    return this->blockIndices;
}