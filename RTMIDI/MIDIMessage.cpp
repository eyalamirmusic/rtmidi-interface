#include "MIDIMessage.h"

namespace MIDI
{

std::string Message::toString() const
{
    if (auto n = getAs<NoteOn>())
    {
        return std::string("Note On ") + "Channel:" + std::to_string(n->channel)
               + " Note:" + std::to_string(n->note)
               + " Vel: " + std::to_string(n->velocity);
    }
    if (auto n = getAs<NoteOff>())
    {
        return std::string("Note Off ") + "Channel:" + std::to_string(n->channel)
               + " Note:" + std::to_string(n->note)
               + " Vel: " + std::to_string(n->velocity);
    }
    if (auto n = getAs<ControlChange>())
    {
        return std::string("Control Change") + "Channel:"
               + std::to_string(n->channel) + " CC:" + std::to_string(n->controller)
               + " Val: " + std::to_string(n->value);
    }

    return "Unkown";
}

MessageData parseData(const RTMIDI::RawData& bytes)
{
    if (bytes.empty() && bytes.size() > 3)
        return Unknown(bytes);

    uint8_t statusByte = bytes[0];

    if (statusByte >= 0xF8)
        return Unknown(bytes);

    auto command = statusByte & 0xF0;
    auto channel = statusByte & 0x0F;

    auto data1 = bytes[1];
    auto data2 = bytes[2];

    switch (command)
    {
        case 0x80:
            return NoteOff {channel, data1, data2};

        case 0x90:
            if (data2 == 0)
                return NoteOff {channel, data1, data2};

            return NoteOn {channel, data1, data2};

        case 0xB0:
            return ControlChange {channel, data1, data2};

        default:
            return Unknown(bytes);
    }
}
Message parse(const RTMIDI::RawData& bytes)
{
    auto m = Message();
    m.data = parseData(bytes);
    return m;
}
} // namespace MIDI