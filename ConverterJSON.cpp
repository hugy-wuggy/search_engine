#include "ConverterJSON.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

ConverterJSON::ConverterJSON() {
    std::ifstream file("config.json");
    if (!file) throw std::runtime_error("config file is missing");

    json config;
    file >> config;
    if (config.find("config") == config.end())
        throw std::runtime_error("config file is empty");

    name = config["config"]["name"].get<std::string>();
    version = config["config"]["version"].get<std::string>();
    if (version != "0.1")
        throw std::runtime_error("config.json has incorrect file version");

    if (config["config"].contains("max_responses"))
        max_responses = config["config"]["max_responses"];
}

std::string ConverterJSON::getName() { return name; }
std::string ConverterJSON::getVersion() { return version; }

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    std::ifstream file("config.json");
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

int ConverterJSON::GetResponsesLimit() { return max_responses; }

std::vector<std::string> ConverterJSON::GetRequests() {
    std::ifstream file("requests.json");
    json reqs;
    file >> reqs;
    return reqs["requests"].get<std::vector<std::string>>();
}

void ConverterJSON::putAnswers(const std::vector<std::vector<std::pair<int, float>>>& answers) {
    json j;
    for (size_t i = 0; i < answers.size(); ++i) {
        std::string key = "request" + std::string(3 - std::to_string(i + 1).size(), '0') + std::to_string(i + 1);
        if (answers[i].empty()) {
            j["answers"][key]["result"] = "false";
        } else {
            j["answers"][key]["result"] = "true";
            j["answers"][key]["relevance"] = json::array();
            for (const auto& [docid, rank] : answers[i]) {
                j["answers"][key]["relevance"].push_back({{"docid", docid}, {"rank", rank}});
            }
        }
    }
    std::ofstream out("answers.json");
    out << j.dump(4);
}