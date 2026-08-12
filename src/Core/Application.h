#include "Window.h"
#include "Events/WindowEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

void AppInit();

void AppUpdate(float deltaTime);

void AppClose();

bool ClientKeyCallBack(KeyPressEvent& e);
bool ClientMouseMoveCallBack(MouseMoveEvent &e);
bool ClientMouseButtonCallBack(MouseButtonEvent &e);

class Application {
public:
    Application();
    ~Application();

    void Run();
    void Close();
    void Initialize();

    static Application* GetInstance();

    Window& GetWindow();
private:
    void OnEvent(Event& e);
    bool ProcessKeyPress(KeyPressEvent& e);
    bool OnWindowClose(WindowCloseEvent& e);
    bool isRunning;

private:
    // Window related members
    std::unique_ptr<Window> window;
    

};