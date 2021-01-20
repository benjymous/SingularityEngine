#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

static const int MAX_SPRITE=500;

struct SpriteData
{
    glm::vec2 position;
    glm::vec2 size;
    glm::vec2 uv;
};

int numSprites = 0;
SpriteData sprites[MAX_SPRITE];

extern "C" {

#include "Singularity.h"

    void Init();
    void Shutdown();
    int Update();

    int gamepadVal = 0;

    int SE_GetGamepad(void)
    {
        return gamepadVal;
    }

    SpriteHandle SE_CreateSprite(float x, float y, int w, int h, int image)
    {
        int index = numSprites++;
        sprites[index].position = { x, y };
        sprites[index].size = { w, h };
        return index;
    }


    void SE_MoveSpriteAbs(SpriteHandle index, SENumber x, SENumber y)
    {
        sprites[index].position = glm::vec2(x, y);
    }

    // Translate the 8 bit '6 bit fixed point' at 50fps into modern 60fps
    const float fudge = 2.5; // fiddled until speed seemed to roughly match
    const float frametime = 1.0f / 60.0f * fudge;

    void SE_MoveSpriteRel(SpriteHandle index, float dx, float dy)
    {
        sprites[index].position += glm::vec2(dx * frametime, dy * frametime);
    }

    void SE_GetSpritePos(SpriteHandle index, SENumber* x, SENumber* y)
    {
        *x = sprites[index].position.x;
        *y = sprites[index].position.y;
    }

    void SE_DestroySprite(SpriteHandle handle)
    {

    }

    void SE_SetSpriteImage(SpriteHandle handle, int image)
    {

    }

    void SE_PutTile(int x, int y, int tile)
    {
        // Temp!
        SE_CreateSprite(x * 8, y * 8, 8, 8, 0);
    }

    int SE_BoundaryDistance(SENumber val, int mod)
    {
        float f = abs(fmodf(val, (float)mod));
        if (f > mod / 2) f = mod - f;        

        return (int)f;
    }

}

const GLchar* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 sprPos;
layout (location = 2) in vec2 sprSize;
layout (location = 3) in vec2 sprUV;
uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4((aPos*sprSize) + sprPos, 0.0, 1.0);
}
)";

const GLchar* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(0.7f, 0.7f, 0.7f, 1.0f);
}
)";

float vertices[] = {
    0, 0,
    1, 0,
    1, 1,

    0, 1,
    0, 0,
    1, 1
};

glm::mat4 projection;
glm::mat4 view = glm::mat4(1.0f);

void reshapeWindow(GLFWwindow* window, int width, int height)
{
    // spectrum = 256x192
    // hd wide = 340x192      42 : 256 : 42

    const float virtualWidth = 256.0f;
    const float virtualHeight = 192.0f;

    const float multiplier = (float)height / virtualHeight;

    const float scaledWidth = (float)width / multiplier;

    const float margin = (scaledWidth - virtualWidth) / 2.0f;

    projection = glm::ortho(-margin, virtualWidth + margin, virtualHeight, 0.0f, -1.0f, 1.0f);
    glViewport(0, 0, width, height);
}

void errorCallback(int, const char* err_str)
{
    std::cout << "GLFW Error: " << err_str << std::endl;
}

void GLAPIENTRY
MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
}



void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if ((key == GLFW_KEY_A || key == GLFW_KEY_LEFT) && action == GLFW_PRESS)
        gamepadVal |= SE_Gamepad_Left;

    if ((key == GLFW_KEY_A || key == GLFW_KEY_LEFT) && action == GLFW_RELEASE)
        gamepadVal &= ~SE_Gamepad_Left;

    if ((key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) && action == GLFW_PRESS)
        gamepadVal |= SE_Gamepad_Right;

    if ((key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) && action == GLFW_RELEASE)
        gamepadVal &= ~SE_Gamepad_Right;

    if ((key == GLFW_KEY_W || key == GLFW_KEY_UP) && action == GLFW_PRESS)
        gamepadVal |= SE_Gamepad_Up;

    if ((key == GLFW_KEY_W || key == GLFW_KEY_UP) && action == GLFW_RELEASE)
        gamepadVal &= ~SE_Gamepad_Up;

    if ((key == GLFW_KEY_S || key == GLFW_KEY_DOWN) && action == GLFW_PRESS)
        gamepadVal |= SE_Gamepad_Down;

    if ((key == GLFW_KEY_S || key == GLFW_KEY_DOWN) && action == GLFW_RELEASE)
        gamepadVal &= ~SE_Gamepad_Down;
}

int main(void)
{
    GLFWwindow* window;

    glfwSetErrorCallback(errorCallback);

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int winHeight = 720;
    int winWidth = 1280;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(winWidth, winHeight, "Singularity Engine", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }



    reshapeWindow(window, winWidth, winHeight);

    // During init, enable debug output
    glEnable(GL_DEBUG_OUTPUT);
    //glDebugMessageCallback(MessageCallback, 0);

    //glfwSetFramebufferSizeCallback(window, reshapeWindow);
    glfwSetWindowSizeCallback(window, reshapeWindow);

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertex_shader);

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragment_shader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) 
    {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    Init();

    GLint sprPosAttr = glGetAttribLocation(program, "sprPos");
    GLint sprSizeAttr = glGetAttribLocation(program, "sprSize");
    GLint sprUVAttr = glGetAttribLocation(program, "sprUV");
    GLint projectionUniform = glGetUniformLocation(program, "projection");

    unsigned int VBO, VAO, ibo;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &ibo);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // create instance buffer

    glBindBuffer(GL_ARRAY_BUFFER, ibo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sprites), sprites, GL_STATIC_DRAW);

    glEnableVertexAttribArray(sprPosAttr);
    glVertexAttribPointer(sprPosAttr, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteData), 0);
    glVertexAttribDivisor(sprPosAttr, 1);

    glEnableVertexAttribArray(sprSizeAttr);
    glVertexAttribPointer(sprSizeAttr, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteData), (void*)sizeof(glm::vec2));
    glVertexAttribDivisor(sprSizeAttr, 1);

    glEnableVertexAttribArray(sprUVAttr);
    glVertexAttribPointer(sprUVAttr, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteData), (void*)(sizeof(glm::vec2)*2));
    glVertexAttribDivisor(sprUVAttr, 1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glfwSwapInterval(1);

    glfwSetKeyCallback(window, keyCallback);

    bool running = true;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window) && running)
    {
        running = Update() != 0;

        glClear(GL_COLOR_BUFFER_BIT);

        // draw our sprites
        glUseProgram(program);
        glBindVertexArray(VAO); 

        glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));

        glBindBuffer(GL_ARRAY_BUFFER, ibo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(sprites), sprites, GL_STATIC_DRAW);

        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, numSprites);


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    Shutdown();

    glfwTerminate();
    return 0;
}

