#include "Button.h"

Button::Button(utilities::Vec2 position, int width, int height, texture tex, utilities::Vec3 colour, void(*inputCallbackPtr)())
	: UIElement(position, width, height, tex, colour) {

	callbackptr = inputCallbackPtr;
	makeTransform();
}

Button::Button(std::string textureAddress, bool rgba, utilities::Vec2 position, int width, int height, utilities::Vec3 colour, void(*inputCallbackPtr)())
	: UIElement(textureAddress, rgba, position, width, height, colour) {

	callbackptr = inputCallbackPtr;
	makeTransform();
}

void Button::makeTransform() {
	float xf = 2.0f * m_elementPosition.x / 800.0f - 1;
	float yf = -2.0f * m_elementPosition.y / 800.0f + 1;
	float xi = -(float)m_width / 800.0f;
	float yi = (float)m_height / 800.0f;

	float xtrans = xf - xi;
	float ytrans = yf - yi;

	m_transform = glm::scale(m_transform, glm::vec3(m_width / 100.0f, m_height / 100.0f, 0));
	m_transform = glm::translate(m_transform, glm::vec3(100.0f * xtrans / (float)m_width, 100.0f * ytrans / (float)m_height, 0));
}

buttonState Button::getState(utilities::Vec2* mousePos, mouseState* ms) {
	if (*ms == mouseState::leftDown) {
		if (m_elementPosition.x <= mousePos->x && mousePos->x <= m_elementPosition.x + m_width &&
			m_elementPosition.y <= mousePos->y && mousePos->y <= m_elementPosition.y + m_height) {

			m_buttonState = buttonState::down;
			return m_buttonState;
		}
		m_buttonState = buttonState::hover;
		return m_buttonState;
	}
	m_buttonState = buttonState::up;
	return m_buttonState;
}

void Button::addDownTexture(std::string textureAddress, bool rgba) {
	utilities::genTexture(textureAddress, &m_downTex, rgba);
}

void Button::runCallback() {
	callbackptr();
}

void Button::drawElement() {

}

texture Button::getTexture() {
	return (m_buttonState == buttonState::down ? m_downTex == -1 ? m_tex : m_downTex  : m_tex);
}

void Button::update(utilities::Vec2* mousePos, mouseState* ms, std::unordered_map<int, bool>* inputMap) {
	if (getState(mousePos, ms) == buttonState::down) {
		callbackptr();
	}
}

glm::mat4 Button::getTransform() {
	return m_transform;
}