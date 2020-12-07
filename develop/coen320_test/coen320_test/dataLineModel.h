#ifndef DATALINEMODEL_h
#define DATALINEMODEL_h

class dataLineModel
{
private:
	double currentTime;
	double fuelConsumption;
	double engineSpeed;
	double engineCoolant;
	double currentGear;
	double transmissionOilTemperature;
	double vehicleSpeed;
	double accelerationSpeedLongitudinal;
	double indicationOfBreakSwitch;


public:

	dataLineModel(){};


	double engine_coolant() const
	{
		return engineCoolant;
	}

	void set_engine_coolant(double engine_coolant)
	{
		engineCoolant = engine_coolant;
	}

	double current_time() const
	{
		return currentTime;
	}

	void set_current_time(double current_time)
	{
		currentTime = current_time;
	}

	double fuel_consumption() const
	{
		return fuelConsumption;
	}

	void set_fuel_consumption(double fuel_consumption)
	{
		fuelConsumption = fuel_consumption;
	}

	double engine_speed() const
	{
		return engineSpeed;
	}

	void set_engine_speed(double engine_speed)
	{
		engineSpeed = engine_speed;
	}

	double current_gear() const
	{
		return currentGear;
	}

	void set_current_gear(double current_gear)
	{
		currentGear = current_gear;
	}

	double transmission_oil_temperature() const
	{
		return transmissionOilTemperature;
	}

	void set_transmission_oil_temperature(double transmission_oil_temperature)
	{
		transmissionOilTemperature = transmission_oil_temperature;
	}

	double vehicle_speed() const
	{
		return vehicleSpeed;
	}

	void set_vehicle_speed(double vehicle_speed)
	{
		vehicleSpeed = vehicle_speed;
	}

	double acceleration_speed_longitudinal() const
	{
		return accelerationSpeedLongitudinal;
	}

	void set_acceleration_speed_longitudinal(double acceleration_speed_longitudinal)
	{
		accelerationSpeedLongitudinal = acceleration_speed_longitudinal;
	}

	double indication_of_break_switch() const
	{
		return indicationOfBreakSwitch;
	}

	void set_indication_of_break_switch(double indication_of_break_switch)
	{
		indicationOfBreakSwitch = indication_of_break_switch;
	}
	
};

#endif 