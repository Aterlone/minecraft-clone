#include "world.hpp"

World::World() {}

// Load the chunks a certain distance from the player if they don't already exist.
int World::checkAndLoadChunks(glm::vec3 cameraPos) {
    // Check for change in camera's (aka the player's) change in position.
    cameraPos[0] = (int)(cameraPos[0] / WIDTH);
    cameraPos[1] = (int)(cameraPos[1] / HEIGHT);
    cameraPos[2] = (int)(cameraPos[2] / WIDTH);
    if (loadPos == cameraPos) return 0;
    loadPos = cameraPos

    // Load chunks.
    for (int i = loadPos[0]-8; i < loadPos[0]+8; ++i) {
        for (int j = loadPos[2]-8; j < loadPos[2]+8; ++j) {
            Chunk chunk = Chunk();
            chunk.setWorldPos({i, 0, j});
            if (std::find(chunks.begin(), chunks.end(), chunk) == chunks.end()) {
                this->chunks.push_back(chunk);
                this->chunks.back().createChunk();
            }
        }
    }
    return 1;
}

std::vector<float> World::getVertices() {
    this->allVertices.clear();
    for (int i = 0; i < (int)chunks.size(); ++i) {
        // If not in Render Distance.
        if (!chunks[i].inRange({(int)(loadPos[0]), (int)(loadPos[1]), (int)(loadPos[2])})) {
            continue;
        }
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
        // If not in Render Distance.
        if (!chunks[i].inRange({(int)(loadPos[0]), (int)(loadPos[1]), (int)(loadPos[2])})) {
            continue;
        }
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