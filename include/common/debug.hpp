#pragma once

#include <iostream>
#include <streambuf>
#include <string>

namespace hotel {
namespace debug {

namespace detail {
template <class CharT, class Fn, class Traits = std::char_traits<CharT>>
class basic_debug_buf : public std::basic_streambuf<CharT, Traits> {
    using string_type = std::basic_string<CharT, Traits>;
public:
    virtual ~basic_debug_buf() { sync(); };
protected:
    virtual typename Traits::int_type overflow(typename Traits::int_type c) {
        if (not Traits::eq_int_type(c, Traits::eof())) {
            buffer += Traits::to_char_type(c);
            if (c == Traits::to_int_type('\n')) {
                sync();
            }
        }
        return Traits::not_eof(c);
    };
    virtual int sync() {
        if (not buffer.empty()) {
            func(buffer);
        }
        buffer.clear();
        return 0;
    };
private:
    string_type buffer;
    Fn func;
};

template <class CharT>
class debug_log_functor {
    using string_type = std::basic_string<CharT>;
public:
    void operator() (string_type s) {
#ifndef NDEBUG
        std::cout << "Debug: " << s;
#else
#endif
    }
};

} // detail

using debug_streambuf = hotel::debug::detail::basic_debug_buf<char, hotel::debug::detail::debug_log_functor<char>>;
extern std::ostream out;

} // debug
} // hotel
