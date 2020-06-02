#include<iostream>
#include<cmath>

//GLEW
#define GLEW_STATIC
#include<GL/glew.h>

//GLFW
#include<GLFW/glfw3.h>

#include "Utility/Shader.hpp"
#include "Utility/Camera.hpp"
#include "Utility/Textures.hpp"
#include "Utility/SkyBox.hpp"
#include "Utility/Fonts.hpp"

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int screenwidth, int screenheight);
void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
GLuint loadCubemap(std::vector<std::string> faces);

//Window dimensions
const GLint WIDTH = 800, HEIGHT = 800;
GLfloat xrot,yrot,zrot, xpos, ypos, zpos, fov=45.0f;
GLint scrwidth, scrheight;

GLfloat deltaTime = 0.0f;    // Time between current frame and last frame
GLfloat lastFrame = 0.0f; // Time of last frame
GLfloat lastTime = 0.0f;


bool firstMouse = true;
float lastX =  WIDTH / 2.0;
float lastY =  HEIGHT / 2.0;

Camera camera;

int shadeMode = 1; //Blinn Phong Shading
int lightMode = 0; // 0 = Point, 1 = Directional, 2 = spot

GLfloat cubeVertices[] = {
    // positions          // texture Coords     //normal                //tangent
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,           0.0f,  0.0f, -1.0f,    -1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,           0.0f,  0.0f, -1.0f,    -1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,           0.0f,  0.0f, -1.0f,    -1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,           0.0f,  0.0f, -1.0f,    -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,           0.0f,  0.0f, -1.0f,    -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,           0.0f,  0.0f, -1.0f,    -1.0f,  0.0f,  0.0f,
    
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,           0.0f,  0.0f,  1.0f,     1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,           0.0f,  0.0f,  1.0f,     1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,           0.0f,  0.0f,  1.0f,     1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,           0.0f,  0.0f,  1.0f,     1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,           0.0f,  0.0f,  1.0f,     1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,           0.0f,  0.0f,  1.0f,     1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,          -1.0f,  0.0f,  0.0f,     0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,          -1.0f,  0.0f,  0.0f,     0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,          -1.0f,  0.0f,  0.0f,     0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,          -1.0f,  0.0f,  0.0f,     0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,          -1.0f,  0.0f,  0.0f,     0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,          -1.0f,  0.0f,  0.0f,     0.0f,  0.0f,  1.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,           1.0f,  0.0f,  0.0f,     0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,           1.0f,  0.0f,  0.0f,     0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,           1.0f,  0.0f,  0.0f,     0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,           1.0f,  0.0f,  0.0f,     0.0f,  0.0f, -1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,           1.0f,  0.0f,  0.0f,     0.0f,  0.0f, -1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,           1.0f,  0.0f,  0.0f,     0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,           0.0f, -1.0f,  0.0f,     1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,           0.0f, -1.0f,  0.0f,     1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,           0.0f, -1.0f,  0.0f,     1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,           0.0f, -1.0f,  0.0f,     1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,           0.0f, -1.0f,  0.0f,     1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,           0.0f, -1.0f,  0.0f,     1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,           0.0f,  1.0f,  0.0f,     1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,           0.0f,  1.0f,  0.0f,     1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,           0.0f,  1.0f,  0.0f,     1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,           0.0f,  1.0f,  0.0f,     1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,           0.0f,  1.0f,  0.0f,     1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,           0.0f,  1.0f,  0.0f,     1.0f,  0.0f,  0.0f
};

int main(){
    //Initialise glfw
    glfwInit();
    
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr );
    
    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    glewExperimental = GL_TRUE;
    if(GLEW_OK != glewInit()){
        std::cout<<"Failed to initialise GLEW"<<std::endl;
        
        return EXIT_FAILURE;
    }
    
    glfwSetKeyCallback(window, Key_Callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    Shader shader("Shaders/vshad.glsl","Shaders/fshad.glsl");
    Texture albedo("TextureMaps/Texture4/albedo.jpg");
    Texture ao("TextureMaps/Texture4/ao.jpg");
    Texture normal("TextureMaps/Texture4/normal.jpg");
    Texture roughness("TextureMaps/Texture4/roughness.jpg");
    Texture depth("TextureMaps/Texture4/height.png");
    
    GLuint VAO,VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    //tangent attribute
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glBindVertexArray(0);
    
    //Set depth test to less-than
    glDepthFunc(GL_LESS);
    //Enable depth testing
    glEnable(GL_DEPTH_TEST);
    
    glm::vec3 position = glm::vec3(0.9,0.9,0.9);
    
    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    shader.setUniformMat4("model", model);
    shader.setUniformInt("material.albedoMap", 0);
    shader.setUniformInt("material.normalMap", 1);
    shader.setUniformInt("material.roughnessMap", 2);
    shader.setUniformInt("material.aoMap", 3);
    shader.setUniformInt("material.depthMap", 4);
    shader.setUniformVec3("pointLight.position", position);
    shader.setUniformFloat("pointLight.constant", 1.0f);
    shader.setUniformFloat("pointLight.linear", 0.09f);
    shader.setUniformFloat("pointLight.quadratic", 0.032f);
    shader.setUniformVec3("dirLight.direction", glm::vec3(1.0,1.0,1.0));
    shader.setUniformFloat("spotLight.constant", 1.0f);
    shader.setUniformFloat("spotLight.linear", 0.09);
    shader.setUniformFloat("spotLight.quadratic", 0.032);
    shader.setUniformFloat("spotLight.cutOff", glm::cos(glm::radians(10.0f)));
    shader.setUniformFloat("spotLight.outerCutOff", glm::cos(glm::radians(11.0f)));
    shader.setUniformFloat("heightScale", 0.1f);
    
    //Generate CubeMap
    Shader skyboxShader("Shaders/sky_vshad.glsl","Shaders/sky_fshad.glsl");
    std::vector<std::string> faces{
        "Images/skybox/right.jpg",
        "Images/skybox/left.jpg",
        "Images/skybox/top.jpg",
        "Images/skybox/bottom.jpg",
        "Images/skybox/front.jpg",
        "Images/skybox/back.jpg"
    };
    
    SkyBox skybox(faces);
    
    skyboxShader.use();
    skyboxShader.setUniformInt("skybox", 0);
    
    //To depict light
    Shader light("Shaders/light_vert.glsl","Shaders/light_frag.glsl");
    light.use();
    model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, glm::vec3(0.1));
    light.setUniformMat4("model", model);
    light.setUniformVec3("color", glm::vec3(1.0f,1.0f,1.0f));
    
    GLuint lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    
    fontInit();
    // Compile and setup the shader
    Shader font("shaders/font_vert.glsl", "shaders/font_frag.glsl");
    glm::mat4 projtext = glm::ortho(0.0f, static_cast<GLfloat>(WIDTH), 0.0f, static_cast<GLfloat>(HEIGHT));
    font.use();
    font.setUniformMat4("projection", projtext);
    
    int frames = 0;
    std::string fps = "60";
    std::string count;
    int vertexCount = 0;
    //Now we draw in the game loop
    while(!glfwWindowShouldClose(window)){
        
        processInput(window);
        //Check if any events have been activated - key press or something
        glfwPollEvents();
        
        //Render and Clear the color buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective((float)(glm::radians(fov)), (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);
        
        shader.use();
        albedo.use(0);
        normal.use(1);
        roughness.use(2);
        ao.use(3);
        depth.use(4);
        shader.setUniformMat4("view", view);
        shader.setUniformMat4("projection", projection);
        shader.setUniformVec3("cameraPos", camera.GetCamPos());
        shader.setUniformInt("Blinn", shadeMode);
        shader.setUniformInt("lightType", lightMode);
        shader.setUniformVec3("spotLight.position", camera.GetCamPos());
        shader.setUniformVec3("spotLight.direction", camera.GetCamFront());
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        vertexCount+=36;
        
        if(lightMode==0){
            //generate light
            light.use();
            light.setUniformMat4("view", view);
            light.setUniformMat4("projection", projection);
            glBindVertexArray(lightVAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            vertexCount+=36;
        }
        
        //In the end we generate the skybox (actually should be rendered first but we optimize it by setting it at maximum depth, so we can discard fragments which dont pass depth test)
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix to give feeling of infiniteness to skybox
        skyboxShader.setUniformMat4("view", view);
        skyboxShader.setUniformMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skybox.skyVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default
        vertexCount+=36;
    
        frames++;
        if(currentFrame - lastTime >= 1.0f){
            fps = std::to_string(frames);
            frames = 0;
            lastTime = currentFrame;
        }
        vertexCount += RenderText(font, fps, 25.0f, 770.0f, 0.5f, glm::vec3(0.7,0.7,0.0));
        if(vertexCount + 12 < 100)
            count = std::to_string(vertexCount+12);
        else
            count = std::to_string(vertexCount+18);
        
        RenderText(font, count, 25.0f, 740.0f, 0.5f, glm::vec3(0.9,0.0,0.0));
        vertexCount = 0;
        
        //swap screen buffers
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    
    return EXIT_SUCCESS;
}

void framebuffer_size_callback(GLFWwindow* window, int screenwidth, int screenheight){
    scrwidth = screenwidth;
    scrheight = screenheight;
    glViewport(0, 0, screenwidth, screenheight);
}

void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(key == GLFW_KEY_B && action == GLFW_PRESS)
        shadeMode = (shadeMode+1)%2;
    if(key == GLFW_KEY_N && action == GLFW_PRESS)
        lightMode = (lightMode+1)%3;
    
}

void processInput(GLFWwindow *window)
{   
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.KeyboardInput(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.KeyboardInput(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.KeyboardInput(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.KeyboardInput(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
        camera.KeyboardInput(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
        camera.KeyboardInput(UP, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    
    camera.MouseMovement(xoffset, yoffset);  
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (fov >= 1.0f && fov <= 89.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 89.0f)
        fov = 89.0f;
}

