#ifndef _ENEMYMANAGER_INCLUDE
#define _ENEMYMANAGER_INCLUDE

#include "Soldier.h"
#include "Rifleman.h"
#include "Player.h"
#include "Bridge.h"
#include "WallTurret.h"
#include "BossTurret.h"
#include "Cannon.h"
#include "UpgradeBox.h"
#include "GunUpgrade.h"
#include "BossCore.h"
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
	void initLvl2(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap, Player *p1
	);
	void update(int deltaTime, float left, float right, float bottom, float top);
	void updateLvl2(int deltaTime, float left, float right, float bottom, float top);
	void render();
	void renderLvl2();
	bool isOffScreenY(Enemy &pj);
	bool isOffScreen(glm::ivec2 pj);
	void spawnProjectilePlayer(glm::ivec2 position);
	void spawnProjectileSPREADPlayer(glm::ivec2 position);
	void spawnProjectileRifleman(glm::ivec2 position, Rifleman* badguy);
	void spawnProjectileWallTurret(glm::ivec2 position, WallTurret* badguy);
	void spawnProjectileBossTurret(glm::ivec2 position, BossTurret* badguy);
	void spawnProjectileCannon(glm::ivec2 position, Cannon* badguy);
	void despawnOffScreenProjectiles();
	void despawnOffScreenEnemies();
	void didthePlayerFuckingFall();
	void despawnDeadEnemies();
	bool areTouching(glm::ivec2 lpos1, glm::ivec2 rpos1, glm::ivec2 lpos2, glm::ivec2 rpos2);
	bool areTouchingYcoord(glm::ivec2 lpos1, glm::ivec2 rpos1, glm::ivec2 lpos2, glm::ivec2 rpos2);
	void checkPhysics();
	void setSublvl(int x) { sublvl = x; }
	int getSublvl() { return sublvl; }

	list<Enemy*> *enemies;
	TileMap *map;
	Projectile *projectile;
	ProjectileBoss *projectileBoss;
	Player* player;
	list<Projectile> *projlist;
	list<Projectile> *projlistRifleman;
	list<Projectile> *projlistWallTurret;
	list<ProjectileBoss> *projlistBossTurret;
	list<Projectile> *projlistCannon;
	glm::ivec2 tilemap;
	ShaderProgram texProgram;

private:
	float left, right, bottom, top;
	int sublvl;
};

#endif