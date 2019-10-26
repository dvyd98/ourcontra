#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define JUMP_HEIGHT_2 60
#define FALL_STEP 4
#define GLUT_KEY_SPACEBAR 32

enum States
{
	ALIVE, DEAD, DYING
};


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, MOVE_LEFT_AIM, MOVE_RIGHT_AIM, AIM_UP_LOOK_LEFT, AIM_UP_LOOK_RIGHT, CROUCH_LOOK_LEFT, 
	CROUCH_LOOK_RIGHT, AIM_UP_WALK_RIGHT, AIM_UP_WALK_LEFT, AIM_DOWN_WALK_RIGHT, AIM_DOWN_WALK_LEFT, AIRBONE_LEFT, AIRBONE_RIGHT,
	DROPPED, UNDERWATER, SWIM_LEFT, SWIM_RIGHT, SWIM_AIM_LEFT, SWIM_AIM_RIGHT, SWIM_AIM_UPRIGHT, SWIM_AIM_UPLEFT, SWIM_AIM_UP_LOOK_LEFT, SWIM_AIM_UP_LOOK_RIGHT,
	ANIM_DYING, ANIM_DEAD, LVL2_ANIM_DYING,
	LVL2_IDLE, LVL2_IDLE_SHOOT, LVL2_CROUCH, LVL2_CROUCH_SHOOT, LVL2_FORWARD, LVL2_MOVE_LEFT, LVL2_MOVE_RIGHT, LVL2_ZAP, LVL2_ANIM_DEAD

};

enum LookingTo
{
	LOOKING_LEFT, LOOKING_RIGHT
};

enum upgradeRank
{
	RIFLEMAN, RANK1, SPREAD
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	bShooting = false;
	bWater = false;
	coreDestroyed = false;
	lvl2boss = false;
	lookingTo = LOOKING_RIGHT;
	life = 99;
	frameCount = 120;
	rolldistance = 40;
	state = ALIVE;
	projectile = RANK1;
	LandedFrame = 15;
	lvl = 1;
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

		sprite->setAnimationSpeed(DROPPED, 8);
		sprite->addKeyframe(DROPPED, glm::vec2(0.0f, 0.7f));

		sprite->setAnimationSpeed(SWIM_LEFT, 8);
		sprite->addKeyframe(SWIM_LEFT, glm::vec2(0.0f, 0.9f));
		sprite->addKeyframe(SWIM_LEFT, glm::vec2(0.1f, 0.9f));

		sprite->setAnimationSpeed(SWIM_RIGHT, 8);
		sprite->addKeyframe(SWIM_RIGHT, glm::vec2(0.0f, 0.8f));
		sprite->addKeyframe(SWIM_RIGHT, glm::vec2(0.1f, 0.8f));

		sprite->setAnimationSpeed(SWIM_AIM_RIGHT, 8);
		sprite->addKeyframe(SWIM_AIM_RIGHT, glm::vec2(0.2f, 0.8f));

		sprite->setAnimationSpeed(SWIM_AIM_UPRIGHT, 8);
		sprite->addKeyframe(SWIM_AIM_UPRIGHT, glm::vec2(0.3f, 0.8f));

		sprite->setAnimationSpeed(SWIM_AIM_UP_LOOK_RIGHT, 8);
		sprite->addKeyframe(SWIM_AIM_UP_LOOK_RIGHT, glm::vec2(0.4f, 0.8f));

		sprite->setAnimationSpeed(SWIM_AIM_LEFT, 8);
		sprite->addKeyframe(SWIM_AIM_LEFT, glm::vec2(0.2f, 0.9f));

		sprite->setAnimationSpeed(SWIM_AIM_UPLEFT, 8);
		sprite->addKeyframe(SWIM_AIM_UPLEFT, glm::vec2(0.3f, 0.9f));

		sprite->setAnimationSpeed(SWIM_AIM_UP_LOOK_LEFT, 8);
		sprite->addKeyframe(SWIM_AIM_UP_LOOK_LEFT, glm::vec2(0.4f, 0.9f));


		sprite->setAnimationSpeed(LVL2_IDLE, 8);
		sprite->addKeyframe(LVL2_IDLE, glm::vec2(0.5f, 0.3f));

		sprite->setAnimationSpeed(LVL2_IDLE_SHOOT, 8);
		sprite->addKeyframe(LVL2_IDLE_SHOOT, glm::vec2(0.4f, 0.3f));
		sprite->addKeyframe(LVL2_IDLE_SHOOT, glm::vec2(0.5f, 0.3f));

		sprite->setAnimationSpeed(LVL2_CROUCH, 8);
		sprite->addKeyframe(LVL2_CROUCH, glm::vec2(0.6f, 0.3f));

		sprite->setAnimationSpeed(LVL2_CROUCH_SHOOT, 8);
		sprite->addKeyframe(LVL2_CROUCH_SHOOT, glm::vec2(0.7f, 0.3f));
		sprite->addKeyframe(LVL2_CROUCH_SHOOT, glm::vec2(0.6f, 0.3f));

		sprite->setAnimationSpeed(LVL2_FORWARD, 8);
		sprite->addKeyframe(LVL2_FORWARD, glm::vec2(0.8f, 0.3f));
		sprite->addKeyframe(LVL2_FORWARD, glm::vec2(0.9f, 0.3f));

		sprite->setAnimationSpeed(LVL2_MOVE_RIGHT, 8);
		sprite->addKeyframe(LVL2_MOVE_RIGHT, glm::vec2(0.4f, 0.4f));
		sprite->addKeyframe(LVL2_MOVE_RIGHT, glm::vec2(0.5f, 0.4f));
		sprite->addKeyframe(LVL2_MOVE_RIGHT, glm::vec2(0.6f, 0.4f));

		sprite->setAnimationSpeed(LVL2_MOVE_LEFT, 8);
		sprite->addKeyframe(LVL2_MOVE_LEFT, glm::vec2(0.7f, 0.4f));
		sprite->addKeyframe(LVL2_MOVE_LEFT, glm::vec2(0.8f, 0.4f));
		sprite->addKeyframe(LVL2_MOVE_LEFT, glm::vec2(0.9f, 0.4f));

		sprite->setAnimationSpeed(LVL2_ZAP, 32);
		sprite->addKeyframe(LVL2_ZAP, glm::vec2(0.6f, 0.5f));
		sprite->addKeyframe(LVL2_ZAP, glm::vec2(0.7f, 0.5f));
		sprite->addKeyframe(LVL2_ZAP, glm::vec2(0.8f, 0.5f));

		sprite->setAnimationSpeed(ANIM_DYING, 8);
		sprite->addKeyframe(ANIM_DYING, glm::vec2(0.5f, 0.7f));
		sprite->addKeyframe(ANIM_DYING, glm::vec2(0.6f, 0.7f));
		sprite->addKeyframe(ANIM_DYING, glm::vec2(0.7f, 0.7f));
		sprite->addKeyframe(ANIM_DYING, glm::vec2(0.8f, 0.7f));
		sprite->addKeyframe(ANIM_DYING, glm::vec2(0.9f, 0.7f));

		sprite->setAnimationSpeed(ANIM_DEAD, 8);
		sprite->addKeyframe(ANIM_DEAD, glm::vec2(0.9f, 0.7f));

		sprite->setAnimationSpeed(LVL2_ANIM_DEAD, 8);
		sprite->addKeyframe(LVL2_ANIM_DEAD, glm::vec2(0.9f, 0.5f));

		sprite->setAnimationSpeed(LVL2_ANIM_DYING, 8);
		sprite->addKeyframe(LVL2_ANIM_DYING, glm::vec2(0.8f, 0.5f));
		sprite->addKeyframe(LVL2_ANIM_DYING, glm::vec2(0.9f, 0.5f));


		
	sprite->changeAnimation(1);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Player::update(int deltaTime, float left, float right, float bottom, float top)
{
	
	currentKeyframe = sprite->keyframe();
	sprite->update(deltaTime);
	if (state == ALIVE) {
		if (invtimer > 0) --invtimer;
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
			wasShooting = true;
		}
		else if (bShooting) bShooting = false;
		else wasShooting = false;

		if (lvl == 1) {
			if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				if (sprite->animation() != AIM_UP_WALK_LEFT && !bJumping) {
					if (bWater) sprite->changeAnimation(SWIM_AIM_UPLEFT);
					else sprite->changeAnimation(AIM_UP_WALK_LEFT);
				}
				posPlayer.x -= 2;
				lookingTo = LOOKING_LEFT;
				if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 58), &posPlayer.y) || posPlayer.x - 2 <= left)
				{
					posPlayer.x += 2;
					if (bJumping) {
						if (sprite->animation() != AIRBONE_LEFT)
							sprite->changeAnimation(AIRBONE_LEFT);
					}
					else if (!bWater) sprite->changeAnimation(STAND_LEFT);
					else sprite->changeAnimation(SWIM_LEFT);
				}
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				if (sprite->animation() != AIM_UP_WALK_RIGHT && !bJumping) {
					if (bWater) sprite->changeAnimation(SWIM_AIM_UPRIGHT);
					else sprite->changeAnimation(AIM_UP_WALK_RIGHT);
				}
				posPlayer.x += 2;
				lookingTo = LOOKING_RIGHT;
				if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 58), &posPlayer.y))
				{
					posPlayer.x -= 2;
					sprite->changeAnimation(STAND_RIGHT);
				}
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT) && Game::instance().getSpecialKey(GLUT_KEY_DOWN))
			{
				if (sprite->animation() != AIM_DOWN_WALK_LEFT && !bJumping && !bWater)
					sprite->changeAnimation(AIM_DOWN_WALK_LEFT);
				else if (bWater && sprite->animation() != SWIM_LEFT) {
					sprite->changeAnimation(SWIM_LEFT);
				}
				posPlayer.x -= 2;
				lookingTo = LOOKING_LEFT;
				if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 58), &posPlayer.y) || posPlayer.x - 2 <= left)
				{
					posPlayer.x += 2;
					if (bJumping) {
						if (sprite->animation() != AIRBONE_LEFT)
							sprite->changeAnimation(AIRBONE_LEFT);
					}
					else if (!bWater) sprite->changeAnimation(STAND_LEFT);
					else sprite->changeAnimation(SWIM_LEFT);
				}
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT) && Game::instance().getSpecialKey(GLUT_KEY_DOWN))
			{
				if (sprite->animation() != AIM_DOWN_WALK_RIGHT && !bJumping && !bWater)
					sprite->changeAnimation(AIM_DOWN_WALK_RIGHT);
				else if (bWater && sprite->animation() != SWIM_RIGHT) {
					sprite->changeAnimation(SWIM_RIGHT);
				}
				posPlayer.x += 2;
				lookingTo = LOOKING_RIGHT;
				if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 58), &posPlayer.y))
				{
					posPlayer.x -= 2;
					sprite->changeAnimation(STAND_RIGHT);
				}
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
			{
				if ((sprite->animation() != MOVE_LEFT && !bShooting || sprite->animation() != MOVE_LEFT_AIM && bShooting) && !bJumping)
					if (bShooting) {
						if (bWater) sprite->changeAnimation(SWIM_AIM_LEFT);
						else sprite->changeAnimation(MOVE_LEFT_AIM, currentKeyframe);
					}
					else
						if (bWater) {
							if (sprite->animation() != SWIM_LEFT) sprite->changeAnimation(SWIM_LEFT);
						}
						else sprite->changeAnimation(MOVE_LEFT);
						posPlayer.x -= 2;
						lookingTo = LOOKING_LEFT;
						if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 58), &posPlayer.y) || posPlayer.x - 2 <= left)
						{
							posPlayer.x += 2;
							if (bJumping) {
								if (sprite->animation() != AIRBONE_LEFT)
									sprite->changeAnimation(AIRBONE_LEFT);
							}
							else if (!bWater) sprite->changeAnimation(STAND_LEFT);
							else sprite->changeAnimation(SWIM_LEFT);
						}
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
			{
				if ((sprite->animation() != MOVE_RIGHT && !bShooting || sprite->animation() != MOVE_RIGHT_AIM && bShooting) && !bJumping)
					if (bShooting) {
						if (bWater) sprite->changeAnimation(SWIM_AIM_RIGHT);
						else sprite->changeAnimation(MOVE_RIGHT_AIM, currentKeyframe);
					}
					else
						if (bWater) {
							if (sprite->animation() != SWIM_RIGHT) sprite->changeAnimation(SWIM_RIGHT);
						}
						else sprite->changeAnimation(MOVE_RIGHT);
						posPlayer.x += 2;
						lookingTo = LOOKING_RIGHT;
						if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 58), &posPlayer.y))
						{
							posPlayer.x -= 2;
							if (!bWater) sprite->changeAnimation(STAND_RIGHT);
							else sprite->changeAnimation(SWIM_RIGHT);
						}
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				if (lookingTo == LOOKING_LEFT && !bJumping) {
					if (bWater) sprite->changeAnimation(SWIM_AIM_UP_LOOK_LEFT);
					else sprite->changeAnimation(AIM_UP_LOOK_LEFT);
				}
				else if (lookingTo == LOOKING_RIGHT && !bJumping) {
					if (bWater) sprite->changeAnimation(SWIM_AIM_UP_LOOK_RIGHT);
					else sprite->changeAnimation(AIM_UP_LOOK_RIGHT);
				}

			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
			{
				if (lookingTo == LOOKING_LEFT && !bJumping && !bWater)
					sprite->changeAnimation(CROUCH_LOOK_LEFT);
				else if (lookingTo == LOOKING_RIGHT && !bJumping && !bWater)
					sprite->changeAnimation(CROUCH_LOOK_RIGHT);
			}
			else
			{
				if (bWater && bShooting) {
					if (lookingTo == LOOKING_LEFT) sprite->changeAnimation(SWIM_AIM_LEFT);
					else sprite->changeAnimation(SWIM_AIM_RIGHT);
				}
				else if (sprite->animation() == SWIM_AIM_UP_LOOK_LEFT)
					sprite->changeAnimation(SWIM_LEFT);
				else if (sprite->animation() == SWIM_AIM_UP_LOOK_RIGHT)
					sprite->changeAnimation(SWIM_RIGHT);
				else if (sprite->animation() == SWIM_AIM_LEFT && !bShooting)
					sprite->changeAnimation(SWIM_LEFT);
				else if (sprite->animation() == SWIM_AIM_RIGHT && !bShooting)
					sprite->changeAnimation(SWIM_RIGHT);
				else if (sprite->animation() == DROPPED && lookingTo == LOOKING_LEFT)
					sprite->changeAnimation(SWIM_LEFT);
				else if (sprite->animation() == DROPPED && lookingTo == LOOKING_RIGHT)
					sprite->changeAnimation(SWIM_RIGHT);


				else if (sprite->animation() == MOVE_LEFT)
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
				else if (sprite->animation() == ANIM_DEAD) 
					sprite->changeAnimation(STAND_RIGHT);

			}
		}
		else if (lvl == 2) {
			if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
			{
				if ((sprite->animation() != LVL2_MOVE_LEFT && !bJumping)) sprite->changeAnimation(LVL2_MOVE_LEFT);
				posPlayer.x -= 2;
				lookingTo = LOOKING_LEFT;
				if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 58), &posPlayer.y) || posPlayer.x - 2 <= left)
				{
					posPlayer.x += 2;
					if (bJumping) {
						if (sprite->animation() != AIRBONE_LEFT)
							sprite->changeAnimation(AIRBONE_LEFT);
					}
					else sprite->changeAnimation(LVL2_IDLE);
				}
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
			{
				if ((sprite->animation() != LVL2_MOVE_RIGHT && !bJumping)) sprite->changeAnimation(LVL2_MOVE_RIGHT);
				posPlayer.x += 2;
				lookingTo = LOOKING_RIGHT;
				if (map->collisionMoveRight(posPlayer, glm::ivec2(32, 58), &posPlayer.y))
				{
					posPlayer.x -= 2;
					if (bJumping) {
						if (sprite->animation() != AIRBONE_RIGHT)
							sprite->changeAnimation(AIRBONE_RIGHT);
					}
					else sprite->changeAnimation(LVL2_IDLE);
				}
			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				if (coreDestroyed) {
					if (sprite->animation() != LVL2_FORWARD) sprite->changeAnimation(LVL2_FORWARD);
				}
				else {
					if (sprite->animation() != LVL2_ZAP) sprite->changeAnimation(LVL2_ZAP);
				}

			}
			else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
			{
				if (bShooting) {
					if (sprite->animation() != LVL2_CROUCH_SHOOT) sprite->changeAnimation(LVL2_CROUCH_SHOOT);
				}
				else {
					if (sprite->animation() != LVL2_CROUCH) sprite->changeAnimation(LVL2_CROUCH);
				}
			}
			else {
				if (sprite->animation() == LVL2_MOVE_LEFT || sprite->animation() == LVL2_MOVE_RIGHT)
					sprite->changeAnimation(LVL2_IDLE);
				else if (sprite->animation() == LVL2_CROUCH || sprite->animation() == LVL2_CROUCH_SHOOT)
					sprite->changeAnimation(LVL2_IDLE);
				else if (sprite->animation() == AIRBONE_LEFT && !bJumping)
					sprite->changeAnimation(LVL2_IDLE);
				else if (sprite->animation() == AIRBONE_RIGHT && !bJumping)
					sprite->changeAnimation(LVL2_IDLE);
			}
			
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
				if (lvl == 1) posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
				else if (lvl == 2) posPlayer.y = int(startY - JUMP_HEIGHT_2 * sin(3.14159f * jumpAngle / 180.f));
				glm::ivec2 aux = posPlayer + glm::ivec2(8, 44);
				if (jumpAngle > 90)
					bJumping = !map->collisionMoveDown(aux, glm::ivec2(12, 20), &aux.y, bBridge);
				posPlayer = aux + glm::ivec2(-8, -44);
			}
		}
		else
		{
			posPlayer.y += FALL_STEP;
			glm::ivec2 aux = posPlayer + glm::ivec2(10, 30);
			if (lvl == 1 && Game::instance().getKey(' ') && Game::instance().getSpecialKey(GLUT_KEY_DOWN) && aux.y < 180);
			else if (map->collisionMoveDown(aux, glm::ivec2(10, 28), &aux.y, bBridge))
			{
				if (map->isSwimming(aux, glm::ivec2(10, 28)))
				{
					bWater = true;
				}
				else {
					bWater = false;
					justLanded = true;
					LandedFrame = 15;
					if (Game::instance().getKey(' '))
					{
						bJumping = true;
						jumpAngle = 0;
						startY = posPlayer.y;
					}
				}
			}
			posPlayer = aux + glm::ivec2(-10, -30);
		}
	}
	else if (state == DYING) {
		if (lvl == 1 && lvl2boss == false) {
			if (frameCount > 0) {
				if (sprite->animation() != ANIM_DYING && sprite->animation() != ANIM_DEAD) sprite->changeAnimation(ANIM_DYING);
				else if (lastKeyframe != 0 && sprite->keyframe() == 0) {
					sprite->changeAnimation(ANIM_DEAD);
				}
				if (rolldistance > 0) {
					--rolldistance;
					posPlayer.x -= 1;
					if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 58), &posPlayer.y, false))
					{
						posPlayer.y -= 1;
					}
					
				}
				posPlayer.y += 1;
				if (map->collisionMoveLeft(posPlayer, glm::ivec2(32, 58), &posPlayer.y))
				{
					posPlayer.x += 1;
				}
				lastKeyframe = sprite->keyframe();
				--frameCount;
			}
			else if (frameCount == 0) state = DEAD;
		}
		else if (lvl == 2 || lvl2boss) {
			if (frameCount > 0) {
				if (sprite->animation() != LVL2_ANIM_DYING && sprite->animation() != LVL2_ANIM_DEAD) sprite->changeAnimation(LVL2_ANIM_DYING);
				else if (lastKeyframe != 0 && sprite->keyframe() == 0) {
					sprite->changeAnimation(LVL2_ANIM_DEAD);
				}
				if (frameCount > 110) {
					posPlayer.y -= 2;
				}
				posPlayer.y += 1;
				if (posPlayer.y > 140)
				{
					posPlayer.y -= 1;
				}
				lastKeyframe = sprite->keyframe();
				--frameCount;
			}
			else if (frameCount == 0) state = DEAD;
		}
	}
	else if (state == DEAD) {
		frameCount = 120;
		projectile = RANK1;
		if (lvl == 1 && lvl2boss == false) {
			rolldistance = 40;
			posPlayer.y = top + 20;
			posPlayer.x = left + 20;
			state = ALIVE;
			life -= 1;
			invtimer = 120;
		}
		else if (lvl == 2) {
			rolldistance = 40;
			posPlayer.y = top + 60;
			posPlayer.x = left + 80;
			state = ALIVE;
			life -= 1;
			invtimer = 120;
		}
		else if (lvl2boss) {
			rolldistance = 40;
			posPlayer.y = top + 60;
			posPlayer.x = left + 80;
			state = ALIVE;
			life -= 1;
			invtimer = 120;
		}
	}
	if (bWater) {
		if (justLanded) {
			justLanded = false;
			LandedFrame = 15;
			sprite->changeAnimation(DROPPED);
		}
		if (LandedFrame > 0) {
			LandedFrame -= 1;
			sprite->changeAnimation(DROPPED);
		}
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

bool Player::isOnBridge() 
{
	return bBridge;
}

vector<glm::ivec2> Player::buildHitBox()
{
	if (sprite->animation() == CROUCH_LOOK_LEFT || sprite->animation() == CROUCH_LOOK_RIGHT) {
		glm::ivec2 lpos1 = posPlayer + glm::ivec2{ 4,50 };
		glm::ivec2 rpos1 = posPlayer + glm::ivec2{ 28,63 };
		return vector<glm::ivec2> {lpos1, rpos1 };
	}
	else if (sprite->animation() == LVL2_CROUCH || sprite->animation() == LVL2_CROUCH_SHOOT) {
		glm::ivec2 lpos1 = posPlayer + glm::ivec2{ 4,45 };
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
	else if (sprite->animation() == SWIM_AIM_LEFT)
		return glm::ivec2{ 4, 52 };
	else if (sprite->animation() == SWIM_AIM_RIGHT)
		return glm::ivec2{ 4, 52 };
	else if (sprite->animation() == SWIM_AIM_UPLEFT)
		return glm::ivec2{ 2, 46 };
	else if (sprite->animation() == SWIM_AIM_UPRIGHT)
		return glm::ivec2{ 11, 46 };
	else if (sprite->animation() == SWIM_AIM_UP_LOOK_LEFT)
		return glm::ivec2{ 3, 31 };
	else if (sprite->animation() == SWIM_AIM_UP_LOOK_RIGHT)
		return glm::ivec2{ 11, 31 };
	else return glm::ivec2{ 10, 46 };
}
glm::ivec2 Player::getProjectileSpawnlvl2()
{
	if (sprite->animation() == LVL2_IDLE || sprite->animation() == LVL2_IDLE_SHOOT)
		return glm::ivec2{ 7,14 };
	else if (sprite->animation() == LVL2_MOVE_LEFT || sprite->animation() == LVL2_MOVE_RIGHT)
		return glm::ivec2{ 8,14 };
	else if (sprite->animation() == LVL2_CROUCH || sprite->animation() == LVL2_CROUCH_SHOOT)
		return glm::ivec2{ 7,24 };
	else if (bJumping)
		return glm::ivec2{ 10, 46 };
	else return glm::ivec2{ 7,14 };
}

bool Player::isJumping()
{
	return bJumping;
}