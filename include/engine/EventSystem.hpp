#pragma once

#include <vector>
#include <functional>

class EventSystem
{
public:
    void addEvent(std::function<void()> func);
    void callEvents();
private:
    std::vector<std::function<void()>> funcs;
};