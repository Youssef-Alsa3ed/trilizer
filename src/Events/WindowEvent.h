#include "Events/Event.h"

class WindowCloseEvent : public Event
{
public:
    WindowCloseEvent() {}

    EVENT_CLASS_TYPE(WindowClose)
    EVENT_CLASS_CATEGORY(EventCategoryWindow)
};

class WindowResizeEvent : public Event
{
public:
    WindowResizeEvent(int width, int height)
        : m_Width(width), m_Height(height) {}

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
        return ss.str();
    }
    EVENT_CLASS_TYPE(WindowResize)
    EVENT_CLASS_CATEGORY(EventCategoryWindow)

private:
    int m_Width, m_Height;
};
