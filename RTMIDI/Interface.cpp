#include "Interface.h"
#include "RTLib.h"

namespace RTMIDI
{
using StringVec = std::vector<std::string>;

template <typename T>
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

PortList getPortList()
{
    auto res = PortList();

    res.inputs = getInputPortNames();
    res.outputs = getOutputPortNames();

    return res;
}
} // namespace RTMIDI