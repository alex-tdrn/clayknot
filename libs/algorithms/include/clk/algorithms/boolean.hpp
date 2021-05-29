#pragma once

#include "clk/base/algorithm.hpp"

namespace clk::algorithms
{
class boolean_not final : public clk::algorithm_builder<boolean_not>
{
public:
	static constexpr std::string_view name = "NOT";

	boolean_not();

private:
	clk::input_of<bool> _a{"A"};
	clk::output_of<bool> _result{"Result"};

	void update() override;
};

class boolean_and final : public clk::algorithm_builder<boolean_and>
{
public:
	static constexpr std::string_view name = "AND";

	boolean_and();

private:
	clk::input_of<bool> _a{"A"};
	clk::input_of<bool> _b{"B"};
	clk::output_of<bool> _result{"Result"};

	void update() override;
};
class boolean_nand final : public clk::algorithm_builder<boolean_nand>
{
public:
	static constexpr std::string_view name = "NAND";

	boolean_nand();

private:
	clk::input_of<bool> _a{"A"};
	clk::input_of<bool> _b{"B"};
	clk::output_of<bool> _result{"Result"};

	void update() override;
};

class boolean_or final : public clk::algorithm_builder<boolean_or>
{
public:
	static constexpr std::string_view name = "OR";

	boolean_or();

private:
	clk::input_of<bool> _a{"A"};
	clk::input_of<bool> _b{"B"};
	clk::output_of<bool> _result{"Result"};

	void update() override;
};

class boolean_nor final : public clk::algorithm_builder<boolean_nor>
{
public:
	static constexpr std::string_view name = "NOR";

	boolean_nor();

private:
	clk::input_of<bool> _a{"A"};
	clk::input_of<bool> _b{"B"};
	clk::output_of<bool> _result{"Result"};

	void update() override;
};

class boolean_xor final : public clk::algorithm_builder<boolean_xor>
{
public:
	static constexpr std::string_view name = "XOR";

	boolean_xor();

private:
	clk::input_of<bool> _a{"A"};
	clk::input_of<bool> _b{"B"};
	clk::output_of<bool> _result{"Result"};

	void update() override;
};

class boolean_xnor final : public clk::algorithm_builder<boolean_xnor>
{
public:
	static constexpr std::string_view name = "XNOR";

	boolean_xnor();

private:
	clk::input_of<bool> _a{"A"};
	clk::input_of<bool> _b{"B"};
	clk::output_of<bool> _result{"Result"};

	void update() override;
};
} // namespace clk::algorithms