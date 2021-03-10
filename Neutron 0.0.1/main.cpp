#include "main.h"
#include "ValueAnimation.h"
#include "FileIO/FileChooser.h"
#include "SerialImage.h"
#include "VectorImage.h"
#include "Neutron.h"
#include "Quark.h"
#include "ButtonQuark.h"

#include "World.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <Windows.h>

using namespace neutron;
using namespace neutron::processor;

int main() {
	// Start neutron and set OpenGL configs
	neutron::set();
	
	glDisable(GL_CULL_FACE);

	//Draw lines
	passes::def_cust_textures["planetShader"] = processor::LoadShader("res/shaders/planet.shader");
	passes::def_cust_textures["planet"] = processor::getTextureFrom("res/textures/Vector Testing/Ball.png");
	world::loadFont("res/fonts/Arial.ttf", "arial", 48);

	modes::post_processing = false;

	w = new World();

	while (neutron::frame::running()) {

		neutron::beforeLoop();

		render();

		neutron::afterLoop();
	}

	// terminate
	glfwTerminate();
	return 0;
}

void render() {
	// render
	glViewport(0, 0, frame::width, frame::height);
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f); // Set the clear color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear
	
	glEnable(GL_DEPTH_TEST);
	w->tick();
	w->render();

	glDisable(GL_DEPTH_TEST);
	w->render_gui();
}