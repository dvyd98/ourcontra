#include "ProjectileBoss.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


enum upgradeRank
{
	RIFLEMAN, RANK1, SPREAD
};


void ProjectileBoss::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, float speed, const glm::vec2 projDir)
{
	rank = RANK1;
	damage = 1;
	peak = false;
	loop = 0;
	spritesheet.loadFromFile("images/projectile.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.1f, 0.1f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(100);

	sprite->setAnimationSpeed(RIFLEMAN, 8);
	sprite->addKeyframe(RIFLEMAN, glm::vec2(0.1f, 0.0f));

	sprite->setAnimationSpeed(RANK1, 8);
	sprite->addKeyframe(RANK1, glm::vec2(0.0f, 0.0f));

	sprite->setAnimationSpeed(SPREAD, 8);
	sprite->addKeyframe(SPREAD, glm::vec2(0.0f, 0.2f));

	sprite->changeAnimation(2);
	tileMapDispl = tileMapPos;
	newPos = projDir * speed;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posProjectile.x), float(tileMapDispl.y + posProjectile.y)));

}

void ProjectileBoss::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (loop > 15) peak = true;
	if (!peak) {
		posProjectile += glm::vec2(-2, -1);
		loop += 1;
	}
	else
		posProjectile += glm::vec2(-1, 2);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posProjectile.x), float(tileMapDispl.y + posProjectile.y)));
}

void ProjectileBoss::render()
{
	sprite->render();
}

void ProjectileBoss::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void ProjectileBoss::setPosition(const glm::vec2 &pos)
{
	posProjectile = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posProjectile.x), float(tileMapDispl.y + posProjectile.y)));
}

glm::ivec2 ProjectileBoss::getPos()
{
	return posProjectile;
}

int ProjectileBoss::getRank()
{
	return rank;
}

int ProjectileBoss::getDmg()
{
	return damage;
}

vector<glm::ivec2> ProjectileBoss::buildHitBox()
{
	glm::ivec2 lpos1 = posProjectile + glm::vec2{ 5,5 };
	glm::ivec2 rpos1 = lpos1 + glm::ivec2{ 8,8 };
	return vector<glm::ivec2> {lpos1, rpos1 };
}
