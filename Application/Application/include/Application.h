#pragma once

#include "Engine.h"
#include "GameObjects/iGameObject.h"

class Application : public Engine
{
public:
	Application();
	virtual ~Application();

	// Instantiate game manager and set it to load all his info
	void LoadGM();

	virtual void Update(double fixedDeltaTime);

	virtual void ChangeMode();
};