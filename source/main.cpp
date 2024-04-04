
// Local includes
#include "problem_01.hpp"
#include "problem_02.hpp"

// System includes
#include <iostream>

int main(int argc, char* argv[]) 
{
    // Exit if there are no arguments
    if (argc <= 1) 
    {
    
        // Error
        std::cerr << "Error: No arguments provided" << std::endl;
        return 1;

    }

    // Get the problem number
    int problem_number = std::atoi(argv[1]);

    // Switch on the problem number
    switch (problem_number)
    {

		case 1:

			// Call the main function of the first problem
			return problem_01::main();

        case 2:

			// Call the main function of the second problem
			return problem_02::main();

		default:

			// Error
			std::cerr << "Error: Invalid problem number" << std::endl;
			return 1;
	}   

}
