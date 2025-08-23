#include <iostream>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

using namespace std;

int main() {
    try {
        ConverterJSON json("config.json", "requests.json", "answers.json");
        cout << "Start " << json.getName() << " [" << json.getVersion() << "]" << endl;

        InvertedIndex index;
        index.updateDocumentBase(json.getTextDocuments());

        SearchServer server(index);
        auto results = server.search(json.getRequests(), json.getResponsesLimit());

        json.putAnswers(results);

        cout << "Done." << endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}