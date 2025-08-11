#ifndef WORLD_HPP
#define WORLD_HPP

#include "chunk.hpp"
#include <string>

class World {
    private:
        std::vector<Chunk> chunks;
        std::vector<float> allVertices;
        std::vector<unsigned int> allIndices;

        // glm::vec3 loadPos;

        int chunk_count;
    
    public:
        World(int chunk_count);
        int createWorld();
        // int checkAndLoadChunks(glm::vec3 cameraPos);

        std::vector<float> getVertices();
        std::vector<unsigned int> getIndices();
        template<typename T>
        std::vector<T> flatten(const std::vector<std::vector<T>>& nested);
};

#endif