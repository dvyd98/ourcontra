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


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();
	void spawnProjectile(glm::ivec2 position);

private:
	void initShaders();

private:
	TileMap *map;
	Player *player;
	Enemy *soldier;
	Projectile *projectile;
	list<Projectile> *projlist;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	float left, right, bottom, top;
};


#endif // _SCENE_INCLUDE

