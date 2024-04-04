#pragma once

// System includes
#include <condition_variable>
#include <mutex>

// Local includes
#include "mars_rover.hpp"

namespace problem_02
{

	// Forward declaration of MarsRover
	class MarsRover;

	class TemperatureSensor
	{

	private:
		
		// Working rover
		MarsRover& rover;

		// Temperature
		double temperature;

		// Settters
		void set_temperature(double temperature);

		// Utility functions
		void seed_random_number_generator();
		double generate_random_temperature(double min, double max);

	public:

		// Constructor
		TemperatureSensor(MarsRover& rover);

		// Gettters
		double get_temperature() const;

		// Thread's main function
		void collect_data();

	};

}