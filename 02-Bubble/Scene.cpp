#include <iostream>
#include <cmath>
#include <list>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "Player.h"
#include "Soldier.h"
#include "Windows.h"

#include <GL/glew.h>
#include <GL/glut.h>


#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 3
#define INIT_PLAYER_Y_TILES 3

#define SELECT_DELAY 8
#define PAUSE_DELAY 6
#define BLINK_ANIMATION_DURATION 10
#define BLINKS 6
#define LVL2_ANIMATION_DELAY 8

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

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
	if (audiomanager != NULL)
		delete audiomanager;
}


void Scene::init()
{
	_2Playermode = true;
	godmode = false;
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
	lvl2Delay = LVL2_ANIMATION_DELAY;
	pauseDelay = PAUSE_DELAY;
	paused = false;

	score1 = score2 = 0;
	
	if (!score1Text.init("fonts/OpenSans-Regular.ttf"))
		_RPT0(0, "falla el init");

	audiomanager = new Audio();
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;

	// godmode shortcuts
	godMode();

	if (--pauseDelay == 0) {
		if (Game::instance().getKey('p')) paused = !paused;
		pauseDelay = PAUSE_DELAY;
	}

	if (!paused) {
		// updates segons la scene
		switch (currentState)
		{
		case LOADING_MENU: {
			if (left < 0) {
				left += 2;
				right += 2;
			}
			else changeToScene(MENU);
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

				case MENU_HOW_TO_PLAY: map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, MENU_HOW_TO_PLAY_BLINK); break;
				case MENU_HOW_TO_PLAY_BLINK: map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, MENU_HOW_TO_PLAY); break;
				case MENU_CREDITS: map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, MENU_CREDITS_BLINK); break;
				case MENU_CREDITS_BLINK: map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, MENU_CREDITS); break;
				}
				blinkAnimation = BLINK_ANIMATION_DURATION;
			}
			else if (blinks == 0) {
				if (map->getFrame() == MENU_1_PLAYER || map->getFrame() == MENU_2_PLAYER)
					changeToScene(LVL1);
				else if (map->getFrame() == MENU_HOW_TO_PLAY)
					changeToScene(HOW_TO);
				else if (map->getFrame() == MENU_CREDITS)
					changeToScene(CREDITS);
			}
			break;
		}
		case HOW_TO: {
			if (Game::instance().getKey(' ')) {
				changeToScene(LOADING_MENU);
			}
			break;
		}
		case CREDITS: {
			if (Game::instance().getKey(' '))
				changeToScene(LOADING_MENU);
			break;
		}
		case END: {
			if (Game::instance().getKey(' '))
				changeToScene(LOADING_MENU);
			break;
		}
		case LVL1: updateLvl1(deltaTime); break;
		case LVL2: updateLvl2(deltaTime); break;
		case LVL2_ANIMATION: {
			lvl2AnimationDoor(deltaTime); break;
		}
		case GAMEOVER: updateGameover(deltaTime); break; 
		case GAMEOVER_ANIMATION: {
			if (--blinkAnimation == 0 && blinks-- > 0) {
				switch (map->getFrame()) {
				case GAMEOVER_CONTINUE: map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, GAMEOVER_CONTINUE_BLINK); break;
				case GAMEOVER_CONTINUE_BLINK: map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, GAMEOVER_CONTINUE); break;
				case GAMEOVER_END: map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, GAMEOVER_END_BLINK); break;
				case GAMEOVER_END_BLINK: map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, GAMEOVER_END); break;
				}
				blinkAnimation = BLINK_ANIMATION_DURATION;
			}
			else if (blinks == 0) {
				if (map->getFrame() == GAMEOVER_CONTINUE || map->getFrame() == GAMEOVER_CONTINUE_BLINK) changeToScene(LOADING_MENU);
				else changeToScene(CREDITS);
			}
			break;
		}
		}
	}

		
}

void Scene::updateMenu(int deltaTime) {
	// click start to toggle 1 or 2 players
	if (--selectDelay == 0) {
		if (Game::instance().getKey(','))
			map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram,
				((map->getFrame()+2) % 8));
		// start game with enter
		if (Game::instance().getKey('.')) {
			_2Playermode = !map->getFrame() == MENU_1_PLAYER;
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
		right <= (map->getMapSize().x * map->getTileSize())) {

		right += PLAYER_VEL;
		left += PLAYER_VEL;
	}
	player->update(deltaTime, left, right, bottom, top);
	life->update(deltaTime, left, right, bottom, top, player->life);
	if (_2Playermode) {
		player2->update(deltaTime, left, right, bottom, top);
		life2->update(deltaTime, left, right, bottom, top, player2->life);
		if (player2->life <= 0) {
			changeToScene(GAMEOVER);
		}
		else if (player->life <= 0) {
			changeToScene(GAMEOVER);
		}

		else if (player->getPos().x + 20 >= map->getMapSize().x * map->getTileSize()) {
			changeToScene(LVL2);
		}
	}

	else if (player->life <= 0) {
		changeToScene(GAMEOVER);
	}

	else if (player->getPos().x + 20 >= map->getMapSize().x * map->getTileSize()) {
		changeToScene(LVL2);
	}
	enemymanager->godmode = godmode;
	enemymanager->update(deltaTime, left, right, bottom, top);
}

void Scene::updateLvl2(int deltaTime) {
	player->update(deltaTime, left, right, bottom, top);
	if (_2Playermode) {
		player2->update(deltaTime, left, right, bottom, top);
		life2->update(deltaTime, left, right, bottom, top, player2->life);
		if (player2->life <= 0) {
			currentState = GAMEOVER;
			changeToScene(GAMEOVER);
		}
	}
	enemymanager->godmode = godmode;
	enemymanager->updateLvl2(deltaTime, left, right, bottom, top);

	life->update(deltaTime, left, right, bottom, top, player->life);

	if (enemymanager->coreDestroyed) {
		map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, ANIM1);
		currentState = LVL2_ANIMATION;
	}
	if (player->life <= 0) {
		currentState = GAMEOVER;
		changeToScene(GAMEOVER);
	}
}

void Scene::updateGameover(int deltaTime) {
	if (--selectDelay == 0) {
		if (Game::instance().getKey(','))
			map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram,
			((map->getFrame() == GAMEOVER_CONTINUE) ? GAMEOVER_END : GAMEOVER_CONTINUE));
		// start game with enter
		if (Game::instance().getKey('.')) {
			currentState = GAMEOVER_ANIMATION;
			blinks = BLINKS;
			blinkAnimation = BLINK_ANIMATION_DURATION;
		}
		selectDelay = SELECT_DELAY;
	}
}

void Scene::lvl2AnimationDoor(int deltaTime) {
	player->update(deltaTime, left, right, bottom, top);
	if (_2Playermode) player2->update(deltaTime, left, right, bottom, top);
	if (--lvl2Delay == 0) {
		if (Game::instance().getSpecialKey(GLUT_KEY_UP) || Game::instance().getKey('w') && enemymanager->coreDestroyed) {
			switch (map->getFrame()) {
			case ANIM1: map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, ANIM2); break;
			case ANIM2: map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, ANIM3); break;
			case ANIM3: map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, ANIM4); break;
			case ANIM4: {
				enemymanager->setSublvl(enemymanager->getSublvl() + 1);
				enemymanager->coreDestroyed = false;
				player->coreDestroyed = false;
				if (_2Playermode) player2->coreDestroyed = false;
				switch (enemymanager->getSublvl()) {
				case 0:  map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, SLVL1); break;
				case 1:  map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, SLVL1); break;
				case 2:  map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, SLVL3); break;
				case 3:  map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, SLVL4); break;
				case 4:  map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, SLVL5); break;
				case 5: {
					map->toggleFrame(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, BOSS); 
					player->setPosition(glm::vec2(player->getPos().x, 200));
					audiomanager->stopAllSounds();
					audiomanager->play(BASEBOSS_MUSIC, true);
					break;
				}
				}
				currentState = LVL2;
				break;
			}
			}
		}
		lvl2Delay = LVL2_ANIMATION_DELAY;
	}
}

void Scene::changeToScene(int scene) {
	currentState = scene;
	switch (currentState) {
	case LOADING_MENU : {
		map = TileMap::createTileMap("levels/menu.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		left = -float(SCREEN_WIDTH - 1) / 2;
		right = 0;
		break;
	}
	case MENU: {
		audiomanager->play(TITLE_MUSIC, false);
		break;
	}
	case HOW_TO: {
		map = TileMap::createTileMap("levels/howto.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		break;
	}
	case CREDITS: {
		map = TileMap::createTileMap("levels/credits.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		break;
	}
	case LVL1: {
		map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		initEntitiesLvl1();
		audiomanager->stopAllSounds();
		audiomanager->play(STAGE1_MUSIC, true);
		break;
	}
	case LVL2: {
		map = TileMap::createTileMap("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		initEntitiesLvl2();
		left = 0;
		right = float(SCREEN_WIDTH - 1) / 2;
		audiomanager->stopAllSounds();
		audiomanager->play(STAGE2_MUSIC, true);
		break;
	}
	case GAMEOVER: {
		map = TileMap::createTileMap("levels/gameover.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		left = 0;
		right = float(SCREEN_WIDTH - 1) / 2;
		audiomanager->stopAllSounds();
		audiomanager->play(GAMEOVER_MUSIC, true);
		break;
	}
	case END: {
		map = TileMap::createTileMap("levels/endgame.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		left = 0; right = float(SCREEN_WIDTH - 1) / 2;
		audiomanager->stopAllSounds();
	}
	}
}

void Scene::godMode() {
	if (Game::instance().getKey('1')) {
		left = 0; right = float(SCREEN_WIDTH - 1) / 2;
		map = TileMap::createTileMap("levels/menu.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		currentState = MENU;
		audiomanager->stopAllSounds();
		audiomanager->play(TITLE_MUSIC, false);
	}
	if (Game::instance().getKey('2')) {
		left = 0; right = float(SCREEN_WIDTH - 1) / 2;
		currentState = LVL1;
		map = TileMap::createTileMap("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		initEntitiesLvl1();
		audiomanager->stopAllSounds();
		audiomanager->play(STAGE1_MUSIC, true);
	}
	if (Game::instance().getKey('3')) {
		left = 0; right = float(SCREEN_WIDTH - 1) / 2;
		currentState = LVL2;
		map = TileMap::createTileMap("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		initEntitiesLvl2();		
		audiomanager->stopAllSounds();
		audiomanager->play(STAGE2_MUSIC, true);
	}
	if (Game::instance().getKey('4')) {
		left = 0; right = float(SCREEN_WIDTH - 1) / 2;
		currentState = HOW_TO;
		map = TileMap::createTileMap("howto/howto.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	}
	if (Game::instance().getKey('5')) {
		left = 0; right = float(SCREEN_WIDTH - 1) / 2;
		currentState = CREDITS;
		map = TileMap::createTileMap("credits/credits.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	}
	if (Game::instance().getKey('0')) {
		left = 0; right = float(SCREEN_WIDTH - 1) / 2;
		map = TileMap::createTileMap("levels/gameover.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		currentState = GAMEOVER;
		audiomanager->stopAllSounds();
		audiomanager->play(GAMEOVER_MUSIC, true);
	}
	if (Game::instance().getKey('9')) {
		godmode = !godmode;
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
	if (currentState == LVL1 || currentState == LVL2 || currentState == LVL2_ANIMATION) {
		if (currentState == LVL1) enemymanager->render();
		if (currentState == LVL2) enemymanager->renderLvl2();
		player->render();
		if (_2Playermode) {
			player2->render();
			life2->render();
		}
		life->render();
	}

	if (currentState == GAMEOVER) {
		score1Text.render(std::to_string(score1), glm::vec2(10, 10), 32, glm::vec4(1, 1, 1, 1));
		score1Text.render('s', glm::vec2(10, 10), 32, glm::vec4(1, 1, 1, 1));
	}
}

void Scene::initEntitiesLvl1() {
	player = new Player();
	player2 = new Player2();
	life = new Life();
	life2 = new Life();
	life->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 0);
	life->setPosition(glm::vec2(3 * map->getTileSize(), 0 * map->getTileSize()));
	life->setTileMap(map);

	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(3 * map->getTileSize(), 0 * map->getTileSize()));
	player->setTileMap(map);

	if (_2Playermode) {
		player2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		player2->setPosition(glm::vec2(3 * map->getTileSize(), 0 * map->getTileSize()));
		player2->setTileMap(map);

		life2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 1);
		life2->setPosition(glm::vec2(9 * map->getTileSize(), 0 * map->getTileSize()));
		life2->setTileMap(map);
	}

	enemymanager = new EnemyManager(audiomanager);
	enemymanager->_2Playermode = _2Playermode;
	enemymanager->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map, player, player2);

}

void Scene::initEntitiesLvl2() {
	player = new Player();
	player2 = new Player2();
	life = new Life();
	life2 = new Life();
	life->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 0);
	life->setPosition(glm::vec2(3 * map->getTileSize(), 0 * map->getTileSize()));
	life->setTileMap(map);

	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(128, 124));
	player->setTileMap(map);

	if (_2Playermode) {
		player2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		player2->setPosition(glm::vec2(128, 124));
		player2->setTileMap(map);

		life2->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 1);
		life2->setPosition(glm::vec2(9 * map->getTileSize(), 0 * map->getTileSize()));
		life2->setTileMap(map);
	}

	enemymanager = new EnemyManager(audiomanager);
	enemymanager->_2Playermode = _2Playermode;
	enemymanager->initLvl2(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map, player, player2);
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
