#include "ostream.hpp"

#include <cstdint>
#include <ios>
#include <source_location>
#include <span>

#include "core/string/string.hpp"
#include "core/string/stringView.hpp"
#include "core/sys_defs.hpp"

using namespace ymd;

OutputStream& OutputStream::operator<<(std::ios_base& (*func)(std::ios_base&)) {
    do {
        if (func == &std::oct) {
            set_radix(8);
            break;
        }
        if (func == &std::hex) {
            set_radix(16);
            break;
        }
        if (func == &std::dec) {
            set_radix(10);
            break;
        }
        if (func == &std::fixed) {
            // TODO
            break;
        }
        // boolalpha 是 C++ 中的一个操纵符，用于设置流的格式标志，使得布尔值在输入/输出时以 true 或 false 的形式显示，而不是 1 或 0
        if (func == &std::boolalpha) {
            config_.boolalpha = true;
            break;
        }
        if (func == &std::noboolalpha) {
            config_.boolalpha = false;
            break;
        }
        // 使输出流在显示正数时自动加上正号（+）
        if (func == &std::showpos) {
            config_.showpos = true;
            break;
        }
        if (func == &std::noshowpos) {
            config_.showpos = false;
            break;
        }
        // 在输出整数时，显示进制前缀（如 0x 表示十六进制、0 表示八进制）
        if (func == &std::showbase) {
            config_.showbase = true;
            break;
        }
        if (func == &std::noshowbase) {
            config_.showbase = false;
            break;
        }
    } while (false);
    return *this;
}

#define PRINT_FLOAT_TEMPLATE(convfunc)                  \
    char str[12] = {0};                                 \
    const auto len = convfunc(value, str, this->eps()); \
    if (config_.showbase and value > 0) *this << '+';   \
    this->write(str, len);

int OutputStream::transform_char(const char chr) const {
    if (likely(!config_.flags)) return chr;
    if (unlikely(config_.nospace) and unlikely(chr == ' ')) return -1;
    if (unlikely(config_.no_brackets)) {
        switch (chr) {
            case '(':
            case ')':
            case '[':
            case ']':
            case '{':
            case '}':
            case '<':
            case '>':
                return -1;
            default:
                return chr;
        }
    }
    return chr;
}

void OutputStream::print_source_loc(const std::source_location& loc) {
    const auto guard = this->create_guard();
    this->println();
    this->set_splitter('\0');
    this->set_indent(this->indent());
    this->println(loc.function_name());
    this->println(loc.file_name(), '(', loc.line(), ':', loc.column(), ')');
}

void OutputStream::checked_write(const char* pbuf, const size_t len) {
    // Buf buf;
    for (size_t i = 0; i < len; i++) {
        const auto res = transform_char(pbuf[i]);
        if (likely(res) >= 0) {
            write(char(res));
        }
    }
}

OutputStream& OutputStream::operator<<(const float value) {
    PRINT_FLOAT_TEMPLATE(StringUtils::ftoa);
    return *this;
}

OutputStream& OutputStream::operator<<(const double value) {
    PRINT_FLOAT_TEMPLATE(StringUtils::ftoa);
    return *this;
}

#define PRINT_INT_TEMPLATE(blen, convfunc)                    \
    if (config_.showpos and val >= 0) this->write('+');       \
    if (config_.showbase and (radix() != 10)) {               \
        *this << get_basealpha(radix());                      \
    }                                                         \
    char str[blen];                                           \
    const auto len = convfunc(val, str, this->config_.radix); \
    this->write(str, len);

void OutputStream::print_int(const int val) { PRINT_INT_TEMPLATE(12, StringUtils::itoa); }
void OutputStream::print_int(const uint64_t val) { PRINT_INT_TEMPLATE(24, StringUtils::iutoa); }
void OutputStream::print_int(const int64_t val) { PRINT_INT_TEMPLATE(24, StringUtils::iltoa); }

#undef PUT_FLOAT_TEMPLATE

OutputStream& OutputStream::operator<<(const bool val) {
    if (config_.boolalpha == false) {
        write(val ? '1' : '0');
        return *this;
    } else {
        return *this << (val ? "true" : "false");
    }
}

OutputStream& OutputStream::flush() {
    buf_.flush([&](const std::span<const char> pbuf) { this->sendout(pbuf); });
    return *this;
}

void OutputStreamByRoute::sendout(const std::span<const char> pbuf) {
    if (!p_route_.has_value())
        while (true);
    p_route_->writeN(pbuf.data(), pbuf.size());
}

OutputStream& OutputStream::operator<<(const String& str) {
    checked_write(str.c_str(), str.length());
    return *this;
}
OutputStream& OutputStream::operator<<(const StringView& str) {
    checked_write(str.data(), str.length());
    return *this;
}