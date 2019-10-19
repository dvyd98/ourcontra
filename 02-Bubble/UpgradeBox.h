#ifndef _UPGRADEBOX_INCLUDE
#define _UPGRADEBOX_INCLUDE


#include "Enemy.h"
#include "Projectile.h"
#include <list>

class UpgradeBox : public Enemy
{

public:
	UpgradeBox();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	vector<glm::ivec2> buildHitBox();

	glm::vec2 projDir;

private:

	int openTime;
	bool isOpen;

};


#endif