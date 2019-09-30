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
	if (bJumping) // lookingTo es una variable que sactualitza segons si el player mira left or right, l'utilitzo per estalviar mirar cada sprite individualment si es left or right
	{
		if (lookingTo == LOOKING_RIGHT)
			if (sprite->animation() != AIRBONE_RIGHT)
				sprite->changeAnimation(AIRBONE_RIGHT);
		if (lookingTo == LOOKING_LEFT)
			if (sprite->animation() != AIRBONE_LEFT)
				sprite->changeAnimation(AIRBONE_LEFT);

	}

	if (false) {

	}
	else {
		if (sprite->animation() == AIRBONE_LEFT && !bJumping)
			sprite->changeAnimation(MOVE_LEFT);
		else if (sprite->animation() == AIRBONE_RIGHT && !bJumping)
			sprite->changeAnimation(MOVE_RIGHT);
	}
	if (bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle == 180)
		{
			bJumping = false;
			posEnemy.y = startY;
		}
		else
		{
			posEnemy.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle > 90)
				bJumping = !map->collisionMoveDown(posEnemy, glm::ivec2(16,32), &posEnemy.y);
		}
	}
	else
	{
		posEnemy.y += FALL_STEP;
		if (map->collisionMoveDown(posEnemy, glm::ivec2(16, 32), &posEnemy.y))
		{
			if ((rand() % 1000) == 1)
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posEnemy.y;
			}
		}
	}
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