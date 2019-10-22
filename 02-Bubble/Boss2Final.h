#ifndef _BOSS2FINAL_INCLUDE
#define _BOSS2FINAL_INCLUDE


#include "Enemy.h"
#include "ProjectileBoss2.h"
#include <list>

class Boss2Final : public Enemy
{

public:
	Boss2Final();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	vector<glm::ivec2> buildHitBox();
	glm::ivec2 getProjectileSpawn();

	glm::vec2 projDir;

	bool justSpawned;
	int steps;
	bool left;
	bool hasShot;
	int projectileCd;

private:



};


#endif