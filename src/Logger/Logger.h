#pragma once
#include "Core.h"
#include <memory>
#include <string>
#include <iostream>

class Logger {
private:
    static std::shared_ptr<Logger> s_Instance;
public:
    enum class Color {
        Default,
        Red,
        Green,
        Yellow,
        Blue,
        Magenta,
        Cyan,
        White
    };

    Logger();

    ~Logger();
    

    // template <typename... Args>
    // void Log(Color color, const Args&... args) {
    //     DyeConsole(color);
    //     Log(args...);
    //     DyeConsole(Color::Default);
    // }
    // template <typename First, typename... Args>
    // void Log(const First first,const Args&... args){
    //     std::cout << first << ' ';
    //     Log(args...);
    // }

    void Log(std::string str){
        std::cout << str << std::endl;
    }

    static std::shared_ptr<Logger> GetInstance(){
        return s_Instance;
    }
    void DyeConsole(Color color) const;
};

#ifdef DEBUG

#define ENGINELOG(x) Logger::GetInstance()->DyeConsole(Logger::Color::Green); \ 
                     Logger::GetInstance()->Log(x);


#define CORELOG(x)   Logger::GetInstance()->DyeConsole(Logger::Color::Cyan); \ 
                     Logger::GetInstance()->Log(x);

#define TRACELOG(x)    Logger::GetInstance()->DyeConsole(Logger::Color::White); \ 
                     Logger::GetInstance()->Log(x);

#define WARNLOG(x)   Logger::GetInstance()->DyeConsole(Logger::Color::Yellow); \ 
                     Logger::GetInstance()->Log(x);

#define ERRLOG(x)    Logger::GetInstance()->DyeConsole(Logger::Color::Red); \ 
                     Logger::GetInstance()->Log(x);

#define INITLOG(x)     Logger::GetInstance()->DyeConsole(Logger::Color::Magenta); \ 
                     Logger::GetInstance()->Log(x);

#else

    #define ENGINELOG(x) 
    #define CORELOG(x) 
    #define TRACELOG(x) 
    #define WARNLOG(x)  
    #define ERRLOG(x) 
    #define INITLOG(x) 
#endif