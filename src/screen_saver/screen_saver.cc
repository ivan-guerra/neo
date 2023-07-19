#include "screen_saver/screen_saver.hpp"

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

std::size_t ScreenSaver::GetRandomNumInRange(int lower_bound,
                                             int upper_bound) const {
    std::random_device rd;  /* obtain a random number from hardware */
    std::mt19937 gen(rd()); /* seed the generator */
    std::uniform_int_distribution<> distr(lower_bound,
                                          upper_bound); /* define the range */
    return distr(gen);
}

char ScreenSaver::GetRandomBinDigit() const {
    static const std::string kBinDigits("01");
    return kBinDigits.at(
        GetRandomNumInRange(0, static_cast<int>(kBinDigits.size()) - 1));
}

Rgb ScreenSaver::GetColor(std::size_t stream_idx) const {
    using Range = std::pair<int, int>;
    const Range kCharRange(0, height_ / 2);
    const Range kColorRange(0, 255);

    int slope = (kColorRange.second - kColorRange.first) /
                (kCharRange.second - kCharRange.first);
    auto GetRgbComponent = [&slope](int input) { return slope * input; };

    return {.red = 0,
            .green = GetRgbComponent(streams_[stream_idx].Size()),
            .blue = 0};
}

void ScreenSaver::InsertChar(std::size_t stream_idx) {
    Char new_char = {
        .c = GetRandomBinDigit(),
        .color = GetColor(stream_idx),
    };
    streams_[stream_idx].InsertChar(new_char);
}

void ScreenSaver::ScrollScreen() {
    for (std::size_t i = 0; i < streams_.size(); ++i) {
        streams_[i].RemoveChar();
    }
}

ScreenSaver::ScreenSaver(int width, int height)
    : width_(width),
      height_(height),
      streams_(width, CharStream(height, height / 2)) {}

void ScreenSaver::Update() {
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

CharBuffer ScreenSaver::GetBuffer() const {
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
