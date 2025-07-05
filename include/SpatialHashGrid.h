#pragma once
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include "boid.h"

namespace flock {

/**
 * @brief Spatial hash grid for efficient neighbor queries in flocking simulation
 * 
 * This class implements a spatial partitioning system that reduces the complexity
 * of neighbor searches from O(N²) to approximately O(N) by dividing space into
 * a grid and only checking boids in nearby grid cells.
 */
class SpatialHashGrid {
public:
    SpatialHashGrid(float cellSize = 10.0f);
    ~SpatialHashGrid() = default;
    
    // Clear all boids from the grid
    void clear();
    
    // Add a boid to the grid at its current position
    void addBoid(Boid* boid, int boidIndex);
    
    // Get all nearby boids within the specified radius
    std::vector<std::pair<Boid*, int>> getNearbyBoids(const glm::vec3& position, float radius) const;
    
    // Get all boids in the same cell and neighboring cells
    std::vector<std::pair<Boid*, int>> getNearbyBoidsInCells(const glm::vec3& position) const;
    
private:
    float m_cellSize;
    
    // Hash function for 3D grid coordinates
    struct Vec3Hash {
        std::size_t operator()(const glm::ivec3& key) const {
            return std::hash<int>()(key.x) ^ 
                   (std::hash<int>()(key.y) << 1) ^ 
                   (std::hash<int>()(key.z) << 2);
        }
    };
    
    // Grid storage: maps grid coordinates to lists of boids
    std::unordered_map<glm::ivec3, std::vector<std::pair<Boid*, int>>, Vec3Hash> m_grid;
    
    // Convert world position to grid coordinates
    glm::ivec3 getGridCoords(const glm::vec3& position) const;
    
    // Get all 27 neighboring cells (including center cell)
    std::vector<glm::ivec3> getNeighboringCells(const glm::ivec3& cellCoords) const;
};

} // namespace flock
