#include "GunUpgrade.h"
#include "Game.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>

#define JUMP_ANGLE_STEP 1
#define JUMP_HEIGHT 20
#define FALL_STEP 4
#define GLUT_KEY_SPACEBAR 32
#define PLAYER_VEL 2

enum States
{
	ALIVE, DEAD, DYING
};

enum BoxAnims
{
	ANIM_ALIVE, ANIM_DEAD
};

GunUpgrade::GunUpgrade()
{

}

void GunUpgrade::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	type = "gunupgrade";
	state = ALIVE;
	life = 10;
	lastKeyframe = 0;
	bJumping = false;
	first = true;
	spritesheet.loadFromFile("images/gunupgrade.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1f, 0.1f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(100);

	sprite->setAnimationSpeed(ANIM_ALIVE, 1);
	sprite->addKeyframe(ANIM_ALIVE, glm::vec2(0.0f, 0.0f));

	sprite->setAnimationSpeed(ANIM_DEAD, 1);
	sprite->addKeyframe(ANIM_DEAD, glm::vec2(0.9f, 0.9f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void GunUpgrade::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (state == ALIVE) {

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
					bJumping = !map->collisionMoveDown(posEnemy, glm::ivec2(12, 20), &posEnemy.y, false);
			}
		}
		else
		{
			posEnemy.y += FALL_STEP;
			if (first || map->collisionMoveDown(posEnemy, glm::ivec2(10, 28), &posEnemy.y, false))
			{

					if (first)
					{
						bJumping = true;
						first = false;
						jumpAngle = 0;
						startY = posEnemy.y;
					}
				
			}
		}
	}
	else if (state == DYING) {
		state == DEAD;
		sprite->changeAnimation(ANIM_DEAD);
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

vector<glm::ivec2> GunUpgrade::buildHitBox()
{
	glm::ivec2 lpos1 = posEnemy;
	glm::ivec2 rpos1 = lpos1 + glm::ivec2{ 32,32 };
	return vector<glm::ivec2> {lpos1, rpos1 };
}