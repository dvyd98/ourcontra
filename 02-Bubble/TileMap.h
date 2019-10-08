#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include <vector>
#include <fstream>
#include <sstream>


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.

enum enemyType {
	SOLDIER
};

enum MENU_FRAMES {
	MENU_1_PLAYER, MENU_1_PLAYER_BLINK, MENU_2_PLAYER, MENU_2_PLAYER_BLINK
};

struct infoEnemies {
	int x; int y; enemyType type;
};

class TileMap
{

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);
	~TileMap();

	void render() const;
	void free();
	
	int getTileSize() const { return tileSize.x; }

	int getNumEnemies() const { return numEnemies; }

	glm::ivec2 getMapSize() const { return mapSize; }

	infoEnemies getEnemy(int i) { return (*enemies)[i]; }

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;

	// funcions de menu
	void toggleFrame(const glm::vec2 &minCoords, ShaderProgram &program, int frame);
	int getMenuFrame() { return map[0]; }

private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);
	void loadTileMap();
	void loadMenu();

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize, tileSize, blockSize;
	int numEnemies;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;

	vector<infoEnemies> *enemies;

	ifstream fin;

};


#endif // _TILE_MAP_INCLUDE


