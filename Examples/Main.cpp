#include "RTMIDI/Interface.h"
#include <thread>

int main()
{
    auto ports = RTMIDI::getPortList();

    auto input = RTMIDI::InputPort();

    if (ports.inputs.empty())
        return 0;

    auto cb = [](const MIDI::Message& m) { RTMIDI::LOG(m.toString()); };

    input.open(0, ports.inputs[0], cb);

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}