#ifndef PERFORMANCEPROFILER_H
#define PERFORMANCEPROFILER_H

#include <chrono>
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <limits>

//----------------------------------------------------------------------------------------------------------------------
/// @brief High-resolution performance profiler for identifying bottlenecks
/// @details Uses RAII timing with automatic reporting and statistics
//----------------------------------------------------------------------------------------------------------------------
class PerformanceProfiler {
public:
    struct ProfileData {
        std::chrono::high_resolution_clock::time_point startTime;
        std::chrono::high_resolution_clock::time_point endTime;
        double duration_ms = 0.0;
        size_t callCount = 0;
        double totalTime_ms = 0.0;
        double minTime_ms = std::numeric_limits<double>::max();
        double maxTime_ms = 0.0;
        double avgTime_ms = 0.0;
    };

    // Singleton instance
    static PerformanceProfiler& getInstance() {
        static PerformanceProfiler instance;
        return instance;
    }

    // Start timing a profile section
    void startProfile(const std::string& name) {
        auto now = std::chrono::high_resolution_clock::now();
        m_profiles[name].startTime = now;
    }

    // End timing a profile section
    void endProfile(const std::string& name) {
        auto now = std::chrono::high_resolution_clock::now();
        auto& profile = m_profiles[name];
        profile.endTime = now;
        
        // Calculate duration in milliseconds
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
            profile.endTime - profile.startTime
        ).count();
        profile.duration_ms = duration / 1000.0;
        
        // Update statistics
        profile.callCount++;
        profile.totalTime_ms += profile.duration_ms;
        profile.minTime_ms = std::min(profile.minTime_ms, profile.duration_ms);
        profile.maxTime_ms = std::max(profile.maxTime_ms, profile.duration_ms);
        profile.avgTime_ms = profile.totalTime_ms / profile.callCount;
    }

    // Get profile data
    const ProfileData& getProfile(const std::string& name) const {
        static ProfileData empty;
        auto it = m_profiles.find(name);
        return (it != m_profiles.end()) ? it->second : empty;
    }

    // Print comprehensive performance report
    void printReport() const {
        std::cout << "\n=== PERFORMANCE PROFILER REPORT ===" << std::endl;
        std::cout << std::fixed << std::setprecision(3);
        std::cout << std::left << std::setw(25) << "Section"
                  << std::setw(8) << "Calls"
                  << std::setw(10) << "Last(ms)"
                  << std::setw(10) << "Avg(ms)"
                  << std::setw(10) << "Min(ms)"
                  << std::setw(10) << "Max(ms)"
                  << std::setw(12) << "Total(ms)" << std::endl;
        std::cout << std::string(85, '-') << std::endl;

        for (const auto& [name, profile] : m_profiles) {
            if (profile.callCount > 0) {
                std::cout << std::left << std::setw(25) << name
                          << std::setw(8) << profile.callCount
                          << std::setw(10) << profile.duration_ms
                          << std::setw(10) << profile.avgTime_ms
                          << std::setw(10) << profile.minTime_ms
                          << std::setw(10) << profile.maxTime_ms
                          << std::setw(12) << profile.totalTime_ms << std::endl;
            }
        }
        std::cout << std::string(85, '=') << std::endl;
    }

    // Reset all statistics
    void reset() {
        m_profiles.clear();
    }

    // Enable/disable profiling
    void setEnabled(bool enabled) { m_enabled = enabled; }
    bool isEnabled() const { return m_enabled; }

private:
    std::unordered_map<std::string, ProfileData> m_profiles;
    bool m_enabled = true;
};

//----------------------------------------------------------------------------------------------------------------------
/// @brief RAII profiler that automatically starts/stops timing
//----------------------------------------------------------------------------------------------------------------------
class ScopedProfiler {
public:
    ScopedProfiler(const std::string& name) : m_name(name) {
        if (PerformanceProfiler::getInstance().isEnabled()) {
            PerformanceProfiler::getInstance().startProfile(m_name);
        }
    }

    ~ScopedProfiler() {
        if (PerformanceProfiler::getInstance().isEnabled()) {
            PerformanceProfiler::getInstance().endProfile(m_name);
        }
    }

private:
    std::string m_name;
};

// Convenient macro for automatic profiling
#define PROFILE_SCOPE(name) ScopedProfiler _prof(name)
#define PROFILE_FUNCTION() ScopedProfiler _prof(__FUNCTION__)

#endif // PERFORMANCEPROFILER_H
