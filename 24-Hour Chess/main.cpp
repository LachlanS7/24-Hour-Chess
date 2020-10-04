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

#include "stb_image.h"
#include "utilities.h"
#include "Board.h"

unsigned int XPOS, YPOS;
unsigned int BUTTON, ACTION;


//------------------------------- Function Declaration --------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void processInput(GLFWwindow* window, std::unordered_map<int, bool>* inputMap);
std::string fromFile(std::string location);
inline void loadImage(std::string address, unsigned int* data, bool rgba = false);


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

    //---------------------------------- Textures ---------------------------------------
    
    unsigned int textureArray[13] = { 0 };
    loadImage("assets/boards/green.png", &textureArray[0]);
    loadImage("assets/pieces/wp.png", &textureArray[1], true);
    loadImage("assets/pieces/bp.png", &textureArray[2], true);
    loadImage("assets/pieces/wn.png", &textureArray[3], true);
    loadImage("assets/pieces/bn.png", &textureArray[4], true);
    loadImage("assets/pieces/wr.png", &textureArray[5], true);
    loadImage("assets/pieces/br.png", &textureArray[6], true);
    loadImage("assets/pieces/wb.png", &textureArray[7], true);
    loadImage("assets/pieces/bb.png", &textureArray[8], true);
    loadImage("assets/pieces/wq.png", &textureArray[9], true);
    loadImage("assets/pieces/bq.png", &textureArray[10], true);
    loadImage("assets/pieces/wk.png", &textureArray[11], true);
    loadImage("assets/pieces/bk.png", &textureArray[12], true);

    class PieceTextureMap {
    private:
        std::unordered_map<unsigned int, unsigned int> pieceTextureMap;
    public:
        PieceTextureMap() {
            pieceTextureMap = { 
                {1830, 1},
                {1831, 2},
                {128, 3},
                {129, 4},
                {2096, 5},
                {2097, 6},
                {320, 7},
                {321, 8},
                {112, 9},
                {113, 10},
                {122, 11},
                {123, 12}
            };
        }

        unsigned int getId(Piece piece) {
            if (piece.id >= 4096) {
                return pieceTextureMap[piece.id - 4096];
            }
            return pieceTextureMap[piece.id];
        }
    };

    //----------------------------- Game Intialisation ----------------------------------

    Board board;
    std::unordered_map<int, bool> inputMap;
    PieceTextureMap pieceTextureMap;

    board.insertPiece({ 0, 0 }, rook);
    board.insertPiece({ 1, 0 }, knight);
    board.insertPiece({ 2, 0 }, bishop);
    board.insertPiece({ 3, 0 }, king);
    board.insertPiece({ 4, 0 }, queen);
    board.insertPiece({ 5, 0 }, bishop);
    board.insertPiece({ 6, 0 }, knight);
    board.insertPiece({ 7, 0 }, rook);

    board.insertPiece({ 0, 1 }, pawn);
    board.insertPiece({ 1, 1 }, pawn);
    board.insertPiece({ 2, 1 }, pawn);
    board.insertPiece({ 3, 1 }, pawn);
    board.insertPiece({ 4, 1 }, pawn);
    board.insertPiece({ 5, 1 }, pawn);
    board.insertPiece({ 6, 1 }, pawn);
    board.insertPiece({ 7, 1 }, pawn);

    board.insertPiece({ 0, 6 }, pawn+1);
    board.insertPiece({ 1, 6 }, pawn+1);
    board.insertPiece({ 2, 6 }, pawn+1);
    board.insertPiece({ 3, 6 }, pawn+1);
    board.insertPiece({ 4, 6 }, pawn+1);
    board.insertPiece({ 5, 6 }, pawn+1);
    board.insertPiece({ 6, 6 }, pawn+1);
    board.insertPiece({ 7, 6 }, pawn+1);

    board.insertPiece({ 0, 7 }, rook+1);
    board.insertPiece({ 1, 7 }, knight+1);
    board.insertPiece({ 2, 7 }, bishop+1);
    board.insertPiece({ 3, 7 }, king+1);
    board.insertPiece({ 4, 7 }, queen+1);
    board.insertPiece({ 5, 7 }, bishop+1);
    board.insertPiece({ 6, 7 }, knight+1);
    board.insertPiece({ 7, 7 }, rook+1);

    glClearColor(1.0f, 0.2f, 0.1f, 1.0f);

    bool pressingLeftMouseButton = false;
    Vec2 pos1 = Vec2(0, 0);
    Vec2 pos2 = Vec2(0, 0);
    int index1 = -1;
    int index2 = -1;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //------------------------------------ Game Loop -------------------------------------

    while (!glfwWindowShouldClose(window)) {


        processInput(window, &inputMap);

        if (BUTTON == GLFW_MOUSE_BUTTON_LEFT) {
            if (ACTION == GLFW_PRESS && !pressingLeftMouseButton) {
                pos1 = Vec2(XPOS, YPOS);
                index1 = (int)(8 * pos1.x / SCR_WIDTH) + 8 * (7 - (int)(8 * pos1.y / SCR_HEIGHT));
                pressingLeftMouseButton = true;
            }
            else if (pressingLeftMouseButton && ACTION == GLFW_RELEASE) {
                pos2 = Vec2(XPOS, YPOS);
                index2 = (int)(8 * pos2.x / SCR_WIDTH) + 8 * (7 - (int)(8 * pos2.y / SCR_HEIGHT));
                pressingLeftMouseButton = false;
            }
        }
        if (index1 != -1 && index2 != -1) {
            board.move(index1, index2);
            index1 = -1;
            index2 = -1;
        }

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");

        glm::mat4 transform = glm::mat4(1);
        transform = glm::scale(transform, glm::vec3(8.0f, 8.0f, 0.0f));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glBindTexture(GL_TEXTURE_2D, textureArray[0]);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        for (int i = 0; i < 64; i++) {
            if (board.getPiece(i).id) {

                auto pos = board.position(i);

                glm::mat4 transform = glm::mat4(1);
                transform = glm::translate(transform, glm::vec3(pos.x / 4 - 0.875, pos.y / 4 - 0.875, 0.0f));

                glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
                glBindTexture(GL_TEXTURE_2D, textureArray[pieceTextureMap.getId(board.getPiece(i))]);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            }
        }

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
    XPOS = xpos;
    YPOS = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    BUTTON = button;
    ACTION = action;
}

inline void loadImage(std::string address, unsigned int* texture, bool rgba) {

    glGenTextures(1, texture);
    glBindTexture(GL_TEXTURE_2D, *texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(address.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        rgba ? glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data) :
               glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}