#include "Strings.h"
#include <iostream>

namespace RTMIDI
{
void LOG(const std::string& text)
{
    std::cout << text << std::endl;
}

void LOG(const StringVec& vec)
{
    for (auto& e: vec)
        LOG(e);
}
} // namespace RTMIDI