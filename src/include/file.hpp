#pragma once
#include <filesystem>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <unordered_map>

struct Node {
    int id;
    int x;
    int y;
    std::vector<int> neighbours;

};


struct ITD{
    std::string map;
    glm::vec3 path;
    glm::vec3 in;
    glm::vec3 out;
    int graphSize;
    std::vector<Node> list_adjacency;
};



bool isCommentOrEmpty(const std::string& line);
bool parse(const std::string& line, std::string& keyword, std::vector<std::string>& values);
bool isfileExists(const std::string& filename);
bool isRGB(const std::string& value);
bool isValidITD(std::filesystem::path const& path, ITD &itd);
int getWeight(const std::vector<Node>& nodes, int from, int to);
