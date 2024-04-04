
// System includes
#include <utility>
#include <iostream>
#include <algorithm>

// Local includes
#include "mars_rover.hpp"

problem_02::MarsRover::MarsRover() :
	rover_is_operational(true),
	time(0), 
	samples_collected_during_sample_interval(0),
	data(60 * N_OPERATIONAL_HOURS),
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

	// Store temperature at the current minute
	data.at(get_time()).push_back(temperature);

	// Check if we have enough samples
	if (samples_collected_during_sample_interval == N_TEMPERATURE_SENSORS)
	{


		// Set data collected
		data_collected = true;

		// Notify temperature sensors
		cv_rover_data_collected.notify_all();

	}

}

void problem_02::MarsRover::reset_sample_interval_data()
{

	// Lock data mutex
	std::lock_guard<std::mutex> lock(data_mutex);

	// Reset samples collected during sample interval
	samples_collected_during_sample_interval = 0;

	// Set ready to collect data
	ready_to_collect_data = true;

	// Notify temperature sensors
	cv_rover_ready_to_collect_data.notify_all();

}

void problem_02::MarsRover::reset_report_interval_data()
{

	// Lock data mutex
	std::lock_guard<std::mutex> lock(data_mutex);

	// Clear data
	data.clear();

}

struct TempRecord 
{
	size_t time;
	double temperature;
};

std::ostream& operator<<(std::ostream& os, const TempRecord& record)
{

	// Print temperature record
	os << "Time (min): " << record.time << " :: " << "Temperature (fahrenheit): " << record.temperature << std::endl;
	return os;

}

void problem_02::MarsRover::generate_report()
{

	// Lock data mutex
	std::lock_guard<std::mutex> lock(data_mutex);

	// Get the current time
	size_t current_time = get_time();

	// Vector to store all temperature records
	std::vector<TempRecord> all_temps;

	for (size_t i = 0; i < REPORT_INTERVAL_MIN; i++)
	{

		// Loop through data vector at current minute
		for (size_t j = 0; j < N_TEMPERATURE_SENSORS; j++)
		{

			// Print temperature data
			std::cout << "Time (min): " << i << " - " << data.at(i).at(j) << " degrees fahrenheit" << std::endl;

			// Create a temperature record
			TempRecord record;
			record.time = i;
			record.temperature = data.at(i).at(j);

			// Add the record to all_temps
			all_temps.push_back(record);

		}

	}

	{

		// Temperature statistics
		std::cout << "\nTemperature Report:\n";

		// Find the 5th highest temperatures
		std::nth_element(all_temps.begin(), all_temps.begin() + 4, all_temps.end(), [](const TempRecord& a, const TempRecord& b)
			{
				return a.temperature < b.temperature;
			});

		// Find the 5th lowest temperatures
		std::nth_element(all_temps.begin(), all_temps.end() - 5, all_temps.end(), [](const TempRecord& a, const TempRecord& b)
			{
				return a.temperature < b.temperature;
			});

		// Create vectors for the 5 highest and lowest temperatures
		std::vector<TempRecord> highest_temps(all_temps.end() - 5, all_temps.end());
		std::vector<TempRecord> lowest_temps(all_temps.begin(), all_temps.begin() + 5);

		// Sort the temperatures
		std::sort(highest_temps.begin(), highest_temps.end(), [](const TempRecord& a, const TempRecord& b)
			{
				return a.temperature < b.temperature;
			});
		std::sort(lowest_temps.begin(), lowest_temps.end(), [](const TempRecord& a, const TempRecord& b)
			{
				return a.temperature < b.temperature;
			});

		// Print the 5 highest temperatures
		std::cout << "\n5 Highest Temperatures:\n";
		for (size_t i = 5; i >= 1; i--)
			std::cout << highest_temps.at(i - 1);
		std::cout << std::endl;

		// Print the 5 lowest temperatures
		std::cout << "\n5 Lowest Temperatures:\n";
		for (size_t i = 0; i < 5; i++)
			std::cout << lowest_temps.at(i);
		std::cout << std::endl;

	}

	{

		// Variables to store the start time and largest temperature difference
		size_t start_time_of_largest_diff = 0;
		double largest_diff = 0.0;

		// Iterate over each 10-minute interval
		for (size_t i = 0; i <= all_temps.size() - 10; i++)
		{

			// Find the minimum and maximum temperatures in this interval
			double min_temp = all_temps[i].temperature;
			double max_temp = all_temps[i].temperature;

			for (size_t j = i + 1; j < i + 10; j++)
			{
				min_temp = std::min(min_temp, all_temps[j].temperature);
				max_temp = std::max(max_temp, all_temps[j].temperature);
			}

			// Calculate the temperature difference
			double diff = max_temp - min_temp;

			// If this difference is larger than the current largest difference, update the largest difference and start time
			if (diff > largest_diff)
			{
				largest_diff = diff;
				start_time_of_largest_diff = all_temps[i].time;
			}
		}

		// Print the largest temperature difference
		std::cout << "\nLargest Temperature Difference: " << largest_diff << " degrees fahrenheit" << std::endl;

	}

}

bool problem_02::MarsRover::is_operational()
{

    // Return operational status
    return rover_is_operational.load();

}

void problem_02::MarsRover::shutdown()
{

	// Set operational to false
	rover_is_operational.store(false);

}
