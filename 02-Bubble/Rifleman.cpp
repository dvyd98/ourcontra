#include "Rifleman.h"
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

enum State
{
	ALIVE, DEAD, DYING
};

enum LookingTo
{
	LOOKING_LEFT, LOOKING_RIGHT
};

enum RiflemanAnims
{
	AIM_LEFT, AIM_RIGHT, AIM_UP_LOOK_LEFT, AIM_UP_LOOK_RIGHT, AIM_DOWN_LOOK_LEFT, AIM_DOWN_LOOK_RIGHT
};


Rifleman::Rifleman()
{
}

void Rifleman::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	type = "rifleman";
	state = ALIVE;
	life = 1;
	lookingTo = LOOKING_RIGHT;
	spritesheet.loadFromFile("images/rifleman.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(32, 40), glm::vec2(0.1f, 0.20f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(100);

	sprite->setAnimationSpeed(AIM_LEFT, 8);
	sprite->addKeyframe(AIM_LEFT, glm::vec2(0.0f, 0.20f));
	sprite->addKeyframe(AIM_LEFT, glm::vec2(0.1f, 0.20f));

	sprite->setAnimationSpeed(AIM_RIGHT, 8);
	sprite->addKeyframe(AIM_RIGHT, glm::vec2(0.2f, 0.20f));
	sprite->addKeyframe(AIM_RIGHT, glm::vec2(0.3f, 0.20f));

	sprite->setAnimationSpeed(AIM_UP_LOOK_LEFT, 8);
	sprite->addKeyframe(AIM_UP_LOOK_LEFT, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(AIM_UP_LOOK_LEFT, glm::vec2(0.1f, 0.0f));

	sprite->setAnimationSpeed(AIM_UP_LOOK_RIGHT, 8);
	sprite->addKeyframe(AIM_UP_LOOK_RIGHT, glm::vec2(0.2f, 0.0f));
	sprite->addKeyframe(AIM_UP_LOOK_RIGHT, glm::vec2(0.3f, 0.0f));

	sprite->setAnimationSpeed(AIM_DOWN_LOOK_LEFT, 8);
	sprite->addKeyframe(AIM_DOWN_LOOK_LEFT, glm::vec2(0.0f, 0.40f));

	sprite->setAnimationSpeed(AIM_DOWN_LOOK_RIGHT, 8);
	sprite->addKeyframe(AIM_DOWN_LOOK_RIGHT, glm::vec2(0.1f, 0.40f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Rifleman::update(int deltaTime)
{
	sprite->update(deltaTime);

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

vector<glm::ivec2> Rifleman::buildHitBox()
{
	glm::ivec2 lpos1 = posEnemy + glm::ivec2{ 8,8 };
	glm::ivec2 rpos1 = lpos1 + glm::ivec2{ 16,24 };
	return vector<glm::ivec2> {lpos1, rpos1 };
}
