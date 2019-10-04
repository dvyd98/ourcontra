#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include <list>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Soldier.h"
#include "Projectile.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.

enum sceneState {
	MENU, HOW_TO, PAUSE, CREDITS, LVL1, LVL2
};

class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void changeToScene(sceneState state);
	void update(int deltaTime);
	void render();
	bool Scene::isOffScreen(Projectile &pj);
	bool Scene::isOffScreen(Enemy &pj);
	void spawnProjectile(glm::ivec2 position);
	void Scene::despawnOffScreenProjectiles();
	void Scene::despawnOffScreenEnemies();

private:
	void initShaders();
	void initEntities();

private:
	TileMap *map;
	Player *player;
	Enemy *soldier;
	list<Enemy*> *enemies;
	Projectile *projectile;
	list<Projectile> *projlist;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	float left, right, bottom, top;
};


#endif // _SCENE_INCLUDE

