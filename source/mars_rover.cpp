
// System includes
#include <utility>
#include <iostream>

// Local includes
#include "mars_rover.hpp"

problem_02::MarsRover::MarsRover() : 
	time(1), 
	samples_collected_during_sample_interval(0),
	data(),
	data_mutex(),
	cv_rover_ready_to_collect_data(),
	cv_rover_ready_to_collect_data_mutex(),
	ready_to_collect_data(true),
	cv_rover_data_collected(),
	cv_rover_data_collected_mutex(),
	data_collected(false)

{

	// Create temperature sensors
	for (size_t i = 0; i < N_TEMPERATURE_SENSORS; i++)
	{

		// Create temperature sensor
		TemperatureSensor* sensor = new TemperatureSensor(*this);
		std::thread thread(&TemperatureSensor::collect_data, sensor);

		// Store temperature sensor
		temperature_sensors[i] = { sensor, std::move(thread) };

	}

}

problem_02::MarsRover::~MarsRover()
{

	// Join temperature sensors
	for (size_t i = 0; i < N_TEMPERATURE_SENSORS; i++)
	{

		if (temperature_sensors[i].thread.joinable())
			temperature_sensors[i].thread.join();

	}

}

size_t problem_02::MarsRover::get_time() const
{

	// Return time
	return time;

}

void problem_02::MarsRover::set_time(size_t time)
{

	// Set time
	this->time = time;

}

void problem_02::MarsRover::increment_time()
{

	// Increment time
	time++;

}

std::condition_variable& problem_02::MarsRover::get_rover_condition_variable()
{

	// Return condition variable
	return cv_rover_ready_to_collect_data;

}

std::mutex& problem_02::MarsRover::get_rover_condition_variable_mutex()
{

	// Return mutex
	return cv_rover_ready_to_collect_data_mutex;

}

void problem_02::MarsRover::wait_for_rover_to_be_ready_to_collect_data()
{

	// Create a unique lock for the condition variable
	std::unique_lock<std::mutex> lock(cv_rover_ready_to_collect_data_mutex);

	// Check for spurious wakeups
	while (!ready_to_collect_data)
	{

		// Wait for the rover to be ready to collect data
		cv_rover_ready_to_collect_data.wait(lock, [this] { return ready_to_collect_data; });

	}

}

void problem_02::MarsRover::wait_for_rover_to_collect_data()
{

	// Create a unique lock for the condition variable
	std::unique_lock<std::mutex> lock(cv_rover_data_collected_mutex);

	// Check for spurious wakeups
	while (!data_collected)
	{

		// Wait for the rover to be ready to collect data
		cv_rover_data_collected.wait(lock, [this] { return data_collected; });

	}

}

void problem_02::MarsRover::send_temperature_data_to_rover(double temperature)
{

	// Lock data mutex
	std::lock_guard<std::mutex> lock(data_mutex);

	// Increment samples collected during sample interval
	samples_collected_during_sample_interval++;

	// Store temperature
	data.push_back(temperature);

	// Check if we have enough samples
	if (samples_collected_during_sample_interval == N_TEMPERATURE_SENSORS)
	{

		// Set data collected
		data_collected = true;

		// Notify temperature sensors
		cv_rover_data_collected.notify_all();

	}

}

void problem_02::MarsRover::reset_report_interval_data()
{

	// Lock data mutex
	std::lock_guard<std::mutex> lock(data_mutex);

	// Clear data
	data.clear();

	// Reset samples collected during sample interval
	samples_collected_during_sample_interval = 0;

	// Set ready to collect data
	ready_to_collect_data = true;

	// Notify temperature sensors
	cv_rover_ready_to_collect_data.notify_all();

}

void problem_02::MarsRover::generate_report()
{

	// Lock data mutex
	std::lock_guard<std::mutex> lock(data_mutex);

	// Calculate data offset by hour
	size_t current_data_offset = get_time() % (REPORT_INTERVAL_HOUR * 60);

	// Print report
	std::cout << "Report at time " << time << ":\n";
	for (size_t i = current_data_offset; i < N_TEMPERATURE_SENSORS; i++)
	{

		std::cout << "Sensor " << i << ": " << data[i] << "\n";

	}

}

