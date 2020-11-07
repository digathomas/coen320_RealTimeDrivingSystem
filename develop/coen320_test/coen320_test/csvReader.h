#pragma once
#ifndef CSVREADER_H
#define CSVREADER_H
#include <string>
#include <vector>
#include "dataLineModel.h"
 
class csvReader {
	
private:
	std::vector<dataLineModel> dataFile;

public:
	csvReader() = default;
	std::vector<dataLineModel> read_csv(std::string filename);

	
};

#endif

