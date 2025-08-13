#include "chunk.hpp"

// On initialisation initialise the chunk's blocks vector to the correct size.
Chunk::Chunk() {
    clearChunk();
}

// Create Chunk based on 
int Chunk::createChunk() {
    for (int x = 0; x < WIDTH; ++x) {
        for (int z = 0; z < WIDTH; ++z) {
            for (int y = 0; y < HEIGHT; ++y) {
                int circle = sqrt(abs(x-WIDTH/2) + abs(y-HEIGHT/2) + abs(z-WIDTH/2));
                if (circle > 3) continue;

                std::vector<int> position = {x+worldPos[0]*WIDTH, y+worldPos[1]*HEIGHT, z+worldPos[2]*WIDTH};
                this->blocks[x][z][y] = Block(position);
            }
        }
    }
    return 1;
}

// Clear the blocks vector and resize.
void Chunk::clearChunk() {
    blocks.clear();
    blocks.resize(WIDTH);
    for (auto& plane : blocks) {
        plane.resize(WIDTH);
        for (auto& row : plane) {
            row.resize(HEIGHT);
        }
    }
}

std::vector<std::vector<float>> Chunk::getVertices() {
    std::vector<std::vector<float>> vertices;
    for (int x = 0; x < WIDTH; ++x) {
        for (int z = 0; z < WIDTH; ++z) {
            for (int y = 0; y < HEIGHT; ++y) {
                vertices.push_back(blocks[x][z][y].getVertices());
            }
        }
    }
    return vertices;
}

std::vector<std::vector<unsigned int>> Chunk::getIndices() {
    std::vector<std::vector<unsigned int>> indices;
    unsigned int vertexOffset = 0;

    for (int x = 0; x < WIDTH; ++x) {
        for (int z = 0; z < WIDTH; ++z) {
            for (int y = 0; y < HEIGHT; ++y) {
                std::vector<unsigned int> blockIndices = blocks[x][z][y].getIndices();

                for (auto& index : blockIndices) {
                    index += vertexOffset;
                }

                indices.push_back(blockIndices);

                vertexOffset += blocks[x][z][y].getVertices().size() / 5;
            }
        }
    }
    return indices;
}

void Chunk::setWorldPos(std::vector<int> pos) {
    this->worldPos = pos;
}

// Curving Corners.
int Chunk::inRange(std::vector<int> pos) {
    int x, y, z;
    x = (abs(this->worldPos[0] - pos[0]) <= 4);
    y = (abs(this->worldPos[1] - pos[1]) <= 4); y = 1;
    z = (abs(this->worldPos[2] - pos[2]) <= 4);
    return x && y && z;
}
