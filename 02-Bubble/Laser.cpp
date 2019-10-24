#include "Laser.h"
#include "Game.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>

enum States
{
	ALIVE, DEAD, DYING
};

enum LaserAnims
{
	IDLE, ANIM_DEAD
};



Laser::Laser()
{
}


void Laser::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	type = "laser";
	state = ALIVE;
	life = 30;
	frameCount = 150;
	isOpen = false;
	spritesheet.loadFromFile("images/Laser.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(184, 6), glm::vec2(0.1f, 0.1f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(100);

	sprite->setAnimationSpeed(IDLE, 10);
	sprite->addKeyframe(IDLE, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(IDLE, glm::vec2(0.0f, 0.1f));

	sprite->setAnimationSpeed(ANIM_DEAD, 1);
	sprite->addKeyframe(ANIM_DEAD, glm::vec2(0.9f, 0.9f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Laser::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (state == ALIVE) {
		if (sprite->animation() != IDLE) sprite->changeAnimation(IDLE);
	}
	else if (state == DYING) {
		state = DEAD;
		sprite->changeAnimation(ANIM_DEAD);
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

vector<glm::ivec2> Laser::buildHitBox()
{
	glm::ivec2 lpos1 = posEnemy + glm::ivec2{ 8,8 };;
	glm::ivec2 rpos1 = lpos1 + glm::ivec2{ 16,16 };
	return vector<glm::ivec2> {lpos1, rpos1 };
}

glm::ivec2 Laser::getProjectileSpawn()
{
	return glm::ivec2{ 0,0 };
}