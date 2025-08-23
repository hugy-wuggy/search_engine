#include "InvertedIndex.h"
#include <sstream>
#include <mutex>

using namespace std;

void InvertedIndex::updateDocumentBase(const vector<string>& input_docs) {
    lock_guard<mutex> lock(_dictionary_mtx);
    freq_dictionary.clear();

    for (size_t doc_id = 0; doc_id < input_docs.size(); ++doc_id) {
        istringstream doc(input_docs[doc_id]);
        string word;
        while (doc >> word) {
            updateDocument(word, doc_id);
        }
    }
}

void InvertedIndex::updateDocument(const string& word, size_t doc_id) {
    freq_dictionary[word][doc_id]++;
}

vector<Entry> InvertedIndex::getWordCount(const string& word) {
    lock_guard<mutex> lock(_dictionary_mtx);
    vector<Entry> result;

    auto it = freq_dictionary.find(word);
    if (it != freq_dictionary.end()) {
        for (const auto& [doc_id, count] : it->second) {
            result.push_back(Entry(doc_id, count));
        }
    }
    return result;
}