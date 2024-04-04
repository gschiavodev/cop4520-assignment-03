#pragma once

namespace problem_01
{

    class Present
    {

    private:

        // Static attributes
        static size_t counter;

    public:

        // Attributes
        size_t unique_tag;

        // Constructor
        Present();

        // Helper functions
        static bool compare_by_id(const Present& this_present, const Present& other_present);

    };

}
