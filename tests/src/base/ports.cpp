#include "clk/base/any_input.hpp"
#include "clk/base/any_output.hpp"
#include "clk/base/input.hpp"
#include "clk/base/output.hpp"

#include <array>
#include <catch2/catch.hpp>
#include <memory>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_set>
#include <utility>
#include <vector>

// NOLINTNEXTLINE
#define DATATYPES \
	int, unsigned int, short, char*, (std::tuple<int, float>), (std::vector<int>), (std::array<int, 10>), \
		std::unique_ptr<int>, std::shared_ptr<int>, (clk::input_of<int>), (clk::output_of<int>)

template <typename T, typename U>
void require_connection_impossible(T& portA, U& portB);

template <typename T, typename U>
void require_connected(T& portA, U& portB);

template <typename T, typename U>
void require_unconnected(T& portA, U& portB);

// TODO fix
//  // NOLINTNEXTLINE
//  TEMPLATE_TEST_CASE("Ports cannot connect to themselves", "[base], [ports]",
//  	(clk::input_of<int>, clk::output_of<int>, clk::any_input, clk::any_output))
//  {
//  	GIVEN("a port")
//  	{
//  		TestType A;
//  		THEN("it cannot be connected to itself")
//  		{
//  			REQUIRE(!A.can_connect_to(A));
//  		}
//  	}
//  }

// // NOLINTNEXTLINE
// TEMPLATE_PRODUCT_TEST_CASE(
// 	"Ports of the same type cannot be connected", "[base], [ports]", (clk::input_of, clk::output_of), (DATATYPES))
// {
// 	GIVEN("ports A and B of same type")
// 	{
// 		TestType A;
// 		TestType B;
// 		require_connection_impossible(A, B);
// 	}
// }

TEST_CASE("Ports holding differing datatypes cannot be connected", "[base], [ports]")
{
	GIVEN("input port A of int and output port B of string")
	{
		clk::input_of<int> A;
		clk::output_of<std::string> B;
		require_connection_impossible(A, B);
	}
}

// NOLINTNEXTLINE
TEMPLATE_TEST_CASE("Ports holding the same datatype can be connected", "[base], [ports]", DATATYPES)
{
	GIVEN("input port A and output port B, both holding the same datatype")
	{
		clk::input_of<TestType> A;
		clk::output_of<TestType> B;
		THEN("they can be connected")
		{
			REQUIRE(A.can_connect_to(B));
			REQUIRE(B.can_connect_to(A));

			AND_THEN("connecting A to B does not fail")
			{
				REQUIRE_NOTHROW(A.connect_to(B));
				require_connected(A, B);
			}
			AND_THEN("connecting B to A does not fail")
			{
				REQUIRE_NOTHROW(B.connect_to(A));
				require_connected(A, B);
			}
		}
	}
}

// NOLINTNEXTLINE
TEMPLATE_TEST_CASE("Connected ports can be disconnected", "[base], [ports]", DATATYPES)
{
	GIVEN("connected ports A, and B")
	{
		clk::input_of<TestType> A;
		clk::output_of<TestType> B;
		A.connect_to(B);
		THEN("the connection can be severed")
		{
			WHEN("A disconnects from all")
			{
				REQUIRE_NOTHROW(A.disconnect());
				require_unconnected(A, B);
			}
			WHEN("A disconnects from B")
			{
				REQUIRE_NOTHROW(A.disconnect_from(B));
				require_unconnected(A, B);
			}
			WHEN("B disconnects from all")
			{
				REQUIRE_NOTHROW(B.disconnect());
				require_unconnected(A, B);
			}
			WHEN("B disconnects from A")
			{
				REQUIRE_NOTHROW(B.disconnect_from(A));
				require_unconnected(A, B);
			}
		}
	}
}

TEST_CASE("Ports disconnect automatically when their lifetime ends", "[base], [ports]")
{
	GIVEN("input port A connected to output port B")
	{
		clk::input_of<int> A;
		{
			clk::output_of<int> B;
			A.connect_to(B);
		}
		THEN("when B's lifetime ends, A gets disconnected automatically")
		{
			REQUIRE_FALSE(A.is_connected());
		}
	}

	GIVEN("output port B connected to input port A")
	{
		clk::output_of<int> B;

		{
			clk::input_of<int> A;
			B.connect_to(A);
		}
		THEN("when A's lifetime ends, B gets disconnected automatically")
		{
			REQUIRE_FALSE(B.is_connected());
		}
	}
}

// NOLINTNEXTLINE
TEMPLATE_TEST_CASE("Ports holding the same datatype return the same hash", "[base], [ports]", DATATYPES)
{
	GIVEN("input port A and output port B, both holding the same datatype")
	{
		clk::input_of<TestType> A;
		clk::output_of<TestType> B;
		THEN("they return the same datatype hash")
		{
			REQUIRE(A.data_type_hash() == B.data_type_hash());
		}
	}
}

TEST_CASE("Unconnected input ports can still provide default data", "[base], [ports]")
{
	GIVEN("unconnected input port A")
	{
		clk::input_of<int> A;
		THEN("trying to get data from it, succeeds")
		{
			REQUIRE_NOTHROW(A.data());
			REQUIRE_NOTHROW(*A);
			REQUIRE_NOTHROW(A.operator->());
		}
	}
}

TEST_CASE("Output ports can provide data", "[base], [ports]")
{
	GIVEN("output port A")
	{
		clk::output_of<std::string> A;
		THEN("A can provide modifiable references to its data")
		{
			std::string const v1 = "1";
			A.data() = v1;
			REQUIRE(A.data() == v1);
			REQUIRE(*A == v1);

			std::string const v2 = "2";
			*A = v2;
			REQUIRE(A.data() == v2);
			REQUIRE(*A == v2);

			A->clear();
			REQUIRE(A.data().empty());
			REQUIRE((*A).empty());
			REQUIRE(A->empty());
		}
		AND_GIVEN("B, a const reference to A")
		{
			auto const& B = A;
			THEN("B can provide read-only references to its data")
			{
				const auto* v1 = &(B.data());
				const auto* v2 = &(*B);
				const auto* v3 = B.operator->();

				REQUIRE(v1 == v2);
				REQUIRE(v1 == v3);
				STATIC_REQUIRE(std::is_const_v<std::remove_pointer_t<decltype(v1)>>);
				STATIC_REQUIRE(std::is_const_v<std::remove_pointer_t<decltype(v2)>>);
				STATIC_REQUIRE(std::is_const_v<std::remove_pointer_t<decltype(v3)>>);
			}
		}
	}
}

TEST_CASE("Connected input ports can provide the data from their connection", "[base], [ports]")
{
	GIVEN("output port A connected to multiple input ports")
	{
		clk::output_of<int> A;
		std::array<clk::input_of<int>, 3> inputs;
		for(auto& port : inputs)
			port.connect_to(A);

		THEN("all connected input ports can provide read-only references to A's data")
		{
			auto* original_value = &(A.data());

			for(auto& port : inputs)
			{
				REQUIRE_NOTHROW(port.data());
				REQUIRE_NOTHROW(*port);
				REQUIRE_NOTHROW(port.operator->());

				const auto* v1 = &(port.data());
				const auto* v2 = &(*port);
				const auto* v3 = port.operator->();

				REQUIRE(v1 == v2);
				REQUIRE(v1 == v3);
				REQUIRE(v1 == original_value);

				STATIC_REQUIRE(std::is_const_v<std::remove_pointer_t<decltype(v1)>>);
				STATIC_REQUIRE(std::is_const_v<std::remove_pointer_t<decltype(v2)>>);
				STATIC_REQUIRE(std::is_const_v<std::remove_pointer_t<decltype(v3)>>);
			}
		}
	}
}

TEST_CASE(
	"A push callback can be set on an input port in order to alert the port owner that an update occured", "[port]")
{
	GIVEN("input port A")
	{
		clk::input_of<int> A;
		AND_GIVEN("non-throwing callable F, set as the push callback for A")
		{
			bool called = false;
			auto F = [&](auto&& /*unused*/) {
				called = true;
			};
			A.set_push_callback(std::move(F));
			WHEN("calling A.push(), F is called as well")
			{
				A.push();
				REQUIRE(called);
			}

			AND_GIVEN("output port B")
			{
				clk::output_of<int> B;
				WHEN("A and B are connected")
				{
					B.connect_to(A);

					THEN("calling B.push(), transitively calls A.push(), and F as well")
					{
						B.push();
						REQUIRE(called);
					}
				}
			}
		}
	}
}

TEST_CASE("A pull callback can be set on an output port in order to alert the port owner that an update may be needed",
	"[port]")
{
	GIVEN("output port A")
	{
		clk::output_of<int> A;
		AND_GIVEN("non-throwing callable F, set as the pull callback for A")
		{
			bool called = false;
			auto F = [&](auto&& /*unused*/) {
				called = true;
			};
			A.set_pull_callback(std::move(F));
			WHEN("calling A.pull(), F is called as well")
			{
				REQUIRE_NOTHROW(A.pull());
				REQUIRE(called);
			}

			AND_GIVEN("output port B")
			{
				clk::input_of<int> B;
				WHEN("A and B are connected")
				{
					B.connect_to(A);

					THEN("calling B.pull(), transitively calls A.pull(), and F as well")
					{
						B.pull();
						REQUIRE(called);
					}
				}
			}
		}
	}
}

template <typename T, typename U>
void require_connection_impossible(T& A, U& B)
{
	THEN("they cannot be connected")
	{
		REQUIRE_FALSE(A.can_connect_to(B));
		REQUIRE_FALSE(B.can_connect_to(A));
		AND_WHEN("trying anyway, an exception is thrown")
		{
			REQUIRE_THROWS(A.connect_to(B));
			require_unconnected(A, B);
			REQUIRE_THROWS(B.connect_to(A));
			require_unconnected(A, B);
		}
	}
}

template <typename T, typename U>
void require_connected(T& A, U& B)
{
	REQUIRE(A.is_connected_to(B));
	REQUIRE(B.is_connected_to(A));
}

template <typename T, typename U>
void require_unconnected(T& A, U& B)
{
	REQUIRE_FALSE(A.is_connected_to(B));
	REQUIRE_FALSE(B.is_connected_to(A));
}