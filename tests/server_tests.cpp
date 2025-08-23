#include <gtest/gtest.h>
#include "InvertedIndex.h"
#include "SearchServer.h"
#include <cmath>
#include <vector>
#include <string>

static bool IsNearlyEqual(float a, float b, float eps = 1e-5f) {
    return std::fabs(a - b) < eps;
}

TEST(TestCaseSearchServer, TestSimple) {
    const std::vector<std::string> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuccino"
    };
    const std::vector<std::string> requests = {
        "milk water",
        "sugar"
    };

    InvertedIndex idx;
    idx.updateDocumentBase(docs);
    SearchServer srv(idx);

    auto result = srv.search(requests);

    ASSERT_EQ(result.size(), 2u);
    {
        auto& v = result[0];
        ASSERT_EQ(v.size(), 3u);
        EXPECT_EQ(v[0]._doc_id, 2);
        EXPECT_TRUE(IsNearlyEqual(v[0]._rank, 1.0f));
        EXPECT_EQ(v[1]._doc_id, 0);
        EXPECT_TRUE(IsNearlyEqual(v[1]._rank, 0.7f));
        EXPECT_EQ(v[2]._doc_id, 1);
        EXPECT_TRUE(IsNearlyEqual(v[2]._rank, 0.3f));
    }
    EXPECT_TRUE(result[1].empty());
}

TEST(TestCaseSearchServer, TestTop5Properties) {
    const std::vector<std::string> docs = {
        "london is the capital of great britain",
        "paris is the capital of france",
        "berlin is the capital of germany",
        "rome is the capital of italy",
        "madrid is the capital of spain",
        "lisboa is the capital of portugal",
        "bern is the capital of switzerland",
        "moscow is the capital of russia",
        "kiev is the capital of ukraine",
        "minsk is the capital of belarus",
        "astana is the capital of kazakhstan",
        "beijing is the capital of china",
        "tokyo is the capital of japan",
        "bangkok is the capital of thailand",
        "welcome to moscow the capital of russia the third rome",
        "amsterdam is the capital of netherlands",
        "helsinki is the capital of finland",
        "oslo is the capital of norway",
        "stockholm is the capital of sweden",
        "riga is the capital of latvia",
        "tallinn is the capital of estonia",
        "warsaw is the capital of poland"
    };
    const std::vector<std::string> requests = {
        "moscow is the capital of russia"
    };

    InvertedIndex idx;
    idx.updateDocumentBase(docs);
    SearchServer srv(idx);

    auto result = srv.search(requests);
    ASSERT_EQ(result.size(), 1u);

    const auto& v = result[0];
    ASSERT_EQ(v.size(), 5u);

    EXPECT_TRUE(
        (v[0]._doc_id == 7 && IsNearlyEqual(v[0]._rank, 1.0f) && v[1]._doc_id == 14 && IsNearlyEqual(v[1]._rank, 1.0f)) ||
        (v[0]._doc_id == 14 && IsNearlyEqual(v[0]._rank, 1.0f) && v[1]._doc_id == 7 && IsNearlyEqual(v[1]._rank, 1.0f))
    );

    for (size_t i = 2; i < 5; ++i) {
        EXPECT_TRUE(IsNearlyEqual(v[i]._rank, 0.6666667f))
            << "Unexpected rank at position " << i << ": " << v[i]._rank;
        EXPECT_NE(v[i]._doc_id, 7u);
        EXPECT_NE(v[i]._doc_id, 14u);
    }
}