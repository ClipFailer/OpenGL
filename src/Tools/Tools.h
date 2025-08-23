#pragma once

#include <cstdlib>
#include <glad/gl.h>

#define DEBUG

namespace Tools 
{
    // Очистка ошибок OpenGL
    static void GLClearErrors()
    {
        while (glGetError() != GL_NO_ERROR);
    }

    // Вывод ошибок OpenGL
    static bool GLLogCall(const char* func, const char* file, int line)
    {
        bool noError = true;
        while (GLenum error = glGetError())
        {
            std::cerr << "[OpenGL Error] Code: " << error
                    << ", function: " << func
                    << ", file: " << file
                    << ", line: " << line << "\n";
            noError = false;
        }
        return noError;
    }

#if defined(_MSC_VER)
    // MSVC (Visual Studio)
    #define ASSERT(x) \
        do { if(!(x)) { __debugbreak(); } } while(0) 
#elif defined(__GNUC__) || defined(__clang__)
    // GCC / Clang
    #include <signal.h>
    #if defined(SIGTRAP)
        #define ASSERT(x) \
        do { if(!(x)) { raise(SIGTRAP); } } while(0)
    #else
        #include <intrin.h>
        #define ASSERT(x) \
            do { if(!(x)) { __builtin_trap(); } } while(0)
    #endif
#else
    // fallback
    #define ASSERT(x) \
        do { if(!(x))) { std::abort(); } } while(0)
#endif

// glcall макрос
#define glcall(x) do {        \
    Tools::GLClearErrors();   \
    x;                        \
    ASSERT(Tools::GLLogCall(#x, __FILE__, __LINE__)); \
} while(0)
}
