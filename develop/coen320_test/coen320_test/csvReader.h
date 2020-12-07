#ifndef CSVREADER_H
#define CSVREADER_H
#include <string>
#include <vector>
#include "dataLineModel.h"
 
class csvReader {
	
private:
	std::vector<dataLineModel> dataFile;

public:
	csvReader(){};
	std::vector<dataLineModel> read_csv();

	
};

#endif

