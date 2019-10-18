#ifndef _LIFE_INCLUDE
#define _LIFE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include <string>


class Life
{

public:
	Life();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, float left, float right, float bottom, float top, int life);

	void render();
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	glm::ivec2 getPos();

	int state;
	Sprite *sprite;
	int frameCount;


protected:
	int life;

	string type;
	bool bJumping;
	int lookingTo;
	glm::ivec2 tileMapDispl, posLife;
	int jumpAngle, startY;
	Texture spritesheet;
	TileMap *map;

};


#endif