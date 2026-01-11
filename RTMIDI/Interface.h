#pragma once

#include "Strings.h"

namespace RTMIDI
{
struct PortList
{
    StringVec inputs;
    StringVec outputs;
};

PortList getPortList();
} // namespace RTMIDI
