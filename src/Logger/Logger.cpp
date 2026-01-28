#include "Logger.h"

#ifdef TRILIZER_PLATFORM_WINDOWS
#include "WindowsDyeFunctionality.h"
#endif
std::shared_ptr<Logger> Logger::s_Instance = std::make_shared<Logger>();

Logger::Logger()
{
#ifdef TRILIZER_PLATFORM_WINDOWS
    consolestate = SaveConsoleState(GetStdHandle(STD_OUTPUT_HANDLE));
#endif
}
Logger::~Logger()
{
#ifdef TRILIZER_PLATFORM_WINDOWS
    RestoreConsoleState(GetStdHandle(STD_OUTPUT_HANDLE), consolestate);
#endif
}
void Logger::DyeConsole(Color color) const
{
#ifdef TRILIZER_PLATFORM_WINDOWS
    WindowsDye(color);
#endif
}



