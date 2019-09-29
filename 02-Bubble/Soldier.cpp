#include "Soldier.h"
#include "Game.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define GLUT_KEY_SPACEBAR 32
#define PLAYER_VEL 2

enum SoldierAnims
{
	MOVE_LEFT, MOVE_RIGHT, AIRBONE_LEFT, AIRBONE_RIGHT
};

enum LookingTo
{
	LOOKING_LEFT, LOOKING_RIGHT
};

void Soldier::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	lookingTo = LOOKING_LEFT;
	spritesheet.loadFromFile("images/soldier.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 32), glm::vec2(0.1f, 0.25f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(100);

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.1f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.3f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.4f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.25f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.2f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.3f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.4f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.0f));

	sprite->setAnimationSpeed(AIRBONE_RIGHT, 8);
	sprite->addKeyframe(AIRBONE_RIGHT, glm::vec2(0.0f, 0.50f));

	sprite->setAnimationSpeed(AIRBONE_LEFT, 8);
	sprite->addKeyframe(AIRBONE_LEFT, glm::vec2(0.0f, 0.75f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Soldier::update(int deltaTime, float left)
{
	sprite->update(deltaTime);
	if (sprite->animation() != MOVE_RIGHT)
		sprite->changeAnimation(MOVE_RIGHT);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Soldier::render()
{
	sprite->render();
}

void Soldier::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Soldier::setPosition(const glm::vec2 &pos)
{
	posEnemy = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

glm::ivec2 Soldier::getPos()
{
	return posEnemy;
}