#pragma once
#include "UIElement.h"
class Button : public UIElement
{
private:
    std::string m_text = "";
    void (*callbackptr)();

public:
    Button(utilities::Vec2 position, int width, int height, texture tex, utilities::Vec3 colour, void(*inputCallbackPtr)());
    bool isDown(utilities::Vec2* mousePos, mouseState* ms);
    void runCallback();
    glm::mat4 getTransform();
    void drawElement() override;
};

