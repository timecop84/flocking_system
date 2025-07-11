// FrameCoordinator.h - Modernized, documented, and cleaned up for maintainability
#pragma once

#include <cstddef>

namespace FlockingGraphics {
/**
 * @file FrameCoordinator.h
 * @brief Simplified frame coordination system for graphics rendering.
 *
 * Modernized for clarity and maintainability.
 *
 * @author Dionysios Toufexis
 * @date 2025
 */
class FrameCoordinator {
public:
    static FrameCoordinator& getInstance();
    
    void beginFrame();
    void endFrame();
    size_t getFrameNumber() const;
    
private:
    FrameCoordinator() = default;
    size_t m_frameNumber = 0;
};
} // namespace FlockingGraphics
