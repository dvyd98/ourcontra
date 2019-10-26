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
	IDLE, SHOOTING, ANIM_DYING, ANIM_DEAD
};



BossTurret::BossTurret()
{
}


void BossTurret::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	type = "bossturret";
	state = ALIVE;
	life = 30;
	frameCount = 60;
	projCd = 30;
	left = true;
	hasShot = false;
	spritesheet.loadFromFile("images/BossTurret.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(48, 32), glm::vec2(0.1f, 0.1f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(100);

	sprite->setAnimationSpeed(IDLE, 8);
	sprite->addKeyframe(IDLE, glm::vec2(0.0f, 0.0f));

	sprite->setAnimationSpeed(SHOOTING, 2);
	sprite->addKeyframe(SHOOTING, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(SHOOTING, glm::vec2(0.1f, 0.0f));

	sprite->setAnimationSpeed(ANIM_DYING, 6);
	sprite->addKeyframe(ANIM_DYING, glm::vec2(0.0f, 0.2f));
	sprite->addKeyframe(ANIM_DYING, glm::vec2(0.1f, 0.2f));
	sprite->addKeyframe(ANIM_DYING, glm::vec2(0.2f, 0.2f));
	sprite->addKeyframe(ANIM_DYING, glm::vec2(0.3f, 0.2f));

	sprite->setAnimationSpeed(ANIM_DEAD, 1);
	sprite->addKeyframe(ANIM_DEAD, glm::vec2(0.0f, 0.1f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void BossTurret::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (state == ALIVE) {
		if (frameCount < 30) {
			if (sprite->animation() != SHOOTING)
				sprite->changeAnimation(SHOOTING);
		}
		if (frameCount > 0) --frameCount;
		if (hasShot) {
			if (projCd > 0) --projCd;
			else {
				projCd = 30;
				hasShot = false;
			}
		}
	}
	else if (state == DYING) {
		if (sprite->animation() != ANIM_DYING && sprite->animation() != ANIM_DEAD) sprite->changeAnimation(ANIM_DYING);
		else if (lastKeyframe != 0 && sprite->keyframe() == 0) {
			sprite->changeAnimation(ANIM_DEAD);
		}
		lastKeyframe = sprite->keyframe();
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

vector<glm::ivec2> BossTurret::buildHitBox()
{
	glm::ivec2 lpos1 = posEnemy;
	glm::ivec2 rpos1 = lpos1 + glm::ivec2{ 48,16 };
	return vector<glm::ivec2> {lpos1, rpos1 };
}

glm::ivec2 BossTurret::getProjectileSpawn()
{
	if (left) {
		left = false;
		return glm::ivec2{ -10,-10 };
	}
	else {
		left = true;
		return glm::ivec2{ 10,-10 };
	}
}