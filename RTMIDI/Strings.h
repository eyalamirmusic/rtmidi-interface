#pragma once

#include <string>
#include <vector>

namespace RTMIDI
{
using StringVec = std::vector<std::string>;

void LOG(const std::string& text);
void LOG(const StringVec& vec);
} // namespace RTMIDI