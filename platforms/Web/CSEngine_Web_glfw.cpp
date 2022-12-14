// GLXBasics.c
// Use GLX to setup OpenGL windows
// Draw eyeballs
// OpenGL SuperBible, 5th Edition
// Nick Haemel

#include "../../src/Manager/MainProc.h"
#include "../../src/Manager/InputMgr.h"
#include "../../src/MacroDef.h"
#define GLFW_INCLUDE_ES3

#include <GLFW/glfw3.h>
#include <emscripten.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include <math.h>
#include <chrono>
#include <ctime>

using namespace CSE;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

long long int timeGetTime() {
    auto ms_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

    return ms_since_epoch;
}
long long int elapsedTime = 0;
GLFWwindow* window = nullptr;
MainProc* mainProc = nullptr;

void mainLoop() {
    auto deltaTime = timeGetTime() - elapsedTime;
    // std::cout << deltaTime <<'\n';
    /* Render here */
    mainProc->Update(deltaTime);
    mainProc->Render(deltaTime);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);
    // glfwSwapInterval(0);

    /* Poll for and process events */
    glfwPollEvents();
}

int main(void) {

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // glfwWindowHint(GLFW_CLIENT_API,GLFW_OPENGL_ES_API);
    // glfwWindowHint(GLFW_CONTEXT_CREATION_API,GLFW_EGL_CONTEXT_API);
    // glfwDefaultWindowHints();

    //OpenGL ES
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
//    glfwWindowHint(GLFW_SAMPLES, 4);


    //OpenGL core
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_SAMPLES, 4);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int width = 400;
    int height = 400;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "CSEngine", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    InputMgr::SetCanvasSize(width, height);
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) {
        InputMgr::CursorPositionCallback(x, y);
    });

//     glewExperimental = GL_TRUE;
//     GLenum err=glewInit();
//     if(err!=GLEW_OK) {
//         // Problem: glewInit failed, something is seriously wrong.
//         std::cout << "glewInit failed: " << glewGetErrorString(err) << std::endl;
//         return -1;
//     }

    printf("GL_VERSION  : %s\n", glGetString(GL_VERSION));
    printf("GL_RENDERER : %s\n", glGetString(GL_RENDERER));
    printf("GLSL_VERSION : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    mainProc = new MainProc();
    mainProc->Init(width, height);
    elapsedTime = timeGetTime();
    /* Loop until the user closes the window */
    emscripten_set_main_loop(&mainLoop, 0, 1);
    SAFE_DELETE(mainProc);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}