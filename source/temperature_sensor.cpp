
// System includes
#include <iostream>
#include <thread>
#include <cstdlib>

// Local includes
#include "temperature_sensor.hpp"

problem_02::TemperatureSensor::TemperatureSensor(MarsRover& rover) :
	rover(rover),
	temperature(0.0)
{
	


}

double problem_02::TemperatureSensor::get_temperature() const
{

	// Return temperature
	return temperature;

}

void problem_02::TemperatureSensor::set_temperature(double temperature)
{

	// Set temperature
	this->temperature = temperature;

}

// (thread main function)
void problem_02::TemperatureSensor::collect_data()
{

	// Seed random number generator
	seed_random_number_generator();

	// Wait for the rover to be ready to collect data
	rover.wait_for_rover_to_be_ready_to_collect_data();

	// Collect data (random temperature between -100F and 70F)
	set_temperature(generate_random_temperature(-100, 70));

	// Send data to the rover
	rover.send_temperature_data_to_rover(get_temperature());

	// Wait for the rover to signal that the data has been collected
	rover.wait_for_rover_to_collect_data();

}

void problem_02::TemperatureSensor::seed_random_number_generator()
{

	// Seed random number generator with a hash of the thread id
	std::hash<std::thread::id> hasher;
	std::srand(hasher(std::this_thread::get_id()));

}

double problem_02::TemperatureSensor::generate_random_temperature(double min, double max)
{

	// Generate a random temperature between min and max (inclusive) (Fahrenheit)
	return min + static_cast<double>(rand() % static_cast<int>(max - min));

}

