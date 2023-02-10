//
// Created by Diego S. Seabra on 08/03/22.
//

#include "Window.h"
#include "../profiling/Logger.h"
#include "../common/Config.h"
#include "../input/Input.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <Tracy.hpp>

WindowImpl* mWindowImpl = nullptr;

WindowImpl::WindowImpl(EngineConfig* pConfig)
{
    window = nullptr;

    Logger::Debug("initializing glfw");
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // tell GLFW to not create a OpenGL context (because we might use Vulkan/DirectX)

    // TODO: Pass engine config here
    Logger::Debug("creating window");
    monitor = glfwGetPrimaryMonitor();
    window = glfwCreateWindow(pConfig->windowSize.width, pConfig->windowSize.height, pConfig->gameTitle.c_str(), nullptr, nullptr);
    title = pConfig->gameTitle;

    if (window == nullptr)
        Logger::Error("failed to create window with GLFW", "window == nullptr");
    else
        Logger::Info("glfw window created successfully");

    glfwSetWindowUserPointer(window, this);
    glfwSetWindowPos(window, Config::GetWindowPositionX(), Config::GetWindowPositionY());
//    glfwSetFramebufferSizeCallback(window, WindowResized);
//    glfwSetWindowSizeLimits(window, 480, 320, GLFW_DONT_CARE, GLFW_DONT_CARE);
//    glfwSetKeyCallback(window, keyCallback);

    mWidth = pConfig->windowSize.width;
    mHeight = pConfig->windowSize.height;

    loadIcon();
}

WindowImpl::~WindowImpl()
{
    Logger::Debug("Destroying window implementation");

    glfwDestroyWindow(mWindowImpl->window);
    glfwTerminate();
}

void WindowImpl::loadIcon()
{
    Logger::Debug("loading icon");

    GLFWimage icons[1];
    icons[0].pixels = stbi_load("assets/icons/icon.png", &icons[0].width, &icons[0].height, nullptr, STBI_rgb_alpha);

    if (!icons[0].pixels)
    {
        throw std::runtime_error("failed to load icon image");
    }

    glfwSetWindowIcon(window, 1, icons);

    stbi_image_free(icons[0].pixels);
}

void WindowImpl::SetFullscreen(bool fullscreen)
{
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    int width = mode->width;
    int height = mode->height;

    if (fullscreen) // this will enable fullscreen with the current user resolution
        glfwSetWindowMonitor(window, monitor, 0, 0, width, height, GLFW_DONT_CARE);
    else
        glfwSetWindowMonitor(window, nullptr, 0, 0, width, height, GLFW_DONT_CARE);
}

bool Window::ShouldCloseWindow()
{
    return glfwWindowShouldClose(mWindowImpl->window);
}

void Window::Init(EngineConfig* pConfig)
{
    Logger::Info("Initializing window");
    mWindowImpl = new WindowImpl(pConfig);
}

void Window::Update()
{
    ZoneScopedC(0x2ecc71);
    glfwPollEvents();
}

void Window::Shutdown()
{
    Logger::Info("Shutting down window");
    delete mWindowImpl;
}

WindowSize Window::GetSize()
{
    return WindowSize
            {
        mWindowImpl->mWidth,
        mWindowImpl->mHeight
            };
}

GLFWwindow *Window::GetWindow()
{
    return mWindowImpl->window;
}

std::vector<const char *> Window::GetRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::cout << "extensions count: " << glfwExtensionCount << std::endl;
    std::cout << *glfwExtensions << std::endl;

    std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    return extensions;
}

double Window::GetTime()
{
    return glfwGetTime();
}

// This method tells GLFW that we want to close the window.
// Because all the engine systems are tied to the window "loop", when closing the window all the engine
//      systems also close automatically :)
void Window::Close()
{
    glfwSetWindowShouldClose(mWindowImpl->window, true);
}

void Window::SetFullscreen(bool fullscreen)
{
    mWindowImpl->SetFullscreen(fullscreen);
}

void Window::EnableVSync(bool flag)
{
    if (flag)
        Logger::Debug("Enabling VSync");
    else
        Logger::Debug("Disabling VSync / Uncapped Frame Rate");

    vsyncEnabled = flag;

    // forcing window resize (to actual size) so that our swapchain is forced to be recreated
    // FIXME: There is a better way to do this? Recreate the swapchain in the middle of its process? (I tried once, didn't work)
    int curWidth, curHeight;
    glfwGetWindowSize(GetWindow(), &curWidth, &curHeight);
    Resize(curWidth + 1, curHeight + 1); // Diego, what the fuck man?! You're better than this!
}

void Window::DragWindow()
{
//    auto windowPos = GetWindowPosition();
//    SetWindowPosition(diffPos.x, diffPos.y);
//
//    auto mousePos = Input::GetMousePosition();
//    glm::vec2 offset = glm::vec2(10,10);
//    auto diffPos = (mousePos - windowPos) + offset;

//    SetWindowPosition(mousePos.x, mousePos.y);
//    std::cout << " POS X: " << posX << " | POS Y: " << posY << std::endl;
}

void Window::DropWindow()
{

}

void Window::Resize(uint32_t width, uint32_t height)
{
    glfwSetWindowSize(GetWindow(), width, height);
}

void Window::SetWindowPosition(uint32_t xPos, uint32_t yPos)
{
    glfwSetWindowPos(GetWindow(), xPos, yPos);
}

glm::vec2 Window::GetWindowPosition()
{
    int posX, posY;
    glfwGetWindowPos(GetWindow(), &posX, &posY);
    return glm::vec2(posX, posY);
}
