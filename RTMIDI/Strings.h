#pragma once

#include <string>
#include <vector>

namespace RTMIDI
{
using StringVec = std::vector<std::string>;

void LOG(const std::string& text);
void LOG(const StringVec& vec);

bool contains(const std::string& text, const std::string& subString);
bool containsCaseInsensitive(std::string text, std::string subString);

int findFirstContains(const StringVec& vec, const std::string& text);
int findFirstContainsIgnoreCase(const StringVec& vec, const std::string& text);
} // namespace RTMIDI