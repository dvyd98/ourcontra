#include "Life.h"
#include "Game.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>

enum LifeAnims
{
	FOUR_LIVES, THREE_LIVES, TWO_LIVES, ONE_LIFE, ZERO_LIVES
};

Life::Life()
{

}

void Life::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, int player)
{
	if (player == 0) spritesheet.loadFromFile("images/life.png", TEXTURE_PIXEL_FORMAT_RGBA);
	else spritesheet.loadFromFile("images/life2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setWrapS(GL_CLAMP_TO_EDGE);
	spritesheet.setWrapT(GL_CLAMP_TO_EDGE);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(52, 16), glm::vec2(0.1f, 0.1f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(100);

	sprite->setAnimationSpeed(FOUR_LIVES, 8);
	sprite->addKeyframe(FOUR_LIVES, glm::vec2(0.0f, 0.4f));

	sprite->setAnimationSpeed(THREE_LIVES, 8);
	sprite->addKeyframe(THREE_LIVES, glm::vec2(0.0f, 0.3f));

	sprite->setAnimationSpeed(TWO_LIVES, 8);
	sprite->addKeyframe(TWO_LIVES, glm::vec2(0.0f, 0.2f));

	sprite->setAnimationSpeed(ONE_LIFE, 8);
	sprite->addKeyframe(ONE_LIFE, glm::vec2(0.0f, 0.1f));

	sprite->setAnimationSpeed(ZERO_LIVES, 8);
	sprite->addKeyframe(ZERO_LIVES, glm::vec2(0.0f, 0.0f));

	sprite->changeAnimation(2);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posLife.x), float(tileMapDispl.y + posLife.y)));
}

void Life::update(int deltaTime, float left, float right, float bottom, float top, int life)
{
	sprite->update(deltaTime);
	posLife.y = top + 5;
	posLife.x = left + 16;
	if (life >= 4) sprite->changeAnimation(FOUR_LIVES);
	else if (life == 3) sprite->changeAnimation(THREE_LIVES);
	else if (life == 2) sprite->changeAnimation(TWO_LIVES);
	else if (life == 1) sprite->changeAnimation(ONE_LIFE);
	else sprite->changeAnimation(ZERO_LIVES);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posLife.x), float(tileMapDispl.y + posLife.y)));
}

void Life::render()
{
	sprite->render();
}

void Life::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Life::setPosition(const glm::vec2 &pos)
{
	posLife = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posLife.x), float(tileMapDispl.y + posLife.y)));
}

glm::ivec2 Life::getPos()
{
	return posLife;
}