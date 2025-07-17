#include "InvertedIndex.h"
#include <sstream>
#include <map>
#include <thread>
#include <mutex>

std::mutex index_mutex;

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    docs = input_docs;
    freq_dictionary.clear();

    std::vector<std::thread> threads;

    for (size_t i = 0; i < docs.size(); ++i) {
        threads.emplace_back([this, i]() {
            std::map<std::string, size_t> local_word_count;
            std::istringstream ss(this->docs[i]);
            std::string word;
            while (ss >> word) {
                ++local_word_count[word];
            }

            std::lock_guard<std::mutex> guard(index_mutex);
            for (const auto& [word, count] : local_word_count) {
                freq_dictionary[word].push_back({i, count});
            }
        });
    }

    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) {
    if (freq_dictionary.count(word)) return freq_dictionary[word];
    return {};
}