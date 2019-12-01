#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include <iostream>
#include "LCS.hpp"

TEST_CASE("Testing LCS precompute","[]"){
    // the * at the begginig is a buffer so we index at 1
    std::string my_string = "*aaabbacd";
    RLE_string_helper helper;
    std::vector<RLE_run> rle_s = helper.get_rle_string(my_string);
    std::vector< std::map<char, int> > vec(rle_s.size());
    LCS::precompute(vec, rle_s);
    REQUIRE(vec[1]['a'] == 3);
    REQUIRE(vec[0]['a'] == 0);
    REQUIRE(vec[2]['b'] == 2);
    REQUIRE(vec[1]['b'] == 0);
    REQUIRE(vec[3]['a'] == 4);
    REQUIRE(vec[2]['a'] == 3);
    REQUIRE(vec[4]['c'] == 1);
    REQUIRE(vec[5]['d'] == 1);
    REQUIRE(vec[4]['d'] == 0);
}
