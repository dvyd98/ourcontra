#ifndef _LEVEL2CORE_INCLUDE
#define _LEVEL2CORE_INCLUDE


#include "Enemy.h"
#include <list>

class Level2Core : public Enemy
{

public:
	Level2Core();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	vector<glm::ivec2> buildHitBox();
	glm::ivec2 getProjectileSpawn();

	glm::vec2 projDir;

private:



};


#endif