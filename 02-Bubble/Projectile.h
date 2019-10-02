#ifndef _PROJECTILE_INCLUDE
#define _PROJECTILE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"



class Projectile
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, int dir);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	glm::ivec2 getPos();

private:
	glm::ivec2 tileMapDispl, posProjectile, newPos;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	int direction;

};


#endif // _PROJECTILE_INCLUDE


