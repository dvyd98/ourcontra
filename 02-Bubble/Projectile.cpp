#include "Projectile.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


enum upgradeRank
{
	RANK1, RANK2, RANK3, FLAMETHROWER
};


void Projectile::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/projectile.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.1f, 0.1f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(100);

	sprite->setAnimationSpeed(RANK1, 8);
	sprite->addKeyframe(RANK1, glm::vec2(0.1f, 0.f));

	sprite->setAnimationSpeed(RANK2, 8);
	sprite->addKeyframe(RANK2, glm::vec2(0.0f, 0.0f));

	sprite->changeAnimation(1);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posProjectile.x), float(tileMapDispl.y + posProjectile.y)));
}

void Projectile::update(int deltaTime, float left)
{
	sprite->update(deltaTime);
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
