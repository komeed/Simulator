// main.cpp
#include <Shader.h>
#include <iostream>
#include <Circle.h>
#include <thread>
#include <chrono>
#include <init.h>
#include <Sphere.h>

// Callback to resize the viewport when window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    (void)window; // Tell the compiler we're intentionally ignoring this parameter
    glViewport(0, 0, width, height);
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
    glViewport(0, 0, frameBufferWidth, frameBufferHeight);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    initUI();
    Sphere sphere(1, 20, 20);
    sphere.draw();
    std::vector<float> pointVert = sphere.getVertices();
    std::vector<unsigned int> indicesVert = sphere.getIndices();
    //float* vertices = circle.retrieveVertexData();
    //unsigned int* indices = circle.retrieveIndecesData();
    const char fragFile[29] = "Shaders/fragment_shader.glsl";
    const char vertFile[29] = "Shaders/vertex_shader.glsl";
    Shader shader(vertFile, fragFile);
    unsigned int shaderProgram = shader.returnID();

    Buffers circleBuffer = initBuffers(pointVert, indicesVert);
    Buffers pointBuffer = initBuffers(pointVert);

    glm::mat4 trans = glm::mat4(1.0f);

    unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int colorLoc = glGetUniformLocation(shaderProgram, "color");
    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
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
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

        glBindVertexArray(circleBuffer.VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, circleBuffer.EBO);
        glUniform4f(colorLoc, 1, 1, 1, 1);
        glDrawElements(GL_TRIANGLES, indicesVert.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(pointBuffer.VAO);
        glUniform4f(colorLoc, 1, 0, 0, 1);
        glDrawArrays(GL_POINTS, 0, pointVert.size()/3);
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
