#include "clk/algorithms/text.hpp"

#include <algorithm>

namespace clk::algorithms
{
uppercase::uppercase()
{
    register_port(_in);
    register_port(_out);

    *_in.default_port() = "lowercase text";
}

void uppercase::update()
{
    *_out = *_in;
    std::transform(_out->begin(), _out->end(), _out->begin(), ::toupper);
}

lowercase::lowercase()
{
    register_port(_in);
    register_port(_out);

    *_in.default_port() = "UPPERCASE TEXT";
}

void lowercase::update()
{
    *_out = *_in;
    std::transform(_out->begin(), _out->end(), _out->begin(), ::tolower);
}

} // namespace clk::algorithms
