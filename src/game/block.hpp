#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <cmath>

const float defaultVertices[] = {
    // positions         // UVs
    // Front face
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // 0
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // 1
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 2
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // 3

    // Back face
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // 4
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 5
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 6
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 7

    // Left face
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 8
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // 9
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 10
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 11

    // Right face
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // 12
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // 13
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // 14
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 15

    // Top face
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // 16
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // 17
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 18
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 19

    // Bottom face
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // 20
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // 21
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // 22
    -0.5f, -0.5f, -0.5f,  1.0f, 1.0f  // 23
};



const unsigned int defaultIndices[] = {
    // Front face
    0, 1, 2,
    2, 3, 0,
    // Right face
    1, 5, 6,
    6, 2, 1,
    // Back face
    5, 4, 7,
    7, 6, 5,
    // Left face
    4, 0, 3,
    3, 7, 4,
    // Top face
    3, 2, 6,
    6, 7, 3,
    // Bottom face
    4, 5, 1,
    1, 0, 4
};

class Block {
    private:
        std::vector<float> vertices;
        std::vector<unsigned int> blockIndices;

    public:
        Block(std::vector<int> position);
        std::vector<float> getVertices();
        std::vector<unsigned int> getIndices();
};

#endif