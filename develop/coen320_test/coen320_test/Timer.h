#pragma once

#ifndef TIMER_H
#define TIMER_H

class Timer
{
private:
    bool clear = false;

public:
    void setTimeout(auto function, int delay);
    void setInterval(auto function, int interval);
    void stop()
};

#endif 

