#pragma once
#include "enginepch.h"
#include <Windows.h>
struct ConsoleState
{
    WORD attributes;
    DWORD mode;
};

ConsoleState consolestate;

ConsoleState SaveConsoleState(HANDLE h)
{
    ConsoleState s{};

    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(h, &info);
    s.attributes = info.wAttributes;

    GetConsoleMode(h, &s.mode);

    return s;
}

void RestoreConsoleState(HANDLE h, const ConsoleState& s)
{
    SetConsoleTextAttribute(h, s.attributes);
    SetConsoleMode(h, s.mode);
}



inline void WindowsDye(Logger::Color color){

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (color)
    {
    case Logger::Color::Default:
        SetConsoleTextAttribute(hConsole, 15);
        break; // Default Logger::Color
    case Logger::Color::Red:
        SetConsoleTextAttribute(hConsole, 12);
        break; // Red
    case Logger::Color::Green:
        SetConsoleTextAttribute(hConsole, 10);
        break; // Green
    case Logger::Color::Yellow:
        SetConsoleTextAttribute(hConsole, 14);
        break; // Yellow
    case Logger::Color::Blue:
        SetConsoleTextAttribute(hConsole, 9);
        break; // Blue
    case Logger::Color::Magenta:
        SetConsoleTextAttribute(hConsole, 13);
        break; // Magenta
    case Logger::Color::Cyan:
        SetConsoleTextAttribute(hConsole, 11);
        break; // Cyan
    case Logger::Color::White:
        SetConsoleTextAttribute(hConsole, 15);
        break; // White
    default:
        break;
    }
}