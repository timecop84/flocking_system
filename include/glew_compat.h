#ifndef GLEW_COMPAT_H
#define GLEW_COMPAT_H

#include <QOpenGLFunctions>
#include <QOpenGLContext>

#ifndef APIENTRY
#define APIENTRY
#endif

#ifndef WINAPI
#define WINAPI
#endif

// Simple GLEW compatibility stub
// For a full implementation, link against actual GLEW library

// OpenGL extension function declarations
#ifndef GL_UNIFORM_BUFFER
#define GL_UNIFORM_BUFFER 0x8A11
#endif

#ifndef GL_INVALID_INDEX
#define GL_INVALID_INDEX 0xFFFFFFFFu
#endif

#ifndef GL_ARRAY_BUFFER
#define GL_ARRAY_BUFFER 0x8892
#endif

#ifndef GL_ELEMENT_ARRAY_BUFFER
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#endif

#ifndef GL_STATIC_DRAW
#define GL_STATIC_DRAW 0x88E4
#endif

typedef void (APIENTRY *PFNGLBINDBUFFERBASEPROC)(GLenum target, GLuint index, GLuint buffer);
typedef GLuint (APIENTRY *PFNGLGETUNIFORMBLOCKINDEXPROC)(GLuint program, const GLchar *uniformBlockName);
typedef void (APIENTRY *PFNGLUNIFORMBLOCKBINDINGPROC)(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);

// VAO function pointers for modern OpenGL
typedef void (APIENTRY *PFNGLGENVERTEXARRAYSPROC)(GLsizei n, GLuint *arrays);
typedef void (APIENTRY *PFNGLBINDVERTEXARRAYPROC)(GLuint array);
typedef void (APIENTRY *PFNGLDELETEVERTEXARRAYSPROC)(GLsizei n, const GLuint *arrays);

// VBO function pointers
typedef void (APIENTRY *PFNGLGENBUFFERSPROC)(GLsizei n, GLuint *buffers);
typedef void (APIENTRY *PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
typedef void (APIENTRY *PFNGLBUFFERDATAPROC)(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef void (APIENTRY *PFNGLDELETEBUFFERSPROC)(GLsizei n, const GLuint *buffers);

// Vertex attribute function pointers
typedef void (APIENTRY *PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef void (APIENTRY *PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint index);

// Global function pointers
extern PFNGLBINDBUFFERBASEPROC glBindBufferBase;
extern PFNGLGETUNIFORMBLOCKINDEXPROC glGetUniformBlockIndex;
extern PFNGLUNIFORMBLOCKBINDINGPROC glUniformBlockBinding;

// VAO function pointers
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;

// VBO function pointers
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;

// Vertex attribute function pointers
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;

// Initialize function pointers
inline int glewInit() {
    auto context = QOpenGLContext::currentContext();
    if (!context) return -1;
    
    glBindBufferBase = (PFNGLBINDBUFFERBASEPROC)context->getProcAddress("glBindBufferBase");
    glGetUniformBlockIndex = (PFNGLGETUNIFORMBLOCKINDEXPROC)context->getProcAddress("glGetUniformBlockIndex");
    glUniformBlockBinding = (PFNGLUNIFORMBLOCKBINDINGPROC)context->getProcAddress("glUniformBlockBinding");
    
    // Initialize VAO functions
    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)context->getProcAddress("glGenVertexArrays");
    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)context->getProcAddress("glBindVertexArray");
    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)context->getProcAddress("glDeleteVertexArrays");
    
    // Initialize VBO functions
    glGenBuffers = (PFNGLGENBUFFERSPROC)context->getProcAddress("glGenBuffers");
    glBindBuffer = (PFNGLBINDBUFFERPROC)context->getProcAddress("glBindBuffer");
    glBufferData = (PFNGLBUFFERDATAPROC)context->getProcAddress("glBufferData");
    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)context->getProcAddress("glDeleteBuffers");
    
    // Initialize vertex attribute functions
    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)context->getProcAddress("glVertexAttribPointer");
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)context->getProcAddress("glEnableVertexAttribArray");
    
    return 0; // GLEW_OK
}

#endif // GLEW_COMPAT_H
