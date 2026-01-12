#include "Interface.h"
#include "RTLib.h"

namespace RTMIDI
{
using StringVec = std::vector<std::string>;

StringVec getPortNames(RtMidi& in)
{
    auto res = StringVec();
    auto size = in.getPortCount();
    res.reserve(size);

    for (size_t index = 0; index < size; ++index)
        res.push_back(in.getPortName(index));

    return res;
}

static StringVec getInputPortNames()
{
    auto m = RtMidiIn();
    return getPortNames(m);
}

static StringVec getOutputPortNames()
{
    auto m = RtMidiOut();
    return getPortNames(m);
}

struct InputPort::Native
{
    void open(unsigned int index, const std::string& name)
    {
        auto nativeCb = [](double ts, DataVec* vec, void* user)
        {
            if (vec != nullptr)
            {
                auto in = static_cast<Native*>(user);

                auto message = MIDI::parse(*vec);
                message.timeStamp = ts;
                in->cb(message);
            }
        };

        input.setCallback(nativeCb, this);
        input.openPort(index, name);
    }

    RtMidiIn input;
    MIDI::Callback cb;
};

InputPort::InputPort()
{ native = std::make_unique<Native>(); }

InputPort::~InputPort()
{ close(); }

void InputPort::open(unsigned int index,
                     const std::string& name,
                     const MIDI::Callback& cb)
{
    close();
    native->open(index, name);
    native->cb = [cb](auto& m) { cb(m); };
}

void InputPort::close() const
{
    if (isOpen())
        native->input.closePort();
}

bool InputPort::isOpen() const
{ return native->input.isPortOpen(); }

PortList getPortList()
{
    auto res = PortList();

    res.inputs = getInputPortNames();
    res.outputs = getOutputPortNames();

    return res;
}
} // namespace RTMIDI