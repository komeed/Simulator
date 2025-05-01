// main.cpp
#include <Shader.h>
#include <iostream>
#include <Circle.h>
#include <thread>
#include <chrono>
#include <init.h>
#include <Ball.h>

// Callback to resize the viewport when window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    (void)window; // Tell the compiler we're intentionally ignoring this parameter
    glViewport(0, 0, width, height);
}
void initUI() {
    glEnable(GL_DEPTH_TEST);         // Enables correct 3D depth handling
    glEnable(GL_CULL_FACE);          // Optional: hides back-facing polygons
    glCullFace(GL_BACK);             // Cull back faces
    glFrontFace(GL_CCW);             // Counter-clockwise = front-facing

    glEnable(GL_PROGRAM_POINT_SIZE); // Needed if using gl_PointSize in shaders
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Background color
}

int main() {
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }
//#ifdef __APPLE__
    /* We need to explicitly ask for a 3.3 context on Mac */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//#endif

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
    initUI();

    glViewport(0, 0, frameBufferWidth, frameBufferHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    /*Sphere sphere(1, 20, 20);
    sphere.draw();*/
    Ball sphere(glm::vec3(0, 0, 0),ARADIUS,  10);
    Icosphere constraint(1, 3, false);

    const char fragFile[29] = "Shaders/fragment_shader.glsl";
    const char vertFile[29] = "Shaders/vertex_shader.glsl";
    Shader shader(vertFile, fragFile);
    unsigned int shaderProgram = shader.returnID();


    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    sphere.setLocations(shaderProgram);
    constraint.setLocations(shaderProgram);
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
        //RENDER
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);

        const float radius = 5.0f;
        float camX = sin(glfwGetTime()/2) * radius;
        float camZ = cos(glfwGetTime()/2) * radius;
        glm::mat4 view = glm::lookAt(glm::vec3(camX, 2, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
        //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        sphere.nextFrame(deltaTime);
        constraint.initUniforms(new float[4]{1, 0, 0, 1});
        constraint.drawPoints();
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
