#include <iostream>

// GLEW
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#define GLEW_STATIC

#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include "index.h"

void APIENTRY glDebugOutput(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar *message,
        const void *userParam);

// Function prototypes
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// The MAIN function, from here we start the application and run the game loop
int main() {
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

#ifdef __APPLE__
    /* We need to explicitly ask for a 3.2 context on OS X */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    // important for OS X!!!
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // enable OpenGL debug context if context allows for debug context
    GLint flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        std::cout << "enable gl debug" << std::endl;
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // makes sure errors are displayed synchronously
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
    }

    auto renderer = std::unique_ptr<Renderer>(makeRenderer());
    glfwSetWindowUserPointer(window, renderer.get());

    // 开启深度测试

    glEnable(GL_DEPTH_TEST);
    // Game loop
    while (!glfwWindowShouldClose(window)) {
        // Check if any events have been activated (key pressed, mouse moved etc.)
        // and call corresponding response functions
        glfwPollEvents();

        // Render
        // Clear the color buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer->render(width, height);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    glfwSetWindowUserPointer(window, nullptr);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow *window, int key, __unused int scancode, int action, __unused int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    auto ptr = static_cast<Renderer *>(glfwGetWindowUserPointer(window));
    if (ptr) {
        ptr->handleKeyEvent(key, action);
    }
}

void APIENTRY glDebugOutput(GLenum source,
                            GLenum type,
                            GLuint id,
                            GLenum severity,
                            GLsizei length,
                            const GLchar *message,
                            const void *userParam) {
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return; // ignore these non-significant error codes

    std::cout << "---------------" << std::endl;
    std::cerr << "Debug message (" << id << "): " << message << std::endl;

    switch (source) {
        case GL_DEBUG_SOURCE_API:
            std::cerr << "Source: API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            std::cerr << "Source: Window System";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            std::cerr << "Source: Shader Compiler";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            std::cerr << "Source: Third Party";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            std::cerr << "Source: Application";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            std::cerr << "Source: Other";
            break;
    }
    std::cerr << std::endl;

    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            std::cerr << "Type: Error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            std::cerr << "Type: Deprecated Behaviour";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            std::cerr << "Type: Undefined Behaviour";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            std::cerr << "Type: Portability";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            std::cerr << "Type: Performance";
            break;
        case GL_DEBUG_TYPE_MARKER:
            std::cerr << "Type: Marker";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            std::cerr << "Type: Push Group";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            std::cerr << "Type: Pop Group";
            break;
        case GL_DEBUG_TYPE_OTHER:
            std::cerr << "Type: Other";
            break;
    }
    std::cerr << std::endl;

    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            std::cerr << "Severity: high";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            std::cerr << "Severity: medium";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            std::cerr << "Severity: low";
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            std::cerr << "Severity: notification";
            break;
    }
    std::cerr << std::endl;
    std::cerr << std::endl;
}


#pragma clang diagnostic pop