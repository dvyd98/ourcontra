#ifndef _BRIDGE_INCLUDE
#define _BRIDGE_INCLUDE

#include "Enemy.h"

class Bridge : public Enemy
{

public:
	Bridge();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	vector<glm::ivec2> buildHitBox();

};


#endif