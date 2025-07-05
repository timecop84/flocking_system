#ifndef PERFORMANCE_MONITOR_H
#define PERFORMANCE_MONITOR_H

#include <chrono>
#include <string>
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

namespace flock {

/**
 * @brief Simple performance monitoring utility for comparing legacy vs modern algorithms
 */
class PerformanceMonitor {
public:
    struct TimingResult {
        std::string name;
        double averageMs;
        double minMs;
        double maxMs;
        size_t sampleCount;
    };

private:
    struct Timer {
        std::string name;
        std::chrono::high_resolution_clock::time_point startTime;
        std::vector<double> samples;
        static constexpr size_t MAX_SAMPLES = 300; // Keep last 300 samples (5 seconds at 60 FPS)
    };

    std::vector<Timer> m_timers;
    bool m_enabled = true;

public:
    /**
     * @brief Start timing a named operation
     */
    void startTimer(const std::string& name) {
        if (!m_enabled) return;
        
        auto it = std::find_if(m_timers.begin(), m_timers.end(),
            [&name](const Timer& t) { return t.name == name; });
        
        if (it == m_timers.end()) {
            m_timers.push_back({name, std::chrono::high_resolution_clock::now(), {}});
        } else {
            it->startTime = std::chrono::high_resolution_clock::now();
        }
    }

    /**
     * @brief End timing for a named operation
     */
    void endTimer(const std::string& name) {
        if (!m_enabled) return;
        
        auto endTime = std::chrono::high_resolution_clock::now();
        
        auto it = std::find_if(m_timers.begin(), m_timers.end(),
            [&name](const Timer& t) { return t.name == name; });
        
        if (it != m_timers.end()) {
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
                endTime - it->startTime).count() / 1000.0; // Convert to milliseconds
            
            it->samples.push_back(duration);
            
            // Keep only the last MAX_SAMPLES
            if (it->samples.size() > Timer::MAX_SAMPLES) {
                it->samples.erase(it->samples.begin());
            }
        }
    }

    /**
     * @brief Get timing results for all operations
     */
    std::vector<TimingResult> getResults() const {
        std::vector<TimingResult> results;
        
        for (const auto& timer : m_timers) {
            if (timer.samples.empty()) continue;
            
            TimingResult result;
            result.name = timer.name;
            result.sampleCount = timer.samples.size();
            
            // Calculate statistics
            result.averageMs = std::accumulate(timer.samples.begin(), timer.samples.end(), 0.0) / timer.samples.size();
            result.minMs = *std::min_element(timer.samples.begin(), timer.samples.end());
            result.maxMs = *std::max_element(timer.samples.begin(), timer.samples.end());
            
            results.push_back(result);
        }
        
        return results;
    }

    /**
     * @brief Print performance comparison to console
     */
    void printComparison() const {
        auto results = getResults();
        if (results.empty()) return;
        
        std::cout << "\n=== PERFORMANCE COMPARISON ===" << std::endl;
        std::cout << "Operation          | Avg (ms) | Min (ms) | Max (ms) | Samples" << std::endl;
        std::cout << "-------------------|----------|----------|----------|--------" << std::endl;
        
        for (const auto& result : results) {
            printf("%-18s | %8.3f | %8.3f | %8.3f | %7zu\n",
                result.name.c_str(), result.averageMs, result.minMs, result.maxMs, result.sampleCount);
        }
        std::cout << "===============================" << std::endl;
        
        // Find legacy vs modern comparison
        auto legacyIt = std::find_if(results.begin(), results.end(),
            [](const TimingResult& r) { return r.name.find("Legacy") != std::string::npos; });
        auto modernIt = std::find_if(results.begin(), results.end(),
            [](const TimingResult& r) { return r.name.find("Modern") != std::string::npos; });
        
        if (legacyIt != results.end() && modernIt != results.end()) {
            double speedup = legacyIt->averageMs / modernIt->averageMs;
            std::cout << "\nPerformance Summary:" << std::endl;
            if (speedup > 1.0) {
                printf("Modern is %.2fx FASTER than Legacy\n", speedup);
            } else {
                printf("Modern is %.2fx SLOWER than Legacy\n", 1.0 / speedup);
            }
        }
        std::cout << std::endl;
    }

    /**
     * @brief Enable/disable performance monitoring
     */
    void setEnabled(bool enabled) { m_enabled = enabled; }
    bool isEnabled() const { return m_enabled; }

    /**
     * @brief Clear all timing data
     */
    void clear() {
        for (auto& timer : m_timers) {
            timer.samples.clear();
        }
    }
};

/**
 * @brief RAII timer for automatic timing of scoped operations
 */
class ScopedTimer {
private:
    PerformanceMonitor& m_monitor;
    std::string m_name;

public:
    ScopedTimer(PerformanceMonitor& monitor, const std::string& name)
        : m_monitor(monitor), m_name(name) {
        m_monitor.startTimer(m_name);
    }
    
    ~ScopedTimer() {
        m_monitor.endTimer(m_name);
    }
};

// Macro for easy scoped timing
#define FLOCK_TIMER(monitor, name) flock::ScopedTimer _timer(monitor, name)

} // namespace flock

#endif // PERFORMANCE_MONITOR_H
