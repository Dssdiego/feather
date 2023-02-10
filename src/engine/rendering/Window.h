//
// Created by Diego S. Seabra on 08/03/22.
//

#ifndef FEATHER_WINDOW_H
#define FEATHER_WINDOW_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/vec2.hpp>
#include <yaml.h>
#include "../common/structs.h"

struct WindowImpl
{
    WindowImpl(EngineConfig* pConfig);
    ~WindowImpl();

    GLFWwindow* window;
    GLFWmonitor* monitor;

    uint32_t mWidth, mHeight;
    std::string title;

    void loadIcon();

    void SetFullscreen(bool fullscreen);
    bool isFullscreen = false;
};

class Window
{
public:
    // lifecycle methods
    static void Init(EngineConfig* pConfig);
    static void Update();
    static void Shutdown();

    // vulkan stuff
    static std::vector<const char *> GetRequiredExtensions();

    // methods
    static bool ShouldCloseWindow();
    static WindowSize GetSize();
    static GLFWwindow* GetWindow();
    static double GetTime();
    static void Close();
    static void SetFullscreen(bool fullscreen);
    static void EnableVSync(bool flag);

    static glm::vec2 GetWindowPosition();
    static void SetWindowPosition(uint32_t xPos, uint32_t yPos);
    static void Resize(uint32_t width, uint32_t height);

    inline static void SetFrameInfo(double _fps, uint32_t _frameNumber) {
        fps = _fps;
        frameNumber = _frameNumber;

        if (fpsHistory.size() > 50)
            fpsHistory.resize(0);
        else
            fpsHistory.push_back((float) fps);
    }
    inline static double GetFPS() { return fps; }
    inline static std::vector<float> GetFPSHistory() { return fpsHistory; }
    inline static uint32_t GetFrameNumber() { return frameNumber; }
    inline static std::vector<std::string> GetResolutions() { return resolutions; }
    inline static bool IsVSyncEnabled() { return vsyncEnabled; }

private:
    static void DragWindow();
    static void DropWindow();

    inline static double fps = 0.f;
    inline static uint32_t frameNumber = 0;
    inline static std::vector<float> fpsHistory = {};

    inline static bool vsyncEnabled = false;

    inline static std::vector<std::string> resolutions = {"640x480", "800x600", "1280x720", "1920x1080", "2560x1080"};
};

#endif //FEATHER_WINDOW_H
