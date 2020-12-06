#include <string>
#include <fstream>
#include <vector>
#include <utility> // std::pair
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream

#include "./dataLineModel.h"
#include "./csvReader.h"

#include <iostream>

std::vector<dataLineModel> csvReader::read_csv(std::string filename) {
    // Reads a CSV file into a vector of <string, vector<int>> pairs where
    // each pair represents <column name, column values>

    try
    {
        // Create a vector of <string, int vector> pairs to store the result
        std::vector<dataLineModel> result;

        // Create an input filestream
        std::ifstream myFile(filename);

        // Make sure the file is open
        if (!myFile.is_open()) throw std::runtime_error("Could not open file");

        // Helper vars
        std::string line;

        // Check if file opens
        if (myFile.good())
        {
            //Skip first line (column names)
            std::getline(myFile, line);

            int linenum = 0;

            // Read data, line by line
            while (std::getline(myFile, line))
            {
                dataLineModel model = dataLineModel();

                // Create a stringstream of the current line
                std::stringstream ss(line);

                // Keep track of the current column index
                int x_axis = 0;
                std::string token;

                // Extract each integer
                while (std::getline(ss, token, ',')) {

                    switch (x_axis)
                    {
                        //fuelConsumption = -1;
                    case 0:
                        model.set_fuel_consumption(std::stof(token));
                        break;

                        //engineSpeed = -1;
                    case 12:
                        model.set_engine_speed(std::stof(token));
                        break;

                        //engineCoolant = -1;
                    case 17:
                        model.set_engine_coolant(std::stof(token));
                        break;

                        //currentGear = -1;
                    case 33:
                        model.set_current_gear(std::stof(token));
                        break;

                        //transmissionOilTemperature = -1;
                    case 34:
                        model.set_transmission_oil_temperature(std::stof(token));
                        break;

                        //vehicleSpeed = -1;
                    case 43:
                        model.set_vehicle_speed(std::stof(token));
                        break;

                        //accelerationSpeedLongitudinal = -1;
                    case 44:
                        model.set_acceleration_speed_longitudinal(std::stof(token));
                        break;

                        //indicationOfBreakSwitch = -1;
                    case 45:
                        model.set_indication_of_break_switch(std::stof(token));
                        break;

                    default:
                        break;
                    }

                    // Increment the column index
                    x_axis++;

                    /*x_axis = 0;//A;
                    x_axis = 12;//M;
                    x_axis = 17;//R;
                    x_axis = 33;//AH;
                    x_axis = 34;//AI; //or maybe R?
                    x_axis = 43;//AR;
                    x_axis = 44;//AS;
                    x_axis = 45;//AT;
                    */

                }
                std::cout << linenum++ << "\n";
                result.push_back(model);
            }
        }


        // Close file
        myFile.close();

        return result;
    }
    catch (std::runtime_error e)
    {
        std::cout << "Shits fucked up";
    }
}
