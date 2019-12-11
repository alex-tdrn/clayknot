#include "catch.hpp"
#include "rsp/base/node/Node.hpp"

#include <array>
#include <utility>

class TestSource : public rsp::Source<TestSource, rsp::OutputList<std::string>>
{
public:
	struct OutputPorts
	{
		static inline std::array names = {"Message"};
		enum
		{
			Message,
		};
	};

public:
	static inline std::string const name = registerNode<TestSource>("Test Source");

private:
	std::string message = "No message";
	int updatesRan = 0;

protected:
	void update() override
	{
		getOutputData<OutputPorts::Message>() = message;
		updatesRan++;
	}

public:
	std::string const& getMessage() const
	{
		return message;
	}

	void setMessage(std::string message)
	{
		this->message = message;
	}

	int getUpdatesRan() const
	{
		return updatesRan;
	}
};

class TestSink : public rsp::Sink<TestSink, rsp::InputList<std::string>>
{
public:
	struct InputPorts
	{
		static inline std::array names = {"Message"};
		enum
		{
			Message,
		};
	};

public:
	static inline std::string const name = registerNode<TestSink>("Test Sink");
	static inline std::string const noMessageReceived = "No Message Received";

private:
	std::string message = noMessageReceived;
	int updatesRan = 0;

protected:
	void update() override
	{
		message = getInputData<InputPorts::Message>();
		updatesRan++;
	}

public:
	std::string const& getMessage() const
	{
		return message;
	}

	int getUpdatesRan() const
	{
		return updatesRan;
	}
};

class TestNode : public rsp::Node<TestNode, rsp::InputList<std::string>, rsp::OutputList<std::string, std::string>>
{
public:
	struct InputPorts
	{
		static inline std::array names = {"Message"};
		enum
		{
			Message,
		};
	};

	struct OutputPorts
	{
		static inline std::array names = {"Message1", "Message2"};
		enum
		{
			Message1,
			Message2,
		};
	};

public:
	static inline std::string const name = registerNode<TestNode>("Test Node");

private:
	std::string message = TestSink::noMessageReceived;
	int updatesRan = 0;

protected:
	void update() override
	{
		message = getInputData<InputPorts::Message>();
		getOutputData<OutputPorts::Message1>() = message;
		getOutputData<OutputPorts::Message2>() = message;
		updatesRan++;
	}

public:
	std::string const& getMessage() const
	{
		return message;
	}

	int getUpdatesRan() const
	{
		return updatesRan;
	}
};

TEMPLATE_TEST_CASE("base::nodes::Node registration", "", TestSource, TestSink, TestNode)
{
	GIVEN("Node type '" + TestType::name + "'")
	{
		THEN("it is registered in the node map")
		{
			auto& nodeMap = rsp::AbstractNode::getNodeMap();
			REQUIRE(nodeMap.find(TestType::name) != nodeMap.end());
			AND_THEN("it is constructible from the nodemap factory")
			{
				std::unique_ptr<rsp::AbstractNode> node = nullptr;
				REQUIRE_NOTHROW(node = std::move(rsp::AbstractNode::createNode(TestType::name)));
				REQUIRE(node != nullptr);
				AND_THEN("its name is available from the base class")
				{
					REQUIRE(TestType::name == node->getName());
					if constexpr(std::is_base_of_v<rsp::AbstractSink, TestType>)
					{
						AND_THEN("its input data ports are registered correctly")
						{
							auto sink = dynamic_cast<rsp::AbstractSink*>(node.get());
							auto ports = sink->getInputDataPorts();
							REQUIRE(ports.size() == TestType::InputPorts::names.size());
							for(int i = 0; i < ports.size(); i++)
								REQUIRE(ports[i]->getName() == TestType::InputPorts::names[i]);
						}
					}

					if constexpr(std::is_base_of_v<rsp::AbstractSource, TestType>)
					{
						AND_THEN("its output data ports are registered correctly")
						{
							auto source = dynamic_cast<rsp::AbstractSource*>(node.get());
							auto ports = source->getOutputDataPorts();
							REQUIRE(ports.size() == TestType::OutputPorts::names.size());
							for(int i = 0; i < ports.size(); i++)
								REQUIRE(ports[i]->getName() == TestType::OutputPorts::names[i]);
						}
					}
				}
			}
		}
	}
}

TEST_CASE("base::nodes::Graph update behaviour")
{
	GIVEN("Source A with one output port, AO, and update event AU")
	{
		TestSource A;
		A.setMessage("This message comes from A");
		auto& AO = A.getOutputDataPort<TestSource::OutputPorts::Message>();
		auto& AU = A.getOutputEventPort(rsp::AbstractNode::OutputEvents::Updated);

		AND_GIVEN("test sink B with one input port, BI, and update event BU")
		{
			TestSink B;
			auto& BI = B.getInputDataPort<TestSink::InputPorts::Message>();
			auto& BU = B.getOutputEventPort(rsp::AbstractNode::OutputEvents::Updated);

			WHEN("BI is connected to AO")
			{
				BI.connect(&AO);
				AND_WHEN("B is ran")
				{
					B.run();
					THEN("AU and BU are triggered once")
					{
						REQUIRE(AU.getTimesTriggered() == 1);
						REQUIRE(BU.getTimesTriggered() == 1);
						AND_THEN("A and B's update methods are ran once")
						{
							REQUIRE(A.getUpdatesRan() == 1);
							REQUIRE(B.getUpdatesRan() == 1);
							AND_THEN("BI's value is equal to AO's value")
							{
								REQUIRE(B.getMessage() == A.getMessage());
							}
						}
					}
				}
			}
			AND_GIVEN("test node C with one input port, CI and two output ports, CO1 and CO2, and update event CU")
			{
				TestNode C;
				auto& CI = C.getInputDataPort<TestNode::InputPorts::Message>();
				auto& CO1 = C.getOutputDataPort<TestNode::OutputPorts::Message1>();
				auto& CO2 = C.getOutputDataPort<TestNode::OutputPorts::Message2>();
				auto& CU = C.getOutputEventPort(rsp::AbstractNode::OutputEvents::Updated);

				WHEN("AO is connected to CI, CO1 is connected to BI")
				{
					AO.connect(&CI);
					CO1.connect(&BI);
					AND_WHEN("C is ran")
					{
						C.run();
						THEN("AU and CU are triggered once, but BU is not triggered")
						{
							REQUIRE(AU.getTimesTriggered() == 1);
							REQUIRE(BU.getTimesTriggered() == 0);
							REQUIRE(CU.getTimesTriggered() == 1);
							AND_THEN("A and C's update methods are ran once, but not B's")
							{
								REQUIRE(A.getUpdatesRan() == 1);
								REQUIRE(B.getUpdatesRan() == 0);
								REQUIRE(C.getUpdatesRan() == 1);
								AND_THEN("CI's value is equal to AO's value")
								{
									REQUIRE(C.getMessage() == A.getMessage());
								}
							}
						}
					}
					AND_WHEN("B is ran")
					{
						B.run();
						THEN("AU, BU and CU are triggered once")
						{
							REQUIRE(AU.getTimesTriggered() == 1);
							REQUIRE(BU.getTimesTriggered() == 1);
							REQUIRE(CU.getTimesTriggered() == 1);
							AND_THEN("A, B and C's update methods are ran once")
							{
								REQUIRE(A.getUpdatesRan() == 1);
								REQUIRE(B.getUpdatesRan() == 1);
								REQUIRE(C.getUpdatesRan() == 1);
								AND_THEN("CI's value is equal to AO's value")
								{
									REQUIRE(C.getMessage() == A.getMessage());
									AND_THEN("BI's value is equal to CO1's value")
									{
										REQUIRE(B.getMessage() == C.getMessage());
										AND_THEN("BI's value is equal to AO's value")
										{
											REQUIRE(B.getMessage() == A.getMessage());
										}
									}
								}
							}
						}
					}
					AND_GIVEN("test sink D with one input port DI and an update event DU")
					{
						TestSink D;
						auto& DI = D.getInputDataPort<TestSink::InputPorts::Message>();
						auto& DU = D.getOutputEventPort(rsp::AbstractNode::OutputEvents::Updated);

						WHEN("DI is connected to CO2")
						{
							DI.connect(&CO2);
							AND_WHEN("B is ran")
							{
								B.run();
								THEN("AU, BU and CU are triggered once, but DU is not triggered")
								{
									REQUIRE(AU.getTimesTriggered() == 1);
									REQUIRE(BU.getTimesTriggered() == 1);
									REQUIRE(CU.getTimesTriggered() == 1);
									REQUIRE(DU.getTimesTriggered() == 0);
									AND_THEN("A, B and C's update methods are ran once, but not D's update method")
									{
										REQUIRE(A.getUpdatesRan() == 1);
										REQUIRE(B.getUpdatesRan() == 1);
										REQUIRE(C.getUpdatesRan() == 1);
										REQUIRE(D.getUpdatesRan() == 0);
										AND_THEN("CI's value is equal to AO's value")
										{
											REQUIRE(C.getMessage() == A.getMessage());
											AND_THEN("BI's value is equal to CO1's value")
											{
												REQUIRE(B.getMessage() == C.getMessage());
												AND_THEN("BI's value is equal to AO's value")
												{
													REQUIRE(B.getMessage() == A.getMessage());
													AND_WHEN("D is ran")
													{
														D.run();
														THEN("only DU is triggered again")
														{
															REQUIRE(AU.getTimesTriggered() == 1);
															REQUIRE(BU.getTimesTriggered() == 1);
															REQUIRE(CU.getTimesTriggered() == 1);
															REQUIRE(DU.getTimesTriggered() == 1);
															AND_THEN("only D's update method is ran")
															{
																REQUIRE(A.getUpdatesRan() == 1);
																REQUIRE(B.getUpdatesRan() == 1);
																REQUIRE(C.getUpdatesRan() == 1);
																REQUIRE(D.getUpdatesRan() == 1);
																AND_THEN("CI's value is still equal to AO's value")
																{
																	REQUIRE(C.getMessage() == A.getMessage());
																	AND_THEN("DI's value is equal to CO2's value")
																	{
																		REQUIRE(D.getMessage() == C.getMessage());
																		AND_THEN("DI's value is equal to AO's value")
																		{
																			REQUIRE(D.getMessage() == A.getMessage());
																		}
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}
