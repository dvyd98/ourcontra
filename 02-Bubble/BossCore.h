#ifndef _BOSSCORE_INCLUDE
#define _BOSSCORE_INCLUDE


#include "Enemy.h"
#include "Projectile.h"
#include <list>

class BossCore : public Enemy
{

public:
	BossCore();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	vector<glm::ivec2> buildHitBox();


private:



};


#endif