#include "Task.h"

Task::Task()
{
	Task::taskType = "no type";
	Task::absoluteDeadline = -1;
}

Task::Task(std::string newTaskType, double newAbsoluteDeadline)
{
	taskType = newTaskType;
	absoluteDeadline = newAbsoluteDeadline;
}

void Task::setTaskType(std::string newTaskType)
{
	taskType = newTaskType;
}

void Task::setAbsoluteDeadline(double newAbsoluteDeadline)
{
	absoluteDeadline = newAbsoluteDeadline;
}

std::string Task::getTaskType()
{
	return taskType;
}

double Task::getAbsoluteDeadline()
{
	return absoluteDeadline;
}
