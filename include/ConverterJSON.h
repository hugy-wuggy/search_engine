#pragma once
#include <string>
#include <vector>
#include "SearchServer.h"

class ConverterJSON {
    std::string _config_path;
    std::string _request_path;
    std::string _answers_path;

    std::string _name;
    std::string _version;
    int _max_responses;
    std::vector<std::string> _file_names;
public:
    ConverterJSON(const std::string& _config_path = "${CMAKE_BINARY_DIR}/config/config.json",
                  const std::string& _request_path = "${CMAKE_BINARY_DIR}/config/requests.json",
                  const std::string& _answers_path = "${CMAKE_BINARY_DIR}/config/answers.json"
                  );

    std::vector<std::string> getTextDocuments();

    int getResponsesLimit();

    std::vector<std::string> getRequests();

    void putAnswers(std::vector<std::vector<RelativeIndex>> answers);

    const std::string &getName() const;

    const std::string &getVersion() const;
};
