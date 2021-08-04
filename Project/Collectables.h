#pragma once
#include "SpriteObject.h"

enum CollectableType {key, key2};
class Collectables : public SpriteObject

{
public:
	Collectables(SDL_Rect source, SDL_FRect destination, CollectableType item);
	~Collectables() = default;
	
	
	void Update(float deltaTime) override;
	void Render() override;
};

