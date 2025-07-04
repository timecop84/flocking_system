#include "../include/FrameCoordinator.h"
#include <iostream>

namespace FlockingGraphics {

FrameCoordinator& FrameCoordinator::getInstance() {
    static FrameCoordinator instance;
    return instance;
}

void FrameCoordinator::beginFrame() {
    m_frameNumber++;
}

void FrameCoordinator::endFrame() {
    // Frame ended - no debug output needed
}

size_t FrameCoordinator::getFrameNumber() const {
    return m_frameNumber;
}

} // namespace FlockingGraphics
