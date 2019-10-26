#include "EnemyManager.h"
#include "Game.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define GLUT_KEY_SPACEBAR 32

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, MOVE_LEFT_AIM, MOVE_RIGHT_AIM, AIM_UP_LOOK_LEFT, AIM_UP_LOOK_RIGHT, CROUCH_LOOK_LEFT,
	CROUCH_LOOK_RIGHT, AIM_UP_WALK_RIGHT, AIM_UP_WALK_LEFT, AIM_DOWN_WALK_RIGHT, AIM_DOWN_WALK_LEFT, AIRBONE_LEFT, AIRBONE_RIGHT,
	DROPPED, UNDERWATER, SWIM_LEFT, SWIM_RIGHT, SWIM_AIM_LEFT, SWIM_AIM_RIGHT, SWIM_AIM_UPRIGHT, SWIM_AIM_UPLEFT, SWIM_AIM_UP_LOOK_LEFT, SWIM_AIM_UP_LOOK_RIGHT,
	ANIM_DYING, ANIM_DEAD, LVL2_ANIM_DYING,
	LVL2_IDLE, LVL2_IDLE_SHOOT, LVL2_CROUCH, LVL2_CROUCH_SHOOT, LVL2_FORWARD, LVL2_MOVE_LEFT, LVL2_MOVE_RIGHT, LVL2_ZAP

};

enum BridgeAnims
{
	EDGE_LEFT, EDGE_RIGHT, DESTROYED_LEFT, DESTROYED_RIGHT, CENTRAL
};

enum States
{
	ALIVE, DEAD, DYING, SPECIAL_BRIDGE_STATE
};

enum upgradeRank
{
	RIFLEMAN, RANK1, SPREAD
};

EnemyManager::EnemyManager(Audio *_audiomanager)
{
	audiomanager = _audiomanager;
}


EnemyManager::~EnemyManager()
{
}

void EnemyManager::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap, Player *p1
	)
{
	texProgram = shaderProgram;
	tilemap = tileMapPos;
	map = tileMap;
	player = p1;
	p1->lvl = 1;
	keypressed = false;
	keyreleased = true;
	playerShot = false;
	int n = map->getNumEnemies();
	enemies = new list<Enemy*>();
	projlist = new list<Projectile>();
	projlistRifleman = new list<Projectile>();
	projlistWallTurret = new list<Projectile>();
	projlistBossTurret = new list<ProjectileBoss>();
	projlistCannon = new list<Projectile>();
	projlistBoss2Final = new list<ProjectileBoss2>();
	projlistLevel2Turret = new list<Projectile>();
	for (int i = 0; i < n; ++i) {
		int enemyType = map->getEnemy(i).type;
		if (enemyType == SOLDIER)
		{
			Enemy *aux = new Soldier();
			aux->init(tilemap, texProgram);
			aux->setPosition(glm::vec2(map->getEnemy(i).x * map->getTileSize(), map->getEnemy(i).y * map->getTileSize()));
			aux->setTileMap(map);
			enemies->push_back(aux);
		}
		else if (enemyType  == BRIDGE_EDGE_LEFT)
		{
			Bridge *bridgeboy = new Bridge();
			bridgeboy->init(tilemap, texProgram);
			bridgeboy->setPosition(glm::vec2(map->getEnemy(i).x * map->getTileSize(), map->getEnemy(i).y * map->getTileSize()));
			bridgeboy->setTileMap(map);
			bridgeboy->bridgeType = "left";
			bridgeboy->sprite->changeAnimation(EDGE_LEFT);
			enemies->push_back(bridgeboy);
		}
		else if (enemyType == BRIDGE_EDGE_RIGHT)
		{
			Bridge *bridgeboy = new Bridge();
			bridgeboy->init(tilemap, texProgram);
			bridgeboy->setPosition(glm::vec2(map->getEnemy(i).x * map->getTileSize(), map->getEnemy(i).y * map->getTileSize()));
			bridgeboy->setTileMap(map);
			bridgeboy->bridgeType = "right";
			bridgeboy->sprite->changeAnimation(EDGE_RIGHT);
			enemies->push_back(bridgeboy);
		}
		else if (enemyType == BRIDGE_CENTRAL)
		{
			Bridge *bridgeboy = new Bridge();
			bridgeboy->init(tilemap, texProgram);
			bridgeboy->setPosition(glm::vec2(map->getEnemy(i).x * map->getTileSize(), map->getEnemy(i).y * map->getTileSize()));
			bridgeboy->setTileMap(map);
			bridgeboy->bridgeType = "central";
			bridgeboy->sprite->changeAnimation(CENTRAL);
			enemies->push_back(bridgeboy);
		}
		else if (enemyType == RIFLEMAN_NORMAL)
		{
			Enemy *aux = new Rifleman();
			aux->init(tilemap, texProgram);
			aux->setPosition(glm::vec2(map->getEnemy(i).x * map->getTileSize(), map->getEnemy(i).y * map->getTileSize()));
			aux->setTileMap(map);
			enemies->push_back(aux);
		}
		else if (enemyType == WALLTURRET) {
			Enemy *aux = new WallTurret();
			aux->init(tilemap, texProgram);
			aux->setPosition(glm::vec2(map->getEnemy(i).x * map->getTileSize(), map->getEnemy(i).y * map->getTileSize()));
			aux->setTileMap(map);
			enemies->push_back(aux);
		}
		else if (enemyType == BOSS_TURRET) {
			Enemy *aux = new BossTurret();
			aux->init(tilemap, texProgram);
			aux->setPosition(glm::vec2(map->getEnemy(i).x * map->getTileSize(), map->getEnemy(i).y * map->getTileSize()));
			aux->setTileMap(map);
			enemies->push_back(aux);
		}
		else if (enemyType == BOSS_CORE) {
			Enemy *aux = new BossCore();
			aux->init(tilemap, texProgram);
			aux->setPosition(glm::vec2(map->getEnemy(i).x * map->getTileSize(), map->getEnemy(i).y * map->getTileSize()));
			aux->setTileMap(map);
			enemies->push_back(aux);
		}
		else if (enemyType == CANNON) {
			Enemy *aux = new Cannon();
			aux->init(tilemap, texProgram);
			aux->setPosition(glm::vec2(map->getEnemy(i).x * map->getTileSize(), map->getEnemy(i).y * map->getTileSize()));
			aux->setTileMap(map);
			enemies->push_back(aux);
		}
		else if (enemyType == UPGRADE_BOX) {
			Enemy *aux = new UpgradeBox();
			aux->init(tilemap, texProgram);
			aux->setPosition(glm::vec2(map->getEnemy(i).x * map->getTileSize(), map->getEnemy(i).y * map->getTileSize()));
			aux->setTileMap(map);
			enemies->push_back(aux);
		}
	}


}

void EnemyManager::initLvl2(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *tileMap, Player *p1
)
{
	texProgram = shaderProgram;
	tilemap = tileMapPos;
	map = tileMap;
	player = p1;
	p1->sprite->changeAnimation(LVL2_IDLE);
	p1->lvl = 2;
	sublvl = 0;
	nDestroyed = 0;
	soldierCd = 45;
	coreDestroyed = false;
	player->coreDestroyed = false;
	isLaserSpawned = -1;
	int n = map->getNumEnemies();
	enemies = new list<Enemy*>();
	projlist = new list<Projectile>();
	projlistWallTurret = new list<Projectile>();
	projlistBoss2Turret = new list<Projectile>();
	projlistBoss2Final = new list<ProjectileBoss2>();
	projlistLevel2Turret = new list<Projectile>();
	projlistGreenSoldier = new list<Projectile>();
	for (int i = 0; i < n; ++i) {
		int enemyType = map->getEnemy(i).type;
		if (enemyType == WALLTURRET) {
			Enemy *aux = new WallTurret();
			aux->init(tilemap, texProgram);
			aux->setPosition(glm::vec2(map->getEnemy(i).x * 16, map->getEnemy(i).y *16));
			aux->setTileMap(map);
			aux->sublvl = (map->getEnemy(i).sublvl);
			enemies->push_back(aux);
		}
		else if (enemyType == LEVEL2_CORE) {
			Enemy *aux = new Level2Core();
			aux->init(tilemap, texProgram);
			aux->setTileMap(map);
			aux->sublvl = (map->getEnemy(i).sublvl);
			if (aux->sublvl == 0) {
				aux->setPosition(glm::vec2(map->getEnemy(i).x * 16, map->getEnemy(i).y * 16 -8));
			}
			else if (aux->sublvl == 1) {
				aux->setPosition(glm::vec2(map->getEnemy(i).x * 16 + 2, map->getEnemy(i).y * 16 - 8));
			}
			else if (aux->sublvl == 2) {
				aux->setPosition(glm::vec2(map->getEnemy(i).x * 16, map->getEnemy(i).y * 16));
			}
			else if (aux->sublvl == 3) {
				aux->setPosition(glm::vec2(map->getEnemy(i).x * 16, map->getEnemy(i).y * 16 - 6));
			}
			else if (aux->sublvl == 4) {
				aux->setPosition(glm::vec2(map->getEnemy(i).x * 16, map->getEnemy(i).y * 16 + 4));
			}
			enemies->push_back(aux);
		}
		else if (enemyType == LEVEL2_TURRET) {
			Enemy *aux = new Level2Turret();
			aux->init(tilemap, texProgram);
			aux->setTileMap(map);
			aux->sublvl = (map->getEnemy(i).sublvl);
			if (aux->sublvl == 1) {
				aux->setPosition(glm::vec2(map->getEnemy(i).x * 16 - 1, map->getEnemy(i).y * 16 - 8));
			}
			else if (aux->sublvl == 2) {
				aux->setPosition(glm::vec2(map->getEnemy(i).x * 16, map->getEnemy(i).y * 16 - 8));
			}
			else if (aux->sublvl == 3) {
				aux->setPosition(glm::vec2(map->getEnemy(i).x * 16, map->getEnemy(i).y * 16 - 6));
			}
			else if (aux->sublvl == 4) {
				aux->setPosition(glm::vec2(map->getEnemy(i).x * 16, map->getEnemy(i).y * 16));
			}
			enemies->push_back(aux);
		}
		else if (enemyType == LEVEL2_BOSSCORE) {
			Enemy *aux = new Boss2Core();
			aux->init(tilemap, texProgram);
			aux->setTileMap(map);
			aux->sublvl = (map->getEnemy(i).sublvl);
			if (aux->sublvl == 5) {
				aux->setPosition(glm::vec2(map->getEnemy(i).x * 16, map->getEnemy(i).y * 16));
			}
			enemies->push_back(aux);
		}
		else if (enemyType == LEVEL2_BOSSTURRET) {
			Enemy *aux = new Boss2Turret();
			aux->init(tilemap, texProgram);
			aux->setTileMap(map);
			aux->sublvl = (map->getEnemy(i).sublvl);
			if (aux->sublvl == 5) {
				aux->setPosition(glm::vec2(map->getEnemy(i).x * 16, map->getEnemy(i).y * 16));
			}
			enemies->push_back(aux);
		}
		else if (enemyType == LEVEL2_BOSSFINAL) {
			Enemy *aux = new Boss2Final();
			aux->init(tilemap, texProgram);
			aux->setTileMap(map);
			aux->sublvl = (map->getEnemy(i).sublvl);
			if (aux->sublvl == 5) {
				aux->setPosition(glm::vec2(map->getEnemy(i).x * 16, map->getEnemy(i).y * 16));
			}
			enemies->push_back(aux);
		}
	}
}

void EnemyManager::update(int deltaTime, float leftt, float rightt, float bottomm, float topp)
{
	left = leftt;
	right = rightt;
	bottom = bottomm;
	top = topp;

	despawnOffScreenEnemies();
	despawnDeadEnemies();
	didthePlayerFuckingFall();
	checkPhysics(); // coctel

	list<Enemy*>::iterator it_enemy;
	for (it_enemy = enemies->begin(); it_enemy != enemies->end(); ++it_enemy) {
		vector <glm::ivec2> box = (*it_enemy)->buildHitBox();
		if (!isOffScreen(box[0]) || !isOffScreen(box[1])) {
			if ((*it_enemy)->state == ALIVE && (*it_enemy)->getType() == "rifleman" && projlistRifleman->size() < 4 && (rand() % 2)) {
				Rifleman* rifleguy = dynamic_cast<Rifleman*>(*it_enemy);
				spawnProjectileRifleman(player->getPos(), rifleguy);
			}
			else if ((*it_enemy)->state == ALIVE && (*it_enemy)->getType() == "wallturret" && projlistWallTurret->size() < 1 && (*it_enemy)->frameCount < 1 && (rand() % 2)) {
				WallTurret* turretboi = dynamic_cast<WallTurret*>(*it_enemy);
				spawnProjectileWallTurret(player->getPos(), turretboi);
			}
			else if ((*it_enemy)->state == ALIVE && (*it_enemy)->getType() == "cannon" && projlistCannon->size() < 3 && (*it_enemy)->frameCount < 1 && (rand() % 2)) {
				Cannon* turretboi = dynamic_cast<Cannon*>(*it_enemy);
				spawnProjectileCannon(player->getPos(), turretboi);
			}
			else if ((*it_enemy)->state == ALIVE && (*it_enemy)->getType() == "bossturret" && projlistBossTurret->size() < 1 && (*it_enemy)->frameCount < 1) {
				BossTurret* turretboi = dynamic_cast<BossTurret*>(*it_enemy);
				spawnProjectileBossTurret(player->getPos(), turretboi);
			}
			(*it_enemy)->update(deltaTime);
		}
	}
	if (Game::instance().getKey('a')) {
		keypressed = true;
		keyreleased = false;
	}
	else {
		keypressed = false;
		keyreleased = true;
		playerShot = false;
	}
	if (keypressed && !playerShot && player->state == ALIVE) {
		if (player->projectile == RANK1) {
			if (projlist->size() < 4) {
				playerShot = true;
				spawnProjectilePlayer(player->getPos());
				audiomanager->play(NORMAL_GUN_SOUND, false);
			}
		}
		else if (player->projectile == SPREAD) {
			if (projlist->size() < 10) {
				playerShot = true;
				spawnProjectileSPREADPlayer(player->getPos());
				audiomanager->play(SPREAD_GUN_SOUND, false);
			}
		}
	}
	list<Projectile>::iterator it;
	for (it = projlist->begin(); it != projlist->end(); ++it) {
		it->update(deltaTime);
	}
	for (it = projlistRifleman->begin(); it != projlistRifleman->end(); ++it) {
		it->update(deltaTime);
	}
	for (it = projlistWallTurret->begin(); it != projlistWallTurret->end(); ++it) {
		it->update(deltaTime);
	}



	list<ProjectileBoss>::iterator it_boss;
	for (it_boss = projlistBossTurret->begin(); it_boss != projlistBossTurret->end(); ++it_boss) {
		it_boss->update(deltaTime);
	}
	for (it = projlistCannon->begin(); it != projlistCannon->end(); ++it) {
		it->update(deltaTime);
	}
}

void EnemyManager::updateLvl2(int deltaTime, float leftt, float rightt, float bottomm, float topp)
{
	left = leftt;
	right = rightt;
	bottom = bottomm;
	top = topp;

	if (soldierCd > 0) --soldierCd;
	else if (sublvl < 5) {
		spawnGreenSoldiers();
		soldierCd = 60;
	}
	if (sublvl < 5) {
		if (isLaserSpawned != sublvl) {
			isLaserSpawned = sublvl;
			spawnLaser();
		}
	}
	if (sublvl == 5) {
		player->lvl = 1;
		player->lvl2boss = true;
	}
	despawnDeadEnemies();
	checkPhysicsLevel2(); // coctel

	list<Enemy*>::iterator it_enemy;
	for (it_enemy = enemies->begin(); it_enemy != enemies->end(); ++it_enemy) {
		vector <glm::ivec2> box = (*it_enemy)->buildHitBox();
		if (!isOffScreen(box[0]) || !isOffScreen(box[1])) {
			if ((*it_enemy)->state == ALIVE && (*it_enemy)->getType() == "boss2turret" && (*it_enemy)->sublvl == sublvl) {
				Boss2Turret* turretboi = dynamic_cast<Boss2Turret*>(*it_enemy);
				spawnProjectileBoss2Turret(player->getPos(), turretboi);
			}
			else if ((*it_enemy)->state == ALIVE && (*it_enemy)->getType() == "level2turret" && (*it_enemy)->sublvl == sublvl) {
				Level2Turret* turretboi = dynamic_cast<Level2Turret*>(*it_enemy);
				spawnProjectileLevel2Turret(player->getPos(), turretboi);
			}
			else if ((*it_enemy)->state == ALIVE && (*it_enemy)->getType() == "boss2final" && nDestroyed >= 4) {
				Boss2Final* turretboi = dynamic_cast<Boss2Final*>(*it_enemy);
				spawnProjectileBoss2Final(player->getPos(), turretboi);
			}
			else if ((*it_enemy)->state == ALIVE && (*it_enemy)->getType() == "greensoldier" && (*it_enemy)->sublvl == sublvl) {
				GreenSoldier* turretboi = dynamic_cast<GreenSoldier*>(*it_enemy);
				spawnProjectileGreenSoldier(player->getPos(), turretboi);
			}
			if ((*it_enemy)->sublvl == sublvl) {
				if ((*it_enemy)->getType() == "boss2final") {
					if (nDestroyed >= 4) (*it_enemy)->update(deltaTime);
				}
				else (*it_enemy)->update(deltaTime);
			}
		}
	}
	if (Game::instance().getKey('a')) {
		keypressed = true;
		keyreleased = false;
	}
	else {
		keypressed = false;
		keyreleased = true;
		playerShot = false;
	}
	if (keypressed && !playerShot && player->state == ALIVE) {
		if (player->projectile == RANK1) {
			if (projlist->size() < 4) {
				playerShot = true;
				if (sublvl < 5) spawnProjectilePlayerLVL2(player->getPos());
				else spawnProjectilePlayer(player->getPos());
			}
		}
	}
	list<Projectile>::iterator it;
	for (it = projlist->begin(); it != projlist->end(); ++it) {
		it->update(deltaTime);
	}
	for (it = projlistBoss2Turret->begin(); it != projlistBoss2Turret->end(); ++it) {
		it->update(deltaTime);
	}
	for (it = projlistLevel2Turret->begin(); it != projlistLevel2Turret->end(); ++it) {
		it->update(deltaTime);
	}
	list<ProjectileBoss2>::iterator it_boss2;
	for (it_boss2 = projlistBoss2Final->begin(); it_boss2 != projlistBoss2Final->end(); ++it_boss2) {
		it_boss2->update(deltaTime);
	}
}


void EnemyManager::render()
{
	list<Enemy*>::iterator it_enemy;
	for (it_enemy = enemies->begin(); it_enemy != enemies->end(); ++it_enemy) {
		(*it_enemy)->render();
	}
	list<Projectile>::iterator it;
	for (it = projlist->begin(); it != projlist->end(); ++it) {
		it->render();
	}
	for (it = projlistRifleman->begin(); it != projlistRifleman->end(); ++it) {
		it->render();
	}
	for (it = projlistWallTurret->begin(); it != projlistWallTurret->end(); ++it) {
		it->render();
	}



	
	list<ProjectileBoss>::iterator it_boss;
	for (it_boss = projlistBossTurret->begin(); it_boss != projlistBossTurret->end(); ++it_boss) {
		it_boss->render();
	}
	for (it = projlistCannon->begin(); it != projlistCannon->end(); ++it) {
		it->render();
	}
}

void EnemyManager::renderLvl2()
{
	list<Enemy*>::iterator it_enemy;
	for (it_enemy = enemies->begin(); it_enemy != enemies->end(); ++it_enemy) {
		if ((*it_enemy)->getSublvl() == sublvl)
			if ((*it_enemy)->getType() == "boss2final") {
				if (nDestroyed >= 4) (*it_enemy)->render();
			}
			else (*it_enemy)->render();
	}
	list<Projectile>::iterator it;
	for (it = projlist->begin(); it != projlist->end(); ++it) {
		it->render();
	}
	for (it = projlistLevel2Turret->begin(); it != projlistLevel2Turret->end(); ++it) {
		it->render();
	}
	for (it = projlistBoss2Turret->begin(); it != projlistBoss2Turret->end(); ++it) {
		it->render();
	}
	list<ProjectileBoss2>::iterator it_boss2;
	for (it_boss2 = projlistBoss2Final->begin(); it_boss2 != projlistBoss2Final->end(); ++it_boss2) {
		it_boss2->render();
	}
}


bool EnemyManager::isOffScreen(glm::ivec2 pj)
{
	if (pj.x < left || pj.x > right || pj.y < top || pj.y > bottom) return true;
	return false;
}

bool EnemyManager::isOffScreenLevel2(glm::ivec2 pj)
{
	if (sublvl < 5) {
		if (pj.x < left || pj.x > right || pj.y < top + 70 || pj.y > bottom - 70) return true;
		return false;
	}
	else {
		if (pj.x < left || pj.x > right || pj.y < top || pj.y > bottom) return true;
		return false;
	}
}

bool EnemyManager::isOffScreenLevel2Enemy(glm::ivec2 pj)
{
	if (pj.x < left || pj.x > right || pj.y < top || pj.y > bottom - 70) return true;
	return false;
}

bool EnemyManager::isOffScreenY(Enemy &pj)
{
	vector <glm::ivec2> box = pj.buildHitBox();
	if ((box[0].y < top || box[0].y > bottom) && (box[1].y < top || box[1].y > bottom)) return true;
	return false;
}

void EnemyManager::spawnProjectilePlayer(glm::ivec2 position)
{
	projectile = new Projectile();
	glm::ivec2 newPos = glm::ivec2{ 1,0 };
	int dir = player->sprite->animation();
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && Game::instance().getSpecialKey(GLUT_KEY_UP)) newPos = glm::ivec2{ -1,-1 };
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && Game::instance().getSpecialKey(GLUT_KEY_UP)) newPos = glm::ivec2{ 1,-1 };
	else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && Game::instance().getSpecialKey(GLUT_KEY_DOWN)) newPos = glm::ivec2{ -1,1 };
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && Game::instance().getSpecialKey(GLUT_KEY_DOWN)) newPos = glm::ivec2{ 1,1 };
	else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) newPos = glm::ivec2{ -1,0 };
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) newPos = glm::ivec2{ 1,0 };
	else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) newPos = glm::ivec2{ 0,-1 };
	else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
		if (dir == CROUCH_LOOK_LEFT)
			newPos = glm::ivec2{ -1,0 };
		else if (dir == CROUCH_LOOK_RIGHT)
			newPos = glm::ivec2{ 1,0 };
		else newPos = glm::ivec2{ 0, 1 };
	}
	else {
		if (dir == STAND_LEFT || dir == AIRBONE_LEFT || dir == SWIM_AIM_LEFT || dir ==DROPPED) newPos = glm::ivec2{ -1,0 };
		else if (dir == STAND_RIGHT || dir == AIRBONE_RIGHT || dir == SWIM_AIM_RIGHT || dir == DROPPED) newPos = glm::ivec2{ 1,0 };
	}

	projectile->init(tilemap, texProgram, 3, newPos);
	projectile->rank = RANK1;
	projectile->setPosition(position + player->getProjectileSpawn());
	projectile->setTileMap(map);
	projlist->push_back(*(projectile));
}

void EnemyManager::spawnProjectilePlayerLVL2(glm::ivec2 position)
{
	projectile = new Projectile();
	glm::vec2 newPos = glm::vec2{ 0,-1 };
	glm::vec2 vec = glm::ivec2{ 145, 60 } -position;
	float magnitude = sqrt(vec.x * vec.x + vec.y * vec.y);
	newPos = glm::vec2{ vec.x / magnitude, vec.y / magnitude };
	projectile->init(tilemap, texProgram, 6, newPos);
	projectile->rank = RANK1;
	projectile->setPosition(position + player->getProjectileSpawnlvl2());
	projectile->setTileMap(map);
	projlist->push_back(*(projectile));
}

void EnemyManager::spawnProjectileSPREADPlayer(glm::ivec2 position)
{
	glm::vec2 newPos;
	glm::vec2 newPos2;
	glm::vec2 newPos3;
	glm::vec2 newPos4;
	glm::vec2 newPos5;
	int dir = player->sprite->animation();
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && Game::instance().getSpecialKey(GLUT_KEY_UP)) {
		newPos = glm::vec2{ -1,-1 };
		newPos2 = glm::vec2{ -0.85,-1.15 };
		newPos3 = glm::vec2{ -0.70,-1.30 };
		newPos4 = glm::vec2{ -1.15,-0.85 };
		newPos5 = glm::vec2{ -1.30,-0.70 };
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && Game::instance().getSpecialKey(GLUT_KEY_UP)) {
		newPos = glm::vec2{ 1,-1 };
		newPos2 = glm::vec2{ 0.85,-1.15 };
		newPos3 = glm::vec2{ 0.70,-1.30 };
		newPos4 = glm::vec2{ 1.15,-0.85 };
		newPos5 = glm::vec2{ 1.30,-0.70 };

	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
		newPos = glm::vec2{ -1,1 };
		newPos2 = glm::vec2{ -0.85,1.15 };
		newPos3 = glm::vec2{ -0.70,1.30 };
		newPos4 = glm::vec2{ -1.15,0.85 };
		newPos5 = glm::vec2{ -1.30,0.70 };
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
		newPos = glm::vec2{ 1,1 };
		newPos2 = glm::vec2{ 0.85,1.15 };
		newPos3 = glm::vec2{ 0.70,1.30 };
		newPos4 = glm::vec2{ 1.15,0.85 };
		newPos5 = glm::vec2{ 1.30,0.70 };
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
		newPos = glm::vec2{ -1,0 };
		newPos2 = glm::vec2{ -1,0.15 };
		newPos3 = glm::vec2{ -1,0.30 };
		newPos4 = glm::vec2{ -1,-0.15 };
		newPos5 = glm::vec2{ -1,-0.30 };
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
		newPos = glm::vec2{ 1,0 };
		newPos2 = glm::vec2{ 1,0.15 };
		newPos3 = glm::vec2{ 1,0.30 };
		newPos4 = glm::vec2{ 1,-0.15 };
		newPos5 = glm::vec2{ 1,-0.30 };
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
		newPos = glm::vec2{ 0,-1 };
		newPos2 = glm::vec2{ 0.15,-1 };
		newPos3 = glm::vec2{ 0.30,-1 };
		newPos4 = glm::vec2{ -0.15,-1 };
		newPos5 = glm::vec2{ -0.30,-1 };
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
		if (dir == CROUCH_LOOK_LEFT) {
			newPos = glm::vec2{ -1,0 };
			newPos2 = glm::vec2{ -1,0.15 };
			newPos3 = glm::vec2{ -1,0.30 };
			newPos4 = glm::vec2{ -1,-0.15 };
			newPos5 = glm::vec2{ -1,-0.30 };
		}
		else if (dir == CROUCH_LOOK_RIGHT) {
			newPos = glm::vec2{ 1,0 };
			newPos2 = glm::vec2{ 1,0.15 };
			newPos3 = glm::vec2{ 1,0.30 };
			newPos4 = glm::vec2{ 1,-0.15 };
			newPos5 = glm::vec2{ 1,-0.30 };
		}
		else {
			newPos = glm::vec2{ 0, 1 };
			newPos2 = glm::vec2{ 0.15, 1 };
			newPos3 = glm::vec2{ 0.30, 1 };
			newPos4 = glm::vec2{ -0.15, 1 };
			newPos5 = glm::vec2{ -0.30, 1 };
		}
	}
	else {
		if (dir == STAND_LEFT || dir == AIRBONE_LEFT || dir == SWIM_AIM_LEFT || dir == DROPPED) {
			newPos = glm::vec2{ -1,0 };
			newPos2 = glm::vec2{ -1,0.15 };
			newPos3 = glm::vec2{ -1,0.30 };
			newPos4 = glm::vec2{ -1,-0.15 };
			newPos5 = glm::vec2{ -1,-0.30 };
		}
		else if (dir == STAND_RIGHT || dir == AIRBONE_RIGHT || dir == SWIM_AIM_RIGHT || dir == DROPPED) {
			newPos = glm::vec2{ 1,0 };
			newPos2 = glm::vec2{ 1,0.15 };
			newPos3 = glm::vec2{ 1,0.30 };
			newPos4 = glm::vec2{ 1,-0.15 };
			newPos5 = glm::vec2{ 1,-0.30 };
		}
	}

	projectile = new Projectile();
	projectile->init(tilemap, texProgram, 6, newPos);
	projectile->rank = SPREAD;
	projectile->sprite->changeAnimation(SPREAD);
	projectile->setPosition(position + player->getProjectileSpawn());
	projectile->setTileMap(map);
	projlist->push_back(*(projectile));

	projectile = new Projectile();
	projectile->init(tilemap, texProgram, 6, newPos2);
	projectile->rank = SPREAD;
	projectile->sprite->changeAnimation(SPREAD);
	projectile->setPosition(position + player->getProjectileSpawn());
	projectile->setTileMap(map);
	projlist->push_back(*(projectile));

	projectile = new Projectile();
	projectile->init(tilemap, texProgram, 6, newPos3);
	projectile->rank = SPREAD;
	projectile->sprite->changeAnimation(SPREAD);
	projectile->setPosition(position + player->getProjectileSpawn());
	projectile->setTileMap(map);
	projlist->push_back(*(projectile));

	projectile = new Projectile();
	projectile->init(tilemap, texProgram, 6, newPos4);
	projectile->rank = SPREAD;
	projectile->sprite->changeAnimation(SPREAD);
	projectile->setPosition(position + player->getProjectileSpawn());
	projectile->setTileMap(map);
	projlist->push_back(*(projectile));

	projectile = new Projectile();
	projectile->init(tilemap, texProgram, 6, newPos5);
	projectile->rank = SPREAD;
	projectile->sprite->changeAnimation(SPREAD);
	projectile->setPosition(position + player->getProjectileSpawn());
	projectile->setTileMap(map);
	projlist->push_back(*(projectile));
}

void EnemyManager::spawnProjectileRifleman(glm::ivec2 position, Rifleman* badguy)
{
	glm::ivec2 posEnemy = badguy->getPos();
	projectile = new Projectile();
	glm::ivec2 newPos;
	if ((badguy)->hasShot == false) {
		badguy->hasShot = true;
		if (position.y + 30 < posEnemy.y - 40) {
			if (position.x < posEnemy.x) newPos = glm::ivec2{ -1,-1 };
			else newPos = glm::ivec2{ 1,-1 };
		}
		else if (position.y + 30 > posEnemy.y + 40) {
			if (position.x < posEnemy.x) newPos = glm::ivec2{ -1,1 };
			else newPos = glm::ivec2{ 1,1 };
		}
		else {
			if (position.x < posEnemy.x) newPos = glm::ivec2{ -1,0 };
			else newPos = glm::ivec2{ 1,0 };
		}
		badguy->projDir = newPos;
		projectile->init(tilemap, texProgram, 2, newPos);
		projectile->rank = RIFLEMAN;
		projectile->sprite->changeAnimation(0);
		projectile->setPosition(posEnemy + badguy->getProjectileSpawn());
		projectile->setTileMap(map);
		projlistRifleman->push_back(*(projectile));
	}
}

void EnemyManager::spawnProjectileWallTurret(glm::ivec2 positionPlayer, WallTurret* badguy)
{
	glm::ivec2 posPlayer = positionPlayer + glm::ivec2{ 10, 30 };
	glm::ivec2 posEnemy = badguy->getPos() + glm::ivec2{ 8,8 };
	if (!isOffScreen(posEnemy)) {
		glm::vec2 newPos = glm::vec2{ 1,1 };
		if (posPlayer.y < posEnemy.y - 10) { /*  SHOOT UP  */
			if (posPlayer.x < posEnemy.x - 10) {
				if (posPlayer.x >= posEnemy.x - 30) newPos = glm::vec2{ -0.25,-0.75 }; /* SHOOT SLIGHTLY LEFT */
				else newPos = glm::vec2{ -0.75,-0.25 }; /* SHOOT VERY LEFT */
			}
			else if (posPlayer.x > posEnemy.x + 10) {
				if (posPlayer.x < posEnemy.x + 30) newPos = glm::vec2{ 0.25,-0.75 }; /* SHOOT SLIGHTLY RIGHT */
				else newPos = glm::vec2{ 0.75,-0.25 }; /* SHOOT VERY RIGHT */
			}
			else {
				newPos = glm::ivec2{ 0,-1 };
			}
		}
		else if (posPlayer.y > posEnemy.y + 10) { /*  SHOOT DOWN  */
			if (posPlayer.x < posEnemy.x - 10) {
				if (posPlayer.x >= posEnemy.x - 30) newPos = glm::vec2{ -0.25,0.75 }; /* SHOOT SLIGHTLY LEFT */
				else newPos = glm::vec2{ -0.75,0.25 }; /* SHOOT VERY LEFT */
			}
			else if (posPlayer.x > posEnemy.x + 10) {
				if (posPlayer.x < posEnemy.x + 30) newPos = glm::vec2{ 0.25,0.75 }; /* SHOOT SLIGHTLY RIGHT */
				else newPos = glm::vec2{ 0.75,0.25 }; /* SHOOT VERY RIGHT */
			}
			else {
				newPos = glm::ivec2{ 0,1 };
			}
		}
		else { /*  ===  */
			if (posPlayer.x < posEnemy.x ) newPos = glm::ivec2{ -1,0 };
			else newPos = glm::ivec2{ 1,0 };
		}
		badguy->projDir = newPos;
		projectile = new Projectile();
		projectile->init(tilemap, texProgram, 2, newPos);
		projectile->rank = RIFLEMAN;
		projectile->sprite->changeAnimation(0);
		projectile->setPosition(posEnemy + badguy->getProjectileSpawn());
		projectile->setTileMap(map);
		projlistWallTurret->push_back(*(projectile));
	}
}

void EnemyManager::spawnProjectileBossTurret(glm::ivec2 positionPlayer, BossTurret* badguy)
{
	glm::ivec2 posPlayer = positionPlayer + glm::ivec2{ 10, 30 };
	glm::ivec2 posEnemy = badguy->getPos() + glm::ivec2{ 8,8 };
	if (!isOffScreen(posEnemy)) {
		glm::vec2 newPos = glm::vec2{ 1,1 };
		if (posPlayer.x < posEnemy.x) newPos = glm::ivec2{ -1,0 };
		else newPos = glm::ivec2{ 1,0 };

		badguy->projDir = newPos;
		projectileBoss = new ProjectileBoss();
		projectileBoss->init(tilemap, texProgram, 2, newPos);
		projectileBoss->sprite->changeAnimation(2);
		projectileBoss->rank = SPREAD;
		projectileBoss->setPosition(posEnemy + badguy->getProjectileSpawn());
		projectileBoss->setTileMap(map);
		projlistBossTurret->push_back(*(projectileBoss));
	}
}

void EnemyManager::spawnProjectileBoss2Turret(glm::ivec2 positionPlayer, Boss2Turret* badguy)
{
	glm::ivec2 posPlayer = positionPlayer + glm::ivec2{ 10, 30 };
	glm::ivec2 posEnemy = badguy->getPos() + glm::ivec2{ 8,8 };

	if (!isOffScreen(posEnemy) && badguy->isOpen == true && badguy->hasShot == false) {
		badguy->hasShot = true;
		glm::vec2 newPos = glm::ivec2{ 0,1 };
		glm::vec2 newPos2 = glm::ivec2{ -1,1 };
		glm::vec2 newPos3 = glm::ivec2{ 1,1 };

		projectile = new Projectile();
		projectile->init(tilemap, texProgram, 2, newPos);
		projectile->sprite->changeAnimation(2);
		projectile->rank = SPREAD;
		projectile->setPosition(posEnemy + badguy->getProjectileSpawn());
		projectile->setTileMap(map);
		projlistBoss2Turret->push_back(*(projectile));

		projectile = new Projectile();
		projectile->init(tilemap, texProgram, 2, newPos2);
		projectile->sprite->changeAnimation(2);
		projectile->rank = SPREAD;
		projectile->setPosition(posEnemy + badguy->getProjectileSpawn());
		projectile->setTileMap(map);
		projlistBoss2Turret->push_back(*(projectile));

		projectile = new Projectile();
		projectile->init(tilemap, texProgram, 2, newPos3);
		projectile->sprite->changeAnimation(2);
		projectile->rank = SPREAD;
		projectile->setPosition(posEnemy + badguy->getProjectileSpawn());
		projectile->setTileMap(map);
		projlistBoss2Turret->push_back(*(projectile));
	}
}

void EnemyManager::spawnProjectileBoss2Final(glm::ivec2 positionPlayer, Boss2Final* badguy)
{
	glm::ivec2 posPlayer = positionPlayer + glm::ivec2{ 10, 30 };
	glm::ivec2 posEnemy = badguy->getPos() + glm::ivec2{ 8,8 };
	if (!isOffScreen(posEnemy) && badguy->hasShot == false) {
		badguy->hasShot = true;
		glm::vec2 newPos = glm::vec2{ 1,1 };
		glm::vec2 vec = posPlayer - posEnemy;
		float magnitude = sqrt(vec.x * vec.x + vec.y * vec.y);
		newPos = glm::vec2{ vec.x / magnitude, vec.y / magnitude };
		badguy->projDir = newPos;
		projectileBoss2 = new ProjectileBoss2();
		projectileBoss2->init(tilemap, texProgram, 2, newPos);
		projectileBoss2->setPosition(posEnemy + badguy->getProjectileSpawn());
		projectileBoss2->setTileMap(map);
		projlistBoss2Final->push_back(*(projectileBoss2));
	}
}

void EnemyManager::spawnGreenSoldiers()
{
	GreenSoldier *aux = new GreenSoldier();
	aux->init(tilemap, texProgram);
	aux->setTileMap(map);
	aux->sublvl = sublvl;
	if (rand() % 2) {
		aux->left = true;
		aux->setPosition(glm::vec2(115, 96));
	}
	else {
		aux->left = false;
		aux->setPosition(glm::vec2(182, 96));
	}
	enemies->push_back(aux);
}

void EnemyManager::spawnLaser()
{
	Laser *aux = new Laser();
	aux->init(tilemap, texProgram);
	aux->setTileMap(map);
	aux->sublvl = sublvl;
	aux->setPosition(glm::vec2(68, 145));
	enemies->push_back(aux);

	aux = new Laser();
	aux->init(tilemap, texProgram);
	aux->setTileMap(map);
	aux->sublvl = sublvl;
	aux->setPosition(glm::vec2(68, 160));
	enemies->push_back(aux);
}

void EnemyManager::spawnProjectileLevel2Turret(glm::ivec2 positionPlayer, Level2Turret* badguy)
{
	glm::ivec2 posPlayer = positionPlayer + glm::ivec2{ 10, 30 };
	glm::ivec2 posEnemy = badguy->getPos() + glm::ivec2{ 8,8 };
	if (!isOffScreen(posEnemy) && badguy->isOpen == true && badguy->hasShot == false) {
		badguy->hasShot = true;
		glm::vec2 newPos = glm::vec2{ 1,1 };
		glm::vec2 vec = posPlayer - posEnemy;
		float magnitude = sqrt(vec.x * vec.x + vec.y * vec.y);
		newPos = glm::vec2{ vec.x / magnitude, vec.y / magnitude };
		badguy->projDir = newPos;
		projectile = new Projectile();
		projectile->init(tilemap, texProgram, 2, newPos);
		projectile->sprite->changeAnimation(0);
		projectile->setPosition(posEnemy + badguy->getProjectileSpawn());
		projectile->setTileMap(map);
		projlistLevel2Turret->push_back(*(projectile));

	}
}

void EnemyManager::spawnProjectileCannon(glm::ivec2 positionPlayer, Cannon* badguy)
{
	glm::ivec2 posPlayer = positionPlayer + glm::ivec2{ 10, 30 };
	glm::ivec2 posEnemy = badguy->getPos() + glm::ivec2{ 8,8 };
	if (!isOffScreen(posEnemy) && badguy->hasShot == false) {
		badguy->hasShot = true;
		glm::vec2 newPos = glm::vec2{ 99,99 };
		if (posPlayer.y < posEnemy.y - 10) { /*  SHOOT UP  */
			if (posPlayer.x < posEnemy.x - 10) {
				if (posPlayer.x >= posEnemy.x - 30) newPos = glm::vec2{ -0.25,-0.75 }; /* SHOOT SLIGHTLY LEFT */
				else newPos = glm::vec2{ -0.75,-0.25 }; /* SHOOT VERY LEFT */
			}
		}
		else { /*  ===  */
			if (posPlayer.x < posEnemy.x) newPos = glm::ivec2{ -1,0 };
		}
		if (newPos != glm::vec2{ 99,99 }) {
			badguy->projDir = newPos;
			projectile = new Projectile();
			projectile->init(tilemap, texProgram, 2, newPos);
			projectile->sprite->changeAnimation(0);
			projectile->setPosition(posEnemy + badguy->getProjectileSpawn());
			projectile->setTileMap(map);
			projlistCannon->push_back(*(projectile));
		}
	}
}

void EnemyManager::spawnProjectileGreenSoldier(glm::ivec2 position, GreenSoldier* badguy)
{
	glm::ivec2 posPlayer = position + glm::ivec2{ 10, 30 };
	glm::ivec2 posEnemy = badguy->getPos() + glm::ivec2{ 8,8 };
	if (!isOffScreen(posEnemy) && badguy->isOpen == true && badguy->hasShot == false) {
		badguy->hasShot = true;
		badguy->move = false;
		glm::vec2 newPos = glm::vec2{ 1,1 };
		glm::vec2 vec = posPlayer - posEnemy;
		float magnitude = sqrt(vec.x * vec.x + vec.y * vec.y);
		newPos = glm::vec2{ vec.x / magnitude, vec.y / magnitude };
		badguy->projDir = newPos;
		projectile = new Projectile();
		projectile->init(tilemap, texProgram, 2, newPos);
		projectile->sprite->changeAnimation(0);
		projectile->setPosition(posEnemy + badguy->getProjectileSpawn());
		projectile->setTileMap(map);
		projlistLevel2Turret->push_back(*(projectile));

	}
}
void EnemyManager::despawnOffScreenProjectiles()
{
	list<Projectile>::iterator it = projlist->begin();
	while (it != projlist->end()) {
		if (isOffScreen((it->getPos())))
			it = projlist->erase(it);
		else ++it;
	}
	it = projlistRifleman->begin();
	while (it != projlistRifleman->end()) {
		if (isOffScreen((it->getPos())))
			it = projlistRifleman->erase(it);
		else ++it;
	}
	it = projlistWallTurret->begin();
	while (it != projlistWallTurret->end()) {
		if (isOffScreen((it->getPos())))
			it = projlistWallTurret->erase(it);
		else ++it;
	}
	list<ProjectileBoss>::iterator it_boss = projlistBossTurret->begin();
	while (it_boss != projlistBossTurret->end()) {
		if (isOffScreen((it->getPos())))
			it_boss = projlistBossTurret->erase(it_boss);
		else ++it_boss;
	}
	it = projlistCannon->begin();
	while (it != projlistCannon->end()) {
		if (isOffScreen((it->getPos())))
			it = projlistCannon->erase(it);
		else ++it;
	}
	it = projlistBoss2Turret->begin();
	while (it != projlistBoss2Turret->end()) {
		if (isOffScreen((it->getPos())))
			it = projlistBoss2Turret->erase(it);
		else ++it;
	}
	list<ProjectileBoss2>::iterator it_boss2 = projlistBoss2Final->begin();
	while (it_boss2 != projlistBoss2Final->end()) {
		if (isOffScreen((it->getPos())))
			it_boss2 = projlistBoss2Final->erase(it_boss2);
		else ++it_boss2;
	}
}

void EnemyManager::despawnOffScreenEnemies() {
	list<Enemy*>::iterator it = enemies->begin();
	while (it != enemies->end()) {
	if (isOffScreenY(*(*it))) 
		it = enemies->erase(it);
	else ++it;
	}
}

void EnemyManager::didthePlayerFuckingFall() {
	vector <glm::ivec2> box = player->buildHitBox();
	if (box[0].y > bottom) updatePlayerState();
}

void EnemyManager::despawnDeadEnemies() {
	list<Enemy*>::iterator it = enemies->begin();
	while (it != enemies->end()) {
		if ((*it)->state == DEAD && (*it)->getType() != "bridge") {
			if ((*it)->getType() == "level2core") {
				coreDestroyed = true;
				player->coreDestroyed = true;
			}
			it = enemies->erase(it);
		}
		else ++it;
	}
	list<ProjectileBoss2>::iterator it_boss2 = projlistBoss2Final->begin();
	while (it_boss2 != projlistBoss2Final->end()) {
		if ((it_boss2)->state == DEAD)
			it_boss2 = projlistBoss2Final->erase(it_boss2);
		else ++it_boss2;
	}
}

bool EnemyManager::areTouching(glm::ivec2 obj1_left, glm::ivec2 obj1_right, glm::ivec2 obj2_left, glm::ivec2 obj2_right)
{
	if (obj1_left.x > obj2_right.x || obj2_left.x > obj1_right.x) return false;
	if (obj1_left.y > obj2_right.y || obj2_left.y > obj1_right.y) return false;
	return true;
}

bool EnemyManager::areTouchingYcoord(glm::ivec2 obj1_left, glm::ivec2 obj1_right, glm::ivec2 obj2_left, glm::ivec2 obj2_right)
{
	if (obj1_left.x > obj2_right.x || obj2_left.x > obj1_right.x) return false;
	return true;
}

void EnemyManager::updatePlayerState()
{
	if (player->invtimer == 0 && player->state == ALIVE) {
		player->state = DYING;
		audiomanager->play(DEATH_SOUND, false);
	}
}


void EnemyManager::checkPhysics()
{
	list<Enemy*>::iterator it_enemy;
	list<Projectile>::iterator it_projec = projlist->begin();

	while (it_projec != projlist->end()) { // our pew pew
		if (isOffScreen((it_projec->getPos())))
			it_projec = projlist->erase(it_projec);
		else {
			vector<glm::ivec2> box = it_projec->buildHitBox();
			it_enemy = enemies->begin();
			bool shot = false;
			while ( it_enemy != enemies->end() && !shot) {
				vector<glm::ivec2> boxEnemy = (*it_enemy)->buildHitBox();
				if ((*it_enemy)->state == ALIVE && (*it_enemy)->getType() != "bridge" && (*it_enemy)->getType() != "gunupgrade" && areTouching(box[0], box[1], boxEnemy[0], boxEnemy[1])) {
					if ((*it_enemy)->getType() == "wallturret" || (*it_enemy)->getType() == "cannon") audiomanager->play(TURRET_HIT_SOUND, false);
					if ((*it_enemy)->decreaseLife(it_projec->getDmg())) {
						(*it_enemy)->state = DYING;
						playDeathSound((*it_enemy)->getType());
						if ((*it_enemy)->getType() == "upgradebox") {
							Enemy *pew = new GunUpgrade();
							pew->init(tilemap, texProgram);
							pew->setPosition((*it_enemy)->getPos());
							pew->setTileMap(map);
							enemies->push_back(pew);
						}
					}
					(*it_enemy)->hurt = true;
					shot = true;
				}
				else ++it_enemy;
			}
			if (shot) it_projec = projlist->erase(it_projec);
			else ++it_projec;
		}
	}

	

	it_projec = projlistRifleman->begin();
	vector<glm::ivec2> boxPlayer = player->buildHitBox();
	while (it_projec != projlistRifleman->end()) { // their pew pew
		if (isOffScreen((it_projec->getPos())))
			it_projec = projlistRifleman->erase(it_projec);
		else {
			vector<glm::ivec2> box = it_projec->buildHitBox();
			if (areTouching(boxPlayer[0], boxPlayer[1], box[0], box[1])) {
				updatePlayerState();
			}
			++it_projec;
		}
		
	}

	it_projec = projlistWallTurret->begin();
	while (it_projec != projlistWallTurret->end()) { // their pew pew
		if (isOffScreen((it_projec->getPos())))
			it_projec = projlistWallTurret->erase(it_projec);
		else {
			vector<glm::ivec2> box = it_projec->buildHitBox();
			if (areTouching(boxPlayer[0], boxPlayer[1], box[0], box[1])) {
				updatePlayerState();
			}
			++it_projec;
		}

	}
	list<ProjectileBoss>::iterator it_boss = projlistBossTurret->begin();
	while (it_boss != projlistBossTurret->end()) { // their pew pew
		if (isOffScreen((it_boss->getPos())))
			it_boss = projlistBossTurret->erase(it_boss);
		else {
			vector<glm::ivec2> box = it_boss->buildHitBox();
			if (areTouching(boxPlayer[0], boxPlayer[1], box[0], box[1])) {
				updatePlayerState();
			}
			++it_boss;
		}

	}

	it_projec = projlistCannon->begin();
	while (it_projec != projlistCannon->end()) { // their pew pew
		if (isOffScreen((it_projec->getPos())))
			it_projec = projlistCannon->erase(it_projec);
		else {
			vector<glm::ivec2> box = it_projec->buildHitBox();
			if (areTouching(boxPlayer[0], boxPlayer[1], box[0], box[1])) {
				updatePlayerState();
			}
			++it_projec;
		}

	}

	it_projec = projlistLevel2Turret->begin();
	while (it_projec != projlistLevel2Turret->end()) { // their pew pew
		if (isOffScreen((it_projec->getPos())))
			it_projec = projlistLevel2Turret->erase(it_projec);
		else {
			vector<glm::ivec2> box = it_projec->buildHitBox();
			if (areTouching(boxPlayer[0], boxPlayer[1], box[0], box[1])) {
				updatePlayerState();
			}
			++it_projec;
		}

	}
	// LEVEL 2 //
	
	// LEVEL 2 //
	

	it_enemy = enemies->begin();
	player->bBridge = FALSE;
	while (it_enemy != enemies->end()) {  // are we touching bad guys?
		vector<glm::ivec2> boxEnemy = (*it_enemy)->buildHitBox();
		if ((*it_enemy)->state == ALIVE && (*it_enemy)->getType() != "bridge" && (*it_enemy)->getType() != "upgradebox" && (*it_enemy)->getType() != "gunupgrade") {
			if (areTouching(boxPlayer[0], boxPlayer[1], boxEnemy[0], boxEnemy[1])) {
				updatePlayerState();
			}
		}
		else if ((*it_enemy)->getType() == "bridge"){
			if (areTouching(boxPlayer[0], boxPlayer[1], boxEnemy[0], boxEnemy[1])) {
				if ((*it_enemy)->state != DEAD && (*it_enemy)->state != DYING) {
					(*it_enemy)->state = SPECIAL_BRIDGE_STATE;
					player->bBridge = TRUE;
					if (/*!player->isJumping() && */(*it_enemy)->state == ALIVE);// player->posPlayer.y -= FALL_STEP;
				}
			}
		}
		else if ((*it_enemy)->getType() == "gunupgrade") {
			if (areTouching(boxPlayer[0], boxPlayer[1], boxEnemy[0], boxEnemy[1])) {
				(*it_enemy)->state = DYING;
				player->projectile = SPREAD;
			}
		}
		++it_enemy;
	}
	
}

void EnemyManager::checkPhysicsLevel2() 
{
	list<Enemy*>::iterator it_enemy;
	list<Projectile>::iterator it_projec = projlist->begin();
	vector<glm::ivec2> boxPlayer = player->buildHitBox();

	while (it_projec != projlist->end()) { // our pew pew
		if (isOffScreenLevel2((it_projec->getPos())))
			it_projec = projlist->erase(it_projec);
		else {
			vector<glm::ivec2> box = it_projec->buildHitBox();
			it_enemy = enemies->begin();
			bool shot = false;
			while (it_enemy != enemies->end() && !shot) {
				vector<glm::ivec2> boxEnemy = (*it_enemy)->buildHitBox();
				if ((*it_enemy)->state == ALIVE && (*it_enemy)->getType() != "laser" && (*it_enemy)->sublvl == sublvl && areTouching(box[0], box[1], boxEnemy[0], boxEnemy[1])) {
					if ((*it_enemy)->isOpen) {
						if ((*it_enemy)->decreaseLife(it_projec->getDmg())) {
							(*it_enemy)->state = DYING;
							if ((*it_enemy)->getType() == "boss2core") ++nDestroyed;
						}
					}
					(*it_enemy)->hurt = true;
					shot = true;
				}
				else ++it_enemy;
			}
			if (shot) it_projec = projlist->erase(it_projec);
			else ++it_projec;
		}
	}
	it_projec = projlistLevel2Turret->begin();
	while (it_projec != projlistLevel2Turret->end()) { // their pew pew
		if (isOffScreenLevel2Enemy((it_projec->getPos())))
			it_projec = projlistLevel2Turret->erase(it_projec);
		else {
			vector<glm::ivec2> box = it_projec->buildHitBox();
			if (areTouching(boxPlayer[0], boxPlayer[1], box[0], box[1])) {
				updatePlayerState();
			}
			++it_projec;
		}

	}
	it_projec = projlistBoss2Turret->begin();
	while (it_projec != projlistBoss2Turret->end()) { // their pew pew
		if (isOffScreen((it_projec->getPos())))
			it_projec = projlistBoss2Turret->erase(it_projec);
		else {
			vector<glm::ivec2> box = it_projec->buildHitBox();
			if (areTouching(boxPlayer[0], boxPlayer[1], box[0], box[1])) {
				updatePlayerState();
			}
			++it_projec;
		}

	}
	list<ProjectileBoss2>::iterator it_boss2;
	it_projec = projlist->begin();
	while (it_projec != projlist->end()) { // our pew pew for boss2final projectile 
		if (isOffScreen((it_projec->getPos())))
			it_projec = projlist->erase(it_projec);
		else {
			vector<glm::ivec2> box = it_projec->buildHitBox();
			it_boss2 = projlistBoss2Final->begin();
			bool shot = false;
			while (it_boss2 != projlistBoss2Final->end() && !shot) {
				vector<glm::ivec2> boxBoss = (it_boss2)->buildHitBox();
				if ((it_boss2)->state == ALIVE && areTouching(box[0], box[1], boxBoss[0], boxBoss[1])) {
					(it_boss2)->state = DYING;
					shot = true;
				}
				else ++it_boss2;
			}
			if (shot) it_projec = projlist->erase(it_projec);
			else ++it_projec;
		}
	}

	it_boss2 = projlistBoss2Final->begin();
	while (it_boss2 != projlistBoss2Final->end()) { // their pew pew
		if (isOffScreen((it_boss2->getPos())))
			it_boss2 = projlistBoss2Final->erase(it_boss2);
		else {
			vector<glm::ivec2> box = it_boss2->buildHitBox();
			if (areTouching(boxPlayer[0], boxPlayer[1], box[0], box[1])) {
				updatePlayerState();
			}
			++it_boss2;
		}

	}
}

void EnemyManager::playDeathSound(string type) {
	if (type == "bosscore") audiomanager->play(BOSS_DEATH_1_AUDIO, false);
	else if (type == "bridge" || type == "rifleman" || type == "soldier") 
		audiomanager->play(BRIDGE_EXPLOSION_SOUND, false);
	else audiomanager->play(ENEMY_DEATH_SOUND, false);
}