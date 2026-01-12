#pragma once

#include "Strings.h"
#include "MIDIMessage.h"

namespace RTMIDI
{
struct PortList
{
    StringVec inputs;
    StringVec outputs;
};

struct InputPort
{
    InputPort();
    ~InputPort();

    void open(unsigned int index, const std::string& name, const MIDI::Callback& cb);
    void close() const;
    bool isOpen() const;

    struct Native;
    std::unique_ptr<Native> native;
};

PortList getPortList();
} // namespace RTMIDI
