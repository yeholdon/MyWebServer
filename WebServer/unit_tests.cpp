#include <string>
#include "base/Logging.h"

// #define CATCH_CONFIG_MAIN
// #include "catch2/catch.hpp"


// TEST_CASE( "simple" )
// {
//     REQUIRE(LOG << "dasfas" << "dsfdas", 1);
// }

int main()
{
    for (int i = 0; i < 20000; ++i)
    {
        LOG << "dfssd" << "terhger";
    }
    getchar();
    return 0;
}