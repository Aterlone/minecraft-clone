#include "chunk.hpp"

Chunk::Chunk(int x) : worldPos({x, 0, 0}) {
    std::vector<int> dummyPos = {0, 0, 0};
    Block dummyBlock(dummyPos);
    
    blocks.resize(WIDTH); 
    for (int x = 0; x < WIDTH; ++x) {
        blocks[x].resize(WIDTH); 
        for (int z = 0; z < WIDTH; ++z) {
            blocks[x][z].resize(HEIGHT, dummyBlock); 
        }
    }
}

int Chunk::createChunk() {
    for (int x = 0; x < WIDTH; ++x) {
        for (int z = 0; z < WIDTH; ++z) {
            for (int y = 0; y < HEIGHT; ++y) {
                std::vector<int> position = {x+worldPos[0]*WIDTH, y+worldPos[1]*HEIGHT, z+worldPos[2]*WIDTH};
                this->blocks[x][z][y] = Block(position);
            }
        }
    }
    return 1;
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
