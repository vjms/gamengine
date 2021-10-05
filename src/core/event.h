#pragma once


#include <list>
#include <memory>
#include <concepts>
#include <algorithm>

class Event
{
};

template<typename T>
concept EventType = std::is_base_of_v<T, Event>;

template<EventType T>
class EventDispatcher;

template<EventType T>
class EventListener
{
public:
  ~EventListener<T>()
  {
    unsubscribe();
  }

  virtual void process([[maybe_unused]] T event){};

private:
  friend class EventDispatcher<T>;
  void subscribe(EventDispatcher<T> *dispatcher)
  {
    m_dispatcher = dispatcher;
  }
  void unsubscribe()
  {
    m_dispatcher->unsubscribe(this);
  }

private:
  EventDispatcher<T> *m_dispatcher;
};

template<EventType T>
class EventDispatcher : public EventListener<T>
{
public:
  void subscribe(EventListener<T> *listener)
  {
    listener->subscribe(this);
    m_listeners.emplace_back(listener);
  }
  void unsubscribe(EventListener<T> *listener)
  {
    m_listeners.remove(listener);
  }
  void process(T event) override
  {
    for (auto &listener : m_listeners) {
      listener->process(event);
    }
  };

private:
  std::list<EventListener<T> *> m_listeners;
};
