#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <functional>
#include <list>
#include <iterator>
#include <math.h>

#include "csvReader.h"

using namespace std;

void start();
void scheduler();
string getRunningTask();

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

chrono::high_resolution_clock::time_point startTime;
list<string> taskQueue;
std::mutex taskQueueMutex;

double fuelConsumption = -1;
double engineSpeed = -1;
double engineCoolantTemperature = -1;
double currentGear = -1;
double transmissionOilTemperature = -1;
double vehicleSpeed = -1;
double accelerationSpeedLongitudinal = -1;
double indicationOfBreakSwitch = -1;

std::vector<dataLineModel> model;


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
    taskListener(queueTerminalWriter, 1000 / 16); //terminal writer listener

    //start time
    startTime = chrono::high_resolution_clock::now();

    //start scheduler
    scheduler(); // start scheduler loop
}

void scheduler() {
    while (true) {
        if (taskQueue.size() > 0) {
            string runningTask = getRunningTask();
            if (runningTask == "fuelConsumptionReader") {
                fuelConsumptionReader();
            }
            else if (runningTask == "engineSpeedReader") {
                engineSpeedReader();
            }
            else if (runningTask == "engineCoolantTemperatureReader") {
                engineCoolantTemperatureReader();
            }
            else if (runningTask == "currentGearReader") {
                currentGearReader();
            }
            else if (runningTask == "transmissionOilTemperatureReader") {
                transmissionOilTemperatureReader();
            }
            else if (runningTask == "vehicleSpeedReader") {
                vehicleSpeedReader();
            }
            else if (runningTask == "accelerationSpeedLongitudinalReader") {
                accelerationSpeedLongitudinalReader();
            }
            else if (runningTask == "indicationOfBreakSwitchReader") {
                indicationOfBreakSwitchReader();
            }
            else if (runningTask == "terminalWriter") {
                terminalWriter();
            }
        }
    }
}

string getRunningTask() {
    const std::lock_guard<std::mutex> lock(taskQueueMutex);
    string runningTask = taskQueue.front();
    taskQueue.pop_front();
    return runningTask;
}

void queueFuelConsumptionReader() {
    const std::lock_guard<std::mutex> lock(taskQueueMutex);
    taskQueue.push_back("fuelConsumptionReader");
}

void queueEngineSpeedReader() {
    const std::lock_guard<std::mutex> lock(taskQueueMutex);
    taskQueue.push_back("engineSpeedReader");
}

void queueEngineCoolantTemperatureReader() {
    const std::lock_guard<std::mutex> lock(taskQueueMutex);
    taskQueue.push_back("engineCoolantTemperatureReader");
}

void queueCurrentGearReader() {
    const std::lock_guard<std::mutex> lock(taskQueueMutex);
    taskQueue.push_back("currentGearReader");
}

void queueTransmissionOilTemperatureReader() {
    const std::lock_guard<std::mutex> lock(taskQueueMutex);
    taskQueue.push_back("transmissionOilTemperatureReader");
}

void queueVehicleSpeedReader() {
    const std::lock_guard<std::mutex> lock(taskQueueMutex);
    taskQueue.push_back("vehicleSpeedReader");
}

void queueAccelerationSpeedLongitudinalReader() {
    const std::lock_guard<std::mutex> lock(taskQueueMutex);
    taskQueue.push_back("accelerationSpeedLongitudinalReader");
}

void queueIndicationOfBreakSwitchReader() {
    const std::lock_guard<std::mutex> lock(taskQueueMutex);
    taskQueue.push_back("indicationOfBreakSwitchReader");
}

void queueTerminalWriter() {
    const std::lock_guard<std::mutex> lock(taskQueueMutex);
    taskQueue.push_back("terminalWriter");
}

void fuelConsumptionReader() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationTime = currentTime - startTime;
    int intCurrentTime = floor(durationTime.count() / 1000);
    if (intCurrentTime > 0 && intCurrentTime < model.size()) {
        fuelConsumption = model.at(intCurrentTime).fuel_consumption();
    }
}

void engineSpeedReader() {
    engineSpeed++;
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationTime = currentTime - startTime;
    int intCurrentTime = floor(durationTime.count() / 1000);
    if (intCurrentTime > 0 && intCurrentTime < model.size()) {
        engineSpeed = model.at(intCurrentTime).engine_speed();
    }
}

void engineCoolantTemperatureReader() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationTime = currentTime - startTime;
    int intCurrentTime = floor(durationTime.count() / 1000);
    if (intCurrentTime > 0 && intCurrentTime < model.size()) {
        engineCoolantTemperature = model.at(intCurrentTime).engine_coolant();
    }
}

void currentGearReader() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationTime = currentTime - startTime;
    int intCurrentTime = floor(durationTime.count() / 1000);
    if (intCurrentTime > 0 && intCurrentTime < model.size()) {
        currentGear = model.at(intCurrentTime).current_gear();
    }
}

void transmissionOilTemperatureReader() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationTime = currentTime - startTime;
    int intCurrentTime = floor(durationTime.count() / 1000);
    if (intCurrentTime > 0 && intCurrentTime < model.size()) {
        transmissionOilTemperature = model.at(intCurrentTime).transmission_oil_temperature();
    }
}

void vehicleSpeedReader() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationTime = currentTime - startTime;
    int intCurrentTime = floor(durationTime.count() / 1000);
    if (intCurrentTime > 0 && intCurrentTime < model.size()) {
        vehicleSpeed = model.at(intCurrentTime).vehicle_speed();
    }
}

void accelerationSpeedLongitudinalReader() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationTime = currentTime - startTime;
    int intCurrentTime = floor(durationTime.count() / 1000);
    if (intCurrentTime > 0 && intCurrentTime < model.size()) {
        accelerationSpeedLongitudinal = model.at(intCurrentTime).acceleration_speed_longitudinal();
    }
}

void indicationOfBreakSwitchReader() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationTime = currentTime - startTime;
    int intCurrentTime = floor(durationTime.count() / 1000);
    if (intCurrentTime > 0 && intCurrentTime < model.size()) {
        indicationOfBreakSwitch = model.at(intCurrentTime).indication_of_break_switch();
    }
}

void terminalWriter() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationTime = currentTime - startTime;

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
        while (true)
        {
            function();
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        }
        }).detach();
}