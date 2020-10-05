#include "Button.h"

Button::Button(utilities::Vec2 position, int width, int height, texture tex, utilities::Vec3 colour, void(*inputCallbackPtr)()) 
	: UIElement(position, width, height, tex, colour) {

	callbackptr = inputCallbackPtr;

    float xi =  2.0f * m_elementPosition.x / 800 - 1;
    float yi = -2.0f * m_elementPosition.y / 800 + 1;
    float xf =  2.0f * (m_elementPosition.x + m_width) / 800 - 1;
    float yf = -2.0f * (m_elementPosition.y + m_height) / 800 + 1;

	std::cout << yi << ", " << yf << std::endl;

    m_transform = glm::scale(m_transform, glm::vec3(width/100, height/100, 0));
    m_transform = glm::translate(m_transform, glm::vec3(xi - xf + width / (1600.0f), yi - height / (1600.0f), 0));
}

bool Button::isDown(utilities::Vec2* mousePos, mouseState* ms) {
	if (*ms == mouseState::leftDown) {
		if (m_elementPosition.x <= mousePos->x && mousePos->x <= m_elementPosition.x + m_width &&
			m_elementPosition.y <= mousePos->y && mousePos->y <= m_elementPosition.y + m_height) {

			return true;
		}
	}
	return false;
}

void Button::runCallback() {
	callbackptr();
}

void Button::drawElement() {

}

glm::mat4 Button::getTransform() {
	return m_transform;
}