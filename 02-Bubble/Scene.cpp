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
	currentState = MENU;
	map = TileMap::createTileMap("levels/menu.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	initEntities();

	left = top = 0;
	right = float(SCREEN_WIDTH - 1) / 2;
	bottom = float(SCREEN_HEIGHT - 1) / 2;
	projection = glm::ortho(left, right, bottom, top);
	currentTime = 0.0f;
}

int c = 100;

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;

	// check si volem ser god
	godMode();

	// aixo es com un timer
	if (--c == 0) {
		c = 100;
		//currentState = LVL1;
		// quan es canvia el current state s'ha de canviar el map
	}

	switch (currentState)
	{
	case MENU: updateMenu(deltaTime); break;
	case LVL1: updateLvl1(deltaTime); break;
	}
		
}

void Scene::updateMenu(int deltaTime) {
	// click start to toggle 1 or 2 players
	if (Game::instance().getKey('t'))
		map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
}

void Scene::updateLvl1(int deltaTime) {

	if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) &&
		player->getPos().x >= (right + left) / 2 &&
		right <= (map->getMapSize().x * map->getTileSize() + 5 * map->getTileSize())) {

		right += PLAYER_VEL;
		left += PLAYER_VEL;
	}
	player->update(deltaTime, left);

	enemymanager->update(deltaTime, left, right, bottom, top);
	
	
}

void Scene::godMode() {
	if (Game::instance().getKey('1')) {
		map = TileMap::createTileMap("levels/menu.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		currentState = MENU;
	}
	if (Game::instance().getKey('2')) {
		currentState = LVL1;
		map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		initEntities();
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
	if (currentState == LVL1 || currentState == LVL2) {
		player->render();
		enemymanager->render();
	}
}

void Scene::initEntities() {
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	player->setTileMap(map);

	enemymanager = new EnemyManager();
	enemymanager->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map, player);

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







