#ifndef _RIFLEMAN_INCLUDE
#define _RIFLEMAN_INCLUDE


#include "Enemy.h"
#include "Projectile.h"
#include <list>

class Rifleman : public Enemy
{

public:
	Rifleman();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	vector<glm::ivec2> buildHitBox();
	glm::ivec2 getProjectileSpawn();

	int shotCd;
	bool hasShot = false;
	glm::ivec2 projDir;

};


#endif