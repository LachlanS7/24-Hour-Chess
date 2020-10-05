#include "UIElement.h"

UIElement::UIElement(utilities::Vec2 position, int width, int height, texture tex, utilities::Vec3 colour) {
	m_elementPosition = position;
	m_colour = colour;
	m_height = height;
	m_width = width;
	m_tex = tex;
	m_transform = glm::mat4(1);
}

UIElement::UIElement(std::string textureAddress, bool rgba, utilities::Vec2 position, int width, int height, utilities::Vec3 colour) {
	m_elementPosition = position;
	m_colour = colour;
	m_height = height;
	m_width = width;
	m_transform = glm::mat4(1);
	utilities::genTexture(textureAddress, &m_tex, rgba);
}

void UIElement::drawElement() {
	
}