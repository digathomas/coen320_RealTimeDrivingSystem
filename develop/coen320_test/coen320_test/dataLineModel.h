#pragma once
#ifndef DATALINEMODEL_h
#define DATALINEMODEL_h

class dataLineModel
{
private:
	float currentTime = 0;
	float fuelConsumption = -1;
	float engineSpeed = -1;
	float engineCoolant = -1;
	float currentGear = -1;
	float transmissionOilTemperature = -1;
	float vehicleSpeed = -1;
	float accelerationSpeedLongitudinal = -1;
	float indicationOfBreakSwitch = -1;


public:

	dataLineModel() = default;


	float engine_coolant() const
	{
		return engineCoolant;
	}

	void set_engine_coolant(float engine_coolant)
	{
		engineCoolant = engine_coolant;
	}

	float current_time() const
	{
		return currentTime;
	}

	void set_current_time(float current_time)
	{
		currentTime = current_time;
	}

	float fuel_consumption() const
	{
		return fuelConsumption;
	}

	void set_fuel_consumption(float fuel_consumption)
	{
		fuelConsumption = fuel_consumption;
	}

	float engine_speed() const
	{
		return engineSpeed;
	}

	void set_engine_speed(float engine_speed)
	{
		engineSpeed = engine_speed;
	}

	float current_gear() const
	{
		return currentGear;
	}

	void set_current_gear(float current_gear)
	{
		currentGear = current_gear;
	}

	float transmission_oil_temperature() const
	{
		return transmissionOilTemperature;
	}

	void set_transmission_oil_temperature(float transmission_oil_temperature)
	{
		transmissionOilTemperature = transmission_oil_temperature;
	}

	float vehicle_speed() const
	{
		return vehicleSpeed;
	}

	void set_vehicle_speed(float vehicle_speed)
	{
		vehicleSpeed = vehicle_speed;
	}

	float acceleration_speed_longitudinal() const
	{
		return accelerationSpeedLongitudinal;
	}

	void set_acceleration_speed_longitudinal(float acceleration_speed_longitudinal)
	{
		accelerationSpeedLongitudinal = acceleration_speed_longitudinal;
	}

	float indication_of_break_switch() const
	{
		return indicationOfBreakSwitch;
	}

	void set_indication_of_break_switch(float indication_of_break_switch)
	{
		indicationOfBreakSwitch = indication_of_break_switch;
	}
	
};

#endif 