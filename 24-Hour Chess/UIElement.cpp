#include "UIElement.h"

UIElement::UIElement(utilities::Vec2 position, int width, int height, texture tex, utilities::Vec3 colour) {
	m_elementPosition = position;
	m_colour = colour;
	m_height = height;
	m_width = width;
	m_tex = tex;
	m_transform = glm::mat4(1);
}

void UIElement::drawElement() {
	
}