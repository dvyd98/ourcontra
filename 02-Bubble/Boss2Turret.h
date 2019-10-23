#ifndef _BOSS2TURRET_INCLUDE
#define _BOSS2TURRET_INCLUDE


#include "Enemy.h"
#include "ProjectileBoss.h"
#include <list>

class Boss2Turret : public Enemy
{

public:
	Boss2Turret();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	vector<glm::ivec2> buildHitBox();
	glm::ivec2 getProjectileSpawn();

	glm::vec2 projDir;

	int openTime;
	bool hasShot;

private:



};


#endif