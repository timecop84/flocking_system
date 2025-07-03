#ifndef MATRIX_H
#define MATRIX_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//----------------------------------------------------------------------------------------------------------------------
/// @brief Modern Matrix class using GLM instead of ngl_compat
/// @details Interface-compatible replacement for ngl::Matrix using glm::mat4
//----------------------------------------------------------------------------------------------------------------------
class Matrix {
public:
    Matrix() : m_matrix(1.0f) {}
    Matrix(const glm::mat4& mat) : m_matrix(mat) {}
    
    // Interface compatibility methods
    void identity() { m_matrix = glm::mat4(1.0f); }
    void translate(float x, float y, float z) { m_matrix = glm::translate(m_matrix, glm::vec3(x, y, z)); }
    void scale(float x, float y, float z) { m_matrix = glm::scale(m_matrix, glm::vec3(x, y, z)); }
    void transpose() { m_matrix = glm::transpose(m_matrix); }
    
    // GLM access
    operator glm::mat4() const { return m_matrix; }
    operator glm::mat4&() { return m_matrix; }
    
    // Assignment
    Matrix& operator=(const glm::mat4& mat) { m_matrix = mat; return *this; }
    
    // Matrix operations
    Matrix operator*(const Matrix& other) const { return Matrix(m_matrix * other.m_matrix); }
    glm::vec4 operator*(const glm::vec4& vec) const { return m_matrix * vec; }
    
    // Access to underlying matrix
    glm::mat4 getGLMMat4() const { return m_matrix; }
private:
    glm::mat4 m_matrix;
};

// Keep Mat3x3 as typedef for now
class Mat3x3 {
public:
    glm::mat3 m_matrix;

    Mat3x3() : m_matrix(1.0f) {}
    Mat3x3(const glm::mat3& mat) : m_matrix(mat) {}
    Mat3x3(const Matrix& mat) : m_matrix(glm::mat3(mat.getGLMMat4())) {} // Use public accessor

    void inverse() { m_matrix = glm::inverse(m_matrix); }

    operator glm::mat3() const { return m_matrix; }
    operator glm::mat3&() { return m_matrix; }

    // Assignment operators
    Mat3x3& operator=(const Matrix& mat) {
        m_matrix = glm::mat3(mat.getGLMMat4()); // Use public accessor
        return *this;
    }
};

#endif // MATRIX_H
