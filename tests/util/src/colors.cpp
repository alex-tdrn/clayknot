#include "rsp/util/ColorRGB.hpp"
#include "rsp/util/ColorRGBA.hpp"

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
			auto C1 = rsp::ColorRGB::createRandom(seed);
			auto C2 = rsp::ColorRGBA::createRandom(seed);

			THEN("exactly the same color is generated each time")
			{
				REQUIRE(C1 == rsp::ColorRGB::createRandom(seed));
				REQUIRE(C2 == rsp::ColorRGBA::createRandom(seed));
			}
		}
	}
}

TEST_CASE("Colors can be packed and unpacked")
{
	GIVEN("Random color without alpha, C")
	{
		auto C = rsp::ColorRGB::createRandom();
		WHEN("packing C into C_packed")
		{
			auto C_packed = rsp::ColorRGB::pack(C);
			AND_WHEN("unpacking C_packed into C'")
			{
				auto C_prime = rsp::ColorRGB::unpack(C_packed);
				THEN("C and C' are equal, accounting for precision loss")
				{
					REQUIRE(rsp::ColorRGB::compareEqualLowPrecision(C, C_prime));
				}
			}
		}
	}
	GIVEN("Random color with alpha, C")
	{
		auto C = rsp::ColorRGBA::createRandom();
		WHEN("packing C into C_packed")
		{
			auto C_packed = rsp::ColorRGBA::pack(C);
			AND_WHEN("unpacking C_packed into C'")
			{
				auto C_prime = rsp::ColorRGBA::unpack(C_packed);
				THEN("C and C' are equal, accounting for precision loss")
				{
					REQUIRE(rsp::ColorRGBA::compareEqualLowPrecision(C, C_prime));
				}
			}
		}
	}
}