#pragma once

#include <cassert>
#include <glm/glm.hpp>

namespace rshp::base
{
class ColorRGB
{
private:
	glm::vec3 values = {0.0f, 0.0f, 0.0f};

public:
	ColorRGB() = default;
	explicit ColorRGB(glm::vec3 v) : values(v)
	{
	}
	explicit ColorRGB(float v) : values(v)
	{
	}
	explicit ColorRGB(float r, float g, float b) : values(r, g, b)
	{
	}
	explicit ColorRGB(int r, int g, int b) : values(r / 255.0f, g / 255.0f, b / 255.0f)
	{
	}
	ColorRGB(ColorRGB const&) = default;
	ColorRGB(ColorRGB&&) = default;
	ColorRGB& operator=(ColorRGB const& that) = default;
	ColorRGB& operator=(ColorRGB&& that) = default;
	~ColorRGB() = default;

public:
	bool valid() const
	{
		for(int i = 0; i <= 2; i++)
			if(values[i] < 0)
				return false;
		return true;
	}

	template <typename T>
	ColorRGB& operator+=(T const& that)
	{
		this->values += that;
		assert(valid());
		return *this;
	}

	ColorRGB& operator+=(ColorRGB const& that)
	{
		this->values += that.values;
		assert(valid());
		return *this;
	}

	template <typename T>
	ColorRGB& operator-=(T const& that)
	{
		this->values -= that;
		assert(valid());
		return *this;
	}

	ColorRGB& operator-=(ColorRGB const& that)
	{
		this->values -= that.values;
		assert(valid());
		return *this;
	}

	template <typename T>
	ColorRGB& operator*=(T const& that)
	{
		this->values *= that;
		assert(valid());
		return *this;
	}

	ColorRGB& operator*=(ColorRGB const& that)
	{
		this->values *= that.values;
		assert(valid());
		return *this;
	}

	template <typename T>
	ColorRGB& operator/=(T const& that)
	{
		this->values /= that;
		assert(valid());
		return *this;
	}

	ColorRGB& operator/=(ColorRGB const& that)
	{
		this->values /= that.values;
		assert(valid());
		return *this;
	}

	float* data()
	{
		return &values.r;
	}

	float r() const
	{
		return values.r;
	}

	float g() const
	{
		return values.g;
	}

	float b() const
	{
		return values.b;
	}

	float operator[](std::size_t i) const
	{
		assert(i <= 2);
		return values[i];
	}

	glm::vec3 vector() const
	{
		return values;
	}
};

ColorRGB inline operator+(ColorRGB const& lhs, ColorRGB const& rhs)
{
	ColorRGB result{lhs};
	result += rhs;
	return result;
}

template <typename T>
ColorRGB operator+(ColorRGB const& lhs, T const& rhs)
{
	ColorRGB result{lhs};
	result += rhs;
	return result;
}

template <typename T>
ColorRGB operator+(T const& lhs, ColorRGB const& rhs)
{
	ColorRGB result{rhs};
	result += lhs;
	return result;
}

ColorRGB inline operator-(ColorRGB const& lhs, ColorRGB const& rhs)
{
	ColorRGB result{lhs};
	result -= rhs;
	return result;
}

template <typename T>
ColorRGB operator-(ColorRGB const& lhs, T const& rhs)
{
	ColorRGB result{lhs};
	result -= rhs;
	return result;
}

ColorRGB inline operator*(ColorRGB const& lhs, ColorRGB const& rhs)
{
	ColorRGB result{lhs};
	result *= rhs;
	return result;
}

template <typename T>
ColorRGB operator*(ColorRGB const& lhs, T const& rhs)
{
	ColorRGB result{lhs};
	result *= rhs;
	return result;
}

template <typename T>
ColorRGB operator*(T const& lhs, ColorRGB const& rhs)
{
	ColorRGB result{rhs};
	result *= lhs;
	return result;
}

ColorRGB inline operator/(ColorRGB const& lhs, ColorRGB const& rhs)
{
	ColorRGB result{lhs};
	result /= rhs;
	return result;
}

template <typename T>
ColorRGB operator/(ColorRGB const& lhs, T const& rhs)
{
	ColorRGB result{lhs};
	result /= rhs;
	return result;
}

} // namespace rshp::base