#pragma once

#include <iostream>
#include <chrono>
#include <thread>
#include <functional>
#include <list>
#include <iterator>
#include <string>

#ifndef TASK_H
#define TASK_H

class Task
{
private:
	std::string taskType;
	double absoluteDeadline;

public:
	Task();
	Task(std::string newTaskType, double newAbsoluteDeadline);

	void setTaskType(std::string newTaskType);
	void setAbsoluteDeadline(double newAbsoluteDeadline);

	std::string getTaskType();
	double getAbsoluteDeadline();
};

#endif
