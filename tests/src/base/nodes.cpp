#include "clk/base/node.hpp"

#include <catch2/catch.hpp>
#include <chrono>
#include <string>
#include <unordered_set>

using namespace std::literals;

// FIXME rethink this whole test suite before merging to dev

// TEST_CASE("New node types can be implemented by inheriting from clk::Node", "[base], [nodes], [ports]")
// {
// 	class TestNode final : public clk::Node
// 	{
// 	private:
// 		clk::InputPortOf<int> input0{"I_0"};
// 		clk::OutputPortOf<int> output0{"O_0"};
// 		unsigned int ctUpdateCalled = 0;

// 	public:
// 		TestNode()
// 		{
// 			registerPort(input0);
// 			registerPort(output0);
// 			THEN("ports registered by the derived class, are accessible from the base class in the same order they "
// 				 "where "
// 				 "registered")
// 			{
// 				REQUIRE(&input0 == &getInputPort(0));
// 				REQUIRE(&output0 == &getOutputPort(0));
// 			}
// 		}

// 		void update() override
// 		{
// 			ctUpdateCalled++;
// 			*output0 = *input0;
// 		}

// 		auto getName() const -> std::string_view override
// 		{
// 			static const std::string name = "Test Node";
// 			return name;
// 		}

// 		void requirePullUpdates(unsigned int updateCount)
// 		{
// 			std::string updateText = (updateCount > 0) ? " does not call update()"
// 													   : "calls update() " + std::to_string(updateCount) + "times";
// 			THEN("calling pull on AO_0" + updateText)
// 			{
// 				auto ctOld = ctUpdateCalled;
// 				output0.pull();
// 				auto ctUpdates = ctUpdateCalled - ctOld;
// 				REQUIRE(ctUpdates == updateCount);
// 			}
// 		}

// 		void requirePushUpdates(unsigned int updateCount)
// 		{
// 			std::string updateText = (updateCount > 0) ? " does not call update()"
// 													   : "calls update() " + std::to_string(updateCount) + "times";
// 			THEN("calling push on AI_0" + updateText)
// 			{
// 				auto ctOld = ctUpdateCalled;
// 				input0.push();
// 				auto ctUpdates = ctUpdateCalled - ctOld;
// 				REQUIRE(ctUpdates == updateCount);
// 			}
// 		}
// 	};
// 	GIVEN("TestNode A, with one input port AI_0 and one output port AO_0 of the same datatype")
// 	{
// 		TestNode A;
// 		A.requirePullUpdates(0);
// 		A.requirePushUpdates(0);

// 		// AND_GIVEN("output port O_0, compatible with AI_0")
// 		// {
// 		// 	clk::OutputPort<int> O_0{"O_0"};
// 		// 	WHEN("AI_0 and O_0 are connected")
// 		// 	{
// 		// 		AI_0.connectTo(O_0);
// 		// 		requireNoPullUpdate();
// 		// 		requireNoPushUpdate();
// 		// 		AND_WHEN("O_0's value is set")
// 		// 		{
// 		// 			*O_0 = 0;
// 		// 			requirePullUpdate();
// 		// 			requirePushUpdate();
// 		// 		}
// 		// 	}
// 		// }
// 	}
// }

// // TEST_CASE("Cycles are ignored", "[base], [nodes], [ports]")
// // {
// // 	GIVEN("Nodes A and B, connected to each other in a cycle")
// // 	{
// // 		TestNode A;
// // 		TestNode B;
// // 		A.input0.connectTo(B.output0);
// // 		A.output0.connectTo(B.input0);

// // 		auto cycleTraversedOnce = [&]() {
// // 			THEN("the cycle is traversed once, with no update calls")
// // 			{
// // 				REQUIRE(A.ctUpdateCalled == 0);
// // 				REQUIRE(B.ctUpdateCalled == 0);
// // 			}
// // 		};
// // 		WHEN("calling 'pull' on A")
// // 		{
// // 			A.pull();
// // 			cycleTraversedOnce();
// // 		}
// // 		WHEN("calling 'push' on A")
// // 		{
// // 			A.push();
// // 			cycleTraversedOnce();
// // 		}
// // 		WHEN("calling 'pull' on B")
// // 		{
// // 			B.pull();
// // 			cycleTraversedOnce();
// // 		}
// // 		WHEN("calling 'push' on B")
// // 		{
// // 			B.push();
// // 			cycleTraversedOnce();
// // 		}
// // 		WHEN("calling 'push' and 'pull' on A and B multiple times in any order")
// // 		{
// // 			B.push();
// // 			A.push();
// // 			B.push();
// // 			A.push();
// // 			B.push();
// // 			A.pull();
// // 			B.pull();
// // 			A.push();
// // 			A.pull();
// // 			B.pull();
// // 			B.pull();
// // 			A.pull();
// // 			cycleTraversedOnce();
// // 		}
// // 	}
// // }