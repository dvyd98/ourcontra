#ifndef _SOLDIER_INCLUDE
#define _SOLDIER_INCLUDE


#include "Enemy.h"


class Soldier : public Enemy
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, float left);

private:
	bool bJumping;
	int lookingTo;
	glm::ivec2 tileMapDispl, posEnemy;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};


#endif // _SOLDIER_INCLUDE