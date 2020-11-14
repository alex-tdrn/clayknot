#include "rsp/base/Node.hpp"
#include "rsp/base/Port.hpp"

#include <catch2/catch.hpp>

class TestNode final : public rsp::Node
{
public:
	rsp::InputPort<int> input0{"I_0"};
	rsp::OutputPort<int> output0{"O_0"};
	int ctUpdateCalled = 0;

	TestNode()
	{
		registerPort(input0);
		registerPort(output0);
	}

	void update() override
	{
		ctUpdateCalled++;
		*output0 = *input0;
	}

	std::string const& getName() const override
	{
		static const std::string name = "Test Node";
		return name;
	}
};

TEST_CASE("New node types can be implemented by inheriting from rsp::Node", "[nodes], [ports]")
{
	GIVEN("TestNode A, with one input port AI_0 and one output port AO_0 of the same datatype")
	{
		TestNode A;
		auto& AI_0 = A.input0;
		auto& AO_0 = A.output0;

		auto requireNoPullUpdate = [&]() {
			THEN("calling pull on AO_0 does not call update()")
			{
				auto ctOld = A.ctUpdateCalled;
				AO_0.pull();
				auto ctUpdates = A.ctUpdateCalled - ctOld;
				REQUIRE(ctUpdates == 0);
			}
		};

		auto requireNoPushUpdate = [&]() {
			THEN("calling push on AI_0 does not call update()")
			{
				auto ctOld = A.ctUpdateCalled;
				AI_0.push();
				auto ctUpdates = A.ctUpdateCalled - ctOld;
				REQUIRE(ctUpdates == 0);
			}
		};

		auto requirePushUpdate = [&]() {
			THEN("calling push on AI_0 calls update() once")
			{
				auto ctOld = A.ctUpdateCalled;
				AI_0.push();
				auto ctUpdates = A.ctUpdateCalled - ctOld;
				REQUIRE(ctUpdates == 1);
			}
		};

		auto requirePullUpdate = [&]() {
			THEN("calling pull on AO_0 calls update() once")
			{
				auto ctOld = A.ctUpdateCalled;
				AO_0.pull();
				auto ctUpdates = A.ctUpdateCalled - ctOld;
				REQUIRE(ctUpdates == 1);
			}
		};

		THEN("ports registered by the derived class, are accessible from the base class in the same order they where "
			 "registered")
		{
			REQUIRE(&AI_0 == &A.getInputPort(0));
			REQUIRE(&AO_0 == &A.getOutputPort(0));
		}
		requireNoPullUpdate();
		requireNoPushUpdate();
		AND_GIVEN("output port O_0, compatible with AI_0")
		{
			rsp::OutputPort<int> O_0{"O_0"};
			WHEN("AI_0 and O_0 are connected")
			{
				AI_0.connectTo(O_0);
				requireNoPullUpdate();
				requireNoPushUpdate();
				AND_WHEN("O_0's value is set")
				{
					*O_0 = 0;
					requirePullUpdate();
					requirePushUpdate();
				}
			}
		}
	}
}

TEST_CASE("Cycles are ignored", "[nodes], [ports]")
{
	GIVEN("Nodes A and B, connected to each other in a cycle")
	{
		TestNode A;
		TestNode B;
		A.input0.connectTo(B.output0);
		A.output0.connectTo(B.input0);

		auto cycleTraversedOnce = [&]() {
			THEN("the cycle is traversed once, with no update calls")
			{
				REQUIRE(A.ctUpdateCalled == 0);
				REQUIRE(B.ctUpdateCalled == 0);
			}
		};
		WHEN("calling 'pull' on A")
		{
			A.pull();
			cycleTraversedOnce();
		}
		WHEN("calling 'push' on A")
		{
			A.push();
			cycleTraversedOnce();
		}
		WHEN("calling 'pull' on B")
		{
			B.pull();
			cycleTraversedOnce();
		}
		WHEN("calling 'push' on B")
		{
			B.push();
			cycleTraversedOnce();
		}
		WHEN("calling 'push' and 'pull' on A and B multiple times in any order")
		{
			B.push();
			A.push();
			B.push();
			A.push();
			B.push();
			A.pull();
			B.pull();
			A.push();
			A.pull();
			B.pull();
			B.pull();
			A.pull();
			cycleTraversedOnce();
		}
	}
}