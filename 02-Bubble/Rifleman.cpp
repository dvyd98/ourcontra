#include "Rifleman.h"
#include "Game.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>


enum States
{
	ALIVE, DEAD, DYING
};

enum LookingTo
{
	LOOKING_LEFT, LOOKING_RIGHT
};

enum RiflemanAnims
{
	AIM_LEFT, AIM_RIGHT, AIM_UP_LOOK_LEFT, AIM_UP_LOOK_RIGHT, AIM_DOWN_LOOK_LEFT, AIM_DOWN_LOOK_RIGHT, ANIM_DYING, ANIM_DEAD
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
	lastKeyframe = 0;
	shotCd = 5;
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

	sprite->setAnimationSpeed(ANIM_DYING, 4);
	sprite->addKeyframe(ANIM_DYING, glm::vec2(0.0f, 0.6f));
	sprite->addKeyframe(ANIM_DYING, glm::vec2(0.1f, 0.6f));
	sprite->addKeyframe(ANIM_DYING, glm::vec2(0.2f, 0.6f));

	sprite->setAnimationSpeed(ANIM_DEAD, 1);
	sprite->addKeyframe(ANIM_DEAD, glm::vec2(0.9f, 0.8f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Rifleman::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (state == ALIVE) {
		if (sprite->animation() != AIM_LEFT && projDir == glm::ivec2{ -1,0 }) sprite->changeAnimation(AIM_LEFT);
		else if (sprite->animation() != AIM_RIGHT && projDir == glm::ivec2{ 1,0 }) sprite->changeAnimation(AIM_RIGHT);
		else if (sprite->animation() != AIM_UP_LOOK_LEFT && projDir == glm::ivec2{ -1,-1 }) sprite->changeAnimation(AIM_UP_LOOK_LEFT);
		else if (sprite->animation() != AIM_UP_LOOK_RIGHT && projDir == glm::ivec2{ 1,-1 }) sprite->changeAnimation(AIM_UP_LOOK_RIGHT);
		else if (projDir == glm::ivec2{ -1,1 }) sprite->changeAnimation(AIM_DOWN_LOOK_LEFT);
		else if (projDir == glm::ivec2{ 1,1 }) sprite->changeAnimation(AIM_DOWN_LOOK_RIGHT);
		if (shotCd > 0 && hasShot) --shotCd;
		else if (shotCd == 0) {
			shotCd = 5;
			hasShot = false;
		}
	}
	else if (state == DYING) {
		if (sprite->animation() != ANIM_DYING) sprite->changeAnimation(ANIM_DYING);
		else if (lastKeyframe != 0 && sprite->keyframe() == 0) {
			state = DEAD;
			sprite->changeAnimation(ANIM_DEAD);
		}
		lastKeyframe = sprite->keyframe();
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

vector<glm::ivec2> Rifleman::buildHitBox()
{
	glm::ivec2 lpos1 = posEnemy + glm::ivec2{ 8,8 };
	glm::ivec2 rpos1 = lpos1 + glm::ivec2{ 16,24 };
	return vector<glm::ivec2> {lpos1, rpos1 };
}

glm::ivec2 Rifleman::getProjectileSpawn()
{
	if (sprite->animation() == AIM_LEFT)
		return glm::ivec2{ -4, 5 };
	else if (sprite->animation() == AIM_RIGHT)
		return glm::ivec2{ 18, 5 };
	else if (sprite->animation() == AIM_UP_LOOK_LEFT)
		return glm::ivec2{ 5, 0 };
	else if (sprite->animation() == AIM_UP_LOOK_RIGHT)
		return glm::ivec2{ 10, 0 };
	else if (sprite->animation() == AIM_DOWN_LOOK_LEFT)
		return glm::ivec2{ -4, 16 };
	else if (sprite->animation() == AIM_DOWN_LOOK_RIGHT)
		return glm::ivec2{ 20, 16 };
}