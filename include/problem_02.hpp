#pragma once

// System includes
#include <cstddef>

namespace problem_02
{

	// Compile time constants
	constexpr size_t N_TEMPERATURE_SENSORS { 8 }; // 'x' temperature sensors
	constexpr size_t N_OPERATIONAL_HOURS   { 1 }; // Run the simulation for 'x' hour(s)
	constexpr size_t REPORT_INTERVAL_HOUR  { 1 }; // Generate a report every 'x' hour(s)
	constexpr size_t REPORT_INTERVAL_MIN   { REPORT_INTERVAL_HOUR * 30 };
	constexpr size_t SAMPLE_INTERVAL_MIN   { 1 }; // Get a sample every 'x' minute(s)

	// Main function
	int main();

}
