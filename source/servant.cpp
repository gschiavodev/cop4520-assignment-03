
// System includes
#include <iostream>
#include <iomanip>

// Local includes
#include "main.hpp"
#include "servant.hpp"

// Static attributes
std::atomic<size_t> problem_01::Servant::n_thank_you_cards_written(0);
std::unique_ptr<size_t> problem_01::Servant::minotaur_check_request(nullptr);
bool problem_01::Servant::minotaur_check_request_processed(false);
std::condition_variable problem_01::Servant::cv;
std::mutex problem_01::Servant::cv_mutex;

problem_01::Servant::Servant(std::list<Present>& sorted_presents, std::mutex& sorted_presents_mutex, std::vector<Present>& unsorted_bag, std::mutex& unsorted_bag_mutex) :
    sorted_presents(sorted_presents),
    sorted_presents_mutex(sorted_presents_mutex),
    unsorted_bag(unsorted_bag),
    unsorted_bag_mutex(unsorted_bag_mutex)
{

}

void problem_01::Servant::operator()()
{

	// Choose a random starting action
	action current_action = static_cast<action>(rand() % static_cast<int>(action::size));
	
	// Keep working until the unsorted bag is empty
    while (get_number_of_thank_you_cards_written() != NUM_PRESENTS)
    {
		
		// Decide which action to take...
		// 1. Take a present from the unsorted bag, and insert it into the sorted presents list
		// 2. Write a "Thank you" card to the guest who gave the present, and remove the present from the sorted presents list
		// 3. Per the minotaur's request, check whether a specific present is in the sorted presents list

		// Perform the action
		switch (current_action)
		{

			case action::SORT_A_PRESENT:
			{
				
				// Take a present from the unsorted bag
				std::unique_ptr<Present> present = this->take_present_from_unsorted_bag();

				// Check if the present is valid
				if (present == nullptr)
					break;

				// Insert the present into the sorted presents list
				this->insert_present_into_sorted_presents_list(*present);
				
				// ...
				break;

			}

			case action::WRITE_THANK_YOU_CARD:
			{

				// Take a present from the sorted presents list and write a thank you card
				this->take_present_and_write_thank_you_card();

				// ...
				break;

			}

			case action::CHECK_FOR_PRESENT:
			{

				// Check if the minotaur has made a request
				this->check_minotaur_request();

				// ...
				break;

			}

			default:
			{
				
				// ...
				break;

			}

		}

		// Change to the next action
		current_action = static_cast<action>((static_cast<int>(current_action) + 1) % static_cast<int>(action::size));
		
	}

	// Notify all threads
	cv.notify_all();

}

std::unique_ptr<problem_01::Present> problem_01::Servant::take_present_from_unsorted_bag()
{
    // Lock the unsorted_bag_mutex
    std::lock_guard<std::mutex> unsorted_bag_lock(this->unsorted_bag_mutex);

    // Check if the unsorted bag is empty
    if (this->unsorted_bag.empty()) 
        return nullptr;

    // Get the present from the unsorted bag
	Present present = std::move(this->unsorted_bag.back());

    // Remove the present from the unsorted bag
    this->unsorted_bag.pop_back();

	// Return the present
	return std::make_unique<Present>(present);

}

void problem_01::Servant::insert_present_into_sorted_presents_list(const Present& present)
{

	// Lock the sorted_presents_mutex
	std::lock_guard<std::mutex> sorted_presents_lock(this->sorted_presents_mutex);

	// Find the position to insert the present
	auto pos = std::upper_bound(this->sorted_presents.begin(), this->sorted_presents.end(), present, Present::compare_by_id);

	// Insert the present in the sorted list
	this->sorted_presents.insert(pos, present);

}

void problem_01::Servant::take_present_and_write_thank_you_card()
{

	// Lock the sorted_presents_mutex
	std::lock_guard<std::mutex> sorted_presents_lock(this->sorted_presents_mutex);

	// Check if the sorted presents list is empty
	if (this->sorted_presents.empty())
		return;

	// Get the present from the sorted presents list
	Present present = std::move(this->sorted_presents.front());

	// Remove the present from the sorted presents list
	this->sorted_presents.pop_front();

	// Write a "Thank you" card to the guest who gave the present
	n_thank_you_cards_written.fetch_add(1);

}

size_t problem_01::Servant::get_number_of_thank_you_cards_written()
{

	// Return the number of thank you cards written
	return n_thank_you_cards_written.load();

}

void problem_01::Servant::set_minotaur_check_request(std::unique_ptr<size_t> value) 
{

	// Lock the cv_mutex
	std::lock_guard<std::mutex> cv_lock(cv_mutex);

	// Set the minotaur check request
	minotaur_check_request = std::move(value);

	// Notify all threads
	cv.notify_all();

}

void problem_01::Servant::check_minotaur_request()
{
	// Lock the cv_mutex
	std::lock_guard<std::mutex> cv_lock(cv_mutex);

	// Check if a request has been made
	if (minotaur_check_request != nullptr)
	{

		// Lock the sorted_presents_mutex
		std::lock_guard<std::mutex> sorted_presents_lock(this->sorted_presents_mutex);

		// Check if the present is in the sorted presents list
		auto it = std::find_if(sorted_presents.begin(), sorted_presents.end(),
			[this](const problem_01::Present& present) 
			{
				return present.unique_tag == *minotaur_check_request;
			}
		);

		// Print the result
        std::cout << "Minotaur requested to check for present: " << std::setw(std::to_string(NUM_PRESENTS).length()) << (*minotaur_check_request) << ", it was " << (it != sorted_presents.end() ? "found!" : "not found!") << std::endl;
		
		// Reset the request
		minotaur_check_request.reset();

		// Set the minotaur_check_processed flag to true and notify the condition variable
		set_minotaur_request_processed(true);
		cv.notify_all();

	}

}

bool problem_01::Servant::is_minotaur_request_processed()
{

	// Return the value of the minotaur_check_request_processed flag
	return minotaur_check_request_processed;

}

void problem_01::Servant::set_minotaur_request_processed(bool value)
{

	// Set the value of the minotaur_check_request_processed flag
	minotaur_check_request_processed = value;

}