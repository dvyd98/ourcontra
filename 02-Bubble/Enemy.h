#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


class Enemy
{

public:
	Enemy();
	virtual void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) = 0;
	virtual void update(int deltaTime) = 0;
	virtual vector<glm::ivec2> buildHitBox() = 0;

	void render();
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	bool decreaseLife(int dmg);
	glm::ivec2 getPos();
	

protected:
	int life;
	int state;
	bool bJumping;
	int lookingTo;
	glm::ivec2 tileMapDispl, posEnemy;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};


#endif // _ENEMY_INCLUDE