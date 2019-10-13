#include "catch.hpp"
#include "renderdeck/InputDataPort.hpp"
#include "renderdeck/OutputDataPort.hpp"
#include "renderdeck/InputEventPort.hpp"
#include "renderdeck/OutputEventPort.hpp"

template <typename A, typename B>
void testConnectionIsValid(A& a, B& b)
{
	THEN("connecting A to B is possible")
	{
		REQUIRE(a.canConnect(&b));
	}
	THEN("connecting A to B does not throw")
	{
		REQUIRE_NOTHROW(a.connect(&b));
	}
	THEN("connecting B to A is possible")
	{
		REQUIRE(b.canConnect(&a));
	}
	THEN("connecting B to A does not throw")
	{
		REQUIRE_NOTHROW(b.connect(&a));
	}
}
template <typename A, typename B>
void testConnectionIsInvalid(A& a, B& b)
{
	THEN("connecting A to B is impossible")
	{
		REQUIRE(!a.canConnect(&b));
	}
	THEN("connecting A to B throws")
	{
		REQUIRE_THROWS(a.connect(&b));
	}
	THEN("connecting B to A is impossible")
	{
		REQUIRE(!b.canConnect(&a));
	}
	THEN("connecting B to A throws")
	{
		REQUIRE_THROWS(b.connect(&a));
	}
}

TEST_CASE("base::ports::Connections between input and output ports")
{
	class TestOutputPort;
	class TestInputPort : public InputPort<TestOutputPort> {};
	class TestOutputPort : public OutputPort<TestInputPort> {};

	GIVEN("A disconnected input port A")
	{
		TestInputPort A;
		REQUIRE(!A.isConnected());
		AND_GIVEN("a disconnected output port B")
		{
			TestOutputPort B;
			REQUIRE(!B.isConnected());
			testConnectionIsValid(A, B);
			WHEN("B is connected to another input port C")
			{
				TestInputPort C;
				B.connect(&C);
				THEN("connecting A to B is still possible")
				{
					REQUIRE(A.canConnect(&B));
					AND_WHEN("connecting A to B")
					{
						REQUIRE_NOTHROW(A.connect(&B));
						THEN("C remains connected")
						{
							REQUIRE(C.isConnected());
						}
					}
				}
				THEN("connecting B to A is still possible")
				{
					REQUIRE(B.canConnect(&A));
					AND_WHEN("connecting B to A")
					{
						REQUIRE_NOTHROW(B.connect(&A));
						THEN("C remains connected")
						{
							REQUIRE(C.isConnected());
						}
					}
				}
			}
			WHEN("A is connected to another output port C")
			{
				TestOutputPort C;
				A.connect(&C);
				THEN("connecting A to B is still possible")
				{
					REQUIRE(A.canConnect(&B));
					AND_WHEN("connecting A to B")
					{
						REQUIRE_NOTHROW(A.connect(&B));
						THEN("C gets disconnected")
						{
							REQUIRE(!C.isConnected());
						}
					}
				}
				THEN("connecting B to A is still possible")
				{
					REQUIRE(B.canConnect(&A));
					AND_WHEN("connecting B to A")
					{
						REQUIRE_NOTHROW(B.connect(&A));
						THEN("C gets disconnected")
						{
							REQUIRE(!C.isConnected());
						}
					}
				}
			}
		}
		AND_GIVEN("a disconnected input port B")
		{
			TestInputPort B;
			REQUIRE(!B.isConnected());
			testConnectionIsInvalid(A, B);
		}
	}
	GIVEN("A disconnected output port A")
	{
		TestOutputPort A;
		REQUIRE(!A.isConnected());
		AND_GIVEN("a disconnected output port B")
		{
			TestOutputPort B;
			REQUIRE(!B.isConnected());
			testConnectionIsInvalid(A, B);
		}
	}
}

TEST_CASE("base::ports::Connections between data ports")
{
	GIVEN("A, an input data port holding an int")
	{
		InputDataPort<int> A;
		AND_GIVEN("B, an output data port holding an int")
		{
			OutputDataPort<int> B;
			testConnectionIsValid(A, B);
		}
		AND_GIVEN("B, an output data port holding a float")
		{
			OutputDataPort<float> B;
			testConnectionIsInvalid(A, B);
		}
		AND_GIVEN("B, an input data port holding an int")
		{
			InputDataPort<int> B;
			testConnectionIsInvalid(A, B);
		}
	}
	GIVEN("A, an output data port holding an int")
	{
		OutputDataPort<int> A;
		AND_GIVEN("B, an output data port holding an int")
		{
			OutputDataPort<float> B;
			testConnectionIsInvalid(A, B);
		}
	}
}

TEST_CASE("base::ports::Connections between event ports")
{
	GIVEN("A, an input event port referencing an empty lambda")
	{
		InputEventPort A{[](){}};
		AND_GIVEN("B, an output event port")
		{
			OutputEventPort B;
			testConnectionIsValid(A, B);
		}
		AND_GIVEN("B, an input event port referencing an empty lambda")
		{
			InputEventPort B{[](){}};
			testConnectionIsInvalid(A, B);
		}
	}
	GIVEN("A, an output event port")
	{
		OutputEventPort A;
		AND_GIVEN("B, an output event port")
		{
			OutputEventPort B;
			testConnectionIsInvalid(A, B);
		}
	}
}

TEST_CASE("base::ports::Event triggering")
{
	GIVEN("F a boolean flag initially set to false")
	{
		bool F = false;
		AND_GIVEN("L, a lambda function that sets F to true when ran")
		{
			auto L = [&F]() {
				F = true;
			};
			AND_GIVEN("A, an input event port referencing L")
			{
				InputEventPort A{L};
				THEN("triggering A calls L and thus, sets F to true")
				{
					A.trigger();
					REQUIRE(F == true);
				}
				AND_GIVEN("B, an output event port connected to A")
				{
					OutputEventPort B;
					B.connect(&A);
					THEN("triggering B also triggers A and thus, sets F to true")
					{
						B.trigger();
						REQUIRE(F == true);
					}
					WHEN("disconnecting B")
					{
						B.disconnect();
						THEN("triggering B has no effect")
						{
							B.trigger();
							REQUIRE(F == false);
						}
					}
				}
			}

		}
	}
	GIVEN("F, G, and H boolean flags initially set to false")
	{
		bool F = false;
		bool G = false;
		bool H = false;
		AND_GIVEN("L, M and N, lambda functions that sets F, G and H to true when ran")
		{
			auto L = [&F]() {
				F = true;
			};
			auto M = [&G]() {
				G = true;
			};
			auto N = [&H]() {
				H = true;
			};
			AND_GIVEN("A, B and C input event ports referencing L, M and N respectively")
			{
				InputEventPort A{L};
				InputEventPort B{M};
				InputEventPort C{N};
				AND_GIVEN("D, an output event port connected to A, B and C")
				{
					OutputEventPort D;
					D.connect(&A);
					D.connect(&B);
					D.connect(&C);
					THEN("triggering D also triggers A, B and C, thus setting F, G and H to true")
					{
						D.trigger();
						REQUIRE(F == true);
						REQUIRE(G == true);
						REQUIRE(H == true);
					}
					WHEN("disconnecting D from A")
					{
						D.disconnect(&A);
						THEN("triggering D has no effect on F, but still sets G, and H to true")
						{
							D.trigger();
							REQUIRE(F == false);
							REQUIRE(G == true);
							REQUIRE(H == true);
						}
					}
					WHEN("disconnecting D")
					{
						D.disconnect();
						THEN("triggering D has no effect")
						{
							D.trigger();
							REQUIRE(F == false);
							REQUIRE(G == false);
							REQUIRE(H == false);
						}
					}
				}
			}

		}
	}
}
