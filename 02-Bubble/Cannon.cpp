#include "Cannon.h"
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

enum CannonAnims
{
	CLOSED, OPENING_FIRST, OPENING_SECOND, OPENING_LAST, AIM_UP_LEFT_CLOSE, AIM_UP_LEFT_FAR, AIM_LEFT, ANIM_DYING, ANIM_DEAD
};



Cannon::Cannon()
{
}


void Cannon::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	type = "cannon";
	state = ALIVE;
	life = 10;
	frameCount = 30;
	lastKeyframe = 0;
	spritesheet.loadFromFile("images/cannon.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1f, 0.1f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(100);

	sprite->setAnimationSpeed(CLOSED, 8);
	sprite->addKeyframe(CLOSED, glm::vec2(0.0f, 0.0f));

	sprite->setAnimationSpeed(OPENING_FIRST, 8);
	sprite->addKeyframe(OPENING_FIRST, glm::vec2(0.1f, 0.0f));

	sprite->setAnimationSpeed(OPENING_SECOND, 8);
	sprite->addKeyframe(OPENING_SECOND, glm::vec2(0.2f, 0.0f));

	sprite->setAnimationSpeed(OPENING_LAST, 8);
	sprite->addKeyframe(OPENING_LAST, glm::vec2(0.3f, 0.0f));

	sprite->setAnimationSpeed(AIM_UP_LEFT_FAR, 6);
	sprite->addKeyframe(AIM_UP_LEFT_FAR, glm::vec2(0.0f, 0.1f));
	sprite->addKeyframe(AIM_UP_LEFT_FAR, glm::vec2(0.1f, 0.1f));
	sprite->addKeyframe(AIM_UP_LEFT_FAR, glm::vec2(0.2f, 0.1f));

	sprite->setAnimationSpeed(AIM_UP_LEFT_CLOSE, 6);
	sprite->addKeyframe(AIM_UP_LEFT_CLOSE, glm::vec2(0.0f, 0.2f));
	sprite->addKeyframe(AIM_UP_LEFT_CLOSE, glm::vec2(0.1f, 0.2f));
	sprite->addKeyframe(AIM_UP_LEFT_CLOSE, glm::vec2(0.2f, 0.2f));

	sprite->setAnimationSpeed(AIM_LEFT, 6);
	sprite->addKeyframe(AIM_LEFT, glm::vec2(0.0f, 0.3f));
	sprite->addKeyframe(AIM_LEFT, glm::vec2(0.1f, 0.3f));
	sprite->addKeyframe(AIM_LEFT, glm::vec2(0.2f, 0.3f));

	sprite->setAnimationSpeed(ANIM_DYING, 6);
	sprite->addKeyframe(ANIM_DYING, glm::vec2(0.0f, 0.4f));
	sprite->addKeyframe(ANIM_DYING, glm::vec2(0.1f, 0.4f));
	sprite->addKeyframe(ANIM_DYING, glm::vec2(0.2f, 0.4f));
	sprite->addKeyframe(ANIM_DYING, glm::vec2(0.3f, 0.4f));

	sprite->setAnimationSpeed(ANIM_DEAD, 1);
	sprite->addKeyframe(ANIM_DEAD, glm::vec2(0.9f, 0.8f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Cannon::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (state == ALIVE) {
		if (frameCount <= 30 && frameCount > 20) {
			sprite->changeAnimation(OPENING_FIRST);
		}
		else if (frameCount < 20 && frameCount >= 10) {
			sprite->changeAnimation(OPENING_SECOND);
		}
		else if (frameCount > 0) {
			sprite->changeAnimation(OPENING_LAST);
		}
		else if (frameCount < 1) {
			if (projDir == glm::vec2{ -1, 0 } && sprite->animation() != AIM_LEFT) sprite->changeAnimation(AIM_LEFT);
			else if (projDir == glm::vec2{ -0.75,-0.25 } && sprite->animation() != AIM_UP_LEFT_FAR) sprite->changeAnimation(AIM_UP_LEFT_FAR);
			else if (projDir == glm::vec2{ -0.25,-0.75 } && sprite->animation() != AIM_UP_LEFT_CLOSE) sprite->changeAnimation(AIM_UP_LEFT_CLOSE);
		}

		if (frameCount > 0) --frameCount;
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

vector<glm::ivec2> Cannon::buildHitBox()
{
	glm::ivec2 lpos1 = posEnemy;
	glm::ivec2 rpos1 = lpos1 + glm::ivec2{ 32,32 };
	return vector<glm::ivec2> {lpos1, rpos1 };
}

glm::ivec2 Cannon::getProjectileSpawn()
{
	return glm::ivec2{ 0,0 };
}