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

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, MOVE_LEFT_AIM, MOVE_RIGHT_AIM, AIM_UP_LOOK_LEFT, AIM_UP_LOOK_RIGHT, CROUCH_LOOK_LEFT,
	CROUCH_LOOK_RIGHT, AIM_UP_WALK_RIGHT, AIM_UP_WALK_LEFT, AIM_DOWN_WALK_RIGHT, AIM_DOWN_WALK_LEFT, AIRBONE_LEFT, AIRBONE_RIGHT
};


void Projectile::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, int dir, const glm::ivec2 projDir)
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

	sprite->setAnimationSpeed(RANK1, 8);
	sprite->addKeyframe(RANK1, glm::vec2(0.0f, 0.0f));

	sprite->setAnimationSpeed(RANK2, 8);
	sprite->addKeyframe(RANK2, glm::vec2(0.0f, 0.1f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	newPos = projDir,
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
	switch (rank)
	{
	case RANK1: {
		glm::ivec2 lpos1 = posProjectile + glm::ivec2{ 8,9 };
		glm::ivec2 rpos1 = lpos1 + glm::ivec2{ 2,2 };
		return vector<glm::ivec2> {lpos1, rpos1 };
	}


	}
}
