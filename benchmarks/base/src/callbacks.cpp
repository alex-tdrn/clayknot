#include "rsp/base/port/InputEventPort.h"
#include "rsp/base/port/OutputEventPort.h"

#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include <catch2/catch.hpp>
#include <functional>
#include <vector>

static void static_function()
{
	volatile int a = 0;
}

class Callback
{
private:
	struct CallbackImplBase
	{
		virtual void operator()() = 0;
		virtual ~CallbackImplBase() = default;
	};

	template <typename F>
	struct CallbackImpl final : public CallbackImplBase
	{
		F callable;
		CallbackImpl(F&& f) : callable(std::forward<F>(f))
		{
		}
		void operator()() override
		{
			callable();
		}
	};

	std::unique_ptr<CallbackImplBase> f;

public:
	template <typename F>
	Callback(F&& f) : f(std::make_unique<CallbackImpl<F>>(std::forward<F>(f)))
	{
	}
	Callback(Callback&&) noexcept = default;
	Callback& operator=(Callback&&) noexcept = default;

	void operator()() const
	{
		f->operator()();
	}
};

TEST_CASE("Event triggering compared to other callback mechanisms")
{
	SECTION("Unicast")
	{
		BENCHMARK("Baseline")
		{
			static_function();
		};

		std::function f = static_function;

		BENCHMARK("STD Function")
		{
			f();
		};

		Callback c{static_function};

		BENCHMARK("Callback")
		{
			c();
		};

		rsp::InputEventPort A{static_function};

		BENCHMARK("Event port trigger")
		{
			A.trigger();
		};

		rsp::OutputEventPort B;
		B.connect(&A);

		BENCHMARK("Event port trigger 1 Hop")
		{
			B.trigger();
		};
	}

	SECTION("Multicast")
	{
		class TestClass
		{
		public:
			volatile int a;
			void modify()
			{
				a = 0;
			}
		};

		TestClass t;

		auto lambda1 = []() {
			static_function();
		};
		volatile int b = 0;

		auto lambda2 = [&]() {
			b = 0;
		};

		auto lambda3 = []() {
			volatile int a = 0;
		};

		auto lambda4 = [&]() {
			t.modify();
		};

		BENCHMARK("Baseline")
		{
			static_function();
			lambda1();
			lambda2();
			lambda3();
			lambda4();
		};

		std::vector<std::function<void()>> stdFunctions{static_function, lambda1, lambda2, lambda3, lambda4};

		BENCHMARK("STD Function")
		{
			for(auto& f : stdFunctions)
				f();
		};

		std::vector<Callback> callbacks;
		callbacks.emplace_back(static_function);
		callbacks.emplace_back(lambda1);
		callbacks.emplace_back(lambda2);
		callbacks.emplace_back(lambda3);
		callbacks.emplace_back(lambda4);

		BENCHMARK("Callback")
		{
			for(auto& c : callbacks)
				c();
		};

		std::vector<rsp::InputEventPort> eventPorts;
		eventPorts.emplace_back(static_function);
		eventPorts.emplace_back(lambda1);
		eventPorts.emplace_back(lambda2);
		eventPorts.emplace_back(lambda3);
		eventPorts.emplace_back(lambda4);

		BENCHMARK("Event port trigger")
		{
			for(auto& port : eventPorts)
				port.trigger();
		};

		rsp::OutputEventPort B;
		for(auto& port : eventPorts)
			B.connect(&port);

		BENCHMARK("Event port trigger 1 Hop")
		{
			B.trigger();
		};
	}
}
