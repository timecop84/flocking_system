#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <direct.h>

namespace FlockingGraphics {
// Loads shader source from a file. Returns empty string on failure.
inline std::string loadShaderSource(const std::string& filepath) {
    std::cout << "[INLINE TEST] Entered loadShaderSource with path: " << filepath << std::endl;
    try {
        char cwd[1024];
        if (_getcwd(cwd, sizeof(cwd))) {
            std::cout << "[INLINE TEST] Current working directory: " << cwd << std::endl;
        } else {
            std::cout << "[INLINE TEST] Could not get current working directory." << std::endl;
        }
        std::cout << "[INLINE TEST] Attempting to open file: " << filepath << std::endl;
        std::ifstream file(filepath.c_str());
        if (!file.is_open()) {
            std::cerr << "[INLINE TEST] ERROR: Failed to open file: " << filepath << std::endl;
            return "";
        }
        std::cout << "[INLINE TEST] File is open: " << filepath << std::endl;
        std::string source((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        std::cout << "[INLINE TEST] File read complete. Size: " << source.size() << std::endl;
        file.close();
        return source;
    } catch (const std::exception& e) {
        std::cerr << "[INLINE TEST] Exception: " << e.what() << std::endl;
        return "";
    } catch (...) {
        std::cerr << "[INLINE TEST] Unknown exception" << std::endl;
        return "";
    }
}
}
