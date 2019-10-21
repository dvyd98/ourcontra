#ifndef _PROJECTILEBOSS_INCLUDE
#define _PROJECTILEBOSS_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"



class ProjectileBoss
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

	int rank;
	int damage;
	bool peak;
	int loop;


private:
	glm::ivec2 tileMapDispl;
	glm::vec2 newPos, posProjectile;
	Texture spritesheet;

	TileMap *map;
	int direction;

};


#endif // _PROJECTILE_INCLUDE


