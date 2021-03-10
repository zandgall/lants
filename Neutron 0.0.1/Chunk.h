#pragma once
#include <vector>
#include <glm/glm.hpp>
#ifndef CHUNK_H
#define CHUNK_H
class Chunk {
public:
	static int width, height;
	static float freezetime;
	unsigned int texture;
	bool frozen = true;
	float ping = 0;
	Chunk();
	~Chunk() {

	}
	void freeze();
	void unfreeze();
	void update();
	void clear();
	glm::ivec3 get(int i, int j) {
		int a = j * width + i;
		if (a<0 || a>=width*height || a>=colors.size())
			return glm::ivec3(255);
		return colors[a];
	}
	void set(int i, int j, glm::ivec3 color) {
		int a = j * width + i;
		if (a < 0 || a >= width * height || a >= colors.size())
			return;
		colors[a] = color;
	}
private:
	std::vector<glm::ivec3> colors = std::vector<glm::ivec3>();
	unsigned int framebuffer;
	static unsigned int vao, vbo, shader;
};
#endif