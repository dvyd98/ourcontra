#include "ProjectileBoss2.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

enum States
{
	ALIVE, DEAD, DYING
};

enum upgradeRank
{
	RIFLEMAN, RANK1, SPREAD, ANIM_DYING, ANIM_DEAD
};


void ProjectileBoss2::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, float speed, const glm::vec2 projDir)
{
	state = ALIVE;
	rank = RANK1;
	damage = 1;
	spritesheet.loadFromFile("images/projectileBoss2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1f, 0.1f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(100);

	sprite->setAnimationSpeed(RIFLEMAN, 8);
	sprite->addKeyframe(RIFLEMAN, glm::vec2(0.1f, 0.0f));

	sprite->setAnimationSpeed(RANK1, 8);
	sprite->addKeyframe(RANK1, glm::vec2(0.0f, 0.0f));

	sprite->setAnimationSpeed(SPREAD, 6);
	sprite->addKeyframe(SPREAD, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(SPREAD, glm::vec2(0.1f, 0.0f));
	sprite->addKeyframe(SPREAD, glm::vec2(0.2f, 0.0f));
	sprite->addKeyframe(SPREAD, glm::vec2(0.3f, 0.0f));

	sprite->setAnimationSpeed(ANIM_DYING, 6);
	sprite->addKeyframe(ANIM_DYING, glm::vec2(0.0f, 0.1f));
	sprite->addKeyframe(ANIM_DYING, glm::vec2(0.1f, 0.1f));
	sprite->addKeyframe(ANIM_DYING, glm::vec2(0.2f, 0.1f));
	sprite->addKeyframe(ANIM_DYING, glm::vec2(0.3f, 0.1f));

	sprite->setAnimationSpeed(ANIM_DEAD, 6);
	sprite->addKeyframe(ANIM_DEAD, glm::vec2(0.9f, 0.9f));

	sprite->changeAnimation(2);
	tileMapDispl = tileMapPos;
	newPos = projDir * speed;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posProjectile.x), float(tileMapDispl.y + posProjectile.y)));

}

void ProjectileBoss2::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (state == ALIVE) {
		posProjectile += newPos;
	}
	else if (state == DYING) {
		if (sprite->animation() != ANIM_DYING && sprite->animation() != ANIM_DEAD) sprite->changeAnimation(ANIM_DYING);
		else if (lastKeyframe != 0 && sprite->keyframe() == 0) {
			state = DEAD;
			sprite->changeAnimation(ANIM_DEAD);
		}
		lastKeyframe = sprite->keyframe();
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posProjectile.x), float(tileMapDispl.y + posProjectile.y)));
}

void ProjectileBoss2::render()
{
	sprite->render();
}

void ProjectileBoss2::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void ProjectileBoss2::setPosition(const glm::vec2 &pos)
{
	posProjectile = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posProjectile.x), float(tileMapDispl.y + posProjectile.y)));
}

glm::ivec2 ProjectileBoss2::getPos()
{
	return posProjectile;
}

int ProjectileBoss2::getRank()
{
	return rank;
}

int ProjectileBoss2::getDmg()
{
	return damage;
}

vector<glm::ivec2> ProjectileBoss2::buildHitBox()
{
	glm::ivec2 lpos1 = posProjectile;
	glm::ivec2 rpos1 = lpos1 + glm::ivec2{ 30,30 };
	return vector<glm::ivec2> {lpos1, rpos1 };
}
