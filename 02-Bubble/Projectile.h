#ifndef _PROJECTILE_INCLUDE
#define _PROJECTILE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"



class Projectile
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, float speed, const glm::vec2 projDir);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	int getRank();
	int getDmg();
	vector<glm::ivec2> buildHitBox();
	Sprite *sprite;
	glm::ivec2 getPos();

private:
	glm::ivec2 tileMapDispl;
	glm::vec2 newPos, posProjectile;
	Texture spritesheet;
	
	TileMap *map;
	int direction;
	int rank;
	int damage;

};


#endif // _PROJECTILE_INCLUDE


