#include "rsp/base/Port.hpp"

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
		std::unique_ptr<int>, std::shared_ptr<int>, (rsp::InputPort<int>), (rsp::OutputPort<int>)

template <typename T, typename U>
void requireConnectionImpossible(T& portA, U& portB);

template <typename T, typename U>
void requireConnected(T& portA, U& portB);

template <typename T, typename U>
void requireUnconnected(T& portA, U& portB);

// NOLINTNEXTLINE
TEMPLATE_PRODUCT_TEST_CASE(
	"Ports cannot connect to themselves", "[ports]", (rsp::InputPort, rsp::OutputPort), (DATATYPES))
{
	GIVEN("a port")
	{
		TestType A;
		THEN("it cannot be connected to itself")
		{
			REQUIRE(!A.canConnectTo(A));
			AND_WHEN("trying anyway, an exception is thrown")
			{
				REQUIRE_THROWS(A.connectTo(A));
				REQUIRE(!A.isConnected());
			}
		}
	}
}

// NOLINTNEXTLINE
TEMPLATE_PRODUCT_TEST_CASE(
	"Ports of the same type cannot be connected", "[ports]", (rsp::InputPort, rsp::OutputPort), (DATATYPES))
{
	GIVEN("ports A and B of same type")
	{
		TestType A;
		TestType B;
		requireConnectionImpossible(A, B);
	}
}

TEST_CASE("Ports holding differing datatypes cannot be connected", "[ports]")
{
	GIVEN("input port A of int and output port B of string")
	{
		rsp::InputPort<int> A;
		rsp::OutputPort<std::string> B;
		requireConnectionImpossible(A, B);
	}
}

// NOLINTNEXTLINE
TEMPLATE_TEST_CASE("Ports holding the same datatype can be connected", "[ports]", DATATYPES)
{
	GIVEN("input port A and output port B, both holding the same datatype")
	{
		rsp::InputPort<TestType> A;
		rsp::OutputPort<TestType> B;
		THEN("they can be connected")
		{
			REQUIRE(A.canConnectTo(B));
			REQUIRE(B.canConnectTo(A));

			AND_THEN("connecting A to B does not fail")
			{
				REQUIRE_NOTHROW(A.connectTo(B));
				requireConnected(A, B);
			}
			AND_THEN("connecting B to A does not fail")
			{
				REQUIRE_NOTHROW(B.connectTo(A));
				requireConnected(A, B);
			}
		}
	}
}

// NOLINTNEXTLINE
TEMPLATE_TEST_CASE("Connected ports can be disconnected", "[ports]", DATATYPES)
{
	GIVEN("connected ports A, and B")
	{
		rsp::InputPort<TestType> A;
		rsp::OutputPort<TestType> B;
		A.connectTo(B);
		THEN("the connection can be severed")
		{
			WHEN("A disconnects from all")
			{
				REQUIRE_NOTHROW(A.disconnect());
				requireUnconnected(A, B);
			}
			WHEN("A disconnects from B")
			{
				REQUIRE_NOTHROW(A.disconnectFrom(B));
				requireUnconnected(A, B);
			}
			WHEN("B disconnects from all")
			{
				REQUIRE_NOTHROW(B.disconnect());
				requireUnconnected(A, B);
			}
			WHEN("B disconnects from A")
			{
				REQUIRE_NOTHROW(B.disconnectFrom(A));
				requireUnconnected(A, B);
			}
		}
	}
}

TEST_CASE("Ports disconnect automatically when their lifetime ends", "[ports]")
{
	GIVEN("input port A connected to output port B")
	{
		rsp::InputPort<int> A;
		{
			rsp::OutputPort<int> B;
			A.connectTo(B);
		}
		THEN("when B's lifetime ends, A gets disconnected automatically")
		{
			REQUIRE_FALSE(A.isConnected());
		}
	}

	GIVEN("output port B connected to input port A")
	{
		rsp::OutputPort<int> B;

		{
			rsp::InputPort<int> A;
			B.connectTo(A);
		}
		THEN("when A's lifetime ends, B gets disconnected automatically")
		{
			REQUIRE_FALSE(B.isConnected());
		}
	}
}

// NOLINTNEXTLINE
TEMPLATE_TEST_CASE("Ports holding the same datatype return the same hash", "[ports]", DATATYPES)
{
	GIVEN("input port A and output port B, both holding the same datatype")
	{
		rsp::InputPort<TestType> A;
		rsp::OutputPort<TestType> B;
		THEN("they return the same datatype hash")
		{
			REQUIRE(A.getDataTypeHash() == B.getDataTypeHash());
		}
	}
}

TEST_CASE("Unconnected input ports cannot provide data", "[ports]")
{
	GIVEN("unconnected input port A")
	{
		rsp::InputPort<int> A;
		THEN("trying to get data from it, fails")
		{
			REQUIRE_THROWS(A.get());
			REQUIRE_THROWS(*A);
			REQUIRE_THROWS(A.operator->());
		}
	}
}

TEST_CASE("Output ports can provide data", "[ports]")
{
	GIVEN("output port A")
	{
		rsp::OutputPort<std::string> A;
		THEN("A can provide modifiable references to its data")
		{
			std::string const v1 = "1";
			A.get() = v1;
			REQUIRE(A.get() == v1);
			REQUIRE(*A == v1);

			std::string const v2 = "2";
			*A = v2;
			REQUIRE(A.get() == v2);
			REQUIRE(*A == v2);

			A->clear();
			REQUIRE(A.get().empty());
			REQUIRE((*A).empty());
			REQUIRE(A->empty());
		}
		AND_GIVEN("B, a const reference to A")
		{
			auto const& B = A;
			THEN("B can provide read-only references to its data")
			{
				const auto* v1 = &(B.get());
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

TEST_CASE("Connected input ports can provide the data from their connection", "[ports]")
{
	GIVEN("output port A connected to multiple input ports")
	{
		rsp::OutputPort<int> A;
		std::array<rsp::InputPort<int>, 3> inputPorts;
		for(auto& port : inputPorts)
			port.connectTo(A);

		THEN("all connected input ports can provide read-only references to A's data")
		{
			auto* originalValue = &(A.get());

			for(auto& port : inputPorts)
			{
				REQUIRE_NOTHROW(port.get());
				REQUIRE_NOTHROW(*port);
				REQUIRE_NOTHROW(port.operator->());

				const auto* v1 = &(port.get());
				const auto* v2 = &(*port);
				const auto* v3 = port.operator->();

				REQUIRE(v1 == v2);
				REQUIRE(v1 == v3);
				REQUIRE(v1 == originalValue);

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
		rsp::InputPort<int> A;
		AND_GIVEN("non-throwing callable F, set as the push callback for A")
		{
			bool called = false;
			auto F = [&](auto&& /*unused*/) {
				called = true;
			};
			A.setPushCallback(std::move(F));
			WHEN("calling A.push(), F is called as well")
			{
				A.push();
				REQUIRE(called);
			}

			AND_GIVEN("output port B")
			{
				rsp::OutputPort<int> B;
				WHEN("A and B are connected")
				{
					B.connectTo(A);

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
		rsp::OutputPort<int> A;
		AND_GIVEN("non-throwing callable F, set as the pull callback for A")
		{
			bool called = false;
			auto F = [&](auto&& /*unused*/) {
				called = true;
			};
			A.setPullCallback(std::move(F));
			WHEN("calling A.pull(), F is called as well")
			{
				REQUIRE_NOTHROW(A.pull());
				REQUIRE(called);
			}

			AND_GIVEN("output port B")
			{
				rsp::InputPort<int> B;
				WHEN("A and B are connected")
				{
					B.connectTo(A);

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
void requireConnectionImpossible(T& A, U& B)
{
	THEN("they cannot be connected")
	{
		REQUIRE_FALSE(A.canConnectTo(B));
		REQUIRE_FALSE(B.canConnectTo(A));
		AND_WHEN("trying anyway, an exception is thrown")
		{
			REQUIRE_THROWS(A.connectTo(B));
			requireUnconnected(A, B);
			REQUIRE_THROWS(B.connectTo(A));
			requireUnconnected(A, B);
		}
	}
}

template <typename T, typename U>
void requireConnected(T& A, U& B)
{
	REQUIRE(A.isConnectedTo(B));
	REQUIRE(B.isConnectedTo(A));
}

template <typename T, typename U>
void requireUnconnected(T& A, U& B)
{
	REQUIRE_FALSE(A.isConnectedTo(B));
	REQUIRE_FALSE(B.isConnectedTo(A));
}