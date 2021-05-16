#include "rsp/util/color_rgb.hpp"
#include "rsp/util/color_rgba.hpp"

#include <catch2/catch.hpp>
#include <chrono>
#include <random>

TEST_CASE("Colors can be randomly generated")
{
	GIVEN("A random seed S")
	{
		std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
		std::uniform_int_distribution<std::uint64_t> distribution;
		std::uint64_t seed = distribution(generator);
		WHEN("generating a random color from S")
		{
			auto C1 = rsp::color_rgb::create_random(seed);
			auto C2 = rsp::color_rgba::create_random(seed);

			THEN("exactly the same color is generated each time")
			{
				REQUIRE(C1 == rsp::color_rgb::create_random(seed));
				REQUIRE(C2 == rsp::color_rgba::create_random(seed));
			}
		}
	}
}

TEST_CASE("Colors can be packed and unpacked")
{
	GIVEN("Random color without alpha, C")
	{
		auto C = rsp::color_rgb::create_random();
		WHEN("packing C into C_packed")
		{
			auto C_packed = rsp::color_rgb::pack(C);
			AND_WHEN("unpacking C_packed into C'")
			{
				auto C_prime = rsp::color_rgb::unpack(C_packed);
				THEN("C and C' are equal, accounting for precision loss")
				{
					REQUIRE(rsp::color_rgb::compare_equal_low_precision(C, C_prime));
				}
			}
		}
	}
	GIVEN("Random color with alpha, C")
	{
		auto C = rsp::color_rgba::create_random();
		WHEN("packing C into C_packed")
		{
			auto C_packed = rsp::color_rgba::pack(C);
			AND_WHEN("unpacking C_packed into C'")
			{
				auto C_prime = rsp::color_rgba::unpack(C_packed);
				THEN("C and C' are equal, accounting for precision loss")
				{
					REQUIRE(rsp::color_rgba::compare_equal_low_precision(C, C_prime));
				}
			}
		}
	}
}