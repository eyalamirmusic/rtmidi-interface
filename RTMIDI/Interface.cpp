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
    ~Native() { close(); }
    void open(unsigned int index,
              const std::string& name,
              const MIDI::Callback& cbToUse)
    {
        cb = cbToUse;
        close();

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

    void close() { input.closePort(); }

    RtMidiIn input;
    MIDI::Callback cb;
};

InputPort::InputPort(unsigned int index,
                     const std::string& name,
                     const MIDI::Callback& cb)
{
    native = std::make_unique<Native>();
    open(index, name, cb);
}

InputPort::~InputPort() = default;

void InputPort::open(unsigned int index,
                     const std::string& name,
                     const MIDI::Callback& cb)
{ native->open(index, name, cb); }

void InputPort::close() const
{
    if (isOpen())
        native->input.closePort();
}

bool InputPort::isOpen() const
{ return native->input.isPortOpen(); }

struct OutputPort::Native
{
    ~Native() { close(); }
    void open(unsigned int index, const std::string& name)
    { out.openPort(index, name); }

    void close()
    {
        if (isOpen())
            out.closePort();
    }

    bool isOpen() const { return out.isPortOpen(); }

    RtMidiOut out;
};

OutputPort::OutputPort(unsigned int index, const std::string& name)
{
    native = std::make_unique<Native>();
    vec.reserve(200);

    open(index, name);
}

OutputPort::~OutputPort() = default;

void OutputPort::open(unsigned int index, const std::string& name)
{ native->open(index, name); }

void OutputPort::close() const
{ native->close(); }

bool OutputPort::isOpen() const
{ return native->isOpen(); }

void OutputPort::send(const MIDI::Message& m)
{
    MIDI::serialize(m, vec);
    native->out.sendMessage(&vec);
}

PortList getPortList()
{
    auto res = PortList();

    res.inputs = getInputPortNames();
    res.outputs = getOutputPortNames();

    return res;
}
} // namespace RTMIDI