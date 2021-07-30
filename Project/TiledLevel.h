#pragma once
#include <map>
#include <vector>

#include "SpriteObject.h"

class Tile : public SpriteObject
{
public:
	Tile(SDL_Rect source, SDL_FRect destination, bool obstacle, bool hazard)
		: SpriteObject(source, destination)
		, m_obstacle(obstacle)
		, m_hazard(hazard)
	{ }
	
	Tile* Clone() 
	{ 
		return new Tile(m_sourceTransform, m_destinationTransform, m_obstacle, m_hazard);
	}
	bool IsObstacle() { return m_obstacle; }
	void SetXY(float x, float y) { m_destinationTransform.x = x; m_destinationTransform.y = y; }
	void Update(float deltaTime) override {}
	void Render() override {}
private:
	bool m_obstacle;
	bool m_hazard;
};

class TiledLevel : public GameObject
{
public:
	TiledLevel(int rows, int cols, int tileWidth, int tileHeight,
		const char* tileData, const char* levelData, const char* tileKey);
	~TiledLevel();
	virtual void Update(float DeltaTime) override;
	virtual void Render() override;
	std::vector<Tile*>& GetObstacles() { return m_obstacles;  }

private:
	const char* m_tileKey;
	int m_rows;
	int m_cols;

	// Our map of 16 prototype Tile objects. From the Tiledata file
	std::map<char, Tile*> m_tiles; 
	std::vector<std::vector<Tile*>> m_levelTiles; // 2D vector.
	std::vector<Tile*> m_obstacles;
};

