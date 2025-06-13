#include "engine/EventSystem.hpp"

void EventSystem::addEvent(std::function<void()> func) {
    funcs.push_back(func);
}

void EventSystem::callEvents() {
    for (auto& fnc : funcs) {
        fnc();
    }
    funcs.clear();
}