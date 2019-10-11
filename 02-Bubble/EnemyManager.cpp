#include "EnemyManager.h"
#include "Game.h"


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, MOVE_LEFT_AIM, MOVE_RIGHT_AIM, AIM_UP_LOOK_LEFT, AIM_UP_LOOK_RIGHT, CROUCH_LOOK_LEFT,
	CROUCH_LOOK_RIGHT, AIM_UP_WALK_RIGHT, AIM_UP_WALK_LEFT, AIM_DOWN_WALK_RIGHT, AIM_DOWN_WALK_LEFT, AIRBONE_LEFT, AIRBONE_RIGHT
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
		switch (map->getEnemy(i).type)
		{
		case SOLDIER: {
			Enemy *aux = new Soldier();
			aux->init(tilemap, texProgram);
			aux->setPosition(glm::vec2(map->getEnemy(i).x * map->getTileSize(), map->getEnemy(i).y * map->getTileSize()));
			aux->setTileMap(map);
			enemies->push_back(aux);
		}
		}
	}
	Enemy *test = new Rifleman();
	test->init(tilemap, texProgram);
	test->setPosition(glm::vec2(6 * map->getTileSize(), 3 * map->getTileSize()));
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
		(*it_enemy)->update(deltaTime);
		if ((*it_enemy)->getType() == "rifleman") {
			spawnProjectileRifleman(player->getPos(), *it_enemy);
		}
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
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && Game::instance().getSpecialKey(GLUT_KEY_UP)) newPos = glm::ivec2{ -6,-6 };
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && Game::instance().getSpecialKey(GLUT_KEY_UP)) newPos = glm::ivec2{ 6,-6 };
	else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && Game::instance().getSpecialKey(GLUT_KEY_DOWN)) newPos = glm::ivec2{ -6,6 };
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && Game::instance().getSpecialKey(GLUT_KEY_DOWN)) newPos = glm::ivec2{ 6,6 };
	else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) newPos = glm::ivec2{ -6,0 };
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) newPos = glm::ivec2{ 6,0 };
	else if (Game::instance().getSpecialKey(GLUT_KEY_UP)) newPos = glm::ivec2{ 0,-6 };
	else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
		if (dir == CROUCH_LOOK_LEFT)
			newPos = glm::ivec2{ -6,0 };
		else if (dir == CROUCH_LOOK_RIGHT)
			newPos = glm::ivec2{ 6,0 };
		else newPos = glm::ivec2{ 0, 6 };
	}
	else {
		if (dir == STAND_LEFT || dir == AIRBONE_LEFT) newPos = glm::ivec2{ -6,0 };
		else if (dir == STAND_RIGHT || dir == AIRBONE_RIGHT) newPos = glm::ivec2{ 6,0 };
	}

	projectile->init(tilemap, texProgram, dir, newPos);
	projectile->setPosition(glm::vec2(position.x + 16, position.y + 32));
	projectile->setTileMap(map);
	projlist->push_back(*(projectile));
}

void EnemyManager::spawnProjectileRifleman(glm::ivec2 position, Enemy* badguy)
{
	projectile = new Projectile();
	glm::ivec2 posEnemy = badguy->getPos();
	glm::ivec2 newPos;
	if (position.y < posEnemy.y) {
		if (position.x < posEnemy.x) newPos = glm::ivec2{ -6,-6 };
		else newPos = glm::ivec2{ 6,-6 };
	}
	else {
		if (position.x < posEnemy.x) newPos = glm::ivec2{ -6,6 };
		else newPos = glm::ivec2{ 6,6 };
	}
	projectile->init(tilemap, texProgram, 0, newPos);
	projectile->setPosition(glm::vec2(posEnemy.x, posEnemy.y + 26));
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
}

void EnemyManager::despawnOffScreenEnemies() {
	list<Enemy*>::iterator it = enemies->begin();
	while (it != enemies->end()) {
	if (isOffScreen(*(*it))) 
		it = enemies->erase(it);
	else ++it;
	}
}

bool EnemyManager::areTouching(glm::ivec2 lpos1, glm::ivec2 rpos1, glm::ivec2 lpos2, glm::ivec2 rpos2)
{
	if (lpos1.x > rpos2.x || lpos2.x > rpos1.x) return false;
	if (lpos1.y < rpos2.y || lpos2.y < rpos1.y) return false;
	return true;
}


void EnemyManager::checkPhysics()
{
	list<Enemy*>::iterator it_enemy;
	list<Projectile>::iterator it_projec = projlist->begin();

	while (it_projec != projlist->end()) {
		if (isOffScreen((*it_projec)))
			it_projec = projlist->erase(it_projec);
		else {
			vector<glm::ivec2> box = it_projec->buildHitBox();
			it_enemy = enemies->begin();
			bool shot = false;
			while ( it_enemy != enemies->end() && !shot) {
				vector<glm::ivec2> boxEnemy = (*it_enemy)->buildHitBox();
				if (areTouching(box[0], box[1], boxEnemy[0], boxEnemy[1])) {
					if ((*it_enemy)->decreaseLife(it_projec->getDmg())) it_enemy = enemies->erase(it_enemy);
					shot = true;
				}
				else ++it_enemy;
			}
			if (shot) it_projec = projlist->erase(it_projec);
			else ++it_projec;
		}
	}
	
}