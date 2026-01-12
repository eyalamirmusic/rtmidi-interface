#include "Strings.h"
#include <iostream>

namespace RTMIDI
{
void LOG(const std::string& text)
{ std::cout << text << std::endl; }

void LOG(const StringVec& vec)
{
    for (auto& e: vec)
        LOG(e);
}

bool contains(const std::string& text, const std::string& subString)
{ return text.find(subString) != std::string::npos; }

bool containsCaseInsensitive(std::string text, std::string subString)
{
    auto toLower = [](unsigned char c) { return std::tolower(c); };
    std::transform(text.begin(), text.end(), text.begin(), toLower);
    std::transform(subString.begin(), subString.end(), subString.begin(), toLower);

    return text.find(subString) != std::string::npos;
}
int findFirstContains(const StringVec& vec, const std::string& text)
{
    int index = 0;

    for (auto& element: vec)
    {
        if (contains(element, text))
            return index;

        ++index;
    }

    return -1;
}

int findFirstContainsIgnoreCase(const StringVec& vec, const std::string& text)
{
    int index = 0;

    for (auto& element: vec)
    {
        if (containsCaseInsensitive(element, text))
            return index;

        ++index;
    }

    return -1;
}
} // namespace RTMIDI