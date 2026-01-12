#pragma once

#include <span>
#include <vector>
#include <functional>

namespace RTMIDI
{
using DataVec = std::vector<unsigned char>;
using RawData = std::span<unsigned char>;
}