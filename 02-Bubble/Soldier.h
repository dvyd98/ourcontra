#ifndef _SOLDIER_INCLUDE
#define _SOLDIER_INCLUDE


#include "Enemy.h"


class Soldier : public Enemy
{

public:
	Soldier();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	vector<glm::ivec2> buildHitBox();

};


#endif // _SOLDIER_INCLUDE