// main.cpp
#include <Shader.h>
#include <iostream>
#include <Circle.h>
#include <thread>
#include <chrono>
#include <init.h>

#include "Camera.h"


// Callback to resize the viewport when window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    (void)window; // Tell the compiler we're intentionally ignoring this parameter
    glViewport(0, 0, width, height);
}
/*void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    UNUSED(scancode);
    UNUSED(mods);
    Circle* circle = (Circle*)glfwGetWindowUserPointer(window);
    if (key == GLFW_KEY_SPACE && action == GLFW_REPEAT) {
        circle->applyForce(glm::vec3(1, 0, 0), 5);
    }
}*/
void processInput(GLFWwindow *window, float deltaTime) {
    Camera* camera = (Camera*)glfwGetWindowUserPointer(window);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera->processKeyboard("LEFT", deltaTime);

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera->processKeyboard("RIGHT", deltaTime);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera->processKeyboard("FORWARD", deltaTime);

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera->processKeyboard("BACKWARD", deltaTime);
}

int main() {
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
#ifdef __APPLE__
    /* We need to explicitly ask for a 3.3 context on Mac */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Window", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD before calling any OpenGL function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // Set viewport and callback
    int frameBufferWidth, frameBufferHeight;
    glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
    glViewport(0, 0, frameBufferWidth, frameBufferHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    initUI();
    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);  // A bit away from the origin
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);  // Up is in positive Y direction
    float yaw   = -90.0f;  // Facing toward negative Z
    float pitch =  0.0f;   // Level (no vertical tilt)

    // Create the camera
    Camera cam(cameraPos, cameraUp, yaw, pitch);
    Circle circle(RADIUS, new glm::vec3(-0.5, 0,0), numTriangles, 0);
    glfwSetWindowUserPointer(window, &cam);
    std::vector<float> vertices = circle.retrieveVertexData();
    std::vector<unsigned int> indices = circle.retrieveIndecesData();
    Circle constraint(LARGERADIUS, new glm::vec3(0, 0,0), NUMLINES, 1);
    std::vector<float> lineVert = constraint.retrieveVertexData();
    std::vector<unsigned int> indecesVert = constraint.retrieveIndecesData();
    //float* vertices = circle.retrieveVertexData();
    //unsigned int* indices = circle.retrieveIndecesData();
    const char fragFile[29] = "Shaders/fragment_shader.glsl";
    const char vertFile[29] = "Shaders/vertex_shader.glsl";
    Shader shader(vertFile, fragFile);
    unsigned int shaderProgram = shader.returnID();

    Buffers c1Buffer = initBuffers(vertices, indices);
    Buffers lineBuffer = initBuffers(lineVert);

    glm::vec4 vec(0.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4 trans = glm::mat4(1.0f);
    glm::mat4 trans2 = glm::mat4(1.0f);

    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
    unsigned int lineLoc = glGetUniformLocation(shaderProgram, "transform");
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int colorLoc = glGetUniformLocation(shaderProgram, "color");
    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    circle.initializeVerlet();
    circle.accel.y = -9.8f;
    // Render loop
    double lastTime = glfwGetTime();
    double timer = lastTime;
    while (!glfwWindowShouldClose(window))
    {
        double const startTime = glfwGetTime();
        double const deltaTime = startTime - lastTime;
        if (int(timer + deltaTime) > int(timer)) {
           // std::cout << int(timer) << std::endl;
        }
        timer = timer + deltaTime;

        lastTime = glfwGetTime();
        ///////////////////////////////////////////////
        glfwPollEvents();
        //glfwSetKeyCallback(window, key_callback);
        processInput(window, deltaTime);
        //RENDER
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        //attempt at drawing line
        glBindVertexArray(lineBuffer.VAO);
        glUniformMatrix4fv(lineLoc, 1, GL_FALSE, glm::value_ptr(trans2));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(cam.getViewMatrix()));
        glm::mat4 projection = glm::perspective(glm::radians(cam.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
       // glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
        glDrawArrays(GL_POINTS, 0, NUMLINES);
        //movement controls
        glBindVertexArray(c1Buffer.VAO);
        circle.nextFrame(deltaTime);
        //std::cout << "time: " << timer << " at: " << position.y << std::endl;
        //glUseProgram(shaderProgram);
        trans = glm::translate(trans, circle.dPos); // move 0.2 units to the right
        vec = trans * vec;
        //std::cout << "time: " << timer << " at: " << vec.y << std::endl;
        //std::cout << vec.y << std::endl;
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        //glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(cam.getViewMatrix()));
        glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
        glDrawElements(GL_TRIANGLES, numTriangles*3, GL_UNSIGNED_INT, 0);
        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        double const elapsed = glfwGetTime() - startTime;
        if (elapsed < TARGET_FRAME_TIME) {
            std::this_thread::sleep_for(std::chrono::duration<double>(TARGET_FRAME_TIME - elapsed));
        }
    }

    // Clean up and exit
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
