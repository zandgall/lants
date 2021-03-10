#include "World.h"
#include "Neutron.h"
#include "AntRules.h"
#include "Quark.h"
#include "ParagraphQuark.h"
using namespace neutron;

std::map<glm::i32, std::map<glm::i32, Chunk>> World::chunks = std::map<glm::i32, std::map<glm::i32, Chunk>>();
std::vector<Ant> World::ants = std::vector<Ant>();

ParagraphQuark antsteps = ParagraphQuark();

float zoom = 0.1, xOff = 0, yOff = 0, xVel = 0, yVel = 0, speed = 0.1;
void fw() { yVel += speed; }
void bw() { yVel -= speed; }
void rw() { xVel += speed; }
void lw() { xVel -= speed; }
World::World() {
	if (neutron::isSet()) {
		frame::keys::whenKeyPressedDo(GLFW_KEY_W, &fw);
		frame::keys::whenKeyPressedDo(GLFW_KEY_S, &bw);
		frame::keys::whenKeyPressedDo(GLFW_KEY_D, &rw);
		frame::keys::whenKeyPressedDo(GLFW_KEY_A, &lw);
		world::camera.Zoom = 90;
		AntRuleset::init_rules();
		AntRuleset::ant_rules.insert(std::make_pair("square", AntRuleset::createRuleFromSequence("LRRRRRLLR")));
		AntRuleset::ant_rules.insert(std::make_pair("convolution", AntRuleset::createRuleFromSequence("LLRRRLRLRLLR")));
		AntRuleset::ant_rules.insert(std::make_pair("triangle", AntRuleset::createRuleFromSequence("RRLLLRLLLRRR")));
		AntRuleset::ant_rules.insert(std::make_pair("test", AntRuleset::createRuleFromSequence("FFFFFFFRL")));
		ants.push_back(Ant(glm::i64vec2(0), "queen"));
		chart(glm::i32vec2(0));
		chart(glm::i32vec2(-1, 0));
		chart(glm::i32vec2(-1));
		chart(glm::i32vec2(0, -1));
		antsteps = ParagraphQuark(glm::vec3(400, 700, 0), "0", 200, 24.0f, 0);
		antsteps.setB(Quark::GUI, true);
		antsteps.setF4(Quark::COLOR, glm::vec4(1, 0, 0, 1));
	}
}

void World::tick() {
	zoom += (pow(2, zoom)-1)*(frame::mouse::y_s_g*0.05);
	zoom += zoom == 0 * 0.05;
	world::camera.MovementSpeed = 0.1;
	speed = 0.005/zoom;
	xOff += xVel;
	yOff += yVel;
	xVel *= 0.8;
	yVel *= 0.8;
	tile.x = floor(((frame::mouse::x - frame::width / 2.0) / (frame::width / 2.0)) / zoom + xOff);
	tile.y = floor(((frame::height / 2.0 - frame::mouse::y) / (frame::height / 2.0)) / zoom + yOff);

	if (frame::mouse::r || frame::mouse::mClicked)
		for(unsigned int j = 0; j < pow(10, frame::keys::keys[340] + frame::keys::keys[341] + frame::keys::keys[342]); j++)
			for (unsigned int i = 0; i < ants.size(); i++) {
				ants[i].tick(1);
				if (ants[i].dead)
					ants.erase(ants.begin() + i);
			}
	if (frame::mouse::lReleased)
		ants.push_back(Ant(tile, "RL"));
	//antsteps.setS(Quark::TEXT, std::to_string(ants[0].steps));
}

void World::render_gui() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, frame::width, frame::height);
	modes::gui = false;
	passes::globalTransform = processor::scaled(glm::vec3(zoom));
	glm::i32vec2 start = glm::i32vec2(-1 / zoom + xOff, -1 / zoom + yOff);
	glm::i32vec2 endp = glm::i32vec2(1 / zoom + xOff, 1 / zoom + yOff);
	
	for (auto i = chunks.begin(), end = chunks.end(); i != end; i++) {
		for (auto j = i->second.begin(), jend = i->second.end(); j != jend; j++) {
			if (i->first > endp.x/Chunk::width || i->first < start.x / Chunk::width - 1 || j->first > endp.y / Chunk::height || j->first < start.y / Chunk::height - 1)
				continue;
			passes::transform = processor::rect(i->first*Chunk::width-xOff, j->first*Chunk::height-yOff, Chunk::width, Chunk::height);
			draw(j->second.texture);
		}
	}

	for (unsigned int i = 0, size = ants.size(); i < size; i++) {
		passes::transform = processor::rect(ants[i].pos.x-xOff + (1.0/6.0), ants[i].pos.y-yOff + (1.0/6.0), 2.0/3.0, 2.0/3.0);
		draw(0, glm::vec4(1, 0, 0, 1));
	}

	passes::shader = processor::LoadShader("res/shaders/buttonOutline.shader", true);
	passes::cust_floats["rounding"] = 0.2;
	passes::cust_floats["bevel"] = 1;
	passes::cust_float4s["bounds"] = glm::vec4(2.0/3.0);
	passes::transform = processor::rect(tile.x-xOff + (1.0 / 6.0), tile.x-yOff+ (1.0 / 6.0), 2.0 / 3.0, 2.0 / 3.0);
	draw(0, glm::vec4(1, 1, 0, 1));

	//passes::globalTransform = processor::scaled(glm::vec3(1));
	//antsteps.render();
}