#pragma once

#include "TerminalWriter.h"

#ifndef SCHEDULER_H
#define SCHEDULER_H

void start();
void scheduler();
void taskListener(std::function<void(void)> func, unsigned int interval);
void queueFuelConsumptionReader();
void queueTerminalWriter();
void fuelConsumptionReader();
void terminalWriter();

#endif