#include "Window.h"
#include "Events/WindowEvent.h"

class Application {
public:
    Application();
    ~Application();

    void Run();
    void Close();
    void Initialize();
private:
    void OnEvent(Event& e);
    bool OnWindowClose(WindowCloseEvent& e);
    bool isRunning;

private:
    // Window related members
    std::unique_ptr<Window> window;
    

};