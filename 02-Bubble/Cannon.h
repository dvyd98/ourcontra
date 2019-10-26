#ifndef _CANNON_INCLUDE
#define _CANNON_INCLUDE


#include "Enemy.h"
#include "Projectile.h"
#include <list>

class Cannon : public Enemy
{

public:
	Cannon();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	vector<glm::ivec2> buildHitBox();
	glm::ivec2 getProjectileSpawn();

	glm::vec2 projDir;
	int shotCd;
	bool hasShot = false;

private:



};


#endif