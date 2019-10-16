#ifndef _BOSSTURRET_INCLUDE
#define _BOSSTURRET_INCLUDE


#include "Enemy.h"
#include "Projectile.h"
#include <list>

class BossTurret : public Enemy
{

public:
	BossTurret();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	vector<glm::ivec2> buildHitBox();
	glm::ivec2 getProjectileSpawn();

	glm::vec2 projDir;

private:



};


#endif