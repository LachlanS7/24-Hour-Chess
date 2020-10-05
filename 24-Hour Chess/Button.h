#pragma once
#include "UIElement.h"

enum buttonState {
    down, 
    hover,
    up
};

class Button : public UIElement
{
private:
    std::string m_text = "";
    buttonState m_buttonState = up;
    texture m_downTex = -1;
    void (*callbackptr)();

private:
    void makeTransform();

public:
    Button(utilities::Vec2 position, int width, int height, texture tex, utilities::Vec3 colour, void(*inputCallbackPtr)());
    Button(std::string textureAddress, bool rgba, utilities::Vec2 position, int width, int height, utilities::Vec3 colour, void(*inputCallbackPtr)());

    void addDownTexture(std::string textureAddress, bool rgba);
    buttonState getState(utilities::Vec2* mousePos, mouseState* ms);
    void runCallback();
    glm::mat4 getTransform();
    texture getTexture();
    void drawElement() override;
};

