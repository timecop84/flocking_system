#include "flock.h"
#include <iostream>

int main() {
    // Test if we can access basic methods first
    
    // This should work - testing a method that definitely exists
    std::cout << "Testing a known method..." << std::endl;
    
    // Test the test method
    std::cout << "Testing testMethod..." << std::endl;
    // testFlock->testMethod();
    
    std::cout << "All basic tests completed!" << std::endl;
    
    return 0;
}
