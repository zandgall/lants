#pragma once
#ifndef ANT_H
#define ANT_H
#include "AntRules.h"
class Ant {
public:
	glm::i64vec2 pos;
	char direction = 0;
	bool dead = false;
	long long steps = 0;
	Ant() {
		pos = glm::i64vec2(0);
		ruleset = &AntRuleset::ant_rules["RL"];
	}
	Ant(glm::i64vec2 pos, const char* rule) {
		this->pos = pos;
		ruleset = &AntRuleset::ant_rules[rule];
	}
	void tick(int iterations);
	void move(int forward, int right) {
		pos.y += moveyf[direction] * forward + moveyr[direction] * right;
		pos.x += movexf[direction] * forward + movexr[direction] * right;
	}
private:
	AntRuleset *ruleset;
	static int movexr[4], movexf[4], moveyr[4], moveyf[4];
};
#endif