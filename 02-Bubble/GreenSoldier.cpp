#include "GreenSoldier.h"
#include "Game.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>

enum States
{
	ALIVE, DEAD, DYING
};

enum GreenSoldierAnims
{
	IDLE, MOVE_LEFT, MOVE_RIGHT, ANIM_DYING, ANIM_DEAD
};



GreenSoldier::GreenSoldier()
{
}


void GreenSoldier::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	type = "greensoldier";
	state = ALIVE;
	life = 1;
	frameCount = 0;
	projectileCd =100;
	moveCd = 30;
	move = false;
	hurt = false;
	steps = 67;
	hasShot = false;
	spritesheet.loadFromFile("images/GreenSoldier.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(24, 24), glm::vec2(0.1f, 0.1f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(100);

	sprite->setAnimationSpeed(IDLE, 2);
	sprite->addKeyframe(IDLE, glm::vec2(0.0f, 0.2f));
	sprite->addKeyframe(IDLE, glm::vec2(0.0f, 0.3f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.1f, 0.0f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2f, 0.0f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f, 0.1f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1f, 0.1f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.2f, 0.1f));

	sprite->setAnimationSpeed(ANIM_DYING, 4);
	sprite->addKeyframe(ANIM_DYING, glm::vec2(0.0f, 0.4f));
	sprite->addKeyframe(ANIM_DYING, glm::vec2(0.1f, 0.4f));
	sprite->addKeyframe(ANIM_DYING, glm::vec2(0.2f, 0.4f));

	sprite->setAnimationSpeed(ANIM_DEAD, 1);
	sprite->addKeyframe(ANIM_DEAD, glm::vec2(0.9f, 0.9f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void GreenSoldier::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (state == ALIVE) {
		if (!move) {
			sprite->changeAnimation(IDLE);
			if (moveCd > 0) --moveCd;
			else {
				moveCd = 30;
				move = true;
			}
		}
		if (hasShot) {
			if (projectileCd > 0) --projectileCd;
			if (projectileCd == 0) {
				projectileCd = 100;
				hasShot = false;
			}
		}
		if (move) {
			--steps;
			if (left) {
				posEnemy.x += 1;
				if (sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
			}
			else {
				posEnemy.x -= 1;
				if (sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);
			}
		}
		if (steps == 0) state = DEAD;
		
	}
	else if (state == DYING) {
		if (sprite->animation() != ANIM_DYING && sprite->animation() != ANIM_DEAD) sprite->changeAnimation(ANIM_DYING);
		else if (lastKeyframe != 0 && sprite->keyframe() == 0) {
			state = DEAD;
			sprite->changeAnimation(ANIM_DEAD);
		}
		lastKeyframe = sprite->keyframe();
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

vector<glm::ivec2> GreenSoldier::buildHitBox()
{
	glm::ivec2 lpos1 = posEnemy + glm::ivec2{ 4,0 };
	glm::ivec2 rpos1 = lpos1 + glm::ivec2{ 14,24 };
	return vector<glm::ivec2> {lpos1, rpos1 };
}

glm::ivec2 GreenSoldier::getProjectileSpawn()
{
	return glm::ivec2{ 0,0 };
}