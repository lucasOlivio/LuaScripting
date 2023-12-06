#pragma once

#include "Event.h"
#include "InputProperties.h"

// Key callback events triggered by glfw on window
class KeyEvent : public Event
{
private:
	sKeyInfo m_sKeyInfo;
public:
	KeyEvent();
	virtual ~KeyEvent();

	void UpdateKey(int key, int scancode, int action, int mods);

	sKeyInfo GetKeyInfo();
};