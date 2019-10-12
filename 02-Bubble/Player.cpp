#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4
#define GLUT_KEY_SPACEBAR 32

enum States
{
	ALIVE, DEAD, DYING
};


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, MOVE_LEFT_AIM, MOVE_RIGHT_AIM, AIM_UP_LOOK_LEFT, AIM_UP_LOOK_RIGHT, CROUCH_LOOK_LEFT, 
	CROUCH_LOOK_RIGHT, AIM_UP_WALK_RIGHT, AIM_UP_WALK_LEFT, AIM_DOWN_WALK_RIGHT, AIM_DOWN_WALK_LEFT, AIRBONE_LEFT, AIRBONE_RIGHT
};

enum LookingTo
{
	LOOKING_LEFT, LOOKING_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	bShooting = false;
	lookingTo = LOOKING_RIGHT;
	life = 2;
	state = ALIVE;
	spritesheet.loadFromFile("images/blueguy.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(32, 64), glm::vec2(0.1f, 0.1f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(100);
	
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.1f, 0.f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0f, 0.0f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.2f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.1f, 0.2f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2f, 0.2f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.3f, 0.2f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.4f, 0.2f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.2f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.1f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1f, 0.1f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.2f, 0.1f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.3f, 0.1f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.4f, 0.1f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.1f));

		sprite->setAnimationSpeed(MOVE_LEFT_AIM, 8);
		sprite->addKeyframe(MOVE_LEFT_AIM, glm::vec2(0.7f, 0.2f));
		sprite->addKeyframe(MOVE_LEFT_AIM, glm::vec2(0.8f, 0.2f));
		sprite->addKeyframe(MOVE_LEFT_AIM, glm::vec2(0.9f, 0.2f));

		sprite->setAnimationSpeed(MOVE_RIGHT_AIM, 8);
		sprite->addKeyframe(MOVE_RIGHT_AIM, glm::vec2(0.7f, 0.1f));
		sprite->addKeyframe(MOVE_RIGHT_AIM, glm::vec2(0.8f, 0.1f));
		sprite->addKeyframe(MOVE_RIGHT_AIM, glm::vec2(0.9f, 0.1f));

		sprite->setAnimationSpeed(AIM_UP_LOOK_LEFT, 8);
		sprite->addKeyframe(AIM_UP_LOOK_LEFT, glm::vec2(0.50f, 0.0f));

		sprite->setAnimationSpeed(AIM_UP_LOOK_RIGHT, 8);
		sprite->addKeyframe(AIM_UP_LOOK_RIGHT, glm::vec2(0.40f, 0.0f));

		sprite->setAnimationSpeed(AIM_UP_WALK_RIGHT, 8);
		sprite->addKeyframe(AIM_UP_WALK_RIGHT, glm::vec2(0.0f, 0.5f));
		sprite->addKeyframe(AIM_UP_WALK_RIGHT, glm::vec2(0.1f, 0.5f));
		sprite->addKeyframe(AIM_UP_WALK_RIGHT, glm::vec2(0.2f, 0.5f));

		sprite->setAnimationSpeed(AIM_UP_WALK_LEFT, 8);
		sprite->addKeyframe(AIM_UP_WALK_LEFT, glm::vec2(0.3f, 0.5f));
		sprite->addKeyframe(AIM_UP_WALK_LEFT, glm::vec2(0.4f, 0.5f));
		sprite->addKeyframe(AIM_UP_WALK_LEFT, glm::vec2(0.5f, 0.5f));

		sprite->setAnimationSpeed(CROUCH_LOOK_LEFT, 8);
		sprite->addKeyframe(CROUCH_LOOK_LEFT, glm::vec2(0.3f, 0.0f));

		sprite->setAnimationSpeed(CROUCH_LOOK_RIGHT, 8);
		sprite->addKeyframe(CROUCH_LOOK_RIGHT, glm::vec2(0.2f, 0.0f));

		sprite->setAnimationSpeed(AIM_DOWN_WALK_RIGHT, 8);
		sprite->addKeyframe(AIM_DOWN_WALK_RIGHT, glm::vec2(0.0f, 0.6f));
		sprite->addKeyframe(AIM_DOWN_WALK_RIGHT, glm::vec2(0.1f, 0.6f));
		sprite->addKeyframe(AIM_DOWN_WALK_RIGHT, glm::vec2(0.2f, 0.6f));

		sprite->setAnimationSpeed(AIM_DOWN_WALK_LEFT, 8);
		sprite->addKeyframe(AIM_DOWN_WALK_LEFT, glm::vec2(0.3f, 0.6f));
		sprite->addKeyframe(AIM_DOWN_WALK_LEFT, glm::vec2(0.4f, 0.6f));
		sprite->addKeyframe(AIM_DOWN_WALK_LEFT, glm::vec2(0.5f, 0.6f));

		sprite->setAnimationSpeed(AIRBONE_RIGHT, 8);
		sprite->addKeyframe(AIRBONE_RIGHT, glm::vec2(0.0f, 0.3f));
		sprite->addKeyframe(AIRBONE_RIGHT, glm::vec2(0.1f, 0.3f));
		sprite->addKeyframe(AIRBONE_RIGHT, glm::vec2(0.2f, 0.3f));
		sprite->addKeyframe(AIRBONE_RIGHT, glm::vec2(0.3f, 0.3f));

		sprite->setAnimationSpeed(AIRBONE_LEFT, 8);
		sprite->addKeyframe(AIRBONE_LEFT, glm::vec2(0.0f, 0.4f));
		sprite->addKeyframe(AIRBONE_LEFT, glm::vec2(0.1f, 0.4f));
		sprite->addKeyframe(AIRBONE_LEFT, glm::vec2(0.2f, 0.4f));
		sprite->addKeyframe(AIRBONE_LEFT, glm::vec2(0.3f, 0.4f));
		
	sprite->changeAnimation(1);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime, float left, float right, float bottom, float top)
{
	
	currentKeyframe = sprite->keyframe();
	sprite->update(deltaTime);
	if (state == ALIVE) {
		if (bJumping) // lookingTo es una variable que sactualitza segons si el player mira left or right, l'utilitzo per estalviar mirar cada sprite individualment si es left or right
		{
			if (lookingTo == LOOKING_RIGHT)
				if (sprite->animation() != AIRBONE_RIGHT)
					sprite->changeAnimation(AIRBONE_RIGHT);
			if (lookingTo == LOOKING_LEFT)
				if (sprite->animation() != AIRBONE_LEFT)
					sprite->changeAnimation(AIRBONE_LEFT);

		}
		if (Game::instance().getKey('a')) {
			bShooting = true;
		}
		else bShooting = false;
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && Game::instance().getSpecialKey(GLUT_KEY_UP))
		{
			if (sprite->animation() != AIM_UP_WALK_LEFT && !bJumping)
				sprite->changeAnimation(AIM_UP_WALK_LEFT);
			posPlayer.x -= 2;
			lookingTo = LOOKING_LEFT;
			if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 58)) || posPlayer.x - 2 <= left)
			{
				posPlayer.x += 2;
				sprite->changeAnimation(STAND_LEFT);
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && Game::instance().getSpecialKey(GLUT_KEY_UP))
		{
			if (sprite->animation() != AIM_UP_WALK_RIGHT && !bJumping)
				sprite->changeAnimation(AIM_UP_WALK_RIGHT);
			posPlayer.x += 2;
			lookingTo = LOOKING_RIGHT;
			if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 58)))
			{
				posPlayer.x -= 2;
				sprite->changeAnimation(STAND_RIGHT);
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && Game::instance().getSpecialKey(GLUT_KEY_DOWN))
		{
			if (sprite->animation() != AIM_DOWN_WALK_LEFT && !bJumping)
				sprite->changeAnimation(AIM_DOWN_WALK_LEFT);
			posPlayer.x -= 2;
			lookingTo = LOOKING_LEFT;
			if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 58)) || posPlayer.x - 2 <= left)
			{
				posPlayer.x += 2;
				sprite->changeAnimation(STAND_LEFT);
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && Game::instance().getSpecialKey(GLUT_KEY_DOWN))
		{
			if (sprite->animation() != AIM_DOWN_WALK_RIGHT && !bJumping)
				sprite->changeAnimation(AIM_DOWN_WALK_RIGHT);
			posPlayer.x += 2;
			lookingTo = LOOKING_RIGHT;
			if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 58)))
			{
				posPlayer.x -= 2;
				sprite->changeAnimation(STAND_RIGHT);
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			if ((sprite->animation() != MOVE_LEFT && !bShooting || sprite->animation() != MOVE_LEFT_AIM && bShooting) && !bJumping)
				if (bShooting)
					sprite->changeAnimation(MOVE_LEFT_AIM);
				else
					sprite->changeAnimation(MOVE_LEFT);
			posPlayer.x -= 2;
			lookingTo = LOOKING_LEFT;
			if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 58)) || posPlayer.x - 2 <= left)
			{
				posPlayer.x += 2;
				if (bJumping) {
					if (sprite->animation() != AIRBONE_LEFT)
						sprite->changeAnimation(AIRBONE_LEFT);
				}
				else sprite->changeAnimation(STAND_LEFT);
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
		{
			if ((sprite->animation() != MOVE_RIGHT && !bShooting || sprite->animation() != MOVE_RIGHT_AIM && bShooting) && !bJumping)
				if (bShooting)
					sprite->changeAnimation(MOVE_RIGHT_AIM);
				else
					sprite->changeAnimation(MOVE_RIGHT);
			posPlayer.x += 2;
			lookingTo = LOOKING_RIGHT;
			if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 58)))
			{
				posPlayer.x -= 2;
				sprite->changeAnimation(STAND_RIGHT);
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_UP))
		{
			if (lookingTo == LOOKING_LEFT && !bJumping)
				sprite->changeAnimation(AIM_UP_LOOK_LEFT);
			else if (lookingTo == LOOKING_RIGHT && !bJumping)
				sprite->changeAnimation(AIM_UP_LOOK_RIGHT);
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
		{
			if (lookingTo == LOOKING_LEFT && !bJumping)
				sprite->changeAnimation(CROUCH_LOOK_LEFT);
			else if (lookingTo == LOOKING_RIGHT && !bJumping)
				sprite->changeAnimation(CROUCH_LOOK_RIGHT);
		}
		else
		{
			if (sprite->animation() == MOVE_LEFT)
				sprite->changeAnimation(STAND_LEFT);
			else if (sprite->animation() == MOVE_RIGHT)
				sprite->changeAnimation(STAND_RIGHT);
			else if (sprite->animation() == MOVE_LEFT_AIM)
				sprite->changeAnimation(STAND_LEFT);
			else if (sprite->animation() == MOVE_RIGHT_AIM)
				sprite->changeAnimation(STAND_RIGHT);
			else if (sprite->animation() == AIM_UP_LOOK_LEFT)
				sprite->changeAnimation(STAND_LEFT);
			else if (sprite->animation() == AIM_UP_LOOK_RIGHT)
				sprite->changeAnimation(STAND_RIGHT);
			else if (sprite->animation() == AIM_UP_WALK_LEFT)
				sprite->changeAnimation(STAND_LEFT);
			else if (sprite->animation() == AIM_UP_WALK_RIGHT)
				sprite->changeAnimation(STAND_RIGHT);
			else if (sprite->animation() == CROUCH_LOOK_LEFT)
				sprite->changeAnimation(STAND_LEFT);
			else if (sprite->animation() == CROUCH_LOOK_RIGHT)
				sprite->changeAnimation(STAND_RIGHT);
			else if (sprite->animation() == AIM_DOWN_WALK_LEFT)
				sprite->changeAnimation(STAND_LEFT);
			else if (sprite->animation() == AIM_DOWN_WALK_RIGHT)
				sprite->changeAnimation(STAND_RIGHT);
			else if (sprite->animation() == AIRBONE_LEFT && !bJumping)
				sprite->changeAnimation(STAND_LEFT);
			else if (sprite->animation() == AIRBONE_RIGHT && !bJumping)
				sprite->changeAnimation(STAND_RIGHT);

		}

		if (bJumping)
		{
			jumpAngle += JUMP_ANGLE_STEP;
			if (jumpAngle == 180)
			{
				bJumping = false;
				posPlayer.y = startY;
			}
			else
			{
				posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
				glm::ivec2 aux = posPlayer + glm::ivec2(8, 44);
				if (jumpAngle > 90)
					bJumping = !map->collisionMoveDown(aux, glm::ivec2(12, 20), &posPlayer.y);
				posPlayer = aux + glm::ivec2(-8, -44);
			}
		}
		else
		{
			posPlayer.y += FALL_STEP;
			glm::ivec2 aux = posPlayer + glm::ivec2(10, 30);
			if (map->collisionMoveDown(aux, glm::ivec2(10, 28), &aux.y))
			{
				if (Game::instance().getKey(' '))
				{
					bJumping = true;
					jumpAngle = 0;
					startY = posPlayer.y;
				}
			}
			posPlayer = aux + glm::ivec2(-10, -30);
		}
	}
	else if (state == DEAD) {
		posPlayer.y = top + 20;
		posPlayer.x = left + 20;
		state = ALIVE;
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::ivec2 Player::getPos()
{
	return posPlayer;
}

vector<glm::ivec2> Player::buildHitBox()
{
	if (sprite->animation() == CROUCH_LOOK_LEFT || sprite->animation() == CROUCH_LOOK_RIGHT) {
		glm::ivec2 lpos1 = posPlayer + glm::ivec2{ 4,50 };
		glm::ivec2 rpos1 = posPlayer + glm::ivec2{ 28,63 };
		return vector<glm::ivec2> {lpos1, rpos1 };
	}
	else if (!bJumping) {
		glm::ivec2 lpos1 = posPlayer + glm::ivec2{ 10,30 };
		glm::ivec2 rpos1 = lpos1 + glm::ivec2{ 10,33 };
		return vector<glm::ivec2> {lpos1, rpos1 };
	}
	else {
		glm::ivec2 lpos1 = posPlayer + glm::ivec2{ 6,46 };
		glm::ivec2 rpos1 = posPlayer + glm::ivec2{ 20,63 };
		return vector<glm::ivec2> {lpos1, rpos1 };
	}
}

glm::ivec2 Player::getProjectileSpawn() {
	if (sprite->animation() == MOVE_LEFT_AIM || sprite->animation() == STAND_LEFT)
		return glm::ivec2{ -2, 32 };
	else if (sprite->animation() == MOVE_RIGHT_AIM || sprite->animation() == STAND_RIGHT)
		return glm::ivec2{ 18, 32 };
	else if (sprite->animation() == AIM_UP_LOOK_LEFT)
		return glm::ivec2{ 4, 18 };
	else if (sprite->animation() == AIM_UP_LOOK_RIGHT)
		return glm::ivec2{ 12, 18 };
	else if (sprite->animation() == AIM_UP_WALK_LEFT)
		return glm::ivec2{ 5, 28 };
	else if (sprite->animation() == AIM_UP_WALK_RIGHT)
		return glm::ivec2{ 11, 28 };
	else if (sprite->animation() == AIM_DOWN_WALK_LEFT)
		return glm::ivec2{ 5, 52 };
	else if (sprite->animation() == AIM_DOWN_WALK_RIGHT)
		return glm::ivec2{ 28, 52 };
	else if (sprite->animation() == CROUCH_LOOK_LEFT)
		return glm::ivec2{ -4, 46 };
	else if (sprite->animation() == CROUCH_LOOK_RIGHT)
		return glm::ivec2{ 21, 46 };
	else return glm::ivec2{ 10, 46 };
}