#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "modules/math/include/Matrix.h"

int main() {
    std::cout << "=== GLM Matrix Layout Test ===" << std::endl;
    
    // Create identity matrix and print full matrix
    glm::mat4 matrix = glm::mat4(1.0f);
    std::cout << "Identity matrix:" << std::endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    
    // Apply translation
    matrix = glm::translate(matrix, glm::vec3(50.0f, 0.0f, 0.0f));
    std::cout << "\nAfter translate(50, 0, 0):" << std::endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    
    // Check if GLM is column-major vs row-major
    std::cout << "\nTranslation check (column-major assumption):" << std::endl;
    std::cout << "matrix[3][0] (x translation) = " << matrix[3][0] << std::endl;
    std::cout << "matrix[3][1] (y translation) = " << matrix[3][1] << std::endl;
    std::cout << "matrix[3][2] (z translation) = " << matrix[3][2] << std::endl;
    
    std::cout << "\nTranslation check (row-major assumption):" << std::endl;
    std::cout << "matrix[0][3] (x translation) = " << matrix[0][3] << std::endl;
    std::cout << "matrix[1][3] (y translation) = " << matrix[1][3] << std::endl;
    std::cout << "matrix[2][3] (z translation) = " << matrix[2][3] << std::endl;
    
    return 0;
}
