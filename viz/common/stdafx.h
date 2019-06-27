#define _CRT_SECURE_NO_WARNINGS

#ifndef _STRING_
#include <string>
#endif

#ifndef _IOSTREAM_
#include <iostream>
#endif

#ifndef _INC_STDLIB
#include <stdlib.h>
#endif

#ifndef _VECTOR_
#include <vector>
#endif

#ifndef __glew_h__
#ifdef __linux__
#include <GL/glew.h>
#endif
#ifdef __APPLE__
#include <OPENGL/gl3.h>
#endif
#ifdef _WIN32
#include "glew/include/glew.h"
#pragma comment(lib, "glew32.lib")
#endif
#endif /* __glew_h__ */

#ifndef __glut_h__
#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#ifdef __linux__
#include <GL/glut.h>
#endif
#ifdef _WIN32
#include "glut/include/glut.h"
#pragma comment(lib, "freeglutd.lib")
#endif
#endif /*  __glut_h__ */
