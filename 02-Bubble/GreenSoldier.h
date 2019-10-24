#ifndef _GREENSOLDIER_INCLUDE
#define _GREENSOLDIER_INCLUDE


#include "Enemy.h"
#include <list>

class GreenSoldier : public Enemy
{

public:
	GreenSoldier();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	vector<glm::ivec2> buildHitBox();
	glm::ivec2 getProjectileSpawn();

	glm::vec2 projDir;

	float steps;
	bool left;
	bool hasShot;
	bool move;
	int projectileCd;
	int moveCd;

private:



};


#endif