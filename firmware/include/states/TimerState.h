#pragma once

#include "State.h"

class TimerState : public State
{
public:
    TimerState();

    void enter() override;
    void update() override;
    void exit() override;

    void setTimer(int duration, unsigned long elapsedTime = 0);

private:
    int duration;
    unsigned long elapsedTime;
    unsigned long startTime;
    bool doublePressReceived;  // Flag to handle double press in update loop
};