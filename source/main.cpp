
// System includes
#include <array>
#include <vector>
#include <algorithm>
#include <thread>
#include <iostream>

// Local includes
#include "main.hpp"
#include "present.hpp"
#include "servant.hpp"

int main() 
{

	// Call the main function of the first problem
	problem_01::main();

}

void problem_01::main()
{

	// Create a list of presents
	std::vector<problem_01::Present> unsorted_bag(NUM_PRESENTS);

	// Shuffle the presents
	std::random_shuffle(unsorted_bag.begin(), unsorted_bag.end());

	// Create a mutex to protect the unsorted_bag list
	std::mutex unsorted_bag_mutex;

	// Create a list to sort the presents
	std::list<problem_01::Present> sorted_presents;

	// Create a mutex to protect the sorted_presents list
	std::mutex sorted_presents_mutex;

	// Create an to store the servants
	std::array<std::thread, NUM_SERVANTS> servants;

	// Get the servants to work!
	for (int i = 0; i < NUM_SERVANTS; i++)
	{

		// Create a servant thread by passing the servant as a callable object
		std::thread servant_thread = std::thread(problem_01::Servant(sorted_presents, sorted_presents_mutex, unsorted_bag, unsorted_bag_mutex));

		// Move the thread to the servants array
		servants.at(i) = std::move(servant_thread);

	}

	// Keep working until the unsorted bag is empty
	while (Servant::get_number_of_thank_you_cards_written() != NUM_PRESENTS)
	{

		// Wait for a random amount of time
		std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100));

		// Choose a random present to check
		std::unique_ptr<size_t> minotaur_check_request = std::make_unique<size_t>(rand() % NUM_PRESENTS);

		// Set the minotaur check request
		problem_01::Servant::set_minotaur_check_request(std::move(minotaur_check_request));

		// Reset the minotaur_check_processed flag to false
		Servant::set_minotaur_request_processed(false);

		// Wait for the minotaur request to be processed
		std::unique_lock<std::mutex> lck(Servant::cv_mutex);
		Servant::cv.wait(lck, [] { return ((Servant::is_minotaur_request_processed() || Servant::get_number_of_thank_you_cards_written() == NUM_PRESENTS)); });

	}

	// Wait for the servants to finish (they should be done by now)
	for (int i = 0; i < NUM_SERVANTS; i++)
	{

		// Get the servant thread by reference
		std::thread& servant_thread = servants.at(i);

		// Join the servant thread
		if (servant_thread.joinable())
			servant_thread.join();

	}

	// Output debug information
	std::cout << "\nThank you cards written: " << Servant::get_number_of_thank_you_cards_written() << std::endl;

}
