#include "GUIWindow.h"

GUIWindow::GUIWindow(utilities::Vec2* mousePos, mouseState* mousestate, std::unordered_map<int, bool>* inputMap) {
	m_inputMap = inputMap;
	m_mousePosition = mousePos;
	m_mouseState = mousestate;
}

void GUIWindow::addElement(UIElement* element) {
	m_elements.push_back(element);
}

void GUIWindow::updateElements() {
	for (int i = 0; i < m_elements.size(); i++) {
		m_elements[i]->update(m_mousePosition, m_mouseState, m_inputMap);
	}
}
