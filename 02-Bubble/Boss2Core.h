#ifndef _BOSS2CORE_INCLUDE
#define _BOSS2CORE_INCLUDE


#include "Enemy.h"
#include "ProjectileBoss.h"
#include <list>

class Boss2Core : public Enemy
{

public:
	Boss2Core();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	vector<glm::ivec2> buildHitBox();
	glm::ivec2 getProjectileSpawn();

	glm::vec2 projDir;

private:



};


#endif