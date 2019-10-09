#include "Enemy.h"
#include "Game.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>

enum State
{
	ALIVE, DEAD
};

Enemy::Enemy() 
{

}

void Enemy::render()
{
	sprite->render();
}

void Enemy::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Enemy::setPosition(const glm::vec2 &pos)
{
	posEnemy = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

glm::ivec2 Enemy::getPos()
{
	return posEnemy;
}

bool Enemy::decreaseLife(int dmg)
{
	life -= dmg;
	return (life > 0 ? false : true);
}