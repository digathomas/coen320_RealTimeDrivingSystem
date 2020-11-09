#include "TerminalWriter.h"

void TerminalWriter::execute(double time, double fuelConsumption, double engineSpeed,
    double engineCoolantTemperature, double currentGear, double transmissionOilTemperature, 
    double vehicleSpeed, double accelerationSpeedLongitudinal, double indicationOfBreakSwitch)
{
    system("CLS");
    std::cout << "===================================\n";
    std::cout << "TIME: " << time << std::endl;
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
