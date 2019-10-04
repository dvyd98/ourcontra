#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include <list>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Soldier.h"
#include "Projectile.h"
#include "EnemyManager.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();
	bool Scene::isOffScreen(Projectile &pj);
	bool Scene::isOffScreen(Enemy &pj);
	void spawnProjectile(glm::ivec2 position);
	void Scene::despawnOffScreenProjectiles();
	void Scene::despawnOffScreenEnemies();
	bool Scene::areTouching(glm::ivec2 lpos1, glm::ivec2 rpos1, glm::ivec2 lpos2, glm::ivec2 rpos2);
	void Scene::checkPhysics();

private:
	void initShaders();

private:
	TileMap *map;
	Player *player;
	EnemyManager *enemymanager;
	list<Enemy*> *enemies;
	Projectile *projectile;
	list<Projectile> *projlist;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	float left, right, bottom, top;
};


#endif // _SCENE_INCLUDE

