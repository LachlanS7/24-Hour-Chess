#pragma once
#include <vector>
#include <unordered_map>

#include "UIElement.h"

class GUIWindow
{
private:
	std::vector<UIElement*> m_components = {};
	bool m_active = true;

public:
	std::unordered_map<int, bool>* inputMap;
	utilities::Vec2* mousePos;
	mouseState* ms;

public:
	GUIWindow(utilities::Vec2* i_mousePos, mouseState* mousestate, std::unordered_map<int, bool>* im);
	void addElement(UIElement* element);
};

