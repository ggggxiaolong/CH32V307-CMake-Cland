#include "stringView.hpp"

#include <vector>

using namespace ymd;

std::vector<StringView> StringView::split(const char delimiter, const size_t max_pieces) const {
    std::vector<StringView> strs;
    size_t from = 0;
    for (size_t i = 0; i < this->length(); i++) {
        if ((i == 0 || data_[i - 1] == delimiter) and data_[i] != delimiter) {
            from = i;
        }

        if (data_[i] != delimiter and (i + 1 == length() or data_[i + 1] == delimiter or data_[i + 1] == '\0')) {
            strs.push_back(this->substr(from, i + 1));
        }

        if ((max_pieces != 0) and (strs.size() == max_pieces)) break;
    }

    return strs;
}

std::optional<size_t> StringView::find_from(char ch, size_t from) const {
    if (from >= size_) return std::nullopt;
    for (size_t i = from; i < size_; i++) {
        if (data_[i] == ch) return i;
    }
    return std::nullopt;
}

std::optional<size_t> StringView::find(char c) const { return find_from(c, 0); }