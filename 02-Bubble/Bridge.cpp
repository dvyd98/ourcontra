#include "Bridge.h"
#include "Game.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define GLUT_KEY_SPACEBAR 32
#define PLAYER_VEL 2

enum States
{
	ALIVE, DEAD, DYING
};

enum BridgeAnims
{
	EDGE_LEFT, EDGE_RIGHT, DESTROYED_LEFT, DESTROYED_RIGHT, CENTRAL
};



Bridge::Bridge()
{
}

void Bridge::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	type = "bridge";
	state = ALIVE;
	life = 1;
	spritesheet.loadFromFile("images/bridge.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1f, 0.1f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(100);

	sprite->setAnimationSpeed(EDGE_LEFT, 8);
	sprite->addKeyframe(EDGE_LEFT, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(EDGE_LEFT, glm::vec2(0.1f, 0.0f));
	sprite->addKeyframe(EDGE_LEFT, glm::vec2(0.2f, 0.0f));

	sprite->setAnimationSpeed(EDGE_RIGHT, 8);
	sprite->addKeyframe(EDGE_RIGHT, glm::vec2(0.4f, 0.0f));
	sprite->addKeyframe(EDGE_RIGHT, glm::vec2(0.5f, 0.0f));
	sprite->addKeyframe(EDGE_RIGHT, glm::vec2(0.6f, 0.0f));

	sprite->setAnimationSpeed(CENTRAL, 8);
	sprite->addKeyframe(CENTRAL, glm::vec2(0.0f, 0.1f));
	sprite->addKeyframe(CENTRAL, glm::vec2(0.1f, 0.1f));
	sprite->addKeyframe(CENTRAL, glm::vec2(0.2f, 0.1f));

	sprite->setAnimationSpeed(DESTROYED_LEFT, 8);
	sprite->addKeyframe(DESTROYED_LEFT, glm::vec2(0.0f, 0.3f));

	sprite->setAnimationSpeed(DESTROYED_RIGHT, 8);
	sprite->addKeyframe(DESTROYED_RIGHT, glm::vec2(0.1f, 0.2f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Bridge::update(int deltaTime)
{
	sprite->update(deltaTime);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

vector<glm::ivec2> Bridge::buildHitBox()
{
	glm::ivec2 lpos1 = posEnemy;
	glm::ivec2 rpos1 = lpos1 + glm::ivec2{ 32,32 };
	return vector<glm::ivec2> {lpos1, rpos1 };
}