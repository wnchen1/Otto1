#pragma once
#include "SpriteObject.h"

enum CollectableType {key, key2, p1,};
class Collectables : public SpriteObject
{
private:
	CollectableType m_type;
public:
	Collectables(SDL_Rect source, SDL_FRect destination, CollectableType item);
	~Collectables() = default;
	
	
	void Update(float deltaTime) override;
	void Render() override;
	auto getCollectableType() {return m_type;}
};

