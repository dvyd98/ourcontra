#include "Boss2Core.h"
#include "Game.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>

enum States
{
	ALIVE, DEAD, DYING
};

enum Boss2CoreAnims
{
	IDLE, ANIM_DYING, ANIM_DEAD
};



Boss2Core::Boss2Core()
{
}


void Boss2Core::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	type = "boss2core";
	state = ALIVE;
	life = 10;
	frameCount = 180;
	spritesheet.loadFromFile("images/Boss2Core.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1f, 0.1f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(100);

	sprite->setAnimationSpeed(IDLE, 6);
	sprite->addKeyframe(IDLE, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(IDLE, glm::vec2(0.1f, 0.0f));
	sprite->addKeyframe(IDLE, glm::vec2(0.2f, 0.0f));

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

void Boss2Core::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (state == ALIVE) {
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

vector<glm::ivec2> Boss2Core::buildHitBox()
{
	glm::ivec2 lpos1 = posEnemy;
	glm::ivec2 rpos1 = lpos1 + glm::ivec2{ 32,32 };
	return vector<glm::ivec2> {lpos1, rpos1 };
}

glm::ivec2 Boss2Core::getProjectileSpawn()
{
	return glm::ivec2{ 0,0 };
}