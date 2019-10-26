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
#include "Boss2Turret.h"
#include "Boss2Core.h"
#include "Boss2Final.h"
#include "Level2Core.h"
#include "Level2Turret.h"
#include "GreenSoldier.h"
#include "Laser.h"
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Audio.h"
#include <glm/glm.hpp>
#include <list>
#include <GL/glew.h>
#include <GL/glut.h>

class EnemyManager
{
public:
	EnemyManager(Audio *audiomanger);
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
	bool isOffScreenLevel2(glm::ivec2 pj);
	bool isOffScreenLevel2Enemy(glm::ivec2 pj);
	void spawnProjectilePlayer(glm::ivec2 position);
	void spawnProjectilePlayerLVL2(glm::ivec2 position);
	void spawnProjectileSPREADPlayer(glm::ivec2 position);
	void spawnProjectileRifleman(glm::ivec2 position, Rifleman* badguy);
	void spawnProjectileWallTurret(glm::ivec2 position, WallTurret* badguy);
	void spawnProjectileLevel2Turret(glm::ivec2 positionPlayer, Level2Turret* badguy);
	void spawnProjectileBossTurret(glm::ivec2 position, BossTurret* badguy);
	void spawnProjectileBoss2Turret(glm::ivec2 positionPlayer, Boss2Turret* badguy);
	void spawnProjectileBoss2Final(glm::ivec2 positionPlayer, Boss2Final* badguy);
	void spawnProjectileGreenSoldier(glm::ivec2 position, GreenSoldier* badguy);
	void spawnProjectileCannon(glm::ivec2 position, Cannon* badguy);
	void updatePlayerState();
	void spawnGreenSoldiers();
	void spawnLaser();
	void despawnOffScreenProjectiles();
	void despawnOffScreenEnemies();
	void didthePlayerFuckingFall();
	void despawnDeadEnemies();
	bool areTouching(glm::ivec2 lpos1, glm::ivec2 rpos1, glm::ivec2 lpos2, glm::ivec2 rpos2);
	bool areTouchingYcoord(glm::ivec2 lpos1, glm::ivec2 rpos1, glm::ivec2 lpos2, glm::ivec2 rpos2);
	void checkPhysics();
	void checkPhysicsLevel2();
	void setSublvl(int x) { sublvl = x; }
	int getSublvl() { return sublvl; }

	bool coreDestroyed;
	list<Enemy*> *enemies;
	TileMap *map;
	Projectile *projectile;
	ProjectileBoss *projectileBoss;
	ProjectileBoss2 *projectileBoss2;
	Player* player;
	list<Projectile> *projlist;
	list<Projectile> *projlistRifleman;
	list<Projectile> *projlistWallTurret;
	list<ProjectileBoss> *projlistBossTurret;
	list<Projectile> *projlistBoss2Turret;
	list<Projectile> *projlistLevel2Turret;
	list<ProjectileBoss2> *projlistBoss2Final;
	list<Projectile> *projlistGreenSoldier;
	list<Projectile> *projlistCannon;
	glm::ivec2 tilemap;
	ShaderProgram texProgram;

private:
	float left, right, bottom, top;
	int sublvl;
	int nDestroyed;
	int soldierCd;
	int isLaserSpawned;
	Audio *audiomanager;

	bool keypressed;
	bool keyreleased;
	bool playerShot;

	void playDeathSound(string type);
};

#endif