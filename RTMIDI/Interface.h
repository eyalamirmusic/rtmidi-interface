#pragma once

#include "Strings.h"
#include "MIDIMessage.h"

namespace RTMIDI
{
struct PortInfo
{
    operator bool() const { return !name.empty(); }

    unsigned int index = 0;
    std::string name;
};

using Ports = std::vector<PortInfo>;

inline PortInfo findFirstContains(const Ports& vec, const std::string& text)
{
    for (auto& port: vec)
    {
        if (contains(port.name, text))
            return port;
    }

    return {};
}

struct PortList
{
    Ports inputs;
    Ports outputs;
};

void LOG(const Ports& ports);

class InputPort
{
public:
    InputPort(const PortInfo& infoToUse, const MIDI::Callback& cb);
    ~InputPort();

    std::string getName() const;

private:
    void open(const MIDI::Callback& cb);
    void close() const;
    bool isOpen() const;

    PortInfo info;

    struct Native;
    std::unique_ptr<Native> native;
};

class OutputPort
{
public:
    OutputPort(const PortInfo& infoToUse);
    ~OutputPort();

    void send(const MIDI::Message& m);
    std::string getName() const;

private:
    void open();
    void close() const;
    bool isOpen() const;

    struct Native;
    std::unique_ptr<Native> native;

    PortInfo info;
    DataVec vec;
};

PortList getPortList();
} // namespace RTMIDI
