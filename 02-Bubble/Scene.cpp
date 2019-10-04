#include <iostream>
#include <cmath>
#include <list>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "Player.h"
#include "Soldier.h"

#include <GL/glew.h>
#include <GL/glut.h>


#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 3
#define INIT_PLAYER_Y_TILES 3


Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


void Scene::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	enemymanager = new EnemyManager();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	enemymanager->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map);

	projlist = new list<Projectile>();




	left = top = 0;
	right = float(SCREEN_WIDTH - 1) / 2;
	bottom = float(SCREEN_HEIGHT - 1) / 2;
	projection = glm::ortho(left, right, bottom, top);
	currentTime = 0.0f;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	list<Projectile>::iterator it;
	if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && 
		player->getPos().x >= (right + left) / 2 && 
		right <= (map->getMapSize().x * map->getTileSize() +5*map->getTileSize())) {
		
		right += PLAYER_VEL;
		left += PLAYER_VEL;
	}
	player->update(deltaTime, left);
	if (Game::instance().getKey('a')) {
		if (projlist->size() < 4)
			spawnProjectile(player->getPos());
	}

	enemymanager->update(deltaTime, left, right, bottom, top);
	

	//checkPhysics();
	for (it = projlist->begin(); it != projlist->end(); ++it) {
		it->update(deltaTime);
	}
		
}

void Scene::render()
{
	projection = glm::ortho(left, right, bottom, top);
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();
	player->render();
	enemymanager->render();
	
	list<Projectile>::iterator it;
	for (it = projlist->begin(); it != projlist->end(); ++it) {
		it->render();
	}
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

bool Scene::isOffScreen(Projectile &pj) 
{
	if (pj.getPos().x < left || pj.getPos().x > right || pj.getPos().y < top || pj.getPos().y > bottom) return true;
	return false;
}

bool Scene::isOffScreen(Enemy &pj)
{
	if (pj.getPos().x < left || pj.getPos().x > right) return true;
	return false;
}

void Scene::spawnProjectile(glm::ivec2 position) 
{
	projectile = new Projectile();
	projectile->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, player->sprite->animation());
	projectile->setPosition(glm::vec2(position.x + 16, position.y + 32));
	projectile->setTileMap(map);
	projlist->push_back(*(projectile));
}

void Scene::despawnOffScreenProjectiles()
{
	list<Projectile>::iterator it = projlist->begin();
	while (it != projlist->end()) {
		if (isOffScreen((*it))) 
			it = projlist->erase(it);
		else ++it;
	}
}

void Scene::despawnOffScreenEnemies() {
	list<Enemy*>::iterator it;
	for (it = enemies->begin(); it != enemies->end(); ++it) {
		if (isOffScreen(*(*it))) it = enemies->erase(it);
		else ++it;
	}
}

bool Scene::areTouching(glm::ivec2 lpos1, glm::ivec2 rpos1, glm::ivec2 lpos2, glm::ivec2 rpos2)
{
	if (lpos1.x > rpos2.x || lpos2.x > rpos1.x) return false;
	if (lpos1.y < rpos2.y || lpos2.y < rpos1.y) return false;
	return true;
}

void Scene::checkPhysics() 
{
	list<Enemy*>::iterator it_enemy;
	list<Projectile>::iterator it_projec = projlist->begin();

	while (it_projec != projlist->end()) {
		if (isOffScreen((*it_projec)))
			it_projec = projlist->erase(it_projec);
		else {
			glm::ivec2 lpos1 = (it_projec)->getPos() + glm::ivec2{ 8,-7 };
			glm::ivec2 rpos1 = lpos1 + glm::ivec2{ 2,-2 };
			it_enemy = enemies->begin();
			bool shot = false;
			while ( it_enemy != enemies->end()) {
				glm::ivec2 lpos2 = (*it_enemy)->getPos();
				glm::ivec2 rpos2 = lpos2 + glm::ivec2{ 16,-32 };
				if (areTouching(lpos1, rpos1, lpos2, rpos2)) {
					it_enemy = enemies->erase(it_enemy);
					shot = true;
				}
				else ++it_enemy;
			}
			if (shot) it_projec = projlist->erase(it_projec);
			else ++it_projec;
		}
	}

}





