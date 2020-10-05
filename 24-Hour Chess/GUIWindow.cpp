#include "GUIWindow.h"

GUIWindow::GUIWindow(utilities::Vec2* i_mousePos, mouseState* mousestate, std::unordered_map<int, bool>* im) {
	inputMap = im;
	mousePos = i_mousePos;
	ms = mousestate;
}

void GUIWindow::addElement(UIElement* element) {
	m_components.push_back(element);
}

