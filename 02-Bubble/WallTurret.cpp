#include "WallTurret.h"
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

enum WallTurretAnims
{
	CLOSED, OPENING, AIM_DOWN_LEFT_FAR, AIM_DOWN_LEFT_CLOSE, AIM_DOWN, AIM_DOWN_RIGHT_CLOSE, AIM_DOWN_RIGHT_FAR, AIM_RIGHT, AIM_UP_RIGHT_FAR, AIM_UP_RIGHT_CLOSE, AIM_UP,
	AIM_UP_LEFT_CLOSE, AIM_UP_LEFT_FAR, AIM_LEFT
};



WallTurret::WallTurret()
{
}


void WallTurret::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	type = "wallturret";
	state = ALIVE;
	life = 10;
	frameCount = 60;
	spritesheet.loadFromFile("images/wallturret.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1f, 0.1f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(100);

	sprite->setAnimationSpeed(CLOSED, 8);
	sprite->addKeyframe(CLOSED, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(CLOSED, glm::vec2(0.1f, 0.0f));
	sprite->addKeyframe(CLOSED, glm::vec2(0.2f, 0.0f));

	sprite->setAnimationSpeed(OPENING, 8);
	sprite->addKeyframe(OPENING, glm::vec2(0.0f, 0.1f));

	sprite->setAnimationSpeed(AIM_DOWN_LEFT_FAR, 8);
	sprite->addKeyframe(AIM_DOWN_LEFT_FAR, glm::vec2(0.0f, 0.2f));

	sprite->setAnimationSpeed(AIM_DOWN_LEFT_CLOSE, 8);
	sprite->addKeyframe(AIM_DOWN_LEFT_CLOSE, glm::vec2(0.1f, 0.2f));

	sprite->setAnimationSpeed(AIM_DOWN, 8);
	sprite->addKeyframe(AIM_DOWN, glm::vec2(0.2f, 0.2f));

	sprite->setAnimationSpeed(AIM_DOWN_RIGHT_CLOSE, 8);
	sprite->addKeyframe(AIM_DOWN_RIGHT_CLOSE, glm::vec2(0.3f, 0.2f));

	sprite->setAnimationSpeed(AIM_DOWN_RIGHT_FAR, 8);
	sprite->addKeyframe(AIM_DOWN_RIGHT_FAR, glm::vec2(0.4f, 0.2f));

	sprite->setAnimationSpeed(AIM_UP_LEFT_FAR, 8);
	sprite->addKeyframe(AIM_UP_LEFT_FAR, glm::vec2(0.5f, 0.2f));

	sprite->setAnimationSpeed(AIM_UP_LEFT_CLOSE, 8);
	sprite->addKeyframe(AIM_UP_LEFT_CLOSE, glm::vec2(0.6f, 0.2f));

	sprite->setAnimationSpeed(AIM_UP, 8);
	sprite->addKeyframe(AIM_UP, glm::vec2(0.7f, 0.2f));

	sprite->setAnimationSpeed(AIM_UP_RIGHT_CLOSE, 8);
	sprite->addKeyframe(AIM_UP_RIGHT_CLOSE, glm::vec2(0.8f, 0.2f));

	sprite->setAnimationSpeed(AIM_UP_RIGHT_FAR, 8);
	sprite->addKeyframe(AIM_UP_RIGHT_FAR, glm::vec2(0.9f, 0.2f));

	sprite->setAnimationSpeed(AIM_RIGHT, 8);
	sprite->addKeyframe(AIM_RIGHT, glm::vec2(0.0f, 0.3f));

	sprite->setAnimationSpeed(AIM_LEFT, 8);
	sprite->addKeyframe(AIM_LEFT, glm::vec2(0.1f, 0.3f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void WallTurret::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (frameCount < 30 && frameCount > 0 ) {
		sprite->changeAnimation(OPENING);
	}
	else if (frameCount < 1) {
		sprite->changeAnimation(AIM_LEFT);
	}
	if (frameCount > 0) --frameCount;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

vector<glm::ivec2> WallTurret::buildHitBox()
{
	glm::ivec2 lpos1 = posEnemy;
	glm::ivec2 rpos1 = lpos1 + glm::ivec2{ 32,32 };
	return vector<glm::ivec2> {lpos1, rpos1 };
}

glm::ivec2 WallTurret::getProjectileSpawn()
{
	return glm::ivec2{ 0,0 };
}