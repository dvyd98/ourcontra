#ifndef _PROJECTILE_INCLUDE
#define _PROJECTILE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"



class Projectile
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, float left);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	glm::ivec2 getPos();

private:
	glm::ivec2 tileMapDispl, posProjectile;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};


#endif // _PROJECTILE_INCLUDE


