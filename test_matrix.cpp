#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "modules/math/include/Matrix.h"

int main() {
    std::cout << "=== Matrix Translation Test ===" << std::endl;
    
    // Test 1: Basic translate function
    Matrix testMatrix;
    std::cout << "Initial matrix (identity):" << std::endl;
    std::cout << "Translation components: [0,3]=" << testMatrix.m_matrix[0][3] 
              << ", [1,3]=" << testMatrix.m_matrix[1][3] 
              << ", [2,3]=" << testMatrix.m_matrix[2][3] << std::endl;
    
    // Apply translation
    testMatrix.translate(50.0f, 0.0f, 0.0f);
    std::cout << "\nAfter translate(50, 0, 0):" << std::endl;
    std::cout << "Translation components: [0,3]=" << testMatrix.m_matrix[0][3] 
              << ", [1,3]=" << testMatrix.m_matrix[1][3] 
              << ", [2,3]=" << testMatrix.m_matrix[2][3] << std::endl;
    
    // Test 2: Direct GLM comparison
    std::cout << "\n=== Direct GLM Test ===" << std::endl;
    glm::mat4 glmMatrix = glm::mat4(1.0f);
    std::cout << "GLM initial matrix translation: [0,3]=" << glmMatrix[0][3] 
              << ", [1,3]=" << glmMatrix[1][3] 
              << ", [2,3]=" << glmMatrix[2][3] << std::endl;
    
    glmMatrix = glm::translate(glmMatrix, glm::vec3(50.0f, 0.0f, 0.0f));
    std::cout << "GLM after translate(50, 0, 0): [0,3]=" << glmMatrix[0][3] 
              << ", [1,3]=" << glmMatrix[1][3] 
              << ", [2,3]=" << glmMatrix[2][3] << std::endl;
    
    // Test 3: Test what happens with multiple translations
    std::cout << "\n=== Multiple Translation Test ===" << std::endl;
    Matrix multiMatrix;
    multiMatrix.translate(25.0f, 0.0f, 0.0f);
    std::cout << "After first translate(25, 0, 0): [0,3]=" << multiMatrix.m_matrix[0][3] << std::endl;
    
    multiMatrix.translate(25.0f, 0.0f, 0.0f);
    std::cout << "After second translate(25, 0, 0): [0,3]=" << multiMatrix.m_matrix[0][3] << std::endl;
    
    return 0;
}
