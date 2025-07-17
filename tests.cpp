#include "InvertedIndex.h"
#include "SearchServer.h"
#include <gtest/gtest.h>

void TestInvertedIndexFunctionality(
    const std::vector<std::string>& docs,
    const std::vector<std::string>& requests,
    const std::vector<std::vector<Entry>>& expected
) {
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    for (size_t i = 0; i < requests.size(); ++i) {
        EXPECT_EQ(idx.GetWordCount(requests[i]), expected[i]);
    }
}

TEST(TestCaseInvertedIndex, TestBasic) {
    const std::vector<std::string> docs = {
        "london is the capital of great britain",
        "big ben is the nickname for the Great bell of the striking clock"
    };
    const std::vector<std::string> requests = {"london", "the"};
    const std::vector<std::vector<Entry>> expected = {
        {{0, 1}},
        {{0, 1}, {1, 3}}
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}