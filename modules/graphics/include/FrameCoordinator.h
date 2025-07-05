#pragma once

#include <cstddef>

namespace FlockingGraphics {

//----------------------------------------------------------------------------------------------------------------------
/// @brief Simplified frame coordination system
//----------------------------------------------------------------------------------------------------------------------
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
