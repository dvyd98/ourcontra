#include "Projectile.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


enum upgradeRank
{
	RIFLEMAN, RANK1, RANK2, RANK3, FLAMETHROWER
};


void Projectile::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, float speed, const glm::vec2 projDir)
{
	rank = RANK1;
	damage = 1;
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

	sprite->setAnimationSpeed(RANK2, 8);
	sprite->addKeyframe(RANK2, glm::vec2(0.0f, 0.1f));

	sprite->changeAnimation(1);
	tileMapDispl = tileMapPos;
	newPos = projDir * speed;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posProjectile.x), float(tileMapDispl.y + posProjectile.y)));

}

void Projectile::update(int deltaTime)
{
	sprite->update(deltaTime);
	posProjectile += newPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posProjectile.x), float(tileMapDispl.y + posProjectile.y)));
}

void Projectile::render()
{
	sprite->render();
}

void Projectile::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Projectile::setPosition(const glm::vec2 &pos)
{
	posProjectile = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posProjectile.x), float(tileMapDispl.y + posProjectile.y)));
}

glm::ivec2 Projectile::getPos()
{
	return posProjectile;
}

int Projectile::getRank()
{
	return rank;
}

int Projectile::getDmg()
{
	return damage;
}

vector<glm::ivec2> Projectile::buildHitBox()
{
	glm::ivec2 lpos1 = posProjectile + glm::vec2{ 8,9 };
	glm::ivec2 rpos1 = lpos1 + glm::ivec2{ 2,2 };
	return vector<glm::ivec2> {lpos1, rpos1 };
}
