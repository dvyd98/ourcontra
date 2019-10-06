#include "EnemyManager.h"
#include "Game.h"

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
	}
	if (Game::instance().getKey('a')) {
		if (projlist->size() < 4)
			spawnProjectile(player->getPos());
	}
	list<Projectile>::iterator it;
	for (it = projlist->begin(); it != projlist->end(); ++it) {
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

void EnemyManager::spawnProjectile(glm::ivec2 position)
{
	projectile = new Projectile();
	projectile->init(tilemap, texProgram, player->sprite->animation());
	projectile->setPosition(glm::vec2(position.x + 16, position.y + 32));
	projectile->setTileMap(map);
	projlist->push_back(*(projectile));
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