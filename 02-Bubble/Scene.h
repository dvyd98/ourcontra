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
	LOADING_MENU, MENU, MENU_TO_LVL1, HOW_TO, PAUSE, CREDITS, LVL1, LVL2
};

class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();
	void initEntitiesLvl1();
	void initEntitiesLvl2();
	void updateMenu(int deltaTime);
	void updateLvl1(int deltaTime);
	void updateLvl2(int deltaTime);
	void changeToScene(int scene);
	void godMode();

private:
	TileMap *map;
	Player *player;
	EnemyManager *enemymanager;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	float left, right, bottom, top;

	int currentState;

	int selectDelay;
	bool onePlayer;

	int blinks, blinkAnimation;

};


#endif // _SCENE_INCLUDE

