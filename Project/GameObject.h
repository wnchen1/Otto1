#pragma once

#include <SDL.h>

class State;
class GameObject
{
public:
	GameObject()
		: m_destinationTransform({ 0,0,0,0 })
	{ }

	GameObject(const SDL_FRect destination)
		: m_destinationTransform(destination)
	{ }

	virtual ~GameObject() = default;


	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;
	SDL_FRect* GetDestinationTransform() { return &m_destinationTransform; }
	void setDestinationTransformX( float* destination) { destination = &m_destinationTransform.x; }
	void setDestinationTransformY( float* destination) { destination = &m_destinationTransform.y; }

protected:
	SDL_FRect m_destinationTransform;
	
	State* statepointer;
};

