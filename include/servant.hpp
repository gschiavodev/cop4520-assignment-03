#pragma once

// System includes
#include <list>
#include <vector>
#include <mutex>
#include <memory>

// Local includes
#include "present.hpp"

namespace problem_01
{

	// Class definition
    class Servant
    {

	private:

		// Static attributes
		static std::atomic<size_t> n_thank_you_cards_written;
		static std::unique_ptr<size_t> minotaur_check_request;
		static bool minotaur_check_request_processed;

		// Attributes
		std::list<Present>& sorted_presents;
		std::mutex& sorted_presents_mutex;
		std::vector<Present>& unsorted_bag;
		std::mutex& unsorted_bag_mutex;

		// Enum of possible actions
        enum class action
        {

			// Actions
			SORT_A_PRESENT,
			WRITE_THANK_YOU_CARD,
			CHECK_FOR_PRESENT,

			// Keep this as the last element
			size

		};

	public:

		// Static attributes
		static std::condition_variable cv;
		static std::mutex cv_mutex;

		// Constructor
		Servant(std::list<Present>& sorted_presents, std::mutex& sorted_presents_mutex, std::vector<Present>& unsorted_bag, std::mutex& unsorted_bag_mutex);
		
		// Operators
		void operator()();

		// Helper functions
		static size_t get_number_of_thank_you_cards_written();
		static void set_minotaur_check_request(std::unique_ptr<size_t> value);
		static bool is_minotaur_request_processed();
		static void set_minotaur_request_processed(bool value);

	private:

		// Helper functions
		std::unique_ptr<Present> take_present_from_unsorted_bag();
		void insert_present_into_sorted_presents_list(const Present& present);
		void take_present_and_write_thank_you_card();
		void check_minotaur_request();

    };

}
