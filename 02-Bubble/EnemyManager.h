#ifndef _ENEMYMANAGER_INCLUDE
#define _ENEMYMANAGER_INCLUDE

#include "Soldier.h"
#include "Player.h"
#include "ShaderProgram.h"
#include "TileMap.h"
#include <glm/glm.hpp>
#include <list>
#include <GL/glew.h>
#include <GL/glut.h>

class EnemyManager
{
public:
	EnemyManager();
	~EnemyManager();
	void EnemyManager::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap
		);
	void EnemyManager::update(int deltaTime, float left, float right, float bottom, float top);
	void EnemyManager::render();
	bool EnemyManager::isOffScreen(Enemy &pj);

	list<Enemy*> *enemies;
	TileMap *map;

private:
	float left, right, bottom, top;
};

#endif