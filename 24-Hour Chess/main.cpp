#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION

#include "Board.h"
#include "Button.h"
#include "UIElement.h"
#include "GUIWindow.h"

utilities::Vec2 MOUSEPOS = { 0, 0 };
unsigned int BUTTON, ACTION;

//------------------------------- Function Declaration --------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void processInput(GLFWwindow* window, std::unordered_map<int, bool>* inputMap);
std::string fromFile(std::string location);

void sayHello() {
    std::cout << "Haru" << std::endl;
}

//------------------------------- Settings -------------------------
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 800;

int main() {

    //------------------------------- GLFW Initialisation --------------------------------

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 8);


    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "24-Hourish Chess", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //--------------------- Shaders ----------------------

    std::string vertexShaderSourceT = fromFile("vertexShader.vs");
    const char* vertexShaderSource = vertexShaderSourceT.c_str();
    std::string fragmentShaderSourceT = fromFile("fragmentShader.fs");
    const char* fragmentShaderSource = fragmentShaderSourceT.c_str();

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //------------------ Buffers ----------------------

    float vertices[] = {
        // Positions              //Colour       //Tex Coords
        -0.125f, -0.125f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
        -0.125f,  0.125f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
         0.125f,  0.125f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
         0.125f, -0.125f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
    };

    unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };

    unsigned int VBO, EBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
   
    //----------------------------- Game Intialisation ----------------------------------

    Board board;

    unsigned int boardTex = 0;
    utilities::genTexture("assets/boards/green.png", &boardTex, false);

    // Textures

    board.attachTexture(pawn, 0, "assets/pieces/wp.png", true);
    board.attachTexture(pawn, 1, "assets/pieces/bp.png", true);
    board.attachTexture(bishop, 0, "assets/pieces/wb.png", true);
    board.attachTexture(bishop, 1, "assets/pieces/bb.png", true);
    board.attachTexture(rook, 0, "assets/pieces/wr.png", true);
    board.attachTexture(rook, 1, "assets/pieces/br.png", true);
    board.attachTexture(knight, 0, "assets/pieces/wn.png", true);
    board.attachTexture(knight, 1, "assets/pieces/bn.png", true);
    board.attachTexture(king, 0, "assets/pieces/wk.png", true);
    board.attachTexture(king, 1, "assets/pieces/bk.png", true);
    board.attachTexture(queen, 0, "assets/pieces/wq.png", true);
    board.attachTexture(queen, 1, "assets/pieces/bq.png", true);

    bool pressingLeftMouseButton = false;
    utilities::Vec2 pos1 = utilities::Vec2(0, 0);
    utilities::Vec2 pos2 = utilities::Vec2(0, 0);
    int index1 = -1;
    int index2 = -1;

    std::unordered_map<int, bool>* inputMap = {};
    mouseState ms = mouseState::none;

    GUIWindow guiManager(&MOUSEPOS, &ms, inputMap);

    Button b1("assets/buttons/buttonUp.png", true, { 0, 0 }, 235, 80, { 1, 0, 1 }, sayHello);
    Button b2("assets/buttons/buttonDown.png", true, { 200, 300 }, 235, 80, { 1, 0, 1 }, sayHello);
    b1.addDownTexture("assets/buttons/buttonDown.png", true);
    guiManager.addElement(&b1);
    guiManager.addElement(&b2);

    glClearColor(1.0f, 0.2f, 0.1f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //------------------------------------ Game Loop -------------------------------------

    while (!glfwWindowShouldClose(window)) {

        processInput(window, inputMap);

        if (BUTTON == GLFW_MOUSE_BUTTON_LEFT) {
            if (ACTION == GLFW_PRESS && !pressingLeftMouseButton) {
                ms = mouseState::leftDown;
                pos1 = MOUSEPOS;
                index1 = (int)(8 * pos1.x / SCR_WIDTH) + 8 * (7 - (int)(8 * pos1.y / SCR_HEIGHT));
                pressingLeftMouseButton = true;
            }
            else if (pressingLeftMouseButton && ACTION == GLFW_RELEASE) {
                ms = mouseState::leftUp;
                pos2 = MOUSEPOS;
                index2 = (int)(8 * pos2.x / SCR_WIDTH) + 8 * (7 - (int)(8 * pos2.y / SCR_HEIGHT));
                pressingLeftMouseButton = false;
                ms = mouseState::none;
            }
        }
        else {
            ms = mouseState::none;
        }

        if (index1 != -1 && index2 != -1) {
            board.move(index1, index2);
            index1 = -1;
            index2 = -1;
        }

        guiManager.updateElements();

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");

        glm::mat4 transform = glm::mat4(1);
        transform = glm::scale(transform, glm::vec3(8.0f, 8.0f, 0.0f));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glBindTexture(GL_TEXTURE_2D, boardTex);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        for (int i = 0; i < 64; i++) {
            if (board.getPiece(i).id) {

                if (i != index1) {
                    auto pos = board.position(i);

                    glm::mat4 transform = glm::mat4(1);
                    transform = glm::translate(transform, glm::vec3(pos.x / 4 - 0.875, pos.y / 4 - 0.875, 0.0f));

                    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
                    glBindTexture(GL_TEXTURE_2D, board.getTexture(i));
                    glBindVertexArray(VAO);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                }
            }
        }

        if (index1 != -1) {

            transform = glm::mat4(1);
            transform = glm::translate(transform, glm::vec3(2.0f * MOUSEPOS.x / SCR_WIDTH - 1, -2.0f * MOUSEPOS.y / SCR_HEIGHT + 1, 0.0f));

            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
            glBindTexture(GL_TEXTURE_2D, board.getTexture(index1));
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(b1.getTransform()));
        glBindTexture(GL_TEXTURE_2D, b1.getTexture());
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(b2.getTransform()));
        glBindTexture(GL_TEXTURE_2D, b2.getTexture());
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window, std::unordered_map<int, bool>* inputMap) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    SCR_HEIGHT = height;
    SCR_WIDTH = width;
}

std::string fromFile(std::string location) {
    std::string output;

    std::string line;
    std::ifstream file(location);
    if (file.is_open()) {
        while (std::getline(file, line)) {
            output += line + "\n";
        }
        file.close();
    }

    return output;
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    MOUSEPOS = { (float)xpos, (float)ypos };
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    BUTTON = button;
    ACTION = action;
}
