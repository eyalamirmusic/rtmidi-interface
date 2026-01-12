#include "RTMIDI/Interface.h"
#include <thread>

int main()
{
    auto ports = RTMIDI::getPortList();
    RTMIDI::LOG(ports.inputs);
    RTMIDI::LOG(ports.outputs);

    auto inputName = "Hammer 88";
    auto outputName = "Nord Stage 3";

    auto input = RTMIDI::findFirstContainsIgnoreCase(ports.inputs, inputName);
    auto output = RTMIDI::findFirstContainsIgnoreCase(ports.outputs, outputName);

    if (input >= 0 && output >= 0)
    {
        auto outputPort = RTMIDI::OutputPort(output, outputName);
        auto cb = [&](const MIDI::Message& m) { outputPort.send(m); };

        auto inputPort = RTMIDI::InputPort(input, inputName, cb);

        while (true)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    return 0;
}