#pragma once

#include "Task.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <functional>
#include <list>
#include <iterator>

#ifndef TERMINALWRITER_H
#define TERMINALWRITER_H


class TerminalWriter : public Task
{
public:
    void execute(double time, double fuelConsumption, double engineSpeed,
        double engineCoolantTemperature, double currentGear, double transmissionOilTemperature,
        double vehicleSpeed, double accelerationSpeedLongitudinal, double indicationOfBreakSwitch);
};
#endif

