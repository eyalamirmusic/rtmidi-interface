#include "RTMIDI/Interface.h"

int main()
{
    auto ports = RTMIDI::getPortList();

    RTMIDI::LOG(ports.inputs);
    RTMIDI::LOG(ports.outputs);
}