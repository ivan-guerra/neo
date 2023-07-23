#include "screen_buffer/screen_buffer.hpp"

#include <gtest/gtest.h>

static const int kStreamCapacity = 10;
static const int kStreamCharLimit = 5;
static const neo::Char kTestChar = {.c = '$', .first = false};

TEST(ScreenBufferTests, CharStreamConstructorSetsParamCapacityAndCharLimit) {
    neo::CharStream stream(kStreamCapacity, kStreamCharLimit);

    ASSERT_EQ(stream.Capacity(), kStreamCapacity);
    ASSERT_EQ(stream.CharLimit(), kStreamCharLimit);
}

TEST(ScreenBufferTests, EmptyReturnsTrueWhenStreamIsEmpty) {
    neo::CharStream stream(kStreamCapacity, kStreamCharLimit);

    ASSERT_TRUE(stream.Empty());
}

TEST(ScreenBufferTests, EmptyReturnsFalseWhenStreamIsNonEmpty) {
    neo::CharStream stream(kStreamCapacity, kStreamCharLimit);
    stream.InsertChar(kTestChar);

    ASSERT_FALSE(stream.Empty());
}

TEST(ScreenBufferTests, SizeReturnsNumberOfNonNullCharsInStream) {
    neo::CharStream stream(kStreamCapacity, kStreamCharLimit);
    int i = 0;
    do {
        ASSERT_EQ(i, stream.Size());
        stream.InsertChar(kTestChar);
        i++;
    } while (i < kStreamCharLimit);
}

TEST(ScreenBufferTests, InsertCharInsertsCharAtTheTopOfTheStream) {
    neo::CharStream stream(kStreamCapacity, kStreamCharLimit);
    stream.InsertChar(kTestChar);

    ASSERT_EQ(kTestChar.c, stream[0].c);
    ASSERT_EQ(kTestChar.first, stream[0].first);
}

TEST(ScreenBufferTests, InsertCharDoesNotInsertBeyondCharLimit) {
    neo::CharStream stream(kStreamCapacity, kStreamCharLimit);
    for (int i = 0; i < kStreamCharLimit; ++i) {
        stream.InsertChar(kTestChar);
    }

    ASSERT_EQ(stream.Size(), stream.CharLimit());

    const neo::Char kOverflowChar = {.c = '%', .first = !kTestChar.first};
    stream.InsertChar(kOverflowChar);

    ASSERT_EQ(stream.Size(), stream.CharLimit());
    ASSERT_NE(kOverflowChar.c, stream[0].c);
    ASSERT_NE(kOverflowChar.first, stream[0].first);
}

TEST(ScreenBufferTests, RemoveCharRemovesCharAtTheBottomOfTheCharStream) {
    neo::CharStream stream(kStreamCapacity, kStreamCharLimit);

    stream.InsertChar(kTestChar); /* add a single non NULL char */
    ASSERT_EQ(stream.Size(), 1);

    /* if RemoveChar is working, this code will remove kStreamCapacity - 1 NULL
     * Chars from the stream leaving the non NULL char at the very end of the
     * stream */
    for (int i = 0; i < kStreamCapacity - 1; ++i) {
        stream.RemoveChar();
    }
    ASSERT_EQ(stream.Size(), 1);
    ASSERT_EQ(stream[kStreamCapacity - 1].c, kTestChar.c);
    ASSERT_EQ(stream[kStreamCapacity - 1].first, kTestChar.first);

    /* a final call to RemoveChar() deletes the non NULL char leaving the stream
     * empty */
    stream.RemoveChar();
    ASSERT_TRUE(stream.Empty());
}
