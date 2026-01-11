#include "Interface.h"
#include "RtMidi.h"

namespace RTMIDI
{
using StringVec = std::vector<std::string>;

template<typename T>
StringVec getPortNames()
{
    auto in = T();
    auto res = StringVec();
    auto size = in.getPortCount();
    res.reserve(size);

    for (size_t index = 0; index < size; ++index)
        res.push_back(in.getPortName(index));

    return res;
}

static StringVec getInputPortNames()
{
    return getPortNames<RtMidiIn>();
}

static StringVec getOutputPortNames()
{
    return getPortNames<RtMidiOut>();
}

static void debug(const StringVec& vec)
{
    for (auto& e: vec)
        std::cout << e << std::endl;
}

void printPorts()
{
    debug(getInputPortNames());
    debug(getOutputPortNames());
}
} // namespace RTMIDI