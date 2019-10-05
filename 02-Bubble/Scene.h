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

enum sceneState {
	MENU, HOW_TO, PAUSE, CREDITS, LVL1, LVL2
};

class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();
	bool isOffScreen(Projectile &pj);
	bool isOffScreen(Enemy &pj);
	void spawnProjectile(glm::ivec2 position);
	void despawnOffScreenProjectiles();
	void despawnOffScreenEnemies();
	bool areTouching(glm::ivec2 lpos1, glm::ivec2 rpos1, glm::ivec2 lpos2, glm::ivec2 rpos2);
	void checkPhysics();

private:
	void initShaders();
	void initEntities();
	void updateMenu(int deltaTime);
	void updateLvl1(int deltaTime);
	void godMode();

private:
	TileMap *map;
	Player *player;
	EnemyManager *enemymanager;
	Projectile *projectile;
	list<Projectile> *projlist;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	float left, right, bottom, top;

	int currentState;
};


#endif // _SCENE_INCLUDE

