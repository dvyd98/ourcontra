#include "Bridge.h"
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

enum States
{
	ALIVE, DEAD, DYING, SPECIAL_BRIDGE_STATE
};

enum BridgeAnims
{
	EDGE_LEFT, EDGE_RIGHT, DESTROYED_LEFT, DESTROYED_RIGHT, CENTRAL, EMPTY, ANIM_DYING, ANIM_DEAD
};



Bridge::Bridge()
{
}

void Bridge::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	type = "bridge";
	state = ALIVE;
	life = 1;
	frameCount = 5;
	lastKeyframe = 0;
	spritesheet.loadFromFile("images/bridge.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.1f, 0.1f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(100);

	sprite->setAnimationSpeed(EDGE_LEFT, 8);
	sprite->addKeyframe(EDGE_LEFT, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(EDGE_LEFT, glm::vec2(0.1f, 0.0f));
	sprite->addKeyframe(EDGE_LEFT, glm::vec2(0.2f, 0.0f));

	sprite->setAnimationSpeed(EDGE_RIGHT, 8);
	sprite->addKeyframe(EDGE_RIGHT, glm::vec2(0.6f, 0.0f));
	sprite->addKeyframe(EDGE_RIGHT, glm::vec2(0.5f, 0.0f));
	sprite->addKeyframe(EDGE_RIGHT, glm::vec2(0.4f, 0.0f));

	sprite->setAnimationSpeed(CENTRAL, 8);
	sprite->addKeyframe(CENTRAL, glm::vec2(0.0f, 0.1f));
	sprite->addKeyframe(CENTRAL, glm::vec2(0.1f, 0.1f));
	sprite->addKeyframe(CENTRAL, glm::vec2(0.2f, 0.1f));

	sprite->setAnimationSpeed(DESTROYED_LEFT, 8);
	sprite->addKeyframe(DESTROYED_LEFT, glm::vec2(0.0f, 0.3f));

	sprite->setAnimationSpeed(DESTROYED_RIGHT, 8);
	sprite->addKeyframe(DESTROYED_RIGHT, glm::vec2(0.1f, 0.2f));

	sprite->setAnimationSpeed(EMPTY, 8);
	sprite->addKeyframe(EMPTY, glm::vec2(0.6f, 0.6f));

	sprite->setAnimationSpeed(ANIM_DYING, 6);
	sprite->addKeyframe(ANIM_DYING, glm::vec2(0.0f, 0.4f));
	sprite->addKeyframe(ANIM_DYING, glm::vec2(0.1f, 0.4f));
	sprite->addKeyframe(ANIM_DYING, glm::vec2(0.2f, 0.4f));
	sprite->addKeyframe(ANIM_DYING, glm::vec2(0.3f, 0.4f));

	sprite->setAnimationSpeed(ANIM_DEAD, 1);
	sprite->addKeyframe(ANIM_DEAD, glm::vec2(0.9f, 0.8f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

void Bridge::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (state == SPECIAL_BRIDGE_STATE || state == DYING) {
		if (frameCount < 0) {
			state = DYING;
			if (sprite->animation() != ANIM_DYING) sprite->changeAnimation(ANIM_DYING);
			else if (lastKeyframe != 0 && sprite->keyframe() == 0) {
				state = DEAD;
			}
			lastKeyframe = sprite->keyframe();
		}
		frameCount -= 1;
	}
	if (state == DEAD) {
		if (bridgeType == "left") sprite->changeAnimation(DESTROYED_LEFT);
		else if (bridgeType == "right") sprite->changeAnimation(DESTROYED_RIGHT);
		else if (bridgeType == "central") sprite->changeAnimation(EMPTY);
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posEnemy.x), float(tileMapDispl.y + posEnemy.y)));
}

vector<glm::ivec2> Bridge::buildHitBox()
{
	glm::ivec2 lpos1 = posEnemy;
	glm::ivec2 rpos1 = lpos1 + glm::ivec2{ 32,32 };
	return vector<glm::ivec2> {lpos1, rpos1 };
}