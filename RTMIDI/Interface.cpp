#include "Interface.h"
#include "RTLib.h"

namespace RTMIDI
{
using StringVec = std::vector<std::string>;

Ports getPortNames(RtMidi& in)
{
    auto res = Ports();
    auto size = in.getPortCount();
    res.reserve(size);

    for (size_t index = 0; index < size; ++index)
    {
        auto port = PortInfo();
        port.name = in.getPortName(index);
        port.index = index;
        res.push_back(port);
    }

    return res;
}

static Ports getInputPortNames()
{
    auto m = RtMidiIn();
    return getPortNames(m);
}

static Ports getOutputPortNames()
{
    auto m = RtMidiOut();
    return getPortNames(m);
}

struct InputPort::Native
{
    ~Native() { close(); }
    void open(const PortInfo& info, const MIDI::Callback& cbToUse)
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
        input.openPort(info.index, info.name);
    }

    void close() { input.closePort(); }

    RtMidiIn input;
    MIDI::Callback cb;
};

void LOG(const Ports& ports)
{
    for (auto& port: ports)
        LOG(port.name);
}

InputPort::InputPort(const PortInfo& infoToUse, const MIDI::Callback& cb)
    : info(infoToUse)
{
    native = std::make_unique<Native>();
    open(cb);
}

InputPort::~InputPort() = default;
std::string InputPort::getName() const
{ return info.name; }

void InputPort::open(const MIDI::Callback& cb)
{ native->open(info, cb); }

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
    void open(const PortInfo& info) { out.openPort(info.index, info.name); }

    void close()
    {
        if (isOpen())
            out.closePort();
    }

    bool isOpen() const { return out.isPortOpen(); }

    RtMidiOut out;
};

OutputPort::OutputPort(const PortInfo& infoToUse)
    : info(infoToUse)
{
    native = std::make_unique<Native>();
    vec.reserve(200);

    open();
}

OutputPort::~OutputPort() = default;

void OutputPort::open()
{ native->open(info); }

void OutputPort::close() const
{ native->close(); }

bool OutputPort::isOpen() const
{ return native->isOpen(); }

void OutputPort::send(const MIDI::Message& m)
{
    MIDI::serialize(m, vec);
    native->out.sendMessage(&vec);
}

std::string OutputPort::getName() const
{ return info.name; }

PortList getPortList()
{
    auto res = PortList();

    res.inputs = getInputPortNames();
    res.outputs = getOutputPortNames();

    return res;
}
} // namespace RTMIDI