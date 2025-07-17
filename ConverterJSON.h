#pragma once
#include <string>
#include <vector>

class ConverterJSON {
public:
    ConverterJSON();

    std::vector<std::string> GetTextDocuments();
    int GetResponsesLimit();
    std::vector<std::string> GetRequests();
    void putAnswers(const std::vector<std::vector<std::pair<int, float>>>& answers);

    std::string getName();
    std::string getVersion();

private:
    std::string name;
    std::string version;
    int max_responses = 5;
};