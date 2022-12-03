#include <iostream>
#include <string>

#pragma once

namespace hotel {
namespace util {

struct line {
    std::string data;
    operator std::string() const { return data; };
};

} // util
} // hotel

inline std::istream& operator>>(std::istream& stream, hotel::util::line& line) {
    std::getline(stream, line.data);
    return stream;
}
