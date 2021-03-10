#pragma once
#ifndef min
#define min(a, b) (a<b) ? a : b
#endif
#ifndef max
#define max(a, b) (a<b) ? a : b
#endif
#ifndef MAIN_H 
#define MAIN_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Envelope.h"
#include "camera.h"
#include "World.h"
#include <chrono>

#include <ft2build.h>
#include <freetype/freetype.h>

#include <iostream>

static Envelope *UsedEnvelope;

static World *w;

static float tickTime, renderTime, guiRenderTime;

void render();

#endif 