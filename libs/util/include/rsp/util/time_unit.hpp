#pragma once

#include <array>
#include <chrono>
#include <string>

namespace rsp
{
using namespace std::chrono_literals;
using namespace std::chrono;

struct time_unit
{
	std::string suffix;
	short value;
	short range;

	template <typename Representation, typename Period>
	static auto extractHours(duration<Representation, Period> t) -> time_unit
	{
		return {"h", static_cast<short>(duration_cast<hours>(t).count()), 24};
	}

	template <typename Representation, typename Period>
	static auto extractMinutes(duration<Representation, Period> t) -> time_unit
	{
		auto unit = time_unit{};
		unit.suffix = "m";
		unit.range = 60;
		unit.value = static_cast<short>(duration_cast<minutes>(t).count() % unit.range);
		return unit;
	}

	template <typename Representation, typename Period>
	static auto extractSeconds(duration<Representation, Period> t) -> time_unit
	{
		auto unit = time_unit{};
		unit.suffix = "s";
		unit.range = 60;
		unit.value = static_cast<short>(duration_cast<seconds>(t).count() % unit.range);
		return unit;
	}

	template <typename Representation, typename Period>
	static auto extractMilliseconds(duration<Representation, Period> t) -> time_unit
	{
		auto unit = time_unit{};
		unit.suffix = "ms";
		unit.range = 1'000;
		unit.value = static_cast<short>(duration_cast<milliseconds>(t).count() % unit.range);
		return unit;
	}

	template <typename Representation, typename Period>
	static auto extractMicroseconds(duration<Representation, Period> t) -> time_unit
	{
		auto unit = time_unit{};
		unit.suffix = "us";
		unit.range = 1'000;
		unit.value = static_cast<short>(duration_cast<microseconds>(t).count() % unit.range);
		return unit;
	}

	template <typename Representation, typename Period>
	static auto extractNanoseconds(duration<Representation, Period> t) -> time_unit
	{
		auto unit = time_unit{};
		unit.suffix = "ns";
		unit.range = 1'000;
		unit.value = static_cast<short>(duration_cast<nanoseconds>(t).count() % unit.range);
		return unit;
	}

	template <typename Representation, typename Period>
	static auto decompose(duration<Representation, Period> t) -> std::array<time_unit, 6>
	{
		return {extractHours(t), extractMinutes(t), extractSeconds(t), extractMilliseconds(t), extractMicroseconds(t),
			extractNanoseconds(t)};
	}

	static auto compose(const std::array<time_unit, 6>& units) -> std::chrono::nanoseconds
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