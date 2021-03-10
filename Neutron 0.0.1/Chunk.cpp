#include "Chunk.h"
#include "Neutron.h"
int Chunk::width = 32;
int Chunk::height = 32;

float Chunk::freezetime = 100.0f;

unsigned int Chunk::vao = 0;
unsigned int Chunk::vbo = 0;
unsigned int Chunk::shader = 0;

Chunk::Chunk() {
	ping = glfwGetTime();
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenTextures(1, &texture);				// gen texture,
	glBindTexture(GL_TEXTURE_2D, texture);  // and then bind it

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);	// Set texture to empty as 2d, with RGB and A channels, using unsigned bytes

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);		// Use set interpolation close up
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);		// ..and far away
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// Don't overlap horizontally
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	// ..or vertically

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);	// Set a Color attachment to the frame buffer
	glDrawBuffer(GL_COLOR_ATTACHMENT0);										// Set a Color attachment to the draw buffer
	
	glViewport(0, 0, width, height);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Chunk::freeze() {
	if (frozen)
		return;
	update();
	this->colors.clear();
	this->frozen = true;
}

void Chunk::unfreeze() {
	if (!frozen)
		return;
	ping = glfwGetTime();
	this->colors = neutron::processor::colorsFromTexture3i(this->texture);
	this->frozen = false;
}

void Chunk::update() {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glBindTexture(GL_TEXTURE_2D, texture);
	if (vao == 0) {
		vao = neutron::processor::VAO();
		vbo = neutron::processor::VBO();
		GLfloat box[4][4] = {					// Create box from (-1, -1) to (1, 1) 
						{-1,  1, 0, 1},
						{ 1,  1, 1, 1},
						{-1, -1, 0, 0},
						{ 1, -1, 1, 0},
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_STATIC_DRAW);	// Apply the box to the VBO and VAO
		neutron::processor::attribute(0, 4, 4, 0);

		shader = neutron::processor::LoadShader("res/shaders/colorstexture.shader");
	}
	glUseProgram(shader);
	glViewport(0, 0, width, height);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(vao);

	neutron::processor::uniF4(shader, "scale", glm::vec4(0.5, 0.5, 0, 0)); // pass size to the shader
	neutron::processor::uniM4(shader, "ortho", glm::ortho(0.f, (float)width, 0.f, (float)height, -1.f, 1.f));
	neutron::processor::uniB(shader, "useTex", false);

	for (unsigned int i = 0, size = colors.size(); i < size; i++) {
		if (colors[i] == glm::ivec3(255))
			continue;
		int x = i % width;
		int y = i / width;
		neutron::processor::uniF4(shader, "offset", glm::vec4(x - width / 2.f + 1, y - height / 2.f + 1, 0, 0));		// And Assign them as the location of the pixel

		neutron::processor::uniF4(shader, "inCol", glm::vec4(colors[i].x/255.0, colors[i].y/255.0, colors[i].z/255.0, 1));
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);					// and render
	}
	glBindVertexArray(0);
	glBindBuffer(GL_FRAMEBUFFER, 0);
}

void Chunk::clear() {
	this->colors.clear();
	this->texture = neutron::processor::textureFromColor(glm::vec4(1), width, height);
}