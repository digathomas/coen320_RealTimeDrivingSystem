#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <functional>
#include <list>
#include <vector>
#include <iterator>
#include <math.h>

#include "Task.h"
#include "csvReader.h"

using namespace std;


//function declarations

void start();
void scheduler();

Task* getHighestPriorityTask();

void queueFuelConsumptionReader();
void queueEngineSpeedReader();
void queueEngineCoolantTemperatureReader();
void queueCurrentGearReader();
void queueTransmissionOilTemperatureReader();
void queueVehicleSpeedReader();
void queueAccelerationSpeedLongitudinalReader();
void queueIndicationOfBreakSwitchReader();
void queueTerminalWriter();

void fuelConsumptionReader();
void engineSpeedReader();
void engineCoolantTemperatureReader();
void currentGearReader();
void transmissionOilTemperatureReader();
void vehicleSpeedReader();
void accelerationSpeedLongitudinalReader();
void indicationOfBreakSwitchReader();
void terminalWriter();

void taskListener(std::function<void(void)> func, unsigned int interval);


//global variables

chrono::high_resolution_clock::time_point startTime;
std::vector<Task*> taskQueue;
std::mutex taskQueueMutex;
std::vector<dataLineModel> model;

double fuelConsumption = 0;
double engineSpeed = 0;
double engineCoolantTemperature = 0;
double currentGear = 0;
double transmissionOilTemperature = 0;
double vehicleSpeed = 0;
double accelerationSpeedLongitudinal = 0;
double indicationOfBreakSwitch = 0;


void start() {
    //read csv file and store data in vector
    csvReader reader = csvReader();
    model = reader.read_csv("./DrivingData.csv");
    system("pause");

    //start all timers that trigger events to add a task to the ready queue
    taskListener(queueFuelConsumptionReader, 10); //fuel consumption listener
    taskListener(queueEngineSpeedReader, 500); //engine speed listener
    taskListener(queueEngineCoolantTemperatureReader, 2000); //engine coolant temperature listener
    taskListener(queueCurrentGearReader, 100); //current gear listener
    taskListener(queueTransmissionOilTemperatureReader, 5000); //transmission oil temperature listener
    taskListener(queueVehicleSpeedReader, 100); //vehicle speed listener
    taskListener(queueAccelerationSpeedLongitudinalReader, 150); //acceleration speed longitudinal listener
    taskListener(queueIndicationOfBreakSwitchReader, 100); //indication of break switch listener
    taskListener(queueTerminalWriter, 63); //terminal writer listener

    //start time
    startTime = chrono::high_resolution_clock::now();

    //start scheduler
    scheduler(); 
    //loops
}

void scheduler() {
    while (true) {
        if (taskQueue.size() > 0) {

            //get the highest priority task among the ready queue
            Task* runningTask = getHighestPriorityTask();

            //perform the right read
            if (runningTask->getTaskType() == "fuelConsumptionReader") {
                fuelConsumptionReader();
            }
            else if (runningTask->getTaskType() == "engineSpeedReader") {
                engineSpeedReader();
            }
            else if (runningTask->getTaskType() == "engineCoolantTemperatureReader") {
                engineCoolantTemperatureReader();
            }
            else if (runningTask->getTaskType() == "currentGearReader") {
                currentGearReader();
            }
            else if (runningTask->getTaskType() == "transmissionOilTemperatureReader") {
                transmissionOilTemperatureReader();
            }
            else if (runningTask->getTaskType() == "vehicleSpeedReader") {
                vehicleSpeedReader();
            }
            else if (runningTask->getTaskType() == "accelerationSpeedLongitudinalReader") {
                accelerationSpeedLongitudinalReader();
            }
            else if (runningTask->getTaskType() == "indicationOfBreakSwitchReader") {
                indicationOfBreakSwitchReader();
            }
            else if (runningTask->getTaskType() == "terminalWriter") {
                terminalWriter();
            }
        }
    }
}

Task* getHighestPriorityTask() {
    //mutex lock to perform reads and writes on queue
    const std::lock_guard<std::mutex> lock(taskQueueMutex);

    //earliest-deadline-first 
    int highestPriorityIndex = 0;
    for (int i = 0; i < taskQueue.size(); i++) {
        if (taskQueue[highestPriorityIndex]->getAbsoluteDeadline() > taskQueue[i]->getAbsoluteDeadline()) {
            highestPriorityIndex = i;
        }
    }

    //get highest priority task and pop it from the queue
    Task* runningTask = taskQueue[highestPriorityIndex];
    taskQueue.erase(taskQueue.begin() + highestPriorityIndex);

    //return highest priority task
    return runningTask;
}

void queueFuelConsumptionReader() {
    //get time elapsed since the beginning of the execution
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationTime = currentTime - startTime;

    //Create new task instance with the correct task type and the absolute deadline
    //the absolute deadline is computed by summing the current time with the relative deadline
    //in this case the relative deadline will always be the period of the task
    Task* newTask = new Task("fuelConsumptionReader", durationTime.count() + 10);

    //mutex lock to perform writes on queue
    const std::lock_guard<std::mutex> lock(taskQueueMutex);
    taskQueue.push_back(newTask);
}

void queueEngineSpeedReader() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationTime = currentTime - startTime;
    Task* newTask = new Task("engineSpeedReader", durationTime.count() + 500);

    const std::lock_guard<std::mutex> lock(taskQueueMutex);
    taskQueue.push_back(newTask);
}

void queueEngineCoolantTemperatureReader() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationTime = currentTime - startTime;
    Task* newTask = new Task("engineCoolantTemperatureReader", durationTime.count() + 2000);

    const std::lock_guard<std::mutex> lock(taskQueueMutex);
    taskQueue.push_back(newTask);
}

void queueCurrentGearReader() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationTime = currentTime - startTime;
    Task* newTask = new Task("currentGearReader", durationTime.count() + 100);

    const std::lock_guard<std::mutex> lock(taskQueueMutex);
    taskQueue.push_back(newTask);
}

void queueTransmissionOilTemperatureReader() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationTime = currentTime - startTime;
    Task* newTask = new Task("transmissionOilTemperatureReader", durationTime.count() + 5000);

    const std::lock_guard<std::mutex> lock(taskQueueMutex);
    taskQueue.push_back(newTask);
}

void queueVehicleSpeedReader() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationTime = currentTime - startTime;
    Task* newTask = new Task("vehicleSpeedReader", durationTime.count() + 100);

    const std::lock_guard<std::mutex> lock(taskQueueMutex);
    taskQueue.push_back(newTask);
}

void queueAccelerationSpeedLongitudinalReader() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationTime = currentTime - startTime;
    Task* newTask = new Task("accelerationSpeedLongitudinalReader", durationTime.count() + 150);

    const std::lock_guard<std::mutex> lock(taskQueueMutex);
    taskQueue.push_back(newTask);
}

void queueIndicationOfBreakSwitchReader() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationTime = currentTime - startTime;
    Task* newTask = new Task("indicationOfBreakSwitchReader", durationTime.count() + 100);

    const std::lock_guard<std::mutex> lock(taskQueueMutex);
    taskQueue.push_back(newTask);
}

void queueTerminalWriter() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationTime = currentTime - startTime;
    Task* newTask = new Task("terminalWriter", durationTime.count() + 1000 / 16);

    const std::lock_guard<std::mutex> lock(taskQueueMutex);
    taskQueue.push_back(newTask);
}

void fuelConsumptionReader() {
    //get time elapsed since the beginning of the execution
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationTime = currentTime - startTime;

    //get the csv line where the information can be found (now in the model vector)
    int csvLine = floor(durationTime.count() / 1000);

    //get the value at the elapsed time since the beginning of execution until now
    if (csvLine > 0 && csvLine < model.size()) {
        fuelConsumption = model.at(csvLine).fuel_consumption();
    }
}

void engineSpeedReader() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationTime = currentTime - startTime;
    int csvLine = floor(durationTime.count() / 1000);
    if (csvLine > 0 && csvLine < model.size()) {
        engineSpeed = model.at(csvLine).engine_speed();
    }
}

void engineCoolantTemperatureReader() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationTime = currentTime - startTime;
    int csvLine = floor(durationTime.count() / 1000);
    if (csvLine > 0 && csvLine < model.size()) {
        engineCoolantTemperature = model.at(csvLine).engine_coolant();
    }
}

void currentGearReader() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationTime = currentTime - startTime;
    int csvLine = floor(durationTime.count() / 1000);
    if (csvLine > 0 && csvLine < model.size()) {
        currentGear = model.at(csvLine).current_gear();
    }
}

void transmissionOilTemperatureReader() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationTime = currentTime - startTime;
    int csvLine = floor(durationTime.count() / 1000);
    if (csvLine > 0 && csvLine < model.size()) {
        transmissionOilTemperature = model.at(csvLine).transmission_oil_temperature();
    }
}

void vehicleSpeedReader() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationTime = currentTime - startTime;
    int csvLine = floor(durationTime.count() / 1000);
    if (csvLine > 0 && csvLine < model.size()) {
        vehicleSpeed = model.at(csvLine).vehicle_speed();
    }
}

void accelerationSpeedLongitudinalReader() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationTime = currentTime - startTime;
    int csvLine = floor(durationTime.count() / 1000);
    if (csvLine > 0 && csvLine < model.size()) {
        accelerationSpeedLongitudinal = model.at(csvLine).acceleration_speed_longitudinal();
    }
}

void indicationOfBreakSwitchReader() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationTime = currentTime - startTime;
    int csvLine = floor(durationTime.count() / 1000);
    if (csvLine > 0 && csvLine < model.size()) {
        indicationOfBreakSwitch = model.at(csvLine).indication_of_break_switch();
    }
}

void terminalWriter() {
    //get time elapsed since the beginning of the execution
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationTime = currentTime - startTime;

    //print information to the terminal
    system("CLS");
    std::cout << "===================================\n";
    std::cout << "Tasks in queue: " << taskQueue.size() << endl;
    std::cout << "TIME: " << (durationTime.count())/1000 << "s" << std::endl;
    std::cout << "Fuel Consumption: " << fuelConsumption << std::endl;
    std::cout << "Engine Speed: " << engineSpeed << std::endl;
    std::cout << "Engine Coolant Temperature: " << engineCoolantTemperature << std::endl;
    std::cout << "Current Gear: " << currentGear << std::endl;
    std::cout << "Transmission Oil Temperature: " << transmissionOilTemperature << std::endl;
    std::cout << "Vehicle Speed: " << vehicleSpeed << std::endl;
    std::cout << "Acceleration Speed Longitudinal: " << accelerationSpeedLongitudinal << std::endl;
    std::cout << "Indication Of Break Switch: " << indicationOfBreakSwitch << std::endl;
    std::cout << "===================================\n";
}

void taskListener(std::function<void(void)> function, unsigned int interval)
{
    std::thread([function, interval]() {
        //infinite loop
        while (true)
        {
            //run function passed as argument
            function();

            //sleep for the interval passed as argument
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        }
        }).detach();
}