#ifndef _ENEMYMANAGER_INCLUDE
#define _ENEMYMANAGER_INCLUDE

#include "Soldier.h"
#include "Rifleman.h"
#include "Player.h"
#include "Bridge.h"
#include "WallTurret.h"
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
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap, Player *p1
		);
	void update(int deltaTime, float left, float right, float bottom, float top);
	void render();
	bool isOffScreen(Enemy &pj);
	bool isOffScreen(Projectile &pj);
	void spawnProjectilePlayer(glm::ivec2 position);
	void spawnProjectileRifleman(glm::ivec2 position, Rifleman* badguy);
	void spawnProjectileWallTurret(glm::ivec2 position, WallTurret* badguy);
	void despawnOffScreenProjectiles();
	void despawnOffScreenEnemies();
	bool areTouching(glm::ivec2 lpos1, glm::ivec2 rpos1, glm::ivec2 lpos2, glm::ivec2 rpos2);
	bool areTouchingYcoord(glm::ivec2 lpos1, glm::ivec2 rpos1, glm::ivec2 lpos2, glm::ivec2 rpos2);
	vector<glm::ivec2> buildHitBox(Enemy &badguy);
	void checkPhysics();

	list<Enemy*> *enemies;
	TileMap *map;
	Projectile *projectile;
	Player* player;
	list<Projectile> *projlist;
	list<Projectile> *projlistRifleman;
	list<Projectile> *projlistWallTurret;
	glm::ivec2 tilemap;
	ShaderProgram texProgram;

private:
	float left, right, bottom, top;
};

#endif