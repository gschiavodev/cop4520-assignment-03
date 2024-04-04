#pragma once

// Local includes
#include "problem_02.hpp"
#include "temperature_sensor.hpp"

// System includes
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <vector>

namespace problem_02
{

	// Forward declaration of TemperatureSensor
	class TemperatureSensor;

	class MarsRover
	{

	private:

		// Time
		size_t time;

		struct TemperatureSensorInstance
		{
				
			TemperatureSensor* sensor;
			std::thread thread;

		};

		// Temperature sensors
		TemperatureSensorInstance temperature_sensors[N_TEMPERATURE_SENSORS];

		// Data (enough temperature data for one report interval)
		std::atomic<size_t> samples_collected_during_sample_interval;
		std::vector<double> data;
		std::mutex data_mutex;

		// Condition variable for temperature sensors (shared with TemperatureSensor)
		std::condition_variable cv_rover_ready_to_collect_data;
		std::mutex cv_rover_ready_to_collect_data_mutex;
		bool ready_to_collect_data;

		// Condition variable for temperature sensors (shared with TemperatureSensor)
		std::condition_variable cv_rover_data_collected;
		std::mutex cv_rover_data_collected_mutex;
		bool data_collected;


	public:

		// Constructor
		MarsRover();

		// Destructor
		~MarsRover();

		// Getters
		size_t get_time() const;
		std::condition_variable& get_rover_condition_variable();
		std::mutex& get_rover_condition_variable_mutex();

		// Setters
		void set_time(size_t time);

		// Helper functions
		void increment_time();
		void wait_for_rover_to_be_ready_to_collect_data();
		void wait_for_rover_to_collect_data();
		void send_temperature_data_to_rover(double temperature);
		void reset_report_interval_data();
		void reset_sample_interval_data();
		void generate_report();

	};

}

