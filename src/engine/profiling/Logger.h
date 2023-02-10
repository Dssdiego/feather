//
// Created by Diego S. Seabra on 14/03/22.
//

#ifndef FEATHER_LOGGER_H
#define FEATHER_LOGGER_H

#define SHOW_CONSOLE

#include <iostream>
#include <fstream>

#include <iomanip>
#include <ctime>
#include <sstream>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#endif

#if __linux__ || __APPLE__
#define RESET "\e[0m"
#define COLOR_BLACK "\e[0;30m"
#define COLOR_GRAY "\e[0;37m"
#define COLOR_RED "\e[0;91m"
#define COLOR_GREEN "\e[0;92m"
#define COLOR_YELLOW "\e[0;93m"
#define COLOR_BLUE "\e[0;94m"
#define COLOR_PURPLE "\e[0;95m"
#define COLOR_CYAN "\e[0;96m"
#define COLOR_WHITE "\e[0;97m"
#else
#define COLOR_GRAY 8
#define COLOR_RED 12
#define COLOR_GREEN 10
#define COLOR_YELLOW 14
#define COLOR_BLUE 9
#define COLOR_PURPLE 13
#define COLOR_CYAN 11
#define COLOR_WHITE 7
#endif

const std::string INFO_STR = "INFO::";
const std::string WARN_STR = "WARN::";
const std::string ERROR_STR = "ERROR::";
const std::string DEBUG_STR = "DEBUG::";

class Logger
{
public:
    static void Init();

    static void Info(std::string msg);
    static void Warn(std::string msg);
    static void Error(std::string errMsg, std::string errParam);
    static void Debug(std::string msg);

    inline static std::vector<std::string> GetLogs() { return logs; }

private:
    static void WriteToLogFile(std::string type, std::string msg);

    inline static std::vector<std::string> logs{};
};


#endif //FEATHER_LOGGER_H
