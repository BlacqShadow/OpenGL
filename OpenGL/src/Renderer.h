#pragma once

#include<GL/glew.h>

// MACROS
/* Break the program if the assertion is false */
#define ASSERT(x) if (!(x)) __debugbreak();
/* Macro to call GLClearError and GLLogCall before and after the function */
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

/* Clear all the errors */
void GLClearError();

/* Print the errors */
bool GLLogCall(const char* function, const char* file, int line);
