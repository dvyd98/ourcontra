#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Projectile.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

#define PLAYER_VEL 2

class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, float left, float right, float bottom, float top);
	void render();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);

	glm::ivec2 getPos();
	glm::ivec2 getProjectileSpawn();
	glm::ivec2 getProjectileSpawnlvl2();

	bool isJumping();
	bool isOnBridge();
	bool isOnWater();

	vector<glm::ivec2> buildHitBox();

	int lookingTo;
	int life;
	int state;
	int LandedFrame;
	int projectile;
	int lvl;
	int invtimer = 0;
	int frameCount;
	int rolldistance = 40;
	int lastKeyframe;
	bool bBridge;
	bool bWater;
	bool justLanded;
	bool coreDestroyed;
	glm::ivec2 posPlayer;
	Sprite *sprite;
	
private:
	bool bJumping;
	bool bShooting;
	bool wasShooting;
	
	int currentKeyframe;
	glm::ivec2 tileMapDispl;
	int jumpAngle, startY;
	Texture spritesheet;
	TileMap *map;

};


#endif // _PLAYER_INCLUDE


