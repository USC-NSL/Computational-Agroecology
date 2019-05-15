#define _CRT_SECURE_NO_WARNINGS

#ifndef _STRING_
#include <string>
#endif

#ifndef _IOSTREAM_
#include <iostream>
#endif

#ifndef _INC_MALLOC
#include <malloc.h>
#endif

#ifndef _VECTOR_
#include <vector>
#endif

#ifndef __glew_h__
	#include "glew/include/glew.h"
	#pragma comment(lib, "glew32.lib")
#endif

#ifndef __glut_h__
	#ifdef __APPLE__
	#include "GL/glut.h"
	#endif
	#ifdef __linux__
	#include "GL/glut.h"
	#endif
	#ifdef _WIN32
		#include "glut/glut.h"
		#pragma comment(lib, "glut32.lib")
	#endif
#endif