#pragma once
#include <vector>
#include <unordered_map>

#include "UIElement.h"

class GUIWindow
{
private:
	std::vector<UIElement*> m_elements = {};
	std::unordered_map<int, bool>* m_inputMap;
	mouseState* m_mouseState;
	utilities::Vec2* m_mousePosition;

public:

	GUIWindow(utilities::Vec2* mousePos, mouseState* mousestate, std::unordered_map<int, bool>* inputMap);
	void addElement(UIElement* element);
	void updateElements();
};

