#ifndef MINIGLAD_H
#define MINIGLAD_H

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define APIENTRY __stdcall
#else
#define APIENTRY
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* OpenGL types */
typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef void GLvoid;
typedef signed char GLbyte;
typedef unsigned char GLubyte;
typedef short GLshort;
typedef unsigned short GLushort;
typedef int GLint;
typedef unsigned int GLuint;
typedef int GLsizei;
typedef float GLfloat;
typedef double GLdouble;
typedef char GLchar;
typedef ptrdiff_t GLsizeiptr;
typedef ptrdiff_t GLintptr;

/* OpenGL constants */
#define GL_FALSE 0
#define GL_TRUE 1
#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_DEPTH_TEST 0x0B71
#define GL_CULL_FACE 0x0B44
#define GL_BLEND 0x0BE2
#define GL_TRIANGLES 0x0004
#define GL_UNSIGNED_BYTE 0x1401
#define GL_UNSIGNED_INT 0x1405
#define GL_FLOAT 0x1406
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_TEXTURE_2D 0x0DE1
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_NEAREST 0x2600
#define GL_LINEAR 0x2601
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_RGBA 0x1908
#define GL_RED 0x1903
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_STATIC_DRAW 0x88E4
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_TEXTURE0 0x84C0
#define GL_UNPACK_ALIGNMENT 0x0CF5

/* Function pointer types */
typedef void (APIENTRY *PFNGLCLEARPROC)(GLbitfield mask);
typedef void (APIENTRY *PFNGLCLEARCOLORPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (APIENTRY *PFNGLENABLEPROC)(GLenum cap);
typedef void (APIENTRY *PFNGLDISABLEPROC)(GLenum cap);
typedef void (APIENTRY *PFNGLVIEWPORTPROC)(GLint x, GLint y, GLsizei width, GLsizei height);
typedef GLuint (APIENTRY *PFNGLCREATESHADERPROC)(GLenum type);
typedef void (APIENTRY *PFNGLSHADERSOURCEPROC)(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
typedef void (APIENTRY *PFNGLCOMPILESHADERPROC)(GLuint shader);
typedef void (APIENTRY *PFNGLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint *params);
typedef void (APIENTRY *PFNGLDELETESHADERPROC)(GLuint shader);
typedef GLuint (APIENTRY *PFNGLCREATEPROGRAMPROC)(void);
typedef void (APIENTRY *PFNGLATTACHSHADERPROC)(GLuint program, GLuint shader);
typedef void (APIENTRY *PFNGLLINKPROGRAMPROC)(GLuint program);
typedef void (APIENTRY *PFNGLGETPROGRAMIVPROC)(GLuint program, GLenum pname, GLint *params);
typedef void (APIENTRY *PFNGLDELETEPROGRAMPROC)(GLuint program);
typedef void (APIENTRY *PFNGLUSEPROGRAMPROC)(GLuint program);
typedef GLint (APIENTRY *PFNGLGETUNIFORMLOCATIONPROC)(GLuint program, const GLchar *name);
typedef void (APIENTRY *PFNGLUNIFORM1IPROC)(GLint location, GLint v0);
typedef void (APIENTRY *PFNGLUNIFORM1FPROC)(GLint location, GLfloat v0);
typedef void (APIENTRY *PFNGLUNIFORM2FPROC)(GLint location, GLfloat v0, GLfloat v1);
typedef void (APIENTRY *PFNGLUNIFORM3FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (APIENTRY *PFNGLUNIFORM4FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (APIENTRY *PFNGLUNIFORMMATRIX4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (APIENTRY *PFNGLGENVERTEXARRAYSPROC)(GLsizei n, GLuint *arrays);
typedef void (APIENTRY *PFNGLGENBUFFERSPROC)(GLsizei n, GLuint *buffers);
typedef void (APIENTRY *PFNGLBINDVERTEXARRAYPROC)(GLuint array);
typedef void (APIENTRY *PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
typedef void (APIENTRY *PFNGLBUFFERDATAPROC)(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef void (APIENTRY *PFNGLDELETEVERTEXARRAYSPROC)(GLsizei n, const GLuint *arrays);
typedef void (APIENTRY *PFNGLDELETEBUFFERSPROC)(GLsizei n, const GLuint *buffers);
typedef void (APIENTRY *PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef void (APIENTRY *PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void (APIENTRY *PFNGLDRAWELEMENTSPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices);
typedef void (APIENTRY *PFNGLDRAWARRAYSPROC)(GLenum mode, GLint first, GLsizei count);
typedef void (APIENTRY *PFNGLBLENDFUNCPROC)(GLenum sfactor, GLenum dfactor);
typedef void (APIENTRY *PFNGLGENTEXTURESPROC)(GLsizei n, GLuint *textures);
typedef void (APIENTRY *PFNGLBINDTEXTUREPROC)(GLenum target, GLuint texture);
typedef void (APIENTRY *PFNGLTEXIMAGE2DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (APIENTRY *PFNGLTEXPARAMETERIPROC)(GLenum target, GLenum pname, GLint param);
typedef void (APIENTRY *PFNGLDELETETEXTURESPROC)(GLsizei n, const GLuint *textures);
typedef void (APIENTRY *PFNGLACTIVETEXTUREPROC)(GLenum texture);
typedef void (APIENTRY *PFNGLBUFFERSUBDATAPROC)(GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
typedef void (APIENTRY *PFNGLPIXELSTOREIPROC)(GLenum pname, GLint param);
typedef void (APIENTRY *PFNGLGETINTEGERVPROC)(GLenum pname, GLint *params);

/* Function pointers */
extern PFNGLCLEARPROC glClear;
extern PFNGLCLEARCOLORPROC glClearColor;
extern PFNGLENABLEPROC glEnable;
extern PFNGLDISABLEPROC glDisable;
extern PFNGLVIEWPORTPROC glViewport;
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLUNIFORM1IPROC glUniform1i;
extern PFNGLUNIFORM1FPROC glUniform1f;
extern PFNGLUNIFORM2FPROC glUniform2f;
extern PFNGLUNIFORM3FPROC glUniform3f;
extern PFNGLUNIFORM4FPROC glUniform4f;
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLDRAWELEMENTSPROC glDrawElements;
extern PFNGLDRAWARRAYSPROC glDrawArrays;
extern PFNGLBLENDFUNCPROC glBlendFunc;
extern PFNGLGENTEXTURESPROC glGenTextures;
extern PFNGLBINDTEXTUREPROC glBindTexture;
extern PFNGLTEXIMAGE2DPROC glTexImage2D;
extern PFNGLTEXPARAMETERIPROC glTexParameteri;
extern PFNGLDELETETEXTURESPROC glDeleteTextures;
extern PFNGLACTIVETEXTUREPROC glActiveTexture;
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;
extern PFNGLPIXELSTOREIPROC glPixelStorei;
extern PFNGLGETINTEGERVPROC glGetIntegerv;

/* Loader function */
int gladLoadGL(void);

#ifdef __cplusplus
}
#endif

/* Implementation */
#ifdef MINIGLAD_IMPLEMENTATION

PFNGLCLEARPROC glClear = 0;
PFNGLCLEARCOLORPROC glClearColor = 0;
PFNGLENABLEPROC glEnable = 0;
PFNGLDISABLEPROC glDisable = 0;
PFNGLVIEWPORTPROC glViewport = 0;
PFNGLCREATESHADERPROC glCreateShader = 0;
PFNGLSHADERSOURCEPROC glShaderSource = 0;
PFNGLCOMPILESHADERPROC glCompileShader = 0;
PFNGLGETSHADERIVPROC glGetShaderiv = 0;
PFNGLDELETESHADERPROC glDeleteShader = 0;
PFNGLCREATEPROGRAMPROC glCreateProgram = 0;
PFNGLATTACHSHADERPROC glAttachShader = 0;
PFNGLLINKPROGRAMPROC glLinkProgram = 0;
PFNGLGETPROGRAMIVPROC glGetProgramiv = 0;
PFNGLDELETEPROGRAMPROC glDeleteProgram = 0;
PFNGLUSEPROGRAMPROC glUseProgram = 0;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = 0;
PFNGLUNIFORM1IPROC glUniform1i = 0;
PFNGLUNIFORM1FPROC glUniform1f = 0;
PFNGLUNIFORM2FPROC glUniform2f = 0;
PFNGLUNIFORM3FPROC glUniform3f = 0;
PFNGLUNIFORM4FPROC glUniform4f = 0;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = 0;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = 0;
PFNGLGENBUFFERSPROC glGenBuffers = 0;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = 0;
PFNGLBINDBUFFERPROC glBindBuffer = 0;
PFNGLBUFFERDATAPROC glBufferData = 0;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = 0;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = 0;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = 0;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = 0;
PFNGLDRAWELEMENTSPROC glDrawElements = 0;
PFNGLDRAWARRAYSPROC glDrawArrays = 0;
PFNGLBLENDFUNCPROC glBlendFunc = 0;
PFNGLGENTEXTURESPROC glGenTextures = 0;
PFNGLBINDTEXTUREPROC glBindTexture = 0;
PFNGLTEXIMAGE2DPROC glTexImage2D = 0;
PFNGLTEXPARAMETERIPROC glTexParameteri = 0;
PFNGLDELETETEXTURESPROC glDeleteTextures = 0;
PFNGLACTIVETEXTUREPROC glActiveTexture = 0;
PFNGLBUFFERSUBDATAPROC glBufferSubData = 0;
PFNGLPIXELSTOREIPROC glPixelStorei = 0;
PFNGLGETINTEGERVPROC glGetIntegerv = 0;

#ifdef _WIN32
static HMODULE mg_gl_lib = 0;
typedef void* (APIENTRY *PFNWGLGETPROCADDRESSPROC)(const char*);
static PFNWGLGETPROCADDRESSPROC mg_wglGetProcAddress = 0;

static void* mg_get_proc(const char* name) {
    void* p = 0;
    if (mg_wglGetProcAddress) p = mg_wglGetProcAddress(name);
    if (!p && mg_gl_lib) p = (void*)GetProcAddress(mg_gl_lib, name);
    return p;
}

int gladLoadGL(void) {
    mg_gl_lib = LoadLibraryA("opengl32.dll");
    if (!mg_gl_lib) return 0;
    mg_wglGetProcAddress = (PFNWGLGETPROCADDRESSPROC)GetProcAddress(mg_gl_lib, "wglGetProcAddress");
    if (!mg_wglGetProcAddress) return 0;

    glClear = (PFNGLCLEARPROC)mg_get_proc("glClear");
    glClearColor = (PFNGLCLEARCOLORPROC)mg_get_proc("glClearColor");
    glEnable = (PFNGLENABLEPROC)mg_get_proc("glEnable");
    glDisable = (PFNGLDISABLEPROC)mg_get_proc("glDisable");
    glViewport = (PFNGLVIEWPORTPROC)mg_get_proc("glViewport");
    glCreateShader = (PFNGLCREATESHADERPROC)mg_get_proc("glCreateShader");
    glShaderSource = (PFNGLSHADERSOURCEPROC)mg_get_proc("glShaderSource");
    glCompileShader = (PFNGLCOMPILESHADERPROC)mg_get_proc("glCompileShader");
    glGetShaderiv = (PFNGLGETSHADERIVPROC)mg_get_proc("glGetShaderiv");
    glDeleteShader = (PFNGLDELETESHADERPROC)mg_get_proc("glDeleteShader");
    glCreateProgram = (PFNGLCREATEPROGRAMPROC)mg_get_proc("glCreateProgram");
    glAttachShader = (PFNGLATTACHSHADERPROC)mg_get_proc("glAttachShader");
    glLinkProgram = (PFNGLLINKPROGRAMPROC)mg_get_proc("glLinkProgram");
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)mg_get_proc("glGetProgramiv");
    glDeleteProgram = (PFNGLDELETEPROGRAMPROC)mg_get_proc("glDeleteProgram");
    glUseProgram = (PFNGLUSEPROGRAMPROC)mg_get_proc("glUseProgram");
    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)mg_get_proc("glGetUniformLocation");
    glUniform1i = (PFNGLUNIFORM1IPROC)mg_get_proc("glUniform1i");
    glUniform1f = (PFNGLUNIFORM1FPROC)mg_get_proc("glUniform1f");
    glUniform2f = (PFNGLUNIFORM2FPROC)mg_get_proc("glUniform2f");
    glUniform3f = (PFNGLUNIFORM3FPROC)mg_get_proc("glUniform3f");
    glUniform4f = (PFNGLUNIFORM4FPROC)mg_get_proc("glUniform4f");
    glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)mg_get_proc("glUniformMatrix4fv");
    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)mg_get_proc("glGenVertexArrays");
    glGenBuffers = (PFNGLGENBUFFERSPROC)mg_get_proc("glGenBuffers");
    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)mg_get_proc("glBindVertexArray");
    glBindBuffer = (PFNGLBINDBUFFERPROC)mg_get_proc("glBindBuffer");
    glBufferData = (PFNGLBUFFERDATAPROC)mg_get_proc("glBufferData");
    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)mg_get_proc("glDeleteVertexArrays");
    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)mg_get_proc("glDeleteBuffers");
    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)mg_get_proc("glVertexAttribPointer");
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)mg_get_proc("glEnableVertexAttribArray");
    glDrawElements = (PFNGLDRAWELEMENTSPROC)mg_get_proc("glDrawElements");
    glDrawArrays = (PFNGLDRAWARRAYSPROC)mg_get_proc("glDrawArrays");
    glBlendFunc = (PFNGLBLENDFUNCPROC)mg_get_proc("glBlendFunc");
    glGenTextures = (PFNGLGENTEXTURESPROC)mg_get_proc("glGenTextures");
    glBindTexture = (PFNGLBINDTEXTUREPROC)mg_get_proc("glBindTexture");
    glTexImage2D = (PFNGLTEXIMAGE2DPROC)mg_get_proc("glTexImage2D");
    glTexParameteri = (PFNGLTEXPARAMETERIPROC)mg_get_proc("glTexParameteri");
    glDeleteTextures = (PFNGLDELETETEXTURESPROC)mg_get_proc("glDeleteTextures");
    glActiveTexture = (PFNGLACTIVETEXTUREPROC)mg_get_proc("glActiveTexture");
    glBufferSubData = (PFNGLBUFFERSUBDATAPROC)mg_get_proc("glBufferSubData");
    glPixelStorei = (PFNGLPIXELSTOREIPROC)mg_get_proc("glPixelStorei");
    glGetIntegerv = (PFNGLGETINTEGERVPROC)mg_get_proc("glGetIntegerv");

    return glClear && glCreateShader && glGenVertexArrays;
}

#else
#include <dlfcn.h>
static void* mg_gl_lib = 0;
typedef void* (*PFNGLXGETPROCADDRESSPROC)(const char*);
static PFNGLXGETPROCADDRESSPROC mg_glXGetProcAddress = 0;

static void* mg_get_proc(const char* name) {
    void* p = 0;
    if (mg_glXGetProcAddress) p = mg_glXGetProcAddress(name);
    if (!p && mg_gl_lib) p = dlsym(mg_gl_lib, name);
    return p;
}

int gladLoadGL(void) {
    const char* libs[] = {"libGL.so.1", "libGL.so", 0};
    for (int i = 0; libs[i]; i++) {
        mg_gl_lib = dlopen(libs[i], RTLD_NOW | RTLD_GLOBAL);
        if (mg_gl_lib) break;
    }
    if (!mg_gl_lib) return 0;
    mg_glXGetProcAddress = (PFNGLXGETPROCADDRESSPROC)dlsym(mg_gl_lib, "glXGetProcAddressARB");

    glClear = (PFNGLCLEARPROC)mg_get_proc("glClear");
    glClearColor = (PFNGLCLEARCOLORPROC)mg_get_proc("glClearColor");
    glEnable = (PFNGLENABLEPROC)mg_get_proc("glEnable");
    glDisable = (PFNGLDISABLEPROC)mg_get_proc("glDisable");
    glViewport = (PFNGLVIEWPORTPROC)mg_get_proc("glViewport");
    glCreateShader = (PFNGLCREATESHADERPROC)mg_get_proc("glCreateShader");
    glShaderSource = (PFNGLSHADERSOURCEPROC)mg_get_proc("glShaderSource");
    glCompileShader = (PFNGLCOMPILESHADERPROC)mg_get_proc("glCompileShader");
    glGetShaderiv = (PFNGLGETSHADERIVPROC)mg_get_proc("glGetShaderiv");
    glDeleteShader = (PFNGLDELETESHADERPROC)mg_get_proc("glDeleteShader");
    glCreateProgram = (PFNGLCREATEPROGRAMPROC)mg_get_proc("glCreateProgram");
    glAttachShader = (PFNGLATTACHSHADERPROC)mg_get_proc("glAttachShader");
    glLinkProgram = (PFNGLLINKPROGRAMPROC)mg_get_proc("glLinkProgram");
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)mg_get_proc("glGetProgramiv");
    glDeleteProgram = (PFNGLDELETEPROGRAMPROC)mg_get_proc("glDeleteProgram");
    glUseProgram = (PFNGLUSEPROGRAMPROC)mg_get_proc("glUseProgram");
    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)mg_get_proc("glGetUniformLocation");
    glUniform1i = (PFNGLUNIFORM1IPROC)mg_get_proc("glUniform1i");
    glUniform1f = (PFNGLUNIFORM1FPROC)mg_get_proc("glUniform1f");
    glUniform2f = (PFNGLUNIFORM2FPROC)mg_get_proc("glUniform2f");
    glUniform3f = (PFNGLUNIFORM3FPROC)mg_get_proc("glUniform3f");
    glUniform4f = (PFNGLUNIFORM4FPROC)mg_get_proc("glUniform4f");
    glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)mg_get_proc("glUniformMatrix4fv");
    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)mg_get_proc("glGenVertexArrays");
    glGenBuffers = (PFNGLGENBUFFERSPROC)mg_get_proc("glGenBuffers");
    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)mg_get_proc("glBindVertexArray");
    glBindBuffer = (PFNGLBINDBUFFERPROC)mg_get_proc("glBindBuffer");
    glBufferData = (PFNGLBUFFERDATAPROC)mg_get_proc("glBufferData");
    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)mg_get_proc("glDeleteVertexArrays");
    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)mg_get_proc("glDeleteBuffers");
    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)mg_get_proc("glVertexAttribPointer");
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)mg_get_proc("glEnableVertexAttribArray");
    glDrawElements = (PFNGLDRAWELEMENTSPROC)mg_get_proc("glDrawElements");
    glDrawArrays = (PFNGLDRAWARRAYSPROC)mg_get_proc("glDrawArrays");
    glBlendFunc = (PFNGLBLENDFUNCPROC)mg_get_proc("glBlendFunc");
    glGenTextures = (PFNGLGENTEXTURESPROC)mg_get_proc("glGenTextures");
    glBindTexture = (PFNGLBINDTEXTUREPROC)mg_get_proc("glBindTexture");
    glTexImage2D = (PFNGLTEXIMAGE2DPROC)mg_get_proc("glTexImage2D");
    glTexParameteri = (PFNGLTEXPARAMETERIPROC)mg_get_proc("glTexParameteri");
    glDeleteTextures = (PFNGLDELETETEXTURESPROC)mg_get_proc("glDeleteTextures");
    glActiveTexture = (PFNGLACTIVETEXTUREPROC)mg_get_proc("glActiveTexture");
    glBufferSubData = (PFNGLBUFFERSUBDATAPROC)mg_get_proc("glBufferSubData");
    glPixelStorei = (PFNGLPIXELSTOREIPROC)mg_get_proc("glPixelStorei");
    glGetIntegerv = (PFNGLGETINTEGERVPROC)mg_get_proc("glGetIntegerv");

    return glClear && glCreateShader && glGenVertexArrays;
}
#endif

#endif /* MINIGLAD_IMPLEMENTATION */
#endif /* MINIGLAD_H */
