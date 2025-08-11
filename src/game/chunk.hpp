#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "block.hpp"

const int WIDTH = 10;
const int HEIGHT = 3;

class Chunk {
    private:
        std::vector<std::vector<std::vector<Block>>> blocks;
        std::vector<int> worldPos;
    public:
        Chunk(int x);
        int createChunk();
        std::vector<std::vector<float>> getVertices();
        std::vector<std::vector<unsigned int>> getIndices();

        void setWorldPos(std::vector<int> pos);
};

#endif