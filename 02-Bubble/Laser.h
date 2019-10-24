#ifndef _LASER_INCLUDE
#define _LASER_INCLUDE


#include "Enemy.h"
#include <list>

class Laser : public Enemy
{

public:
	Laser();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	vector<glm::ivec2> buildHitBox();
	glm::ivec2 getProjectileSpawn();

	glm::vec2 projDir;

private:



};


#endif