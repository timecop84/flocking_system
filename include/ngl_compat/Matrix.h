#ifndef NGL_MATRIX_H
#define NGL_MATRIX_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

namespace ngl {

class Matrix {
public:
    glm::mat4 m_matrix;
    float m_m[4][4];
    
    Matrix() : m_matrix(1.0f) {
        updateArray();
    }
    
    Matrix(const glm::mat4& mat) : m_matrix(mat) {
        updateArray();
    }
    
    void rotateX(float angle) {
        m_matrix = glm::rotate(m_matrix, glm::radians(angle), glm::vec3(1, 0, 0));
        updateArray();
    }
    
    void rotateY(float angle) {
        m_matrix = glm::rotate(m_matrix, glm::radians(angle), glm::vec3(0, 1, 0));
        updateArray();
    }
    
    void identity() {
        m_matrix = glm::mat4(1.0f);
        updateArray();
    }
    
    void translate(float x, float y, float z) {
        m_matrix = glm::translate(m_matrix, glm::vec3(x, y, z));
        updateArray();
    }
    
    Matrix operator*(const Matrix& other) const {
        return Matrix(m_matrix * other.m_matrix);
    }
    
    Matrix operator*(const glm::mat4& other) const {
        return Matrix(m_matrix * other);
    }
    
    void transpose() {
        m_matrix = glm::transpose(m_matrix);
        updateArray();
    }
    
    operator glm::mat4() const {
        return m_matrix;
    }
    
    // Conversion to mat3 for normal matrix operations
    operator glm::mat3() const {
        return glm::mat3(m_matrix);
    }
    
private:
    void updateArray() {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                m_m[i][j] = m_matrix[i][j];
            }
        }
    }
};

class Mat3x3 {
public:
    glm::mat3 m_matrix;
    
    Mat3x3() : m_matrix(1.0f) {}
    Mat3x3(const glm::mat3& mat) : m_matrix(mat) {}
    Mat3x3(const Matrix& mat) : m_matrix(mat) {}
    
    Mat3x3& operator=(const Matrix& other) {
        m_matrix = glm::mat3(other.m_matrix);
        return *this;
    }
    
    void inverse() {
        m_matrix = glm::inverse(m_matrix);
    }
    
    operator glm::mat3() const {
        return m_matrix;
    }
};

} // namespace ngl

#endif // NGL_MATRIX_H
