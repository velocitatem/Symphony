//
// Created by velocitatem on 12/8/24.
//

#include <gtest/gtest.h>
#include "definitions.h"
#include "search.h"



TEST(Definitions, State) {
    State state;
    EXPECT_NO_THROW(state.print());
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
