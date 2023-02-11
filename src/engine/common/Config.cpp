//
// Created by Diego S. Seabra on 16/07/22.
//

#include "Config.h"

Config::Config()
{
    reader = INIReader("feather.ini");

    if (reader.ParseError() != 0) 
    {
        std::cerr << "Can't read feather .ini configuration file!" << std::endl;
    }
}

Config& Config::GetSingleton()
{
    static Config instance;
    return instance;
}

uint32_t Config::GetWindowWidth()
{
    return GetSingleton().GetWindowWidthImpl();
}

uint32_t Config::GetWindowHeight()
{
    return GetSingleton().GetWindowHeightImpl();
}

uint32_t Config::GetWindowPositionX()
{
    return GetSingleton().GetWindowPositionXImpl();
}

uint32_t Config::GetWindowPositionY()
{
    return GetSingleton().GetWindowPositionYImpl();
}

bool Config::GetSaveToLogFile()
{
    return GetSingleton().GetSaveToLogFileImpl();
}

std::string Config::GetEngineName()
{
    return GetSingleton().GetEngineNameImpl();
}

std::string Config::GetEngineVersion()
{
    return GetSingleton().GetEngineVersionImpl();
}

// Implementations
uint32_t Config::GetWindowWidthImpl()
{
    return static_cast<uint32_t>(reader.GetInteger("Window", "Width", 800));
}

uint32_t Config::GetWindowHeightImpl()
{
    return static_cast<uint32_t>(reader.GetInteger("Window", "Height", 600));
}

uint32_t Config::GetWindowPositionXImpl()
{
    return static_cast<uint32_t>(reader.GetInteger("Window", "PosX", 0));
}

uint32_t Config::GetWindowPositionYImpl()
{
    return static_cast<uint32_t>(reader.GetInteger("Window", "PosY", 0));
}

bool Config::GetSaveToLogFileImpl()
{
    return static_cast<uint32_t>(reader.GetBoolean("Log", "SaveToFile", false));
}

std::string Config::GetEngineNameImpl()
{
    return reader.Get("Engine", "Name", "Elixir Engine");
}

std::string Config::GetEngineVersionImpl()
{
    return reader.Get("Engine", "Version", "v0.0.0");
}





