#pragma once

#include <array>
#include <chrono>
#include <string>

namespace rsp
{
using namespace std::chrono_literals;
using namespace std::chrono;

struct TimeUnit
{
	std::string suffix;
	short value;
	short range;

	template <typename Representation, typename Period>
	static auto extractHours(duration<Representation, Period> t) -> TimeUnit
	{
		return {"h", static_cast<short>(duration_cast<hours>(t).count()), 24};
	}

	template <typename Representation, typename Period>
	static auto extractMinutes(duration<Representation, Period> t) -> TimeUnit
	{
		const short range = 60;
		return {"m", static_cast<short>(duration_cast<minutes>(t).count() % range), range};
	}

	template <typename Representation, typename Period>
	static auto extractSeconds(duration<Representation, Period> t) -> TimeUnit
	{
		const short range = 60;
		return {"s", static_cast<short>(duration_cast<seconds>(t).count() % range), range};
	}

	template <typename Representation, typename Period>
	static auto extractMilliseconds(duration<Representation, Period> t) -> TimeUnit
	{
		const short range = 1'000;
		return {"ms", static_cast<short>(duration_cast<milliseconds>(t).count() % range), range};
	}

	template <typename Representation, typename Period>
	static auto extractMicroseconds(duration<Representation, Period> t) -> TimeUnit
	{
		const short range = 1'000;
		return {"us", static_cast<short>(duration_cast<microseconds>(t).count() % range), range};
	}

	template <typename Representation, typename Period>
	static auto extractNanoseconds(duration<Representation, Period> t) -> TimeUnit
	{
		const short range = 1'000;
		return {"ns", static_cast<short>(duration_cast<nanoseconds>(t).count() % range), range};
	}

	template <typename Representation, typename Period>
	static auto decompose(duration<Representation, Period> t) -> std::array<TimeUnit, 6>
	{
		return {extractHours(t), extractMinutes(t), extractSeconds(t), extractMilliseconds(t), extractMicroseconds(t),
			extractNanoseconds(t)};
	}

	static auto compose(const std::array<TimeUnit, 6>& units) -> std::chrono::nanoseconds
	{
		auto ret = std::chrono::nanoseconds(units[5].value) + std::chrono::microseconds(units[4].value) +
				   std::chrono::milliseconds(units[3].value) + std::chrono::seconds(units[2].value) +
				   std::chrono::minutes(units[1].value) + std::chrono::hours(units[0].value);
		if(ret.count() < 0)
			return 0ns;
		return ret;
	}
};

} // namespace rsp