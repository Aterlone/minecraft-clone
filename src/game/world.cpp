#include "world.hpp"

World::World(int chunk_count) : chunk_count(chunk_count) {
    Chunk dummyChunk(0);
    chunks.resize(chunk_count, dummyChunk);
}

int World::createWorld() {
    for (int i = 0; i < chunk_count; ++i) {
        this->chunks[i] = Chunk(i);
        this->chunks[i].createChunk();
    }
    return 1;
}

// int checkAndLoadChunks(glm::vec3 cameraPos) {
//     if (cameraPos.equals(loadPos)) return 0;

//     loadPos = cameraPos;
//     // for i in loadpos surrounding if it exists then load it else don't.
//     for 
// }

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

