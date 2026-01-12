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

class InputPort
{
public:
    InputPort(unsigned int index, const std::string& name, const MIDI::Callback& cb);
    ~InputPort();

private:
    void open(unsigned int index, const std::string& name, const MIDI::Callback& cb);
    void close() const;
    bool isOpen() const;

    struct Native;
    std::unique_ptr<Native> native;
};

class OutputPort
{
public:
    OutputPort(unsigned int index, const std::string& name);
    ~OutputPort();

    void send(const MIDI::Message& m);

private:
    void open(unsigned int index, const std::string& name);
    void close() const;
    bool isOpen() const;

    struct Native;
    std::unique_ptr<Native> native;

    DataVec vec;
};

PortList getPortList();
} // namespace RTMIDI
