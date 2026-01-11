#include "Interface.h"
#include "RtMidi.h"

namespace RTMIDI
{


void printPorts()
{
    auto in = RtMidiIn();

    for (size_t index = 0; index < in.getPortCount(); ++index)
    {
        std::cout << in.getPortName(index) << std::endl;
    }
}
}