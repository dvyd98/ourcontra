#include "BossTurret.h"
#include "Game.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>

enum States
{
	ALIVE, DEAD, DYING
};

enum BossTurretAnims
{
	IDLE, SHOOTING
};



BossTurret::BossTurret()
{
}


void BossTurret::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	type = "BossTurret";
	state = ALIVE;
	life = 10;
	frameCount = 60;
	spritesheet.loadFromFile("images/BossTurret.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(16*3, 16), glm::vec2(0.5f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(100);

	sprite->setAnimationSpeed(IDLE, 8);
	sprite->addKeyframe(IDLE, glm::vec2(0.0f, 0.0f));

	sprite->setAnimationSpeed(SHOOTING, 2);
	sprite->addKeyframe(SHOOTING, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(SHOOTING, glm::vec2(0.5f, 0.0f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void BossTurret::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (frameCount < 30) {
		if (sprite->animation() != SHOOTING)
			sprite->changeAnimation(SHOOTING);
	}
	if (frameCount > 0) --frameCount;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

vector<glm::ivec2> BossTurret::buildHitBox()
{
	glm::ivec2 lpos1 = posEnemy;
	glm::ivec2 rpos1 = lpos1 + glm::ivec2{ 16*3,32 };
	return vector<glm::ivec2> {lpos1, rpos1 };
}

glm::ivec2 BossTurret::getProjectileSpawn()
{
	return glm::ivec2{ 0,0 };
}