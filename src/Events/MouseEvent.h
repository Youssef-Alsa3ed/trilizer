#include "Events/Event.h"

class MouseMoveEvent : public Event
{
private:
    float m_MouseX, m_MouseY;

public:
    MouseMoveEvent(float x, float y)
        : m_MouseX(x), m_MouseY(y) {}

    inline float GetX() const { return m_MouseX; }
    inline float GetY() const { return m_MouseY; }
    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseMoveEvent: " << m_MouseX << ", " << m_MouseY;
        return ss.str();
    }
    EVENT_CLASS_TYPE(MouseMove)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCatergoryInput)
};

class MouseButtonEvent : public Event
{
private:
    int m_Button;

public:
    MouseButtonEvent(int button)
        : m_Button(button) {}
    inline int GetMouseButton() const { return m_Button; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseButtonEvent: " << m_Button;
        return ss.str();
    }

    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCatergoryInput)
    EVENT_CLASS_TYPE(MouseButton)
};
