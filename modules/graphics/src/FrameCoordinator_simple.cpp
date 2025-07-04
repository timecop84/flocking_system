#include "../include/FrameCoordinator.h"
#include <iostream>

namespace FlockingGraphics {

FrameCoordinator& FrameCoordinator::getInstance() {
    static FrameCoordinator instance;
    return instance;
}

void FrameCoordinator::beginFrame() {
    m_frameNumber++;
    std::cout << "Frame " << m_frameNumber << " started" << std::endl;
}

void FrameCoordinator::endFrame() {
    std::cout << "Frame " << m_frameNumber << " ended" << std::endl;
}

size_t FrameCoordinator::getFrameNumber() const {
    return m_frameNumber;
}

} // namespace FlockingGraphics
