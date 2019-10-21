#ifndef _ENEMY_INCLUDE
#define _ENEMY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include <string>


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
	string getType();
	int state;
	Sprite *sprite;
	int frameCount;
	int lastKeyframe;
	int getSublvl() { return sublvl; }
	void setSublvl(int x) { sublvl = x; }

protected:
	int life;
	
	string type;
	int sublvl;
	bool bJumping;
	int lookingTo;
	glm::ivec2 tileMapDispl, posEnemy;
	int jumpAngle, startY;
	Texture spritesheet;
	TileMap *map;

};


#endif // _ENEMY_INCLUDE