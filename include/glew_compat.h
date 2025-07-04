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

// Compute shader constants
#ifndef GL_COMPUTE_SHADER
#define GL_COMPUTE_SHADER 0x91B9
#endif

#ifndef GL_SHADER_STORAGE_BUFFER
#define GL_SHADER_STORAGE_BUFFER 0x90D2
#endif

#ifndef GL_SHADER_STORAGE_BARRIER_BIT
#define GL_SHADER_STORAGE_BARRIER_BIT 0x00002000
#endif

// Sync object constants
#ifndef GL_SYNC_GPU_COMMANDS_COMPLETE
#define GL_SYNC_GPU_COMMANDS_COMPLETE 0x9117
#endif

#ifndef GL_SYNC_FLUSH_COMMANDS_BIT
#define GL_SYNC_FLUSH_COMMANDS_BIT 0x00000001
#endif

#ifndef GL_ALREADY_SIGNALED
#define GL_ALREADY_SIGNALED 0x911A
#endif

#ifndef GL_TIMEOUT_EXPIRED
#define GL_TIMEOUT_EXPIRED 0x911B
#endif

#ifndef GL_CONDITION_SATISFIED
#define GL_CONDITION_SATISFIED 0x911C
#endif

#ifndef GL_WAIT_FAILED
#define GL_WAIT_FAILED 0x911D
#endif

// Sync object type
typedef struct __GLsync *GLsync;

// 64-bit unsigned integer type
#ifndef GL_UNSIGNED_INT64_ARB
#define GL_UNSIGNED_INT64_ARB 0x140F
typedef unsigned long long GLuint64;
#endif

#ifndef GL_MAX_COMPUTE_WORK_GROUP_SIZE
#define GL_MAX_COMPUTE_WORK_GROUP_SIZE 0x91BF
#endif

#ifndef GL_READ_ONLY
#define GL_READ_ONLY 0x88B8
#endif

#ifndef GL_DYNAMIC_DRAW
#define GL_DYNAMIC_DRAW 0x88E8
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
typedef void (APIENTRY *PFNGLBUFFERSUBDATAPROC)(GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
typedef void (APIENTRY *PFNGLDELETEBUFFERSPROC)(GLsizei n, const GLuint *buffers);

// Vertex attribute function pointers
typedef void (APIENTRY *PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef void (APIENTRY *PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint index);

// Instanced rendering function pointers
typedef void (APIENTRY *PFNGLDRAWELEMENTSINSTANCEDPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount);
typedef void (APIENTRY *PFNGLVERTEXATTRIBDIVISORPROC)(GLuint index, GLuint divisor);

// Compute shader function pointers
typedef void (APIENTRY *PFNGLDISPATCHCOMPUTEPROC)(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
typedef void (APIENTRY *PFNGLMEMORYBARRIERPROC)(GLbitfield barriers);
typedef void (APIENTRY *PFNGLUSEPROGRAMPROC)(GLuint program);
typedef void (APIENTRY *PFNGLGETINTEGERI_VPROC)(GLenum target, GLuint index, GLint *data);
typedef void* (APIENTRY *PFNGLMAPBUFFERPROC)(GLenum target, GLenum access);
typedef GLboolean (APIENTRY *PFNGLUNMAPBUFFERPROC)(GLenum target);

// Shader program function pointers
typedef GLuint (APIENTRY *PFNGLCREATESHADERPROC)(GLenum type);
typedef void (APIENTRY *PFNGLSHADERSOURCEPROC)(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
typedef void (APIENTRY *PFNGLCOMPILESHADERPROC)(GLuint shader);
typedef void (APIENTRY *PFNGLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint *params);
typedef void (APIENTRY *PFNGLGETSHADERINFOLOGPROC)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (APIENTRY *PFNGLDELETESHADERPROC)(GLuint shader);
typedef GLuint (APIENTRY *PFNGLCREATEPROGRAMPROC)(void);
typedef void (APIENTRY *PFNGLATTACHSHADERPROC)(GLuint program, GLuint shader);
typedef void (APIENTRY *PFNGLLINKPROGRAMPROC)(GLuint program);
typedef void (APIENTRY *PFNGLGETPROGRAMIVPROC)(GLuint program, GLenum pname, GLint *params);
typedef void (APIENTRY *PFNGLGETPROGRAMINFOLOGPROC)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (APIENTRY *PFNGLDELETEPROGRAMPROC)(GLuint program);

// Sync object function typedefs
typedef GLsync (APIENTRY *PFNGLFENCESYNCPROC)(GLenum condition, GLbitfield flags);
typedef GLenum (APIENTRY *PFNGLCLIENTWAITSYNCPROC)(GLsync sync, GLbitfield flags, GLuint64 timeout);
typedef void (APIENTRY *PFNGLDELETESYNCPROC)(GLsync sync);

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
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;

// Vertex attribute function pointers
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;

// Instanced rendering function pointers
extern PFNGLDRAWELEMENTSINSTANCEDPROC glDrawElementsInstanced;
extern PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor;

// Compute shader function pointers
extern PFNGLDISPATCHCOMPUTEPROC glDispatchCompute;
extern PFNGLMEMORYBARRIERPROC glMemoryBarrier;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLGETINTEGERI_VPROC glGetIntegeri_v;
extern PFNGLMAPBUFFERPROC glMapBuffer;
extern PFNGLUNMAPBUFFERPROC glUnmapBuffer;

// Shader program function pointers
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;

// Sync object function pointers
extern PFNGLFENCESYNCPROC glFenceSync;
extern PFNGLCLIENTWAITSYNCPROC glClientWaitSync;
extern PFNGLDELETESYNCPROC glDeleteSync;

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
    glBufferSubData = (PFNGLBUFFERSUBDATAPROC)context->getProcAddress("glBufferSubData");
    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)context->getProcAddress("glDeleteBuffers");
    
    // Initialize vertex attribute functions
    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)context->getProcAddress("glVertexAttribPointer");
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)context->getProcAddress("glEnableVertexAttribArray");
    
    // Initialize instanced rendering functions
    glDrawElementsInstanced = (PFNGLDRAWELEMENTSINSTANCEDPROC)context->getProcAddress("glDrawElementsInstanced");
    glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC)context->getProcAddress("glVertexAttribDivisor");
    
    // Initialize compute shader functions
    glDispatchCompute = (PFNGLDISPATCHCOMPUTEPROC)context->getProcAddress("glDispatchCompute");
    glMemoryBarrier = (PFNGLMEMORYBARRIERPROC)context->getProcAddress("glMemoryBarrier");
    glUseProgram = (PFNGLUSEPROGRAMPROC)context->getProcAddress("glUseProgram");
    glGetIntegeri_v = (PFNGLGETINTEGERI_VPROC)context->getProcAddress("glGetIntegeri_v");
    glMapBuffer = (PFNGLMAPBUFFERPROC)context->getProcAddress("glMapBuffer");
    glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)context->getProcAddress("glUnmapBuffer");
    
    // Initialize shader program functions
    glCreateShader = (PFNGLCREATESHADERPROC)context->getProcAddress("glCreateShader");
    glShaderSource = (PFNGLSHADERSOURCEPROC)context->getProcAddress("glShaderSource");
    glCompileShader = (PFNGLCOMPILESHADERPROC)context->getProcAddress("glCompileShader");
    glGetShaderiv = (PFNGLGETSHADERIVPROC)context->getProcAddress("glGetShaderiv");
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)context->getProcAddress("glGetShaderInfoLog");
    glDeleteShader = (PFNGLDELETESHADERPROC)context->getProcAddress("glDeleteShader");
    glCreateProgram = (PFNGLCREATEPROGRAMPROC)context->getProcAddress("glCreateProgram");
    glAttachShader = (PFNGLATTACHSHADERPROC)context->getProcAddress("glAttachShader");
    glLinkProgram = (PFNGLLINKPROGRAMPROC)context->getProcAddress("glLinkProgram");
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)context->getProcAddress("glGetProgramiv");
    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)context->getProcAddress("glGetProgramInfoLog");
    glDeleteProgram = (PFNGLDELETEPROGRAMPROC)context->getProcAddress("glDeleteProgram");
    
    // Initialize sync object functions
    glFenceSync = (PFNGLFENCESYNCPROC)context->getProcAddress("glFenceSync");
    glClientWaitSync = (PFNGLCLIENTWAITSYNCPROC)context->getProcAddress("glClientWaitSync");
    glDeleteSync = (PFNGLDELETESYNCPROC)context->getProcAddress("glDeleteSync");
    
    return 0; // GLEW_OK
}

#endif // GLEW_COMPAT_H
