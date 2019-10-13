#include <iostream>
#include <vector>
#include "TileMap.h"


using namespace std;

enum BACKGROUND_TILES_LVL1 {
	BLACK, GRASS1, GRASS2, ROCK1, ROCK2, BRIDGE1, BRIDGE2, BLANK1, PALM_TOP1, PALM_TOP2, BLANK2, PALM_TOP3, BLANK3, PALM_TOP4, STAR1, STAR2,
	BLUE, ROCK3, ROCK4, ROCK5, ROCK6, BRIDGE3, BRIDGE4, BRIDGE5, PALM_BOT1, PALM_BOT2, PALM_BOT3, PALM_BOT4, PALM_BOT5, PALM_BOT6, START3, STAR4,
	BLANK4, ROCK7, ROCK8, SEA1, SEA2, SEA3, SEA4, SEA5, SEA6, SEA7, SEA8, SEA9, SEA10, SEA11, BLANK5, BLANK6,
	JUNGLE1, JUNGLE2, BLANK7, JUNGLE3, BLANK8, BLANK9, BLANK10, BLANK11, M1, M2, M3, M4, BLANK12, BLANK13, BLANK14, BLANK15,
	JUNGLE4, JUNGLE5, BLANK16, JUGNLE6, JUNGLE7, BLANK17, BLANK18, M5, M6, M7, M8, M9, M10, BLANK19, BLANK20, BLANK21,
	JUNLE8, JUNGLE10, BLANK22, BLANK23, BLANK24, BLANK25, BLANK26, M11, M12, M13, M14, BLANK27, M15, BLANK28, BLANK, BLANK29,
	JUNGLE11, JUNGLE12
};


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	loadLevel(levelFile);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile)
{
	string line;
	stringstream sstream;
	string typeScene;
	
	fin.open(levelFile.c_str());
	if(!fin.is_open())
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> typeScene;
	if (typeScene == "LEVEL1") loadTileMap();
	if (typeScene == "LEVEL2") loadLevel2();
	if (typeScene == "MENU") loadMenu();
	
	return true;
}

void TileMap::loadTileMap() {
	string line;
	stringstream sstream;
	string tilesheetFile;
	char tile;

	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize.x >> tileSize.y >> blockSize.x >> blockSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	getline(fin, line);
	sstream.str(line);
	sstream >> numEnemies;

	enemies = new vector<infoEnemies>();
	for (int i = 0; i < numEnemies; ++i) {
		string s;
		int x = NULL, y = NULL;
		enemyType type = SOLDIER;
		getline(fin, line);
		sstream.clear();
		sstream.str(line);
		sstream >> x >> y >> s;
		if (s == "Soldier") type = SOLDIER;
		else if (s == "Bridge_left") type = BRIDGE_EDGE_LEFT;
		else if (s == "Bridge_right") type = BRIDGE_EDGE_RIGHT;
		else if (s == "Bridge_central") type = BRIDGE_CENTRAL;
		enemies->push_back(infoEnemies());
		(*enemies)[i].x = x;
		(*enemies)[i].y = y;
		(*enemies)[i].type = type;
	}
	map = new int[mapSize.x * mapSize.y];
	for (int j = 0; j<mapSize.y; j++)
	{
		for (int i = 0; i<mapSize.x; i++)
		{
			fin.get(tile);
			switch (tile) {
			case '.': map[j*mapSize.x + i] = BLACK; break;
			case ',': map[j*mapSize.x + i] = BLUE; break;
			case 'q': map[j*mapSize.x + i] = GRASS1; break;
			case 'w': map[j*mapSize.x + i] = GRASS2; break;
			case 'a': map[j*mapSize.x + i] = ROCK3; break;
			case 's': map[j*mapSize.x + i] = ROCK4; break;
			case 'e': map[j*mapSize.x + i] = ROCK1; break;
			case 'r': map[j*mapSize.x + i] = ROCK2; break;
			case 'd': map[j*mapSize.x + i] = ROCK5; break;
			case 'f': map[j*mapSize.x + i] = ROCK6; break;
			case 'z': map[j*mapSize.x + i] = ROCK7; break;
			case 'x': map[j*mapSize.x + i] = ROCK8; break;
			case 't': map[j*mapSize.x + i] = BRIDGE1; break;
			case 'y': map[j*mapSize.x + i] = BRIDGE2; break;
			case 'c': map[j*mapSize.x + i] = BRIDGE3; break;
			case 'v': map[j*mapSize.x + i] = BRIDGE4; break;
			case 'b': map[j*mapSize.x + i] = BRIDGE5; break;
			case 'n': map[j*mapSize.x + i] = SEA1; break;
			case 'm': map[j*mapSize.x + i] = SEA2; break;
			case 'j': map[j*mapSize.x + i] = SEA3; break;
			case 'u': map[j*mapSize.x + i] = SEA6; break;
			case 'i': map[j*mapSize.x + i] = SEA10; break;
			case 'k': map[j*mapSize.x + i] = SEA4; break;
			case 'o': map[j*mapSize.x + i] = SEA8; break;
			case 'l': map[j*mapSize.x + i] = SEA9; break;
			case 'ñ': map[j*mapSize.x + i] = SEA7; break;
			case 'p': map[j*mapSize.x + i] = SEA5; break;
			}
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}

	fin.close();
}

void TileMap::loadLevel2() {
	string line;
	stringstream sstream;
	string tilesheetFile;

	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);

	map = new int[1];
	map[0] = SLVL1;

	mapSize.x = 1; mapSize.y = 1;
	tileSize.x = 16 * 20;
	tileSize.y = 16 * 15;
	tilesheetSize.x = 5;
	tilesheetSize.y = 2;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
	blockSize.x = 16 * 20;
	blockSize.y = 16 * 15;

	fin.close();
}

void TileMap::loadMenu() {
	string line;
	stringstream sstream;
	string tilesheetFile;

	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);

	map = new int[1];
	map[0] = MENU_1_PLAYER;

	mapSize.x = 1; mapSize.y = 1;
	tileSize.x = 16 * 20;
	tileSize.y = 16 * 15;
	tilesheetSize.x = 4;
	tilesheetSize.y = 1;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
	blockSize.x = 16 * 20;
	blockSize.y = 16 * 15;

	fin.close();
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];

			nTiles++;
			posTile = glm::vec2(minCoords.x + i * tileSize.x, minCoords.y + j * tileSize.y);
			texCoordTile[0] = glm::vec2(float(tile%tileSize.x) / tilesheetSize.x, float(tile/tileSize.y) / tilesheetSize.y);
			texCoordTile[1] = texCoordTile[0] + tileTexSize;
			//texCoordTile[0] += halfTexel;
			texCoordTile[1] -= halfTexel;
			// First triangle
			vertices.push_back(posTile.x); vertices.push_back(posTile.y);
			vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
			vertices.push_back(posTile.x + blockSize.x); vertices.push_back(posTile.y);
			vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
			vertices.push_back(posTile.x + blockSize.x); vertices.push_back(posTile.y + blockSize.y);
			vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
			// Second triangle
			vertices.push_back(posTile.x); vertices.push_back(posTile.y);
			vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
			vertices.push_back(posTile.x + blockSize.x); vertices.push_back(posTile.y + blockSize.y);
			vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
			vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize.y);
			vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = pos.x / tileSize.x;
	y0 = pos.y / tileSize.y;
	y1 = (pos.y + size.y - 1) / tileSize.y;
	for(int y=y0; y<=y1; y++)
	{
		if (mapSize.x == 1 && pos.x < 45)
			return true;
	}
	
	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = (pos.x + size.x - 1) / tileSize.x;
	y0 = pos.y / tileSize.y;
	y1 = (pos.y + size.y - 1) / tileSize.y;
	for(int y=y0; y<=y1; y++)
	{
		if(mapSize.x == 1 && pos.x > 240)
			return true;
	}
	
	return false;
}


bool TileMap::bottomIsSea(const int x, const int y) const {
	return (map[y*mapSize.x + x] == SEA1 || map[y*mapSize.x + x] == SEA2 || map[y*mapSize.x + x] == SEA3 || map[y*mapSize.x + x] == SEA4 || map[y*mapSize.x + x] == SEA5 || map[y*mapSize.x + x] == SEA6 || map[y*mapSize.x + x] == SEA7 || map[y*mapSize.x + x] == SEA8 || map[y*mapSize.x + x] == SEA9 || map[y*mapSize.x + x] == SEA10 || map[y*mapSize.x + x] == SEA11);
}


bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY, bool bBridge) const
{
	int x0, x1, y;
	
	x0 = pos.x / tileSize.x;
	x1 = (pos.x + size.x - 1) / tileSize.x;
	y = (pos.y + size.y - 1) / tileSize.y;
	for(int x=x0; x<=x1; x++)
	{
		if(map[y*mapSize.x + x] == GRASS1 || 
			map[y*mapSize.x + x] == GRASS2 ||
			map[y*mapSize.x + x] == BRIDGE1 ||
			map[y*mapSize.x + x] == BRIDGE2 ||
			bBridge ||
			
			bottomIsSea(x,y))
		{
			if(*posY - tileSize.y * y + size.y <= 4 || bBridge)
			{
				*posY = tileSize.y * y - size.y;
				return true;
			}
		}
		if (mapSize.x == 1 && pos.y > 160)
		{
			*posY = 160;
			return true;
		}
	}
	
	return false;
}

// funcions menu

void TileMap::toggleFrame(const glm::vec2 &minCoords, ShaderProgram &program, int frame) {
	map[0] = frame;
	prepareArrays(minCoords, program);
}