#ifndef SCREEN_BUFFER_HPP_
#define SCREEN_BUFFER_HPP_

#include <vector>

namespace neo {

/** @brief Representation of an on-screen character. */
struct Char {
    char c = '\0';      /**< A printable ASCII character. */
    bool first = false; /**< True if this is the first non NULL Char in a column
                           of characters. */
};

/** @brief Representation of a single screen column. */
class CharStream {
   public:
    CharStream() = delete;

    /**
     * @brief Construct a stream with the parameter capacity and Char limit.
     *
     * @description Providing invalid \p char_limit and \p capacity values will
     *              cause undefined behavior.
     *
     * @param [in] capacity   The max number of combined NULL and non NULL Chars
     *                        supported.
     * @param [in] char_limit The max number of non NULL characters that can be
     *                        in the stream at any given time.
     */
    CharStream(int capacity, int char_limit);

    ~CharStream() = default;
    CharStream(const CharStream&) = default;
    CharStream& operator=(const CharStream&) = default;
    CharStream(CharStream&&) = default;
    CharStream& operator=(CharStream&&) = default;

    /** @brief Insert \p c at the top of the stream. */
    void InsertChar(const Char& c);

    /** @brief Remove the Char at the bottom of the stream. */
    void RemoveChar();

    /** @brief Returns true if the stream only contains NULL Chars. */
    bool Empty() const { return (size_ <= 0); }

    /** @brief Returns the number of non NULL Chars in the stream. */
    int Size() const { return size_; }

    /** @brief Returns the stream capacity. */
    int Capacity() const { return static_cast<int>(chars_.capacity()); }

    /** @brief Returns the Char limit set on this stream. */
    int CharLimit() const { return char_limit_; }

    /** @brief Returns the Char at index \p i of this CharStream. */
    const Char& operator[](int i) const { return chars_[i]; }

   private:
    int size_;
    int char_limit_;
    bool limit_reached_;
    std::vector<Char> chars_;
};

using CharBuffer = std::vector<std::vector<Char>>;

/**
 * @brief A vertically scrolling 2D screen buffer.
 *
 * @details ScreenBuffer acts as a scrolling buffer. Via the Update()
 *          method, a User can scroll streams of characters vertically across
 *          the screen. It's up to the User of ScreenBuffer to update and
 *          render the contents of the buffer using a graphics library.
 */
class ScreenBuffer {
   public:
    /**
     * @brief Construct a screen buffer with the parameter dimensions.
     *
     * @param [in] width  Screen width.
     * @param [in] height Screen height.
     */
    ScreenBuffer(int width, int height);
    ScreenBuffer() = delete;

    ~ScreenBuffer() = default;
    ScreenBuffer(const ScreenBuffer&) = default;
    ScreenBuffer& operator=(const ScreenBuffer&) = default;
    ScreenBuffer(ScreenBuffer&&) = default;
    ScreenBuffer& operator=(ScreenBuffer&&) = default;

    /**
     * @brief Scroll the screen down by a single row.
     *
     * @details Update() performs the following actions in sequence:
     *              -# Shifts all rows down by one. This effectively deletes
     *                  the very bottom row and introduces a new, empty top row.
     *              -# If a column of characters has not yet met its character
     *                  limit, Update() will insert a character at the very top
     *                  of that column.
     *              -# Update() will select a random column index and will
     *                  insert a character only if that column is empty.
     */
    void Update();

    /** @brief Return a copy of the current screen buffer. */
    CharBuffer GetBuffer() const;

   private:
    std::size_t GetRandomNumInRange(int lower_bound, int upper_bound) const;
    char GetRandomBinDigit() const;
    void InsertChar(std::size_t stream_idx);
    void ScrollScreen();

    int width_;
    int height_;
    std::vector<CharStream> streams_;
};

}  // namespace neo

#endif
