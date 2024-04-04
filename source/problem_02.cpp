
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

	// Simulation loop (N_REPORT_INTERVALS per day)
	for (size_t i = 0; i < (24 / REPORT_INTERVAL_HOUR); i++)
	{

		// 
		while (rover.get_time() <= (REPORT_INTERVAL_HOUR * 60))
		{

			// Sleep (seconds so we can test the program)
			// std::this_thread::sleep_for(std::chrono::minutes(SAMPLE_INTERVAL_MIN));
			std::this_thread::sleep_for(std::chrono::seconds(SAMPLE_INTERVAL_MIN));

			// Increment the time
			rover.increment_time();

		}

		// Generate a report
		rover.generate_report();

		// Reset the data in preparation for the next minute
		rover.reset_report_interval_data();

	}

	// Exit
	return 0;

}
