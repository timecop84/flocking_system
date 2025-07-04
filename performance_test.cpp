#include <iostream>
#include <chrono>
#include <thread>
#include "flock.h"
#include "BBox.h"
#include "obstacle.h"

int main() {
    std::cout << "=== FLOCKING SYSTEM PERFORMANCE TEST ===" << std::endl;
    
    // Create test environment
    BBox* bbox = new BBox(Vector(0,0,0), 200, 200, 200);  // Updated to match main application
    Obstacle* obstacle = new Obstacle(Vector(50,0,0), 15.0);
    
    // Test different boid counts
    std::vector<int> boidCounts = {500, 1000, 1500, 2000, 2500, 3000};
    
    for (int boidCount : boidCounts) {
        std::cout << "\n--- Testing with " << boidCount << " boids ---" << std::endl;
        
        // Create flock
        Flock* flock = new Flock(bbox, obstacle);
        flock->setFlockSize(boidCount);
        flock->resetBoids();
        
        // Warm up
        std::cout << "Warming up..." << std::endl;
        for (int i = 0; i < 10; i++) {
            flock->update();
        }
        
        // Performance test
        const int numFrames = 60; // Test 60 frames
        auto startTime = std::chrono::high_resolution_clock::now();
        
        for (int i = 0; i < numFrames; i++) {
            flock->update();
        }
        
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        
        float fps = (numFrames * 1000.0f) / duration.count();
        float msPerFrame = duration.count() / static_cast<float>(numFrames);
        
        std::cout << "Results:" << std::endl;
        std::cout << "  FPS: " << fps << std::endl;
        std::cout << "  ms/frame: " << msPerFrame << std::endl;
        std::cout << "  Total time: " << duration.count() << "ms" << std::endl;
        
        if (fps >= 60.0f) {
            std::cout << "  Status: EXCELLENT (60+ FPS)" << std::endl;
        } else if (fps >= 30.0f) {
            std::cout << "  Status: GOOD (30+ FPS)" << std::endl;
        } else if (fps >= 15.0f) {
            std::cout << "  Status: PLAYABLE (15+ FPS)" << std::endl;
        } else {
            std::cout << "  Status: POOR (<15 FPS)" << std::endl;
        }
        
        delete flock;
    }
    
    delete bbox;
    delete obstacle;
    
    std::cout << "\n=== PERFORMANCE TEST COMPLETE ===" << std::endl;
    std::cout << "Press Enter to exit...";
    std::cin.get();
    
    return 0;
}
