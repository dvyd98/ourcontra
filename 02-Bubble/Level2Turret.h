#ifndef _LEVEL2TURRET_INCLUDE
#define _LEVEL2TURRET_INCLUDE


#include "Enemy.h"
#include <list>

class Level2Turret : public Enemy
{

public:
	Level2Turret();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	vector<glm::ivec2> buildHitBox();
	glm::ivec2 getProjectileSpawn();

	glm::vec2 projDir;

	bool hasShot;
	int projCd;

private:



};


#endif