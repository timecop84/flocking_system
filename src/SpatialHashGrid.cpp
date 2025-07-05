#include "SpatialHashGrid.h"
#include <cmath>

namespace flock {

SpatialHashGrid::SpatialHashGrid(float cellSize) : m_cellSize(cellSize) {
}

void SpatialHashGrid::clear() {
    m_grid.clear();
}

void SpatialHashGrid::addBoid(Boid* boid, int boidIndex) {
    Vector pos = boid->getPosition();
    glm::vec3 position(pos.m_x, pos.m_y, pos.m_z);
    glm::ivec3 cellCoords = getGridCoords(position);
    m_grid[cellCoords].emplace_back(boid, boidIndex);
}

std::vector<std::pair<Boid*, int>> SpatialHashGrid::getNearbyBoids(const glm::vec3& position, float radius) const {
    std::vector<std::pair<Boid*, int>> nearbyBoids;
    
    // Get the grid coordinates for the query position
    glm::ivec3 centerCell = getGridCoords(position);
    
    // Calculate how many cells we need to check based on radius
    int cellRadius = static_cast<int>(std::ceil(radius / m_cellSize)) + 1;
    
    // Check all cells within the radius
    for (int x = -cellRadius; x <= cellRadius; x++) {
        for (int y = -cellRadius; y <= cellRadius; y++) {
            for (int z = -cellRadius; z <= cellRadius; z++) {
                glm::ivec3 cellCoords = centerCell + glm::ivec3(x, y, z);
                
                auto it = m_grid.find(cellCoords);
                if (it != m_grid.end()) {
                    for (const auto& boidPair : it->second) {
                        Vector boidPos = boidPair.first->getPosition();
                        glm::vec3 boidPosition(boidPos.m_x, boidPos.m_y, boidPos.m_z);
                        
                        float distance = glm::length(position - boidPosition);
                        if (distance <= radius) {
                            nearbyBoids.push_back(boidPair);
                        }
                    }
                }
            }
        }
    }
    
    return nearbyBoids;
}

std::vector<std::pair<Boid*, int>> SpatialHashGrid::getNearbyBoidsInCells(const glm::vec3& position) const {
    std::vector<std::pair<Boid*, int>> nearbyBoids;
    
    glm::ivec3 cellCoords = getGridCoords(position);
    std::vector<glm::ivec3> neighboringCells = getNeighboringCells(cellCoords);
    
    for (const glm::ivec3& cell : neighboringCells) {
        auto it = m_grid.find(cell);
        if (it != m_grid.end()) {
            for (const auto& boidPair : it->second) {
                nearbyBoids.push_back(boidPair);
            }
        }
    }
    
    return nearbyBoids;
}

glm::ivec3 SpatialHashGrid::getGridCoords(const glm::vec3& position) const {
    return glm::ivec3(
        static_cast<int>(std::floor(position.x / m_cellSize)),
        static_cast<int>(std::floor(position.y / m_cellSize)),
        static_cast<int>(std::floor(position.z / m_cellSize))
    );
}

std::vector<glm::ivec3> SpatialHashGrid::getNeighboringCells(const glm::ivec3& cellCoords) const {
    std::vector<glm::ivec3> neighbors;
    neighbors.reserve(27); // 3x3x3 = 27 cells
    
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            for (int z = -1; z <= 1; z++) {
                neighbors.push_back(cellCoords + glm::ivec3(x, y, z));
            }
        }
    }
    
    return neighbors;
}

} // namespace flock
