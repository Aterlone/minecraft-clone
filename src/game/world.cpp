#include "world.hpp"

World::World(int chunk_count) : chunk_count(chunk_count) {
    Chunk dummyChunk(0);
    chunks.resize(chunk_count, dummyChunk);
}

int World::checkAndLoadChunks(glm::vec3 cameraPos) {
    cameraPos[0] /= WIDTH;
    cameraPos[1] /= HEIGHT;
    cameraPos[2] /= WIDTH;
    if ((int)cameraPos[0] == loadPos[0] && (int)cameraPos[1] == loadPos[1] && (int)cameraPos[2] == loadPos[2]) return 0;
    loadPos[0] = (int)cameraPos[0];
    loadPos[1] = (int)cameraPos[1];
    loadPos[2] = (int)cameraPos[2];


    for (int i = loadPos[0]-8; i < loadPos[0]+8; ++i) {
        for (int j = loadPos[2]-8; j < loadPos[2]+8; ++j) {
            Chunk chunk = Chunk(0);
            chunk.setWorldPos({i, 0, j});
            if (std::find(chunks.begin(), chunks.end(), chunk) == chunks.end()) {
                this->chunks.push_back(chunk);
                this->chunks.back().createChunk();
                // std::cout << "Size of chunk vector" << chunks.size() << std::endl;
            }
        }
    }
    return 1;
}

// in future change so it doesn't check all and reformat cunks so tha it can just get the values in a specific range because mroe efficient
std::vector<float> World::getVertices() {
    this->allVertices.clear();
    for (int i = 0; i < (int)chunks.size(); ++i) {
        if (!chunks[i].inRange({(int)(loadPos[0]), (int)(loadPos[1]), (int)(loadPos[2])})) continue;
        auto nestedVerts = chunks[i].getVertices(); 
        std::vector<float> flatVerts = flatten<float>(nestedVerts);
        this->allVertices.insert(this->allVertices.end(), flatVerts.begin(), flatVerts.end());
    }
    return this->allVertices;
}


std::vector<unsigned int> World::getIndices() {
    this->allIndices.clear();
    unsigned int vertexOffset = 0;

    for (int i = 0; i < (int)chunks.size(); ++i) {
        if (!chunks[i].inRange({(int)(loadPos[0]), (int)(loadPos[1]), (int)(loadPos[2])})) continue;
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