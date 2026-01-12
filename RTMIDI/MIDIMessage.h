#pragma once

#include <variant>
#include "Common.h"

namespace MIDI
{

struct NoteOn
{
    int channel = 0;
    int note = 0;
    int velocity = 0;
};

struct NoteOff
{
    int channel = 0;
    int note = 0;
    int velocity = 0;
};

struct ControlChange
{
    int channel = 0;
    int controller = 0;
    int value = 0;
};

struct PolyKeyPressure
{
    int channel = 0;
    int note = 0;
    int pressure = 0;
};

struct ProgramChange
{
    int channel = 0;
    int program = 0;
};

struct ChannelPressure
{
    int channel = 0;
    int pressure = 0;
};

struct PitchBend
{
    int channel = 0;
    int value = 0;
};

struct Sysex
{
    Sysex() = default;
    Sysex(const RTMIDI::RawData& in)
        : data(in)
    {
    }

    std::span<const unsigned char> data;
};

struct Unknown
{
    Unknown() = default;
    Unknown(const RTMIDI::RawData& in)
        : data(in)
    {
    }

    std::span<const unsigned char> data;
};

using MessageData = std::variant<NoteOn,
                                 NoteOff,
                                 ControlChange,
                                 PolyKeyPressure,
                                 ProgramChange,
                                 ChannelPressure,
                                 PitchBend,
                                 Sysex,
                                 Unknown>;

struct Message
{
    std::string toString() const;

    template <typename T>
    const T* getAs() const
    { return std::get_if<T>(&data); }

    MessageData data;
    double timeStamp = 0.0;
};

using Callback = std::function<void(const Message&)>;

MessageData parseData(const RTMIDI::RawData& bytes);
Message parse(const RTMIDI::RawData& bytes);

} // namespace MIDI