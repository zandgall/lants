#pragma once
#ifndef ANT_RULES_H
#define ANT_RULES_H
#include <map>
#include <vector>
#include <glm/glm.hpp>
#include <iostream>
#include "VecComp.h"
struct AntRule {
	int rotate;
	int forward;
	int right;
	bool birth, die;
	glm::ivec3 next;
	const char* birthRule;
	AntRule(int rotate, int forward, int right, glm::ivec3 next, bool birth = false, bool die = false, const char* birthRule = "RL") {
		this->rotate = rotate;
		this->forward = forward;
		this->right = right;
		this->next = next;
		this->birth = birth;
		this->die = die;
		this->birthRule = birthRule;
	}
	AntRule() {
		this->rotate = 1;
		this->forward = 1;
		this->right = 0;
		this->next = glm::ivec3(0);
		this->birth = false;
		this->die = false;
		this->birthRule = "RL";
	}
};
class AntRuleset {
public:
	static std::map<const char*, AntRuleset> ant_rules;
	AntRuleset() {
		
	}
	AntRuleset(AntRule defaultRule) {
		this->rules.insert(std::make_pair(glm::ivec3(255), defaultRule));
	}

	void add(glm::ivec3 color, AntRule rule) {
		this->rules.insert(std::make_pair(color, rule));
	}
	AntRule get(glm::ivec3 color) {
		if (this->rules.count(color) == 0) {
			auto r = rules.begin();
			int off = int(double(rand()) / double(RAND_MAX)*(this->rules.size()));
			std::advance(r, off);
			std::cout << "Using rule " << off << std::endl;
			this->rules.insert(std::make_pair(color, r->second));
		}
		return this->rules[color];
	}

	static void HSVtoRGB(float *r, float *g, float *b, float h, float s, float v)
	{
		int i;
		float f, p, q, t;
		if (s == 0) {
			// achromatic (grey)
			*r = *g = *b = v;
			return;
		}
		h /= 60;			// sector 0 to 5
		i = floor(h);
		f = h - i;			// factorial part of h
		p = v * (1 - s);
		q = v * (1 - s * f);
		t = v * (1 - s * (1 - f));
		switch (i) {
		case 0:
			*r = v;
			*g = t;
			*b = p;
			break;
		case 1:
			*r = q;
			*g = v;
			*b = p;
			break;
		case 2:
			*r = p;
			*g = v;
			*b = t;
			break;
		case 3:
			*r = p;
			*g = q;
			*b = v;
			break;
		case 4:
			*r = t;
			*g = p;
			*b = v;
			break;
		default:		// case 5:
			*r = v;
			*g = p;
			*b = q;
			break;
		}
	}

	static AntRuleset createRuleFromSequence(const char* pattern, float hue = 0, float sat = 0, float val = 0) {
		if (strlen(pattern) == 0)
			return ant_rules["RL"];
		if (hue == 0 && sat == 0 && val == 0) {
			hue = (double(std::rand()) / double(RAND_MAX))*360;
			sat = (double(std::rand()) / double(RAND_MAX)) * 0.75 + 0.125;
			val = (double(std::rand()) / double(RAND_MAX)) * 0.75 + 0.125;
		}
		AntRuleset rules = AntRuleset();
		glm::ivec3 nextcol = glm::ivec3(255);
		for (unsigned int i = 0; i < strlen(pattern); i++) {
			char s = pattern[i];
			glm::ivec3 n = glm::ivec3(0);
			if (i != 1) {
				float r, g, b;
				float ho = (std::rand() % 50), so = (std::rand() % 50) / 100.0, vo = (std::rand() % 50) / 100.0;
				HSVtoRGB(&r, &g, &b, hue + ho, sat + so, val + vo);
				n.r = r * 255;
				n.g = g * 255;
				n.b = b * 255;
			}
			AntRule rule = AntRule(s == 'R' ? 1 : s == 'F' || s == 'B' ? 0 : -1, s == 'B' ? -1 : 1, 0, i == strlen(pattern) - 1 ? glm::ivec3(255) : n, s == 'b', s == 'd');
			rules.add(nextcol, rule);
			nextcol = n;
		}
		return rules;
	}
	static void init_rules() {
		AntRuleset RL = AntRuleset(AntRule(1, 1, 0, glm::ivec3(0)));
		RL.add(glm::ivec3(0), AntRule(-1, 1, 0, glm::ivec3(255)));
		ant_rules.insert(std::make_pair("RL", RL));
		AntRuleset RLR = AntRuleset(AntRule(1, 1, 0, glm::ivec3(255, 127, 0)));
		RLR.add(glm::ivec3(255, 127, 0), AntRule(-1, 1, 0, glm::ivec3(0)));
		RLR.add(glm::ivec3(0), AntRule(1, 1, 0, glm::ivec3(255)));
		ant_rules.insert(std::make_pair("RLR", RLR));
		AntRuleset LLRR = AntRuleset(AntRule(-1, 1, 0, glm::ivec3(255, 0, 255)));
		LLRR.add(glm::ivec3(255, 0, 255), AntRule(-1, 1, 0, glm::ivec3(0, 0, 255)));
		LLRR.add(glm::ivec3(0, 0, 255), AntRule(1, 1, 0, glm::ivec3(0)));
		LLRR.add(glm::ivec3(0), AntRule(1, 1, 0, glm::ivec3(255)));
		ant_rules.insert(std::make_pair("LLRR", LLRR));

		AntRuleset RLd = AntRuleset(AntRule(1, 1, 0, glm::ivec3(0)));
		RLd.add(glm::ivec3(0), AntRule(-1, 1, 0, glm::ivec3(0, 125, 255)));
		RLd.add(glm::ivec3(0, 125, 255), AntRule(0, 1, 0, glm::ivec3(255), false, true));
		ant_rules.insert(std::make_pair("RLd", RLd));

		ant_rules.insert(std::make_pair("minion", createRuleFromSequence("RLRLFd")));

		AntRuleset RLbd = AntRuleset(AntRule(1, 1, 0, glm::ivec3(0)));
		RLbd.add(glm::ivec3(0), AntRule(-1, 1, 0, glm::ivec3(255, 125, 0)));
		RLbd.add(glm::ivec3(255, 125, 0), AntRule(0, 1, 0, glm::ivec3(255, 0, 0), true, false, "RLd"));
		RLbd.add(glm::ivec3(255, 0, 0), AntRule(0, 1, 0, glm::ivec3(255), false, true));
		//RLbd.add(glm::ivec3(0, 125, 255), AntRule(0, 1, 0, glm::ivec3(255), false, true));
		ant_rules.insert(std::make_pair("RLbd", RLbd));

		AntRuleset Queen = AntRuleset(AntRule(1, 1, 0, glm::vec3(0, 0, 0), true, false, "minion"));
		Queen.add(glm::ivec3(255, 0, 0), AntRule(0, 0, 0, glm::ivec3(255, 0, 0), true, false, "minion"));
		Queen.add(glm::ivec3(0), AntRule(-1, 1, 0, glm::ivec3(255, 0, 0), true, false, "minion"));
		ant_rules.insert(std::make_pair("queen", Queen));
	}
private:
	std::map<glm::ivec3, AntRule, vec3_comp> rules = std::map<glm::ivec3, AntRule, vec3_comp>();
};
#endif