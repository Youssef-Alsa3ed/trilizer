#pragma once
#include "enginepch.h"
#include <functional>
#define BIT(x) (1 << x)

enum class EventType
{
    None = 0,
    KeyPress,
    KeyRelease,
    MouseMove,
    MouseButton,
    WindowResize,
    WindowClose
};

enum EventCategory
{
    None = 0,
    EventCatergoryInput = BIT(1),
    EventCategoryKeyboard = BIT(2),
    EventCategoryMouse = BIT(3),
    EventCategoryWindow = BIT(4)
};

#define EVENT_CLASS_TYPE(type)                                                  \
    static EventType GetStaticType() { return EventType::type; }                \
    virtual EventType GetEventType() const override { return GetStaticType(); } \
    virtual const char *GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
    virtual int GetCategoryFlags() const override { return category; }

class Event
{
    friend class EventDispatcher;

public:
    virtual EventType GetEventType() const = 0;
    virtual const char *GetName() const = 0;
    virtual int GetCategoryFlags() const = 0;
    virtual std::string ToString() const { return GetName(); }

    inline bool IsInCategory(EventCategory category)
    {
        return GetCategoryFlags() & category;
    }

protected:
    bool m_Handled = false;
};

class EventDispatcher
{
    template <typename T>
    using EventFn = std::function<bool(T &)>;

public:
    EventDispatcher(Event &event)
        : m_Event(event)
    {
    }

    template <typename T, typename F>
    bool Dispatch(const F &func)
    {
        if (m_Event.GetEventType() == T::GetStaticType())
        {
            m_Event.m_Handled = func(static_cast<T &>(m_Event));
            return true;
        }
        return false;
    }

private:
    Event &m_Event;
};
