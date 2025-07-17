#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"
#include <iostream>

int main() {
    try {
        ConverterJSON json;
        std::cout << "Starting " << json.getName() << " version " << json.getVersion() << std::endl;
        InvertedIndex index;
        index.UpdateDocumentBase(json.GetTextDocuments());
        SearchServer server(index);

        auto answers = server.search(json.GetRequests());
        std::vector<std::vector<std::pair<int, float>>> formatted;
        for (const auto& group : answers) {
            std::vector<std::pair<int, float>> inner;
            for (const auto& rel : group) inner.emplace_back(rel.doc_id, rel.rank);
            formatted.push_back(inner);
        }
        json.putAnswers(formatted);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}