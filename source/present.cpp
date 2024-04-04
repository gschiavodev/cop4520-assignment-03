
// System includes
#include <iostream>

// Local includes
#include "present.hpp"

// Static member initialization
size_t problem_01::Present::counter(0);

problem_01::Present::Present() : 
	unique_tag(counter++)
{

}

bool problem_01::Present::compare_by_id(const Present& this_present, const Present& other_present)
{

	// Compare the unique tags of the presents (ascending order)
	return this_present.unique_tag < other_present.unique_tag;

}
