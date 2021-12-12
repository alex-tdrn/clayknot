#include "clk/util/static_string.hpp"

#include <catch2/catch.hpp>

template <clk::static_string c>
class hmm
{
};

TEST_CASE("hmmm")
{
	hmm<"12345"> ss;
	hmm<"123dfhdh45"> sds;
}
