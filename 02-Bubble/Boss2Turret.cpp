#include "Boss2Turret.h"
#include "Game.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>

enum States
{
	ALIVE, DEAD, DYING
};

enum Boss2TurretAnims
{
	CLOSED, OPENING, OPEN ,ANIM_DYING, ANIM_DEAD
};



Boss2Turret::Boss2Turret()
{
}


void Boss2Turret::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	type = "boss2turret";
	state = ALIVE;
	life = 30;
	frameCount = 180;
	openTime = 90;
	isOpen = false;
	hasShot = false;
	spritesheet.loadFromFile("images/Boss2Turret.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1f, 0.1f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(100);

	sprite->setAnimationSpeed(CLOSED, 6);
	sprite->addKeyframe(CLOSED, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(CLOSED, glm::vec2(0.1f, 0.0f));
	sprite->addKeyframe(CLOSED, glm::vec2(0.2f, 0.0f));

	sprite->setAnimationSpeed(OPENING, 1);
	sprite->addKeyframe(OPENING, glm::vec2(0.0f, 0.2f));

	sprite->setAnimationSpeed(OPEN, 6);
	sprite->addKeyframe(OPEN, glm::vec2(0.0f, 0.1f));
	sprite->addKeyframe(OPEN, glm::vec2(0.1f, 0.1f));
	sprite->addKeyframe(OPEN, glm::vec2(0.2f, 0.1f));

	sprite->setAnimationSpeed(ANIM_DYING, 6);
	sprite->addKeyframe(ANIM_DYING, glm::vec2(0.0f, 0.4f));
	sprite->addKeyframe(ANIM_DYING, glm::vec2(0.1f, 0.4f));
	sprite->addKeyframe(ANIM_DYING, glm::vec2(0.2f, 0.4f));
	sprite->addKeyframe(ANIM_DYING, glm::vec2(0.3f, 0.4f));

	sprite->setAnimationSpeed(ANIM_DEAD, 1);
	sprite->addKeyframe(ANIM_DEAD, glm::vec2(0.0f, 0.3f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Boss2Turret::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (state == ALIVE) {
		if (!isOpen) {
			openTime = 90;
			if (frameCount < 10 && frameCount > 0) {
				if (sprite->animation() != OPENING)
					sprite->changeAnimation(OPENING);
			}
			else if (frameCount < 1) {
				isOpen = true;
				if (sprite->animation() != OPEN)
					sprite->changeAnimation(OPEN);
			}
			if (frameCount > 0) --frameCount;
		}
		else {
			frameCount = 180;
			if (openTime < 10 && openTime > 0) {
				if (sprite->animation() != OPENING)
					sprite->changeAnimation(OPENING);
			}
			else if (openTime < 1) {
				hasShot = false;
				isOpen = false;
				if (sprite->animation() != CLOSED)
					sprite->changeAnimation(CLOSED);
			}
			if (openTime > 0) --openTime;
		}
	}
	else if (state == DYING) {
		if (sprite->animation() != ANIM_DYING && sprite->animation() != ANIM_DEAD) sprite->changeAnimation(ANIM_DYING);
		else if (lastKeyframe != 0 && sprite->keyframe() == 0) {
			//state = DEAD;
			sprite->changeAnimation(ANIM_DEAD);
		}
		lastKeyframe = sprite->keyframe();
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

vector<glm::ivec2> Boss2Turret::buildHitBox()
{
	glm::ivec2 lpos1 = posEnemy;
	glm::ivec2 rpos1 = lpos1 + glm::ivec2{ 32,32 };
	return vector<glm::ivec2> {lpos1, rpos1 };
}

glm::ivec2 Boss2Turret::getProjectileSpawn()
{
	return glm::ivec2{ 0,0 };
}