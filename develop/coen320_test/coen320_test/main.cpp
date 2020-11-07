#include <iostream>
#include "csvReader.h"
using namespace std;

int main() {

	cout << "hello world\n";
	csvReader reader = csvReader();
	vector<dataLineModel> model = reader.read_csv("./DrivingData.csv");

	return 0;
}