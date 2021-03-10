#include "Ant.h"
#include "main.h"
#include "World.h"

int Ant::movexr[4] = { 1, 0, -1, 0 };
int Ant::movexf[4] = { 0, 1, 0, -1 };
int Ant::moveyr[4] = { 0, 1, 0, -1 };
int Ant::moveyf[4] = { -1, 0, 1, 0 };
//((World*)(UsedEnvelope))->worldfunc();
void Ant::tick(int iterations) {
	if (dead)
		return;
	glm::i8vec2 inChunk = glm::i8vec2();
	glm::i32vec2 inWorld = glm::i32vec2(floor((float)pos.x / Chunk::width), floor((float)pos.y / Chunk::height));
	if (World::chunks.find(inWorld.x) == World::chunks.end())
		World::chunks.insert(std::make_pair(inWorld.x, std::map<glm::i32, Chunk>()));
	if (World::chunks[inWorld.x].find(inWorld.y) == World::chunks[inWorld.x].end())
		World::chunks[inWorld.x].insert(std::make_pair(inWorld.y, Chunk()));
	Chunk *c = &World::chunks[inWorld.x][inWorld.y];
	for (int i = 0; i < iterations; i++) {
		inChunk.x = pos.x%Chunk::width;
		if (inChunk.x < 0)
			inChunk.x += Chunk::width;
		inChunk.y = pos.y%Chunk::height;
		if (inChunk.y < 0)
			inChunk.y += Chunk::height;

		c->unfreeze();
		AntRule rule = ruleset->get(c->get(inChunk.x, inChunk.y));
		c->set(inChunk.x, inChunk.y, rule.next);
		direction += rule.rotate;
		direction %= 4;
		while (direction < 0)
			direction += 4;
		if (rule.die) {
			dead = true;
			return;
		}
		if (rule.birth)
			World::ants.push_back(Ant(this->pos, rule.birthRule));
		move(rule.forward, rule.right);
		if (i!=iterations-1 && inWorld != glm::i32vec2(floor((float)pos.x / Chunk::width), floor((float)pos.y / Chunk::height))) {
			c->update();
			inWorld = glm::i32vec2(floor((float)pos.x / Chunk::width), floor((float)pos.y / Chunk::height));
			if (World::chunks.find(inWorld.x) == World::chunks.end())
				World::chunks.insert(std::make_pair(inWorld.x, std::map<glm::i32, Chunk>()));
			if (World::chunks[inWorld.x].find(inWorld.y) == World::chunks[inWorld.x].end())
				World::chunks[inWorld.x].insert(std::make_pair(inWorld.y, Chunk()));
			c = &World::chunks[inWorld.x][inWorld.y];
		}
	}
	steps += iterations;
	c->update();
}