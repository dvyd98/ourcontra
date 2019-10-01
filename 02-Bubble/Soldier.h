#ifndef _SOLDIER_INCLUDE
#define _SOLDIER_INCLUDE


#include "Enemy.h"


class Soldier : public Enemy
{

public:
	Soldier();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);

};


#endif // _SOLDIER_INCLUDE