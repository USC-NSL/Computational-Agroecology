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
#ifdef _WIN32
#ifndef __glew_h__
#include "glew/include/glew.h"
#pragma comment(lib, "glew32.lib")
#endif
#endif
#ifdef __APPLE__
#include <GL/glew.h>
#endif
#endif

#ifndef __glut_h__
#ifdef __APPLE__
#include <GLUT/glut.h>
#endif
#ifdef __linux__
#include <GL/glut.h>
#endif
#ifdef _WIN32
#include "glut/glut.h"
#pragma comment(lib, "glut32.lib")
#endif
#endif
