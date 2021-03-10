#pragma once
#include "Envelope.h"
#include "Chunk.h"
#include "VecComp.h"
#include "Ant.h"
#include <map>
#include <vector>
#include <iostream>
#ifndef WORLD_H
#define WORLD_H
class World {
public: 
	static std::vector<Ant> ants;
	static std::map<glm::i32, std::map<glm::i32, Chunk>> chunks;
	glm::i64vec2 tile = glm::i64vec2(0);
	World();
	void tick();

	void render() {

	}

	void render_gui();

	static bool charted(glm::i32vec2 pos) {
		return chunks.size() > 0 && chunks.count(pos.x)>0 && chunks[pos.x].size() > 0 && chunks[pos.x].count(pos.y)>0;
	}

	static void chart(glm::i32vec2 pos) {
		std::cout << "Charted " << pos.x << " : " << pos.y << std::endl;
		if (chunks.count(pos.x) == 0)
			chunks.insert(std::make_pair(pos.x, std::map<glm::i32, Chunk>()));
		if (chunks[pos.x].count(pos.y) == 0) {
			chunks[pos.x].insert(std::make_pair(pos.y, Chunk()));
		}
	}

	static Chunk *get(glm::i32vec2 pos) {
		chart(pos);
		return &chunks[pos.x][pos.y];
	}

};
#endif