#include "ConverterJSON.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <thread>
#include <mutex>

using json = nlohmann::json;

std::mutex write_mutex;

ConverterJSON::ConverterJSON(const std::string& config_path,
                             const std::string& request_path,
                             const std::string& answers_path)
    : _config_path(config_path),
      _request_path(request_path),
      _answers_path(answers_path) {
    std::ifstream file(_config_path);
    if (!file)
        throw std::runtime_error("config file is missing");

    json config;
    file >> config;
    if (config.find("config") == config.end())
        throw std::runtime_error("config file is empty");

    _name = config["config"]["name"].get<std::string>();
    _version = config["config"]["version"].get<std::string>();
    if (_version != "0.1")
        throw std::runtime_error("config.json has incorrect file version");

    if (config["config"].contains("max_responses"))
        _max_responses = config["config"]["max_responses"];
}

const std::string& ConverterJSON::getName() const { return _name; }
const std::string& ConverterJSON::getVersion() const { return _version; }

std::vector<std::string> ConverterJSON::getTextDocuments() {
    std::ifstream file(_config_path);
    json config;
    file >> config;

    std::vector<std::string> docs;
    for (const auto& path : config["files"]) {
        std::ifstream doc(path);
        if (!doc) {
            std::cerr << "File not found: " << path << std::endl;
            docs.emplace_back("");
            continue;
        }
        std::string content((std::istreambuf_iterator<char>(doc)), std::istreambuf_iterator<char>());
        docs.push_back(content);
    }
    return docs;
}

int ConverterJSON::getResponsesLimit() { return _max_responses; }

std::vector<std::string> ConverterJSON::getRequests() {
    std::ifstream file(_request_path);
    json reqs;
    file >> reqs;
    return reqs["requests"].get<std::vector<std::string>>();
}

void ConverterJSON::putAnswers(std::vector<std::vector<RelativeIndex>> answers) {
    json result;

    auto process_request = [&](size_t i) {
        json request;
        if (answers[i].empty()) {
            request["result"] = false;
        } else {
            request["result"] = true;
            json relevance = json::array();
            for (const auto& rel : answers[i]) {
                relevance.push_back({
                    {"docid", rel._doc_id},
                    {"rank", rel._rank}
                });
            }
            request["relevance"] = relevance;
        }

        std::lock_guard<std::mutex> lock(write_mutex);
        result["answers"][std::to_string(i)] = request;
    };

    std::vector<std::thread> threads;
    for (size_t i = 0; i < answers.size(); ++i)
        threads.emplace_back(process_request, i);

    for (auto& t : threads)
        t.join();

    std::ofstream out(_answers_path);
    if (!out.is_open())
        throw std::runtime_error("Cannot open answers.json for writing");

    out << result.dump(4);
}