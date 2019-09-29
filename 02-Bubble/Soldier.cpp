#include "Soldier.h"
#include "Game.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>



Soldier::Soldier()
{
}


Soldier::~Soldier()
{
}

void Soldier::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
}

void Soldier::update(int deltaTime, float left)
{
}

void Soldier::render()
{
	sprite->render();
}

void Soldier::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Soldier::setPosition(const glm::vec2 &pos)
{
	posEnemy = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}