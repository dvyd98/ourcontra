#ifndef _WALLTURRET_INCLUDE
#define _WALLTURRET_INCLUDE


#include "Enemy.h"
#include "Projectile.h"
#include <list>

class WallTurret : public Enemy
{

public:
	WallTurret();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	vector<glm::ivec2> buildHitBox();
	glm::ivec2 getProjectileSpawn();

	glm::ivec2 projDir;

};


#endif