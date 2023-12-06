#include "events/KeyEvent.h"
#include "EngineScripting/ScriptingSystem.h"

KeyEvent::KeyEvent()
{
}

KeyEvent::~KeyEvent()
{
}

void KeyEvent::UpdateKey(int key, int scancode, int action, int mods)
{
	m_sKeyInfo.pressedKey = key;
	m_sKeyInfo.scanCode = scancode;
	m_sKeyInfo.action = action;
	m_sKeyInfo.mods = mods;
	Event::Notify("keyevent");

	ScriptingSystem::Get()->OnKeyInput(m_sKeyInfo);
}

sKeyInfo KeyEvent::GetKeyInfo()
{
	return m_sKeyInfo;
}
