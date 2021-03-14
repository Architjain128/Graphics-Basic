#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include <GL/glut.h>
// #include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <camera.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
GLfloat anglex=0;
GLfloat angley=0;
GLfloat anglez=0;
GLfloat X=0.0f;
GLfloat Y=0.0f;
GLfloat Z=0.0f;
GLfloat Cx = 0.0f;

float deltaTime = 0.01f;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

const char *vertexShaderSource ="#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 proj;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = proj * view * model * vec4(aPos.x,aPos.y,aPos.z, 1.0);\n"
    "   ourColor = aColor;\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(ourColor, 1.0f);\n"
    "}\n\0";


int main()
{
    // glfw: initialize and configure
    // how to handle the windows n all are done with this
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers connect to opengl implementaion from driver
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    
    GLfloat vertices[] = {
        0.5f*cos(0*M_PI/180.0f),  0.5f*sin(0*M_PI/180.0f),0,    0,0,0,
        0.5f*cos(36*M_PI/180.0f), 0.5f*sin(36*M_PI/180.0f),0,   0,0,0,
        0.5f*cos(72*M_PI/180.0f), 0.5f*sin(72*M_PI/180.0f),0,    0,0,0,
        0.5f*cos(0*M_PI/180.0f),  0.5f*sin(0*M_PI/180.0f),0,   0,0,0,
        0.5f*cos(72*M_PI/180.0f), 0.5f*sin(72*M_PI/180.0f),0,   0,0,0,
        0.5f*cos(324*M_PI/180.0f),0.5f*sin(324*M_PI/180.0f),0,   0,0,0,
        0.5f*cos(72*M_PI/180.0f), 0.5f*sin(72*M_PI/180.0f),0,   0,0,0,
        0.5f*cos(288*M_PI/180.0f),0.5f*sin(288*M_PI/180.0f),0,   0,0,0,
        0.5f*cos(324*M_PI/180.0f),0.5f*sin(324*M_PI/180.0f),0,   0,0,0,
        0.5f*cos(72*M_PI/180.0f), 0.5f*sin(72*M_PI/180.0f),0,   0,0,0,
        0.5f*cos(108*M_PI/180.0f),0.5f*sin(108*M_PI/180.0f),0,   0,0,0,
        0.5f*cos(288*M_PI/180.0f),0.5f*sin(288*M_PI/180.0f),0,   0,0,0,
        0.5f*cos(108*M_PI/180.0f),0.5f*sin(108*M_PI/180.0f),0,   0,0,0,
        0.5f*cos(252*M_PI/180.0f),0.5f*sin(252*M_PI/180.0f),0,   0,0,0,
        0.5f*cos(288*M_PI/180.0f),0.5f*sin(288*M_PI/180.0f),0,   0,0,0,
        0.5f*cos(108*M_PI/180.0f),0.5f*sin(108*M_PI/180.0f),0,   0,0,0,
        0.5f*cos(144*M_PI/180.0f),0.5f*sin(144*M_PI/180.0f),0,   0,0,0,
        0.5f*cos(252*M_PI/180.0f),0.5f*sin(252*M_PI/180.0f),0,   0,0,0,
        0.5f*cos(144*M_PI/180.0f),0.5f*sin(144*M_PI/180.0f),0,   0,0,0,
        0.5f*cos(216*M_PI/180.0f),0.5f*sin(216*M_PI/180.0f),0,   0,0,0,
        0.5f*cos(252*M_PI/180.0f),0.5f*sin(252*M_PI/180.0f),0,   0,0,0,
        0.5f*cos(144*M_PI/180.0f),0.5f*sin(144*M_PI/180.0f),0,   0,0,0,
        0.5f*cos(180*M_PI/180.0f),0.5f*sin(180*M_PI/180.0f),0,   0,0,0,
        0.5f*cos(216*M_PI/180.0f),0.5f*sin(216*M_PI/180.0f),0,   0,0,0,
        0.5f*cos(0*M_PI/180.0f),  0.5f*sin(0*M_PI/180.0f),0.5f,   1,1,1,
        0.5f*cos(36*M_PI/180.0f), 0.5f*sin(36*M_PI/180.0f),0.5f,  1,1,1,
        0.5f*cos(72*M_PI/180.0f), 0.5f*sin(72*M_PI/180.0f),0.5f,   1,1,1,
        0.5f*cos(0*M_PI/180.0f),  0.5f*sin(0*M_PI/180.0f),0.5f,   1,1,1,
        0.5f*cos(72*M_PI/180.0f), 0.5f*sin(72*M_PI/180.0f),0.5f,   1,1,1,
        0.5f*cos(324*M_PI/180.0f),0.5f*sin(324*M_PI/180.0f),0.5f,   1,1,1,
        0.5f*cos(72*M_PI/180.0f), 0.5f*sin(72*M_PI/180.0f),0.5f,   1,1,1,
        0.5f*cos(288*M_PI/180.0f),0.5f*sin(288*M_PI/180.0f),0.5f,   1,1,1,
        0.5f*cos(324*M_PI/180.0f),0.5f*sin(324*M_PI/180.0f),0.5f,   1,1,1,
        0.5f*cos(72*M_PI/180.0f), 0.5f*sin(72*M_PI/180.0f),0.5f,   1,1,1,
        0.5f*cos(108*M_PI/180.0f),0.5f*sin(108*M_PI/180.0f),0.5f,   1,1,1,
        0.5f*cos(288*M_PI/180.0f),0.5f*sin(288*M_PI/180.0f),0.5f,   1,1,1,
        0.5f*cos(108*M_PI/180.0f),0.5f*sin(108*M_PI/180.0f),0.5f,   1,1,1,
        0.5f*cos(252*M_PI/180.0f),0.5f*sin(252*M_PI/180.0f),0.5f,   1,1,1,
        0.5f*cos(288*M_PI/180.0f),0.5f*sin(288*M_PI/180.0f),0.5f,   1,1,1,
        0.5f*cos(108*M_PI/180.0f),0.5f*sin(108*M_PI/180.0f),0.5f,   1,1,1,
        0.5f*cos(144*M_PI/180.0f),0.5f*sin(144*M_PI/180.0f),0.5f,   1,1,1,
        0.5f*cos(252*M_PI/180.0f),0.5f*sin(252*M_PI/180.0f),0.5f,   1,1,1,
        0.5f*cos(144*M_PI/180.0f),0.5f*sin(144*M_PI/180.0f),0.5f,   1,1,1,
        0.5f*cos(216*M_PI/180.0f),0.5f*sin(216*M_PI/180.0f),0.5f,   1,1,1,
        0.5f*cos(252*M_PI/180.0f),0.5f*sin(252*M_PI/180.0f),0.5f,   1,1,1,
        0.5f*cos(144*M_PI/180.0f),0.5f*sin(144*M_PI/180.0f),0.5f,   1,1,1,
        0.5f*cos(180*M_PI/180.0f),0.5f*sin(180*M_PI/180.0f),0.5f,   1,1,1,
        0.5f*cos(216*M_PI/180.0f),0.5f*sin(216*M_PI/180.0f),0.5f,   1,1,1,
        0.5f*cos(0*M_PI/180.0f),  0.5f*sin(0*M_PI/180.0f),0,   1,0,0,
        0.5f*cos(0*M_PI/180.0f),  0.5f*sin(0*M_PI/180.0f),0.5f,   1,0,0,
        0.5f*cos(36*M_PI/180.0f), 0.5f*sin(36*M_PI/180.0f),0.5f,   1,0,0,
        0.5f*cos(36*M_PI/180.0f), 0.5f*sin(36*M_PI/180.0f),0,   1,1,0,
        0.5f*cos(36*M_PI/180.0f), 0.5f*sin(36*M_PI/180.0f),0.5f,   1,1,0,
        0.5f*cos(72*M_PI/180.0f), 0.5f*sin(72*M_PI/180.0f),0.5f,   1,1,0,
        0.5f*cos(72*M_PI/180.0f), 0.5f*sin(72*M_PI/180.0f),0,   0,1,1,
        0.5f*cos(72*M_PI/180.0f), 0.5f*sin(72*M_PI/180.0f),0.5f,   0,1,1,
        0.5f*cos(108*M_PI/180.0f), 0.5f*sin(108*M_PI/180.0f),0.5f, 0,1,1,
        0.5f*cos(108*M_PI/180.0f), 0.5f*sin(108*M_PI/180.0f),0,   0,0,1,
        0.5f*cos(108*M_PI/180.0f), 0.5f*sin(108*M_PI/180.0f),0.5f,   0,0,1,
        0.5f*cos(144*M_PI/180.0f), 0.5f*sin(144*M_PI/180.0f),0.5f,   0,0,1,
        0.5f*cos(144*M_PI/180.0f), 0.5f*sin(144*M_PI/180.0f),0,   1,0,1,
        0.5f*cos(144*M_PI/180.0f), 0.5f*sin(144*M_PI/180.0f),0.5f,   1,0,1,
        0.5f*cos(180*M_PI/180.0f), 0.5f*sin(180*M_PI/180.0f),0.5f,   1,0,1,
        0.5f*cos(180*M_PI/180.0f), 0.5f*sin(180*M_PI/180.0f),0,   1,0,0,
        0.5f*cos(180*M_PI/180.0f), 0.5f*sin(180*M_PI/180.0f),0.5f,   1,0,0,
        0.5f*cos(216*M_PI/180.0f), 0.5f*sin(216*M_PI/180.0f),0.5f,   1,0,0,
        0.5f*cos(216*M_PI/180.0f), 0.5f*sin(216*M_PI/180.0f),0,   1,1,0,
        0.5f*cos(216*M_PI/180.0f), 0.5f*sin(216*M_PI/180.0f),0.5f,   1,1,0,
        0.5f*cos(252*M_PI/180.0f), 0.5f*sin(252*M_PI/180.0f),0.5f,   1,1,0,
        0.5f*cos(252*M_PI/180.0f), 0.5f*sin(252*M_PI/180.0f),0,   0,1,1,
        0.5f*cos(252*M_PI/180.0f), 0.5f*sin(252*M_PI/180.0f),0.5f,   0,1,1,
        0.5f*cos(288*M_PI/180.0f), 0.5f*sin(288*M_PI/180.0f),0.5f,   0,1,1,
        0.5f*cos(288*M_PI/180.0f), 0.5f*sin(288*M_PI/180.0f),0,   0,0,1,
        0.5f*cos(288*M_PI/180.0f), 0.5f*sin(288*M_PI/180.0f),0.5f,   0,0,1,
        0.5f*cos(324*M_PI/180.0f), 0.5f*sin(324*M_PI/180.0f),0.5f,   0,0,1,
        0.5f*cos(324*M_PI/180.0f), 0.5f*sin(324*M_PI/180.0f),0,   1,0,1,
        0.5f*cos(324*M_PI/180.0f), 0.5f*sin(324*M_PI/180.0f),0.5f,   1,0,1,
        0.5f*cos(0*M_PI/180.0f),   0.5f*sin(0*M_PI/180.0f),0.5f,   1,0,1,
        0.5f*cos(0*M_PI/180.0f),   0.5f*sin(0*M_PI/180.0f),0,   1,0,1,
        0.5f*cos(324*M_PI/180.0f), 0.5f*sin(324*M_PI/180.0f),0,   1,0,1,
        0.5f*cos(0*M_PI/180.0f),   0.5f*sin(0*M_PI/180.0f),0.5f,   1,0,1,
        0.5f*cos(36*M_PI/180.0f),  0.5f*sin(36*M_PI/180.0f),0,   1,0,0,
        0.5f*cos(0*M_PI/180.0f),   0.5f*sin(0*M_PI/180.0f),0,     1,0,0,
        0.5f*cos(36*M_PI/180.0f),  0.5f*sin(36*M_PI/180.0f),0.5f,   1,0,0,
        0.5f*cos(72*M_PI/180.0f),  0.5f*sin(72*M_PI/180.0f),0,   1,1,0,
        0.5f*cos(36*M_PI/180.0f),  0.5f*sin(36*M_PI/180.0f),0,   1,1,0,
        0.5f*cos(72*M_PI/180.0f),  0.5f*sin(72*M_PI/180.0f),0.5f,   1,1,0,
        0.5f*cos(72*M_PI/180.0f),  0.5f*sin(72*M_PI/180.0f),0,   0,1,1,
        0.5f*cos(108*M_PI/180.0f), 0.5f*sin(108*M_PI/180.0f),0,   0,1,1,
        0.5f*cos(108*M_PI/180.0f), 0.5f*sin(108*M_PI/180.0f),0.5f,   0,1,1,
        0.5f*cos(108*M_PI/180.0f), 0.5f*sin(108*M_PI/180.0f),0,   0,0,1,
        0.5f*cos(144*M_PI/180.0f), 0.5f*sin(144*M_PI/180.0f),0,   0,0,1,
        0.5f*cos(144*M_PI/180.0f), 0.5f*sin(144*M_PI/180.0f),0.5f,   0,0,1,
        0.5f*cos(144*M_PI/180.0f), 0.5f*sin(144*M_PI/180.0f),0,   1,0,1,
        0.5f*cos(180*M_PI/180.0f), 0.5f*sin(180*M_PI/180.0f),0,   1,0,1,
        0.5f*cos(180*M_PI/180.0f), 0.5f*sin(180*M_PI/180.0f),0.5f,   1,0,1,
        0.5f*cos(180*M_PI/180.0f), 0.5f*sin(180*M_PI/180.0f),0,   1,0,0,
        0.5f*cos(216*M_PI/180.0f), 0.5f*sin(216*M_PI/180.0f),0,   1,0,0,
        0.5f*cos(216*M_PI/180.0f), 0.5f*sin(216*M_PI/180.0f),0.5f,   1,0,0,
        0.5f*cos(216*M_PI/180.0f), 0.5f*sin(216*M_PI/180.0f),0,   1,1,0,
        0.5f*cos(252*M_PI/180.0f), 0.5f*sin(252*M_PI/180.0f),0,   1,1,0,
        0.5f*cos(252*M_PI/180.0f), 0.5f*sin(252*M_PI/180.0f),0.5f,   1,1,0,
        0.5f*cos(252*M_PI/180.0f), 0.5f*sin(252*M_PI/180.0f),0,   0,1,1,
        0.5f*cos(288*M_PI/180.0f), 0.5f*sin(288*M_PI/180.0f),0,   0,1,1,
        0.5f*cos(288*M_PI/180.0f), 0.5f*sin(288*M_PI/180.0f),0.5f,   0,1,1,
        0.5f*cos(288*M_PI/180.0f), 0.5f*sin(288*M_PI/180.0f),0,   0,0,1,
        0.5f*cos(324*M_PI/180.0f), 0.5f*sin(324*M_PI/180.0f),0,   0,0,1,
        0.5f*cos(324*M_PI/180.0f), 0.5f*sin(324*M_PI/180.0f),0.5f,   0,0,1,
    };


    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // first parameter is for number of buffer objects to create
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);

    // as we only have a single shader, we could also just activate our shader once beforehand if we want to 
    glUseProgram(shaderProgram);

    // render loop


    // Set up projection
    GLint uniModel = glGetUniformLocation(shaderProgram, "model");

    glm::mat4 view = glm::lookAt(glm::vec3(5.0f*cos(Cx), 5.0f, 5.0f),glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(0.0f, 0.0f, 1.0f));
    GLint uniView = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

    // glm::mat4 proj = glm::ortho(1.0f, 1.0f, 1.0f, 1.0f, 5.0f, 10.0f);
    glm::mat4 proj = glm::perspective(glm::radians(90.0f), 800.0f / 600.0f, 5.0f, 100.0f);
    GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        proj = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));


        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model,(anglex*1.0f),glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model,(angley*1.0f),glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model,(anglez*1.0f),glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
        view = camera.GetViewMatrix();
        glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
        view = glm::translate(view, glm::vec3(-X, -Y, -Z));
        glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));



        // render the triangle
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3*36);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // object rotation
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        anglex+=.01f;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        angley+=.01f;
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        anglez+=.01f;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        X += 0.01f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        X -= 0.01f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Y+=0.01f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Y-=0.01f;


    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        Z+=0.01f;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        Z-=0.01f;

    
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        Cx+=0.1f;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}