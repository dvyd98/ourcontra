#include "EnemyManager.h"



EnemyManager::EnemyManager()
{
}


EnemyManager::~EnemyManager()
{
}

void EnemyManager::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap
	)
{
	map = tileMap;
	int n = map->getNumEnemies();
	enemies = new list<Enemy*>();
	for (int i = 0; i < n; ++i) {
		switch (map->getEnemy(i).type)
		{
		case SOLDIER: {
			Enemy *aux = new Soldier();
			aux->init(tileMapPos, shaderProgram);
			aux->setPosition(glm::vec2(map->getEnemy(i).x * map->getTileSize(), map->getEnemy(i).y * map->getTileSize()));
			aux->setTileMap(map);
			enemies->push_back(aux);
		}
		}
	}
}

void EnemyManager::update(int deltaTime, float leftt, float rightt, float bottomm, float topp)
{
	left = leftt;
	right = rightt;
	bottom = bottomm;
	top = topp;
	list<Enemy*>::iterator it_enemy;
	for (it_enemy = enemies->begin(); it_enemy != enemies->end(); ++it_enemy) {
		(*it_enemy)->update(deltaTime);
	}
}

void EnemyManager::render()
{
	list<Enemy*>::iterator it_enemy;
	for (it_enemy = enemies->begin(); it_enemy != enemies->end(); ++it_enemy) {
		(*it_enemy)->render();
	}
}

bool EnemyManager::isOffScreen(Enemy &pj)
{
	if (pj.getPos().x < left || pj.getPos().x > right) return true;
	return false;
}