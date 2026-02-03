#include "Window.h"
#include "Events/WindowEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

class Application {
public:
    Application();
    ~Application();

    void Run();
    void Close();
    void Initialize();
private:
    void OnEvent(Event& e);
    bool ProcessKeyPress(KeyPressEvent& e);
    bool OnWindowClose(WindowCloseEvent& e);
    bool isRunning;

private:
    // Window related members
    std::unique_ptr<Window> window;
    

};