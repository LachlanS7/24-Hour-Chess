#pragma once
#include "utilities.h"

#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum mouseState {
	none,
	leftDown,
	leftHold,
	leftUp,
	rightDown,
	rightHold,
	rightUp
};

class UIElement
{
protected:
	utilities::Vec2 m_elementPosition = { 0, 0 };
	unsigned int m_width = 0;
	unsigned int m_height = 0;
	texture m_tex = 0;
	utilities::Vec3 m_colour = { 0, 0, 0 };
	glm::mat4 m_transform;

public:
	UIElement(utilities::Vec2 position = { 0, 0 }, int width = 0, int height = 0, texture tex = -1, utilities::Vec3 colour = { 0, 0, 0 });
	UIElement(std::string textureAddress, bool rgba = false, utilities::Vec2 position = { 0, 0 }, int width = 0, int height = 0, utilities::Vec3 colour = { 0, 0, 0 });
	virtual void drawElement() = 0;
	virtual void update(utilities::Vec2* mousePos, mouseState* ms, std::unordered_map<int, bool>* inputMap);
};

