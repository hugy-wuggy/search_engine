#include "SearchServer.h"
#include <sstream>
#include <map>
#include <algorithm>

SearchServer::SearchServer(InvertedIndex& idx) : _index(idx) {}

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input) {
    std::vector<std::vector<RelativeIndex>> result;
    for (const auto& query : queries_input) {
        std::istringstream iss(query);
        std::string word;
        std::map<std::string, std::vector<Entry>> word_entries;
        std::map<size_t, size_t> abs_relevance;

        while (iss >> word) {
            auto entries = _index.GetWordCount(word);
            for (const auto& entry : entries) {
                abs_relevance[entry.doc_id] += entry.count;
            }
        }

        if (abs_relevance.empty()) {
            result.emplace_back();
            continue;
        }

        size_t max_rel = std::max_element(abs_relevance.begin(), abs_relevance.end(),
            [](const auto& a, const auto& b) { return a.second < b.second; })->second;

        std::vector<RelativeIndex> rel;
        for (const auto& [doc_id, abs] : abs_relevance) {
            rel.push_back({doc_id, static_cast<float>(abs) / max_rel});
        }

        std::sort(rel.begin(), rel.end(), [](const auto& a, const auto& b) {
            return b.rank < a.rank;
        });

        result.push_back(rel);
    }
    return result;
}