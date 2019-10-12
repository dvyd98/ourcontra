#include "EnemyManager.h"
#include "Game.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define GLUT_KEY_SPACEBAR 32

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, MOVE_LEFT_AIM, MOVE_RIGHT_AIM, AIM_UP_LOOK_LEFT, AIM_UP_LOOK_RIGHT, CROUCH_LOOK_LEFT,
	CROUCH_LOOK_RIGHT, AIM_UP_WALK_RIGHT, AIM_UP_WALK_LEFT, AIM_DOWN_WALK_RIGHT, AIM_DOWN_WALK_LEFT, AIRBONE_LEFT, AIRBONE_RIGHT
};

enum BridgeAnims
{
	EDGE_LEFT, EDGE_RIGHT, DESTROYED_LEFT, DESTROYED_RIGHT, CENTRAL
};

enum States
{
	ALIVE, DEAD, DYING
};

EnemyManager::EnemyManager()
{
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
	int n = map->getNumEnemies();
	enemies = new list<Enemy*>();
	projlist = new list<Projectile>();
	projlistRifleman = new list<Projectile>();
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
	}
	Enemy *test = new Rifleman();
	test->init(tilemap, texProgram);
	test->setPosition(glm::vec2(14 * map->getTileSize(), 9 * map->getTileSize()));
	test->setTileMap(map);
	enemies->push_back(test);
}

void EnemyManager::update(int deltaTime, float leftt, float rightt, float bottomm, float topp)
{
	left = leftt;
	right = rightt;
	bottom = bottomm;
	top = topp;

	despawnOffScreenEnemies();

	checkPhysics(); // coctel

	list<Enemy*>::iterator it_enemy;
	for (it_enemy = enemies->begin(); it_enemy != enemies->end(); ++it_enemy) {
		if ((*it_enemy)->getType() == "rifleman" && projlistRifleman->size() < 4) {
			Rifleman* rifleguy = dynamic_cast<Rifleman*>(*it_enemy);
			spawnProjectileRifleman(player->getPos(), rifleguy);
		}
		(*it_enemy)->update(deltaTime);
	}
	if (Game::instance().getKey('a')) {
		if (projlist->size() < 4)
			spawnProjectilePlayer(player->getPos());
	}
	list<Projectile>::iterator it;
	for (it = projlist->begin(); it != projlist->end(); ++it) {
		it->update(deltaTime);
	}
	for (it = projlistRifleman->begin(); it != projlistRifleman->end(); ++it) {
		it->update(deltaTime);
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
}

bool EnemyManager::isOffScreen(Projectile &pj)
{
	if (pj.getPos().x < left || pj.getPos().x > right || pj.getPos().y < top || pj.getPos().y > bottom) return true;
	return false;
}

bool EnemyManager::isOffScreen(Enemy &pj)
{
	if (pj.getPos().y < top || pj.getPos().y > bottom) return true;
	return false;
}

void EnemyManager::spawnProjectilePlayer(glm::ivec2 position)
{
	projectile = new Projectile();
	glm::ivec2 newPos;
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
		if (dir == STAND_LEFT || dir == AIRBONE_LEFT) newPos = glm::ivec2{ -1,0 };
		else if (dir == STAND_RIGHT || dir == AIRBONE_RIGHT) newPos = glm::ivec2{ 1,0 };
	}

	projectile->init(tilemap, texProgram, 6, newPos);
	projectile->setPosition(position + player->getProjectileSpawn());
	projectile->setTileMap(map);
	projlist->push_back(*(projectile));
}

void EnemyManager::spawnProjectileRifleman(glm::ivec2 position, Rifleman* badguy)
{
	projectile = new Projectile();
	glm::ivec2 posEnemy = badguy->getPos();
	glm::ivec2 newPos;
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
	projectile->setPosition(posEnemy + badguy->getProjectileSpawn());
	projectile->setTileMap(map);
	projlistRifleman->push_back(*(projectile));
}

void EnemyManager::despawnOffScreenProjectiles()
{
	list<Projectile>::iterator it = projlist->begin();
	while (it != projlist->end()) {
		if (isOffScreen((*it)))
			it = projlist->erase(it);
		else ++it;
	}
	it = projlistRifleman->begin();
	while (it != projlistRifleman->end()) {
		if (isOffScreen((*it)))
			it = projlistRifleman->erase(it);
		else ++it;
	}
}

void EnemyManager::despawnOffScreenEnemies() {
	list<Enemy*>::iterator it = enemies->begin();
	while (it != enemies->end()) {
	if (isOffScreen(*(*it))) 
		it = enemies->erase(it);
	else ++it;
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


void EnemyManager::checkPhysics()
{
	list<Enemy*>::iterator it_enemy;
	list<Projectile>::iterator it_projec = projlist->begin();

	while (it_projec != projlist->end()) { // our pew pew
		if (isOffScreen((*it_projec)))
			it_projec = projlist->erase(it_projec);
		else {
			vector<glm::ivec2> box = it_projec->buildHitBox();
			it_enemy = enemies->begin();
			bool shot = false;
			while ( it_enemy != enemies->end() && !shot) {
				vector<glm::ivec2> boxEnemy = (*it_enemy)->buildHitBox();
				if ((*it_enemy)->getType() != "bridge" && areTouching(box[0], box[1], boxEnemy[0], boxEnemy[1])) {
					if ((*it_enemy)->decreaseLife(it_projec->getDmg())) it_enemy = enemies->erase(it_enemy);
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
		if (isOffScreen((*it_projec)))
			it_projec = projlistRifleman->erase(it_projec);
		else {
			vector<glm::ivec2> box = it_projec->buildHitBox();
			if (areTouching(boxPlayer[0], boxPlayer[1], box[0], box[1])) {
				player->state = DEAD;
			}
			++it_projec;
		}
		
	}

	it_enemy = enemies->begin();
	while (it_enemy != enemies->end()) {  // are we touching bad guys?
		vector<glm::ivec2> boxEnemy = (*it_enemy)->buildHitBox();
		if ((*it_enemy)->getType() != "bridge") {
			if (areTouching(boxPlayer[0], boxPlayer[1], boxEnemy[0], boxEnemy[1])) {
					player->state = DEAD;
			}
		}
		else {
			if (areTouchingYcoord(boxPlayer[0], boxPlayer[1], boxEnemy[0], boxEnemy[1])) {
				(*it_enemy)->state = DEAD;
				if (!player->isJumping() && (*it_enemy)->state == ALIVE) player->posPlayer.y -= FALL_STEP;
			}
		}
		++it_enemy;
	}
	
}