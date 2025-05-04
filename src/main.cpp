// main.cpp
#include <Shader.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <Ball.h>
#include <Camera.h>
#include <Solver.h>
#include <random>

float rf() {
    static std::random_device rd;                      // Non-deterministic seed
    static std::mt19937 gen(rd());                     // Mersenne Twister engine
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);  // Range [0.0, 1.0)

    return dist(gen);  // Call the distribution with the engine
}

// Callback to resize the viewport when window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    UNUSED(window);
    glViewport(0, 0, width, height);
}
void checkArrowKeys(GLFWwindow* window, Camera *camera, float dt) {
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {

        camera->moveLeft(dt);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        camera->moveRight(dt);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        camera->moveUp(dt);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        camera->moveDown(dt);
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        camera->zoom(-dt);
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        camera->zoom(dt);
    }
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    UNUSED(scancode);
    UNUSED(mods);
    Solver* solver = static_cast<Solver*>(glfwGetWindowUserPointer(window));
    if (action == GLFW_REPEAT && key == GLFW_KEY_SPACE) {
        solver->append(new Ball(glm::vec3(rf()*0.8f, rf()*0.8f, rf()*0.8f), 1, ARADIUS, 15, {rf(), rf(), rf()}));
        solver->append(new Ball(glm::vec3(rf()*0.8f, rf()*0.8f, rf()*0.8f), 1, ARADIUS, 15, {rf(), rf(), rf()}));
        solver->append(new Ball(glm::vec3(rf()*0.8f, rf()*0.8f, rf()*0.8f), 1, ARADIUS, 15, {rf(), rf(), rf()}));
    }
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_Q) {
            solver->setConstraint(false);
        }
        if (key == GLFW_KEY_C) {
            std::cout << solver->returnCount() << std::endl;
        }
    }
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
    Icosphere light(0.1f, 5, false);
    Icosphere constraint(1, 3, false);
   // Camera cam(glm::vec3(-5, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    Camera cam(5, 0);
    const char fragFile[29] = "Shaders/fragment_shader.glsl";
    const char vertFile[29] = "Shaders/vertex_shader.glsl";
    Shader shader(vertFile, fragFile);
    unsigned int shaderProgram = shader.returnID();


    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    Solver solver(shaderProgram);
    solver.append(new Ball(glm::vec3(0, 0, 0.7), 1, ARADIUS, 15));
    glfwSetWindowUserPointer(window, &solver);

    constraint.setLocations(shaderProgram);
    light.setLocations(shaderProgram);
    // Render loop
    double lastTime = glfwGetTime();
    double timer = lastTime;
    glm::mat4 lightModel = glm::mat4(1);
    lightModel = glm::translate(lightModel, LIGHTPOS);
    unsigned int lightPosLocation = glGetUniformLocation(shaderProgram, "lightPos");
    unsigned int viewPosLocation = glGetUniformLocation(shaderProgram, "viewPos");
    glfwSetKeyCallback(window, key_callback);
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
        checkArrowKeys(window, &cam, deltaTime);
        //RENDER
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(cam.getView()));
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(cam.getProjection()));
        glUniform3fv(lightPosLocation, 1, glm::value_ptr(LIGHTPOS));
        glUniform3fv(viewPosLocation, 1, glm::value_ptr(cam.getPosition()));
        //glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        solver.nextFrame(deltaTime);
        constraint.initUniforms((float*)CONSTRAINTCOLOR);
        constraint.drawPoints();
        light.initUniforms((float*)LIGHTCOLOR);
        light.setTrans(lightModel);
        light.drawSphere();
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
