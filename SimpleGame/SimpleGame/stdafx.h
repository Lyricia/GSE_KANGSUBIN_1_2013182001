#pragma once

#include "targetver.h"

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <tchar.h>
#include <chrono>
#include <list>

#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"
#include "Vector3D.h"

#define WINDOW_HEIGHT	500
#define WINDOW_WIDTH	500

#define WINDOW_HEIGHT_HALF	250
#define WINDOW_WIDTH_HALF	250


#define EPSILON			0.00001f

using namespace std;

enum DIR { LEFT, RIGHT, TOP, BOTTOM };

enum OBJTYPE {
	OBJ_BUILDING,
	OBJ_CHARACTER,
	OBJ_BULLET,
	OBJ_ARROW
};