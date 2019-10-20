#ifndef _GUNUPGRADE_INCLUDE
#define _GUNUPGRADE_INCLUDE


#include "Enemy.h"
#include "Projectile.h"
#include <list>

class GunUpgrade : public Enemy
{

public:
	GunUpgrade();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	vector<glm::ivec2> buildHitBox();

	glm::vec2 projDir;

	bool first;

private:



};


#endif