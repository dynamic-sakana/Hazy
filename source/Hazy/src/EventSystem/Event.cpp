#include "Hazy/EventSystem/Event.h"

namespace Hazy {
    std::once_flag EventQueue::s_initialzed;
    UniqueRef<EventQueue> EventQueue::s_eventQueue = nullptr;
    std::mutex EventQueue::s_queueMutex;
    std::queue<Ref<Event>> EventQueue::s_toDoList;
}