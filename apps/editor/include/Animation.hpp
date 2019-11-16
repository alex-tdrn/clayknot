#pragma once
#include <chrono>

enum class AnimationCurve
{
	linear,
	spring,
};

template<typename T>
class Animation
{
private:
	mutable bool playing = false;
	std::chrono::steady_clock::time_point startTime;

public:
	Animation() = default;
	Animation(Animation&&) = default;
	Animation(Animation const&) = default;
	Animation& operator=(Animation&&) = default;
	Animation& operator=(Animation const&) = default;
	~Animation() = default;

private:
	T static inline linear(T const& startValue, T const& endValue, float p)
	{
		T difference = endValue - startValue;
		return startValue + p * difference;
	}

	T static inline spring(T const& startValue, T const& endValue, float p)
	{
		float f = std::sin(p * 10 * 3.141) / (p + 1); 
		T difference = endValue - startValue;
		return endValue - f * difference;
	}

public:
	void play()
	{
		startTime = std::chrono::steady_clock::now();
		playing = true;
	}

	T get(T startValue, T endValue, std::chrono::milliseconds const& duration, AnimationCurve curveType) const
	{
		if(!playing)
			return endValue;
		auto const endTime = startTime + duration;
		auto const now = std::chrono::steady_clock::now();
		if(now > endTime)
		{
			playing = false;
			return endValue;
		}
		auto currentDuration = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime);

		float p = double(currentDuration.count()) / double(duration.count());

		switch(curveType)
		{
			case AnimationCurve::linear:
				return linear(startValue, endValue, p);
			case AnimationCurve::spring:
				return spring(startValue, endValue, p);
		}
		return endValue;
	}

};