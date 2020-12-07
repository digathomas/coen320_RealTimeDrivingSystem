//=======================================================================================================
// hey Thomas, how you doinnnn
// start by adding the files Task.h and Task.cpp from my git repo (main branch)
// and then just copy pasta this file instead of your old TaskScheduler.cpp
// hopefully it works, if not, it's because of one of my changes
// all my changes are boxed in a set of equal signs
// and i added comments and readability
// also,
// when you're done could you clean up the code a bitset
// and take screenshots of the output to include in the report
//=======================================================================================================

//include the libraries being used
#include <iostream.h>
#include <math.h>
#include <signal.h>
#include <time.h>
#include <sys/neutrino.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <ctime>
#include "dataLineModel.h"
#include "csvReader.h"
//=======================================================================================================
#include "Task.h"
//=======================================================================================================



#define ONE_THOUSAND	1000
#define ONE_MILLION		1000000
#define OFFSET 1

using namespace std;

//=======================================================================================================
void start();
void scheduler();
//string getRunningTask();
Task* getHighestPriorityTask;
//=======================================================================================================
sigset_t sigst;

void *QueueFuelConsumptionReader(void* interval);
void *QueueEngineSpeedReader(void* interval);
void *QueueEngineCoolantTemperatureReader(void* interval);
void *QueueCurrentGearReader(void* interval);
void *QueueTransmissionOilTemperatureReader(void* interval);
void *QueueVehicleSpeedReader(void* interval);
void *QueueAccelerationSpeedLongitudinalReader(void* interval);
void *QueueIndicationOfBreakSwitchReader(void* interval);
void *QueueTerminalWriter(void* interval);

void fuelConsumptionReader();
void engineSpeedReader();
void engineCoolantTemperatureReader();
void currentGearReader();
void transmissionOilTemperatureReader();
void vehicleSpeedReader();
void accelerationSpeedLongitudinalReader();
void indicationOfBreakSwitchReader();
void terminalWriter();


//global variables

vector<dataLineModel> model;

vector<Task*> taskQueue;
pthread_mutex_t taskQueueMutex = PTHREAD_MUTEX_INITIALIZER;

struct timeval;
struct timespec tv;
static unsigned int starting;
unsigned int current;
clock_t time_req;

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
	model = reader.read_csv();
	std::cout << "Read Success" << std::endl;
	
	//start all timers that trigger events to add a task to the ready queue
   	pthread_create (NULL,NULL, QueueFuelConsumptionReader,(void *)10); //fuel consumption listener
    pthread_create (NULL,NULL, QueueEngineSpeedReader,(void *)500); //engine speed listener
    pthread_create (NULL,NULL, QueueEngineCoolantTemperatureReader,(void *)2000); //engine coolant temperature listener
    pthread_create (NULL,NULL, QueueCurrentGearReader,(void *)100); //current gear listener
    pthread_create (NULL,NULL, QueueTransmissionOilTemperatureReader,(void *)5000); //transmission oil temperature listener
    pthread_create (NULL,NULL, QueueVehicleSpeedReader,(void *)100); //vehicle speed listener
    pthread_create (NULL,NULL, QueueAccelerationSpeedLongitudinalReader,(void *)150); //acceleration speed longitudinal listener
    pthread_create (NULL,NULL, QueueIndicationOfBreakSwitchReader,(void *)100); //indication of break switch listener
    pthread_create (NULL,NULL, QueueTerminalWriter,(void *)63); //terminal writer listener
    
	//start time
    clock_gettime(CLOCK_MONOTONIC, &tv);
    starting = tv.tv_sec * ONE_THOUSAND + tv.tv_nsec / ONE_MILLION; //miliseconds
	//time_req = clock();
	//pthread_create (NULL,NULL, scheduler,NULL); //terminal writer listener
	
	//start scheduler
	scheduler();
	//loops
}

void scheduler() {
    while (true) {
    	//cout << "task size: " +  taskQueue.size() << std::endl;
        if (!taskQueue.empty()) {
			
			//get the highest priority task among the ready queue
            Task* runningTask = getHighestPriorityTask();
			
			//perform the right read
            if (runningTask.compare("fuelConsumptionReader") == 0) {
                fuelConsumptionReader();
            }
            else if (runningTask.compare("engineSpeedReader") == 0) {
                engineSpeedReader();
            }
            else if (runningTask.compare("engineCoolantTemperatureReader") == 0) {
                engineCoolantTemperatureReader();
            }
            else if (runningTask.compare("currentGearReader") == 0) {
                currentGearReader();
            }
            else if (runningTask.compare("transmissionOilTemperatureReader") == 0) {
                transmissionOilTemperatureReader();
            }
            else if (runningTask.compare("vehicleSpeedReader") == 0) {
                vehicleSpeedReader();
            }
            else if (runningTask.compare("accelerationSpeedLongitudinalReader") == 0){
                accelerationSpeedLongitudinalReader();
            }
            else if (runningTask.compare("indicationOfBreakSwitchReader") == 0) {
                indicationOfBreakSwitchReader();
            }
            else if (runningTask.compare("terminalWriter") == 0) {
                terminalWriter();
            }
        }
    }
}
//=======================================================================================================
Task* getHighestPriorityTask() {
	//mutex lock to perform reads and writes on queue
    pthread_mutex_lock(&taskQueueMutex);

    //earliest-deadline-first 
    //Earliest deadline first (EDF): always schedule the task 
    //with absolute deadline earliest for execution. 
    //Optimal algorithm, given a feasible schedule exists.
    int highestPriorityIndex = 0;
    for (int i = 0; i < taskQueue.size(); i++) {
        if (taskQueue[highestPriorityIndex]->getAbsoluteDeadline() > taskQueue[i]->getAbsoluteDeadline()) {
            highestPriorityIndex = i;
        }
    }

    //get highest priority task and pop it from the queue
    Task* runningTask = taskQueue[highestPriorityIndex];
    taskQueue.erase(taskQueue.begin() + highestPriorityIndex);
	
	//mutex unlock releases resource of taskQueue
	pthread_mutex_unlock(&taskQueueMutex);

    //return highest priority task
    return runningTask;
	
	/*
    pthread_mutex_lock(&taskQueueMutex);
    string runningTask = taskQueue.front();
    taskQueue.erase(taskQueue.begin());
    pthread_mutex_unlock(&taskQueueMutex);
    return runningTask;
	*/
}
//=======================================================================================================

void *QueueFuelConsumptionReader(void* interval){
	while(true){
		//=======================================================================================================
		//get time elapsed since the beginning of the execution
		clock_gettime(CLOCK_MONOTONIC, &tv);
		current = tv.tv_sec * ONE_THOUSAND + tv.tv_nsec / ONE_MILLION;
		double durationTime = (double)(current-starting); //in miliseconds
		
		//Create new task instance with the correct task type and the absolute deadline
		//the absolute deadline is computed by summing the current time with the relative deadline
		//in this case the relative deadline will always be the period of the task
		Task* newTask = new Task("fuelConsumptionReader", durationTime + 10);
		//=======================================================================================================
		
		//mutex lock to perform writes on queue
	    pthread_mutex_lock(&taskQueueMutex);
		
		//=======================================================================================================
		//add task to ready queue
	    taskQueue.push_back(newTask);
		//=======================================================================================================
		
		//mutex unlock releases resource of taskQueue
	    pthread_mutex_unlock(&taskQueueMutex);
		
		//sleep for the interval
	    delay ((int)interval);
	}
}

void *QueueEngineSpeedReader(void* interval) {
	while(true){
		clock_gettime(CLOCK_MONOTONIC, &tv);
		current = tv.tv_sec * ONE_THOUSAND + tv.tv_nsec / ONE_MILLION;
		double durationTime = (double)(current-starting);

		Task* newTask = new Task("engineSpeedReader", durationTime.count() + 500);

	    pthread_mutex_lock(&taskQueueMutex);
	    taskQueue.push_back(newTask);
	    pthread_mutex_unlock(&taskQueueMutex);
		
	    delay ((int)interval);
	}
}

void *QueueEngineCoolantTemperatureReader(void* interval) {
	while (true){
	    clock_gettime(CLOCK_MONOTONIC, &tv);
		current = tv.tv_sec * ONE_THOUSAND + tv.tv_nsec / ONE_MILLION;
		double durationTime = (double)(current-starting);

		Task* newTask = new Task("engineCoolantTemperatureReader", durationTime.count() + 2000);

	    pthread_mutex_lock(&taskQueueMutex);
	    taskQueue.push_back(newTask);
	    pthread_mutex_unlock(&taskQueueMutex);
		
	    delay ((int)interval);
	}
}

void *QueueCurrentGearReader(void* interval) {
	while (true){
	    clock_gettime(CLOCK_MONOTONIC, &tv);
		current = tv.tv_sec * ONE_THOUSAND + tv.tv_nsec / ONE_MILLION;
		double durationTime = (double)(current-starting);

		Task* newTask = new Task("currentGearReader", durationTime.count() + 100);

	    pthread_mutex_lock(&taskQueueMutex);
	    taskQueue.push_back(newTask);
	    pthread_mutex_unlock(&taskQueueMutex);
		
	    delay ((int)interval);
	}
}

void *QueueTransmissionOilTemperatureReader(void* interval) {
	while (true){
	    clock_gettime(CLOCK_MONOTONIC, &tv);
		current = tv.tv_sec * ONE_THOUSAND + tv.tv_nsec / ONE_MILLION;
		double durationTime = (double)(current-starting);

		Task* newTask = new Task("transmissionOilTemperatureReader", durationTime.count() + 5000);

	    pthread_mutex_lock(&taskQueueMutex);
	    taskQueue.push_back(newTask);
	    pthread_mutex_unlock(&taskQueueMutex);
		
	    delay ((int)interval);
	}
}

void *QueueVehicleSpeedReader(void* interval) {
	while (true){
	    clock_gettime(CLOCK_MONOTONIC, &tv);
		current = tv.tv_sec * ONE_THOUSAND + tv.tv_nsec / ONE_MILLION;
		double durationTime = (double)(current-starting);

		Task* newTask = new Task("vehicleSpeedReader", durationTime.count() + 100);

	    pthread_mutex_lock(&taskQueueMutex);
	    taskQueue.push_back(newTask);
	    pthread_mutex_unlock(&taskQueueMutex);
		
	    delay ((int)interval);
	}
}

void *QueueAccelerationSpeedLongitudinalReader(void* interval) {
	while (true){
	    clock_gettime(CLOCK_MONOTONIC, &tv);
		current = tv.tv_sec * ONE_THOUSAND + tv.tv_nsec / ONE_MILLION;
		double durationTime = (double)(current-starting);

		Task* newTask = new Task("accelerationSpeedLongitudinalReader", durationTime.count() + 150);

	    pthread_mutex_lock(&taskQueueMutex);
	    taskQueue.push_back(newTask);
	    pthread_mutex_unlock(&taskQueueMutex);
		
	    delay ((int)interval);
	}
}

void *QueueIndicationOfBreakSwitchReader(void* interval) {
	while (true){
	    clock_gettime(CLOCK_MONOTONIC, &tv);
		current = tv.tv_sec * ONE_THOUSAND + tv.tv_nsec / ONE_MILLION;
		double durationTime = (double)(current-starting);

		Task* newTask = new Task("indicationOfBreakSwitchReader", durationTime.count() + 100);

	    pthread_mutex_lock(&taskQueueMutex);
	    taskQueue.push_back(newTask);
	    pthread_mutex_unlock(&taskQueueMutex);
		
	    delay ((int)interval);
	}
}

void *QueueTerminalWriter(void* interval) {
	while (true){
	    clock_gettime(CLOCK_MONOTONIC, &tv);
		current = tv.tv_sec * ONE_THOUSAND + tv.tv_nsec / ONE_MILLION;
		double durationTime = (double)(current-starting);

		Task* newTask = new Task("terminalWriter", durationTime.count() + 63);

	    pthread_mutex_lock(&taskQueueMutex);
	    taskQueue.push_back(newTask);
	    pthread_mutex_unlock(&taskQueueMutex);
		
	    delay ((int)interval);
	}
}

void fuelConsumptionReader() {
	//get time elapsed since the beginning of the execution
	clock_gettime(CLOCK_MONOTONIC, &tv);
	int current1 = tv.tv_sec * ONE_THOUSAND + tv.tv_nsec / ONE_MILLION; 
	
	//get the csv line where the information can be found (now in the model vector)
	//floor function doesn't work with QNX however the double to int conversion does the same job for positive numbers
	int csvLine = (int)((double)(current1-starting)/ ONE_THOUSAND); //seconds
	
	//get the value at the elapsed time since the beginning of execution until now
	if (csvLine > 0 && csvLine < model.size()) {
        fuelConsumption = model.at(csvLine).fuel_consumption();
    }
}

void engineSpeedReader() {
   clock_gettime(CLOCK_MONOTONIC, &tv);
	int current1 = tv.tv_sec * ONE_THOUSAND + tv.tv_nsec / ONE_MILLION;
	int csvLine = (int)((double)(current1-starting)/ ONE_THOUSAND);
	cout << csvLine << endl;
	if (csvLine > 0 && csvLine < model.size()) {
        engineSpeed = model.at(csvLine).engine_speed();
    }
}

void engineCoolantTemperatureReader() {
    clock_gettime(CLOCK_MONOTONIC, &tv);
	int current1 = tv.tv_sec * ONE_THOUSAND + tv.tv_nsec / ONE_MILLION;
	int csvLine = (int)((double)(current1-starting)/ ONE_THOUSAND);
	
	if (csvLine > 0 && csvLine < model.size()) {
        engineCoolantTemperature  = model.at(csvLine).engine_coolant();
    }
}

void currentGearReader() {
    clock_gettime(CLOCK_MONOTONIC, &tv);
	int current1 = tv.tv_sec * ONE_THOUSAND + tv.tv_nsec / ONE_MILLION;
	int csvLine = (int)((double)(current1-starting)/ ONE_THOUSAND);
	
	if (csvLine > 0 && csvLine < model.size()) {
        currentGear = model.at(csvLine).current_gear();
    }
}

void transmissionOilTemperatureReader() {
    clock_gettime(CLOCK_MONOTONIC, &tv);
	int current1 = tv.tv_sec * ONE_THOUSAND + tv.tv_nsec / ONE_MILLION;
	int csvLine = (int)((double)(current1-starting)/ ONE_THOUSAND);
	
	if (csvLine > 0 && csvLine < model.size()) {
        transmissionOilTemperature = model.at(csvLine).transmission_oil_temperature();
    }
}

void vehicleSpeedReader() {
   clock_gettime(CLOCK_MONOTONIC, &tv);
	int current1 = tv.tv_sec * ONE_THOUSAND + tv.tv_nsec / ONE_MILLION;
	int csvLine = (int)((double)(current1-starting)/ ONE_THOUSAND);
	
	if (csvLine > 0 && csvLine < model.size()) {
        vehicleSpeed = model.at(csvLine).vehicle_speed();
    }
}

void accelerationSpeedLongitudinalReader() {
    clock_gettime(CLOCK_MONOTONIC, &tv);
	int current1 = tv.tv_sec * ONE_THOUSAND + tv.tv_nsec / ONE_MILLION;
	int csvLine = (int)((double)(current1-starting)/ ONE_THOUSAND);
	
	if (csvLine > 0 && csvLine < model.size()) {
        accelerationSpeedLongitudinal = model.at(csvLine).acceleration_speed_longitudinal();
    }
}

void indicationOfBreakSwitchReader() {
    clock_gettime(CLOCK_MONOTONIC, &tv);
	int current1 = tv.tv_sec * ONE_THOUSAND + tv.tv_nsec / ONE_MILLION;
	int csvLine = (int)((double)(current1-starting)/ ONE_THOUSAND);
	
	if (csvLine > 0 && csvLine < model.size()) {
        indicationOfBreakSwitch = model.at(csvLine).indication_of_break_switch();
    }
}

void terminalWriter() {
   //clock_t newTime = clock()-time_req;
   //float durationTime = (float)newTime/CLOCKS_PER_SEC;

	//get time elapsed since the beginning of the execution
	clock_gettime(CLOCK_MONOTONIC, &tv);
	current = tv.tv_sec * ONE_THOUSAND + tv.tv_nsec / ONE_MILLION;
	double durationTime = (double)(current-starting)/ ONE_THOUSAND - OFFSET; //in seconds
	
	//print information to the terminal
	//system("clear") 
	//I/O operation takes too much processor resources
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
