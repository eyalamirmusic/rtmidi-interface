#pragma once

#include "MIDIMessage.h"

namespace MIDI
{
enum class StatusType : uint8_t
{
    NoteOff = 0x80,
    NoteOn = 0x90,
    PolyKeyPressure = 0xA0,
    ControlChange = 0xB0,
    ProgramChange = 0xC0,
    ChannelPressure = 0xD0,
    PitchBend = 0xE0,
    System = 0xF0
};

std::string Message::toString() const
{
    if (auto n = getAs<NoteOn>())
    {
        return "Note On | Ch: " + std::to_string(n->channel) + " Note: "
               + std::to_string(n->note) + " Vel: " + std::to_string(n->velocity);
    }
    if (auto n = getAs<NoteOff>())
    {
        return "Note Off | Ch: " + std::to_string(n->channel) + " Note: "
               + std::to_string(n->note) + " Vel: " + std::to_string(n->velocity);
    }
    if (auto n = getAs<ControlChange>())
    {
        return "Control Change | Ch: " + std::to_string(n->channel) + " CC: "
               + std::to_string(n->controller) + " Val: " + std::to_string(n->value);
    }
    if (auto n = getAs<PolyKeyPressure>())
    {
        return "Poly Key Pressure | Ch: " + std::to_string(n->channel) + " Note: "
               + std::to_string(n->note) + " Val: " + std::to_string(n->pressure);
    }
    if (auto n = getAs<ProgramChange>())
    {
        return "Program Change | Ch: " + std::to_string(n->channel)
               + " Prog: " + std::to_string(n->program);
    }
    if (auto n = getAs<ChannelPressure>())
    {
        return "Channel Pressure | Ch: " + std::to_string(n->channel)
               + " Val: " + std::to_string(n->pressure);
    }
    if (auto n = getAs<PitchBend>())
    {
        return "Pitch Bend | Ch: " + std::to_string(n->channel)
               + " Val: " + std::to_string(n->value);
    }

    return "Unknown";
}

MessageData parseData(const RTMIDI::RawData& bytes)
{
    if (bytes.size() < 2)
        return Unknown(bytes);

    uint8_t statusByte = bytes[0];

    if (statusByte >= 0xF0)
        return Unknown(bytes);

    int command = statusByte & 0xF0;
    int channel = statusByte & 0x0F;

    int data1 = bytes[1];
    int data2 = (bytes.size() > 2) ? bytes[2] : 0;

    switch (static_cast<StatusType>(command))
    {
        case StatusType::NoteOff:

            if (bytes.size() < 3)
                return Unknown(bytes);

            return NoteOff {channel, data1, data2};

        case StatusType::NoteOn:

            if (bytes.size() < 3)
                return Unknown(bytes);

            if (data2 == 0)
                return NoteOff(channel, data1, data2);

            return NoteOn {channel, data1, data2};

        case StatusType::PolyKeyPressure:
            if (bytes.size() < 3)
                return Unknown(bytes);

            return PolyKeyPressure {channel, data1, data2};

        case StatusType::ControlChange:
            if (bytes.size() < 3)
                return Unknown(bytes);

            return ControlChange {channel, data1, data2};

        case StatusType::ProgramChange:
            return ProgramChange {channel, data1};

        case StatusType::ChannelPressure:
            return ChannelPressure {channel, data1};

        case StatusType::PitchBend: // Pitch Bend
        {
            if (bytes.size() < 3)
                return Unknown(bytes);

            int bendValue = (data2 << 7) | data1;
            return PitchBend {channel, bendValue};
        }

        case StatusType::System:
            return Sysex(bytes);
    }

    return Unknown(bytes);
}

void serialize(const Message& msg, RTMIDI::DataVec& outBytes)
{
    outBytes.clear();

    auto push = [&](int byte)
    { outBytes.push_back(static_cast<unsigned char>(byte)); };

    auto makeStatusByte = [&](StatusType type, int channel)
    { return static_cast<int>(type) | (channel & 0x0F); };

    if (auto noteOn = msg.getAs<NoteOn>())
    {
        push(makeStatusByte(StatusType::NoteOn, noteOn->channel));
        push(noteOn->note);
        push(noteOn->velocity);
    }
    else if (auto noteOff = msg.getAs<NoteOff>())
    {
        push(makeStatusByte(StatusType::NoteOff, noteOff->channel));
        push(noteOff->note);
        push(noteOff->velocity);
    }
    else if (auto cc = msg.getAs<ControlChange>())
    {
        push(makeStatusByte(StatusType::ControlChange, cc->channel));
        push(cc->controller);
        push(cc->value);
    }
    else if (auto polyKey = msg.getAs<PolyKeyPressure>())
    {
        push(makeStatusByte(StatusType::PolyKeyPressure, polyKey->channel));
        push(polyKey->note);
        push(polyKey->pressure);
    }
    else if (auto progChange = msg.getAs<ProgramChange>())
    {
        push(makeStatusByte(StatusType::ProgramChange, progChange->channel));
        push(progChange->program);
    }
    else if (auto chanPress = msg.getAs<ChannelPressure>())
    {
        push(makeStatusByte(StatusType::ChannelPressure, chanPress->channel));
        push(chanPress->pressure);
    }
    else if (auto pitchBend = msg.getAs<PitchBend>())
    {
        push(makeStatusByte(StatusType::PitchBend, pitchBend->channel));
        push(pitchBend->value & 0x7F);
        push((pitchBend->value >> 7) & 0x7F);
    }
    else if (auto unknown = msg.getAs<Unknown>())
    {
        outBytes.insert(outBytes.end(), unknown->data.begin(), unknown->data.end());
    }
}

Message parse(const RTMIDI::RawData& bytes)
{
    auto m = Message();
    m.data = parseData(bytes);
    return m;
}
} // namespace MIDI