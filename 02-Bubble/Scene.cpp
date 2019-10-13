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

#define SELECT_DELAY 8
#define BLINK_ANIMATION_DURATION 10
#define BLINKS 6
#define LVL2_ANIMATION_DURATION 10


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
	currentState = LOADING_MENU;
	map = TileMap::createTileMap("levels/menu.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	left = - float(SCREEN_WIDTH -1)/2;
	top = 0;
	right = 0;
	bottom = float(SCREEN_HEIGHT - 1) / 2;
	projection = glm::ortho(left, right, bottom, top);

	currentTime = 0.0f;
	selectDelay = SELECT_DELAY;
	onePlayer = true;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;

	// godmode shortcuts
	godMode();

	// updates segons la scene
	switch (currentState)
	{
	case LOADING_MENU: {
		if (left < 0) {
			left += 2;
			right += 2;
		}
		else currentState = MENU;
		break;
	}
	case MENU: updateMenu(deltaTime); break;
	case MENU_TO_LVL1: {
		if (--blinkAnimation == 0 && blinks-- > 0) {
			switch (map->getFrame()) {
			case MENU_1_PLAYER: map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, MENU_1_PLAYER_BLINK); break;
			case MENU_1_PLAYER_BLINK: map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, MENU_1_PLAYER); break;
			case MENU_2_PLAYER: map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, MENU_2_PLAYER_BLINK); break;
			case MENU_2_PLAYER_BLINK: map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, MENU_2_PLAYER); break;
			}
			blinkAnimation = BLINK_ANIMATION_DURATION;
		}
		else if (blinks == 0) changeToScene(LVL1);
		break;
	}
	case LVL1: updateLvl1(deltaTime); break;
	case LVL2: updateLvl2(deltaTime); break;
	case LVL2_ANIMATION: {
		if (--subLevelAnimation == 0 && subLvl++ <= 4) {
			switch (map->getFrame()) {
			case SLVL1: map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, ANIM1); break;
			case SLVL3: map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, ANIM1); break;
			case SLVL4: map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, ANIM1); break;
			case SLVL5: map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, ANIM1); break;
			case BOSS: map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, ANIM1); break;

			case ANIM1: map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, ANIM2); break;
			case ANIM2: map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, ANIM3); break;
			case ANIM3: map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, ANIM4); break;
			}
			subLevelAnimation = LVL2_ANIMATION_DURATION;
		}
		else if (subLvl > 4) currentState = LVL2;
		break;
	}
	}
		
}

void Scene::updateMenu(int deltaTime) {
	// click start to toggle 1 or 2 players
	if (--selectDelay == 0) {
		if (Game::instance().getKey(','))
			map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram,
				((map->getFrame() == MENU_1_PLAYER) ? MENU_2_PLAYER : MENU_1_PLAYER));
		// start game with enter
		if (Game::instance().getKey('.')) {
			onePlayer = map->getFrame() == MENU_1_PLAYER;
			currentState = MENU_TO_LVL1;
			blinks = BLINKS;
			blinkAnimation = BLINK_ANIMATION_DURATION;
		}
		selectDelay = SELECT_DELAY;
	}
}

void Scene::updateLvl1(int deltaTime) {

	if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) &&
		player->getPos().x >= (right + left) / 2 &&
		right <= (map->getMapSize().x * map->getTileSize() + 5 * map->getTileSize())) {

		right += PLAYER_VEL;
		left += PLAYER_VEL;
	}
	player->update(deltaTime, left, right, bottom, top);

	enemymanager->update(deltaTime, left, right, bottom, top);
	
	
}

void Scene::updateLvl2(int deltaTime) {
	left = 0; right = float(SCREEN_WIDTH - 1) / 2;
	player->update(deltaTime, left, right, bottom, top);

	enemymanager->update(deltaTime, left, right, bottom, top);

	if (Game::instance().getKey('x')) {
		currentState = LVL2_ANIMATION;
		subLevelAnimation = LVL2_ANIMATION_DURATION;
	}
}

void Scene::changeToScene(int scene) {
	currentState = scene;
	switch (currentState) {
	case MENU: {
		break;
	}
	case LVL1: {
		// TODO animation de menu a lvl1
		map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		initEntitiesLvl1();
		break;
	}
	case LVL2: {
		map = TileMap::createTileMap("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		subLvl = 0;
		initEntitiesLvl2();
		break;
	}
	}
}

void Scene::godMode() {
	if (Game::instance().getKey('1')) {
		left = 0; right = float(SCREEN_WIDTH - 1) / 2;
		map = TileMap::createTileMap("levels/menu.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		currentState = MENU;
	}
	if (Game::instance().getKey('2')) {
		left = 0; right = float(SCREEN_WIDTH - 1) / 2;
		currentState = LVL1;
		map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		initEntitiesLvl1();
	}
	if (Game::instance().getKey('3')) {
		left = 0; right = float(SCREEN_WIDTH - 1) / 2;
		currentState = LVL2;
		map = TileMap::createTileMap("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		initEntitiesLvl2();
	}
	// TODO click posa flag immortal al personatge
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

void Scene::initEntitiesLvl1() {
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(3 * map->getTileSize(), 0 * map->getTileSize()));
	player->setTileMap(map);

	enemymanager = new EnemyManager();
	enemymanager->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map, player);

}

void Scene::initEntitiesLvl2() {
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(128, 124));
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
