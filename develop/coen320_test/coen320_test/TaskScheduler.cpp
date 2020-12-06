#include <iostream.h>
#include <time.h>
#include <sys/neutrino.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <ctime>


using namespace std;

void start();
string getRunningTask();

void *QueueFCR(void* interval);
void *QueueEngineSpeedReader(void* interval);
void *QueueEngineCoolantTemperatureReader(void* interval);
void *QueueCurrentGearReader(void* interval);
void *QueueTransmissionOilTemperatureReader(void* interval);
void *QueueVehicleSpeedReader(void* interval);
void *QueueAccelerationSpeedLongitudinalReader(void* interval);
void *QueueIndicationOfBreakSwitchReader(void* interval);
void *QueueTerminalWriter(void* interval);
void *scheduler(void *);

void fuelConsumptionReader();
void engineSpeedReader();
void engineCoolantTemperatureReader();
void currentGearReader();
void transmissionOilTemperatureReader();
void vehicleSpeedReader();
void accelerationSpeedLongitudinalReader();
void indicationOfBreakSwitchReader();
void terminalWriter();

vector<string> taskQueue;
pthread_mutex_t taskQueueMutex = PTHREAD_MUTEX_INITIALIZER;

struct timeval;

clock_t time_req;

double fuelConsumption = -1;
double engineSpeed = -1;
double engineCoolantTemperature = -1;
double currentGear = -1;
double transmissionOilTemperature = -1;
double vehicleSpeed = -1;
double accelerationSpeedLongitudinal = -1;
double indicationOfBreakSwitch = -1;


void start() {
   	
   	pthread_create (NULL,NULL, QueueEngineSpeedReader,(void *)10); //fuel consumption listener
    pthread_create (NULL,NULL, QueueEngineSpeedReader,(void *)500); //engine speed listener
    pthread_create (NULL,NULL, QueueEngineCoolantTemperatureReader,(void *)2000); //engine coolant temperature listener
    pthread_create (NULL,NULL, QueueCurrentGearReader,(void *)100); //current gear listener
    pthread_create (NULL,NULL, QueueTransmissionOilTemperatureReader,(void *)5000); //transmission oil temperature listener
    pthread_create (NULL,NULL, QueueVehicleSpeedReader,(void *)100); //vehicle speed listener
    pthread_create (NULL,NULL, QueueAccelerationSpeedLongitudinalReader,(void *)150); //acceleration speed longitudinal listener
    pthread_create (NULL,NULL, QueueIndicationOfBreakSwitchReader,(void *)100); //indication of break switch listener
    pthread_create (NULL,NULL, QueueTerminalWriter,(void *)63); //terminal writer listener
    
	time_req = clock();
	pthread_create (NULL,NULL, scheduler,NULL); //terminal writer listener
}

void *scheduler(void *) {
    while (true) {
    	//cout << "task size: " +  taskQueue.size() << std::endl;
        if (!taskQueue.empty()) {
            string runningTask = getRunningTask();
            if (runningTask.compare("fuelConsumptionReader")) {
                fuelConsumptionReader();
            }
            else if (runningTask.compare("engineSpeedReader")) {
                engineSpeedReader();
            }
            else if (runningTask.compare("engineCoolantTemperatureReader")) {
                engineCoolantTemperatureReader();
            }
            else if (runningTask.compare("currentGearReader")) {
                currentGearReader();
            }
            else if (runningTask.compare("transmissionOilTemperatureReader")) {
                transmissionOilTemperatureReader();
            }
            else if (runningTask.compare("vehicleSpeedReader")) {
                vehicleSpeedReader();
            }
            else if (runningTask.compare("accelerationSpeedLongitudinalReader")){
                accelerationSpeedLongitudinalReader();
            }
            else if (runningTask.compare("indicationOfBreakSwitchReader")) {
                indicationOfBreakSwitchReader();
            }
            else if (runningTask.compare("terminalWriter")) {
                terminalWriter();
            }
        }
    }
}

string getRunningTask() {
    pthread_mutex_lock(&taskQueueMutex);
    string runningTask = taskQueue.front();
    taskQueue.erase(taskQueue.begin());
    pthread_mutex_unlock(&taskQueueMutex);
    return runningTask;
}

void *QueueEngineSpeedReader(void* interval) {
	while(true){
	    pthread_mutex_lock(&taskQueueMutex);
	    taskQueue.push_back("engineSpeedReader");
	    //cout << "engineSpeedReader" << endl;
	    pthread_mutex_unlock(&taskQueueMutex);
	    delay ((int)interval);
	}
}

void *QueueEngineCoolantTemperatureReader(void* interval) {
	while (true){
	    pthread_mutex_lock(&taskQueueMutex);
	    taskQueue.push_back("engineCoolantTemperatureReader");
	    pthread_mutex_unlock(&taskQueueMutex);
	    delay ((int)interval);
	}
}

void *QueueCurrentGearReader(void* interval) {
	while (true){
	    pthread_mutex_lock(&taskQueueMutex);
	    taskQueue.push_back("currentGearReader");
	    pthread_mutex_unlock(&taskQueueMutex);
		delay ((int)interval);
	}
}

void *QueueTransmissionOilTemperatureReader(void* interval) {
	while (true){
	    pthread_mutex_lock(&taskQueueMutex);
	    taskQueue.push_back("transmissionOilTemperatureReader");
	    pthread_mutex_unlock(&taskQueueMutex);
	    delay ((int)interval);
	}
}

void *QueueVehicleSpeedReader(void* interval) {
	while (true){
	    pthread_mutex_lock(&taskQueueMutex);
	    taskQueue.push_back("vehicleSpeedReader");
	    pthread_mutex_unlock(&taskQueueMutex);
	    delay ((int)interval);
	}
}

void *QueueAccelerationSpeedLongitudinalReader(void* interval) {
	while (true){
	    pthread_mutex_lock(&taskQueueMutex);
	    taskQueue.push_back("accelerationSpeedLongitudinalReader");
	    pthread_mutex_unlock(&taskQueueMutex);
	    delay ((int)interval);
	}
}

void *QueueIndicationOfBreakSwitchReader(void* interval) {
	while (true){
	    pthread_mutex_lock(&taskQueueMutex);
	    taskQueue.push_back("indicationOfBreakSwitchReader");
	    pthread_mutex_unlock(&taskQueueMutex);
	    delay ((int)interval);
	}
}

void *QueueTerminalWriter(void* interval) {
	while (true){
	    pthread_mutex_lock(&taskQueueMutex);
	    taskQueue.push_back("terminalWriter");
	    pthread_mutex_unlock(&taskQueueMutex);
	    delay ((int)interval);
	}
}

void fuelConsumptionReader() {
    fuelConsumption++;
}

void engineSpeedReader() {
    engineSpeed++;
}

void engineCoolantTemperatureReader() {
    engineCoolantTemperature++;
}

void currentGearReader() {
    currentGear++;
}

void transmissionOilTemperatureReader() {
    transmissionOilTemperature++;
}

void vehicleSpeedReader() {
    vehicleSpeed++;
}

void accelerationSpeedLongitudinalReader() {
    accelerationSpeedLongitudinal++;
}

void indicationOfBreakSwitchReader() {
    indicationOfBreakSwitch++;
}

void terminalWriter() {
   clock_t newTime = clock()-time_req;
   float durationTime = (float)time_req/CLOCKS_PER_SEC;

    system("CLS");
    std::cout << "===================================\n";
    std::cout << "Tasks in Queue: " << taskQueue.size() << endl;
    std::cout << "TIME: " << durationTime << "s" << std::endl;
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


