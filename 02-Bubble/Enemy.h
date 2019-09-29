#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


class Enemy
{

public:
	virtual void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) = 0;
	virtual void update(int deltaTime, float left) = 0;
	virtual void render() = 0;

	virtual void setTileMap(TileMap *tileMap)= 0;
	virtual void setPosition(const glm::vec2 &pos) = 0;

	virtual glm::ivec2 getPos() = 0;

protected:
	bool bJumping;
	int lookingTo;
	glm::ivec2 tileMapDispl, posEnemy;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};


#endif // _ENEMY_INCLUDE