
// System includes
#include <chrono>
#include <thread>

// Local includes
#include "problem_02.hpp"
#include "mars_rover.hpp"

int problem_02::main()
{

	// Create mars rover
	MarsRover rover;

	// Simulation loop
	for (size_t i = 0; i < (N_OPERATIONAL_HOURS / REPORT_INTERVAL_HOUR); i++)
	{

		// 
		while (rover.get_time() < ((i + 1) * REPORT_INTERVAL_MIN))
		{

			// Sleep (seconds so we can test the program)
			// std::this_thread::sleep_for(std::chrono::minutes(SAMPLE_INTERVAL_MIN));
			std::this_thread::sleep_for(std::chrono::seconds(SAMPLE_INTERVAL_MIN));

			// Increment the time
			rover.increment_time();

			// Reset the data in preparation for the next minute
			rover.reset_sample_interval_data();

		}

		// Generate a report
		rover.generate_report();

	}

	// Shutdown the rover
	rover.shutdown();

	// Exit
	return 0;

}
