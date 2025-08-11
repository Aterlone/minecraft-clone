#include "world.hpp"

World::World(int chunk_count) : chunk_count(chunk_count) {
    Chunk dummyChunk(0);
    chunks.resize(chunk_count, dummyChunk);
}

int World::checkAndLoadChunks(glm::vec3 cameraPos) {
    if (cameraPos == loadPos) return 0;

    loadPos = cameraPos;

    Chunk dummyChunk(0);
    chunks.resize(9, dummyChunk);

    for (int i = loadPos[0]; i < loadPos[0]+2; ++i) {
        for (int j = loadPos[2]; j < loadPos[2]+2; ++j) {
            int pos = (i-loadPos[0])*3 + (j-loadPos[2]);
            this->chunks[pos] = Chunk(0);
            this->chunks[pos].setWorldPos({i, 0, j});
            this->chunks[pos].createChunk();
        }
    }
    return 1;
}

std::vector<float> World::getVertices() {
    this->allVertices.clear();
    for (int i = 0; i < chunk_count; ++i) {
        auto nestedVerts = chunks[i].getVertices(); 
        std::vector<float> flatVerts = flatten<float>(nestedVerts);
        this->allVertices.insert(this->allVertices.end(), flatVerts.begin(), flatVerts.end());
    }
    return this->allVertices;
}


std::vector<unsigned int> World::getIndices() {
    this->allIndices.clear();
    unsigned int vertexOffset = 0;

    for (int i = 0; i < chunk_count; ++i) {
        auto nestedInd = chunks[i].getIndices(); 
        std::vector<unsigned int> flatInd = flatten<unsigned int>(nestedInd);

        for (auto& idx : flatInd) {
            this->allIndices.push_back(idx + vertexOffset);
        }

        auto nestedVerts = chunks[i].getVertices();
        std::vector<float> flatVerts = flatten<float>(nestedVerts);
        vertexOffset += flatVerts.size() / 5; 
    }

    return this->allIndices;
}

template<typename T>
std::vector<T> World::flatten(const std::vector<std::vector<T>>& nested) {
    std::vector<T> flat;
    for (const auto& v2 : nested) {
        flat.insert(flat.end(), v2.begin(), v2.end());
    }
    return flat;
}