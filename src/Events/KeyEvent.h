#include "Events/Event.h"

class KeyEvent : public Event
{
public:
    inline int GetKeyCode() const { return m_KeyCode; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "KeyEvent: " << m_KeyCode;
        return ss.str();
    }

    EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCatergoryInput)

protected:
    KeyEvent(int keycode)
        : m_KeyCode(keycode) {}

    int m_KeyCode;
};

class KeyPressEvent : public KeyEvent
{
public:
    KeyPressEvent(int keycode, int repeatCount)
        : KeyEvent(keycode), m_RepeatCount(repeatCount) {}

    inline int GetRepeatCount() const { return m_RepeatCount; }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "KeyPressEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
        return ss.str();
    }

    EVENT_CLASS_TYPE(KeyPress)
private:
    int m_RepeatCount;
};

class KeyReleaseEvent : public KeyEvent
{
public:
    KeyReleaseEvent(int keycode)
        : KeyEvent(keycode) {}

    EVENT_CLASS_TYPE(KeyRelease)
};