#include "screen_buffer/screen_buffer.hpp"

#include <random>
#include <string>
#include <vector>

namespace neo {

CharStream::CharStream(int capacity, int char_limit)
    : size_(0),
      char_limit_(char_limit),
      limit_reached_(false),
      chars_(capacity) {}

void CharStream::InsertChar(const Char& c) {
    /* if a stream has reached the character limit, we don't add characters
     * until it has been emptied, this policy prevents the screen from getting
     * too cluttered with characters  */
    if (limit_reached_) {
        return;
    }

    chars_.front() = c;

    size_++;
    if (size_ == char_limit_) {
        limit_reached_ = true;
    }
}

void CharStream::RemoveChar() {
    if (Empty()) {
        return;
    }

    if (chars_.back().c != '\0') { /* is this a non NULL Char */
        size_--;
        if (Empty()) {
            limit_reached_ = false;
        }
    }

    /* shift elements in the stream over to the right by one index */
    for (std::size_t i = chars_.size() - 1; i > 0; --i) {
        chars_[i] = chars_[i - 1];
    }

    const Char kNullChar;
    chars_.front() = kNullChar; /* clear first element */
}

std::size_t ScreenBuffer::GetRandomNumInRange(int lower_bound,
                                              int upper_bound) const {
    std::random_device rd;  /* obtain a random number from hardware */
    std::mt19937 gen(rd()); /* seed the generator */
    std::uniform_int_distribution<> distr(lower_bound,
                                          upper_bound); /* define the range */
    return distr(gen);
}

char ScreenBuffer::GetRandomBinDigit() const {
    static const std::string kBinDigits("01");
    return kBinDigits.at(
        GetRandomNumInRange(0, static_cast<int>(kBinDigits.size()) - 1));
}

void ScreenBuffer::InsertChar(std::size_t stream_idx) {
    Char new_char = {
        .c = GetRandomBinDigit(),
        .first = false,
    };

    if (streams_[stream_idx].Empty()) { /* is this the first non NULL char */
        new_char.first = true;
    }

    streams_[stream_idx].InsertChar(new_char);
}

void ScreenBuffer::ScrollScreen() {
    for (std::size_t i = 0; i < streams_.size(); ++i) {
        streams_[i].RemoveChar();
    }
}

ScreenBuffer::ScreenBuffer(int width, int height)
    : width_(width),
      height_(height),
      streams_(width, CharStream(height, height)) {}

void ScreenBuffer::Update() {
    ScrollScreen(); /* scroll the screen down a row */

    /* insert characters into any non empty streams */
    for (std::size_t i = 0; i < streams_.size(); ++i) {
        if (!streams_[i].Empty()) {
            InsertChar(i);
        }
    }

    /* randomly select a stream and append a char to it only if it is empty */
    std::size_t rand_idx =
        GetRandomNumInRange(0, static_cast<int>(streams_.size()) - 1);
    if (streams_.at(rand_idx).Empty()) {
        InsertChar(rand_idx);
    }
}

CharBuffer ScreenBuffer::GetBuffer() const {
    /* transpose streams_ so that we get a vertical scroll effect rather than a
     * horizontal one */
    CharBuffer buffer(height_, std::vector<Char>(width_));
    for (int i = 0; i < height_; ++i) {
        for (int j = 0; j < width_; ++j) {
            buffer[i][j] = streams_[j][i];
        }
    }
    return buffer;
}

}  // namespace neo
