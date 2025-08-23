#include "SearchServer.h"
#include <map>
#include <set>
#include <sstream>
#include <algorithm>
#include <thread>
#include <mutex>

using namespace std;

std::mutex results_mutex;

vector<vector<RelativeIndex>> SearchServer::search(const vector<string>& queries_input, size_t max_responses) {
    vector<vector<RelativeIndex>> results(queries_input.size());
    vector<thread> threads;

    auto process_query = [&](size_t i) {
        vector<RelativeIndex> result;
        handleRequest(queries_input[i], result, max_responses);
        lock_guard<mutex> lock(results_mutex);
        results[i] = move(result);
    };

    for (size_t i = 0; i < queries_input.size(); ++i)
        threads.emplace_back(process_query, i);

    for (auto& t : threads)
        t.join();

    return results;
}

void SearchServer::handleRequest(const string& request, vector<RelativeIndex>& result, size_t max_responses) {
    istringstream ss(request);
    string word;
    set<string> unique_words;
    while (ss >> word) unique_words.insert(word);

    map<size_t, float> doc_scores;

    for (const auto& w : unique_words) {
        auto entries = _index->getWordCount(w);
        for (const auto& entry : entries) {
            doc_scores[entry._doc_id] += entry._count;
        }
    }

    if (doc_scores.empty()) return;

    float max_score = max_element(doc_scores.begin(), doc_scores.end(),
        [](const auto& a, const auto& b) {
            return a.second < b.second;
        })->second;

    vector<RelativeIndex> temp;
    for (const auto& [doc_id, score] : doc_scores) {
        temp.push_back(RelativeIndex{doc_id, score / max_score});
    }

    sort(temp.begin(), temp.end(),
         [](const RelativeIndex& a, const RelativeIndex& b) {
             return a._rank > b._rank;
         });

    if (temp.size() > max_responses) temp.resize(max_responses);

    result = move(temp);
}