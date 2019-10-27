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

enum States
{
	ALIVE, DEAD, DYING
};

Soldier::Soldier()
{

}

enum SoldierAnims
{
	MOVE_LEFT, MOVE_RIGHT, AIRBONE_LEFT, AIRBONE_RIGHT, ANIM_DYING, ANIM_DEAD
};

enum LookingTo
{
	LOOKING_LEFT, LOOKING_RIGHT
};

void Soldier::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	type = "soldier";
	state = ALIVE;
	life = 1;
	bJumping = false;
	lookingTo = LOOKING_RIGHT;
	lastKeyframe = 0;
	spritesheet.loadFromFile("images/soldier.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1f, 0.1f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(100);

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.0f, 0.1f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.1f, 0.1f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2f, 0.1f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.3f, 0.1f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.4f, 0.1f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.1f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.1f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.2f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.3f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.4f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.0f));

	sprite->setAnimationSpeed(AIRBONE_RIGHT, 8);
	sprite->addKeyframe(AIRBONE_RIGHT, glm::vec2(0.0f, 0.2f));

	sprite->setAnimationSpeed(AIRBONE_LEFT, 8);
	sprite->addKeyframe(AIRBONE_LEFT, glm::vec2(0.0f, 0.3f));

	sprite->setAnimationSpeed(ANIM_DYING, 4);
	sprite->addKeyframe(ANIM_DYING, glm::vec2(0.0f, 0.4f));
	sprite->addKeyframe(ANIM_DYING, glm::vec2(0.1f, 0.4f));
	sprite->addKeyframe(ANIM_DYING, glm::vec2(0.2f, 0.4f));

	sprite->setAnimationSpeed(ANIM_DEAD, 1);
	sprite->addKeyframe(ANIM_DEAD, glm::vec2(0.9f, 0.9f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Soldier::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (state == ALIVE) {
		posEnemy.y += FALL_STEP;
		if (lookingTo == LOOKING_LEFT) {
			posEnemy.x -= PLAYER_VEL;
			if (sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);
			if (!map->collisionMoveDownSoldier(posEnemy, glm::ivec2(16, 26) + glm::ivec2{ 8,0 }, &posEnemy.y)) {
				lookingTo = LOOKING_RIGHT;
				posEnemy.x += PLAYER_VEL;
				posEnemy.y -= FALL_STEP;
			}
		}
		else {
			posEnemy.x += PLAYER_VEL;
			if (sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
			if (!map->collisionMoveDownSoldier(posEnemy, glm::ivec2(16, 26) + glm::ivec2{ 8,0 }, &posEnemy.y)) {
				lookingTo = LOOKING_LEFT;
				posEnemy.x -= PLAYER_VEL;
				posEnemy.y -= FALL_STEP;
			}
		}
		if (bJumping) // lookingTo es una variable que sactualitza segons si el player mira left or right, l'utilitzo per estalviar mirar cada sprite individualment si es left or right
		{
			if (lookingTo == LOOKING_RIGHT)
				if (sprite->animation() != AIRBONE_RIGHT)
					sprite->changeAnimation(AIRBONE_RIGHT);
			if (lookingTo == LOOKING_LEFT)
				if (sprite->animation() != AIRBONE_LEFT)
					sprite->changeAnimation(AIRBONE_LEFT);

		}
		posEnemy.y += FALL_STEP;
		if (map->collisionMoveDownSoldier(posEnemy, glm::ivec2(16, 26) + glm::ivec2{ 8,0 }, &posEnemy.y)) {
			posEnemy.y -= FALL_STEP;
		}
		if (false) {}
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
					bJumping = !map->collisionMoveDownSoldier(posEnemy + glm::ivec2{ 8,0 }, glm::ivec2(16, 26), &posEnemy.y);
			}
		}
		else
		{
			posEnemy.y += FALL_STEP;
			if (map->collisionMoveDownSoldier(posEnemy + glm::ivec2{ 8,0 }, glm::ivec2(16, 26), &posEnemy.y))
			{
				if ((rand() % 1000) == 1) // jump randomly
				{
					bJumping = true;
					jumpAngle = 0;
					startY = posEnemy.y;
				}
			}
		}
	}
	else if (state == DYING) {
		if (sprite->animation() != ANIM_DYING) sprite->changeAnimation(ANIM_DYING);
		else if (lastKeyframe != 0 && sprite->keyframe() == 0) {
			state = DEAD;
			sprite->changeAnimation(ANIM_DEAD);
		}
		lastKeyframe = sprite->keyframe();
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

vector<glm::ivec2> Soldier::buildHitBox()
{
	glm::ivec2 lpos1 = posEnemy + glm::ivec2{ 8,0 };
	glm::ivec2 rpos1 = lpos1 + glm::ivec2{ 16,32 };
	return vector<glm::ivec2> {lpos1, rpos1 };
}