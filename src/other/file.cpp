#include "other/file.hpp"
#include "other/utils.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>
#include <algorithm>


bool isCommentOrEmpty(const std::string& line) {
    return line.empty() || line[0] == '#';
}

bool parse(const std::string& line, std::string& keyword, std::vector<std::string>& values) {
    std::istringstream stream(line);
    //On récupère le mot clé
    if (!std::getline(stream, keyword, ' ')) {
        return false;
    }
    //On récupère les valeurs
    std::string value;
    while (std::getline(stream, value, ' ')) {
        values.push_back(value);
    }
    return true;
}

bool isRGB(const std::string& value) {
    int val = std::stoi(value); //convertir value en int
    return val >= 0 && val <= 255;//si value est entre 0 et 255
}

glm::vec3 parseColor(const std::vector<std::string>& values) {
    for (const std::string &value: values) {
        if (!isRGB(value)) {
            std::cerr << "Invalid color: " << value << std::endl;
            throw std::runtime_error{"Invalid color"};
        }
    }
    return glm::vec3(std::stoi(values[0]), std::stoi(values[1]), std::stoi(values[2]));
}

void parseNode(const std::vector<std::string>& values, std::vector<Node>& nodes, int nb_nodes) {
    if (values.size() < 3) {
        std::cerr << "Invalid number of values for node" << std::endl;
        throw std::runtime_error{"Invalid number of values for node"};
    }
    Node current_node;
    current_node.id = std::stoi(values[0]);
    current_node.x = std::stoi(values[1]);
    current_node.y = std::stoi(values[2]);

    if (values.size() > 3) {
        for (size_t i = 3; i < values.size(); i++) {
            if (std::stoi(values[i]) >= nb_nodes) {
                std::cerr << "Invalid neighbour id: " << values[i] << std::endl;
                throw std::runtime_error{"Invalid neighbour id"};
            }
            current_node.neighbours.push_back(std::stoi(values[i]));
        }
    }
    nodes[current_node.id] = current_node;

}

bool isfileExists(const std::string& filename) {
    std::filesystem::path path = std::filesystem::current_path() / ".." / ".." / "images" / filename; //On crée le chemin absolu du fichier à partir du chemin relatif
    return std::filesystem::exists(path); //On vérifie si le fichier existe
}

bool isValidITD(std::filesystem::path const& path, ITD &itd){

    std::ifstream file {make_absolute_path(path)};
    if (!file.is_open()) {
        std::cout << "Could not open file: " << path << std::endl;
        throw std::runtime_error{"Could not open file"};
    }
    std::string line;
    std::getline(file, line);

    if (line != "ITD") {
        std::cerr << "This file is not an ITD file" << std::endl;
        return false;
    }
    int line_number = 0;
    while (std::getline(file, line)) {
        if (isCommentOrEmpty(line)) {
            continue;
        }
        std::string keyword;
        std::vector<std::string> values;
        if (!parse(line, keyword, values)) {
            std::cerr << "Could not parse line: " << line << std::endl;
            return false;
        }
        if (keyword == "map" && values.size() == 1 && line_number == 0) {
            std::string map_path = values[0];
            if (!isfileExists(map_path)) {
                std::cerr << "Map file does not exist: " << map_path << std::endl;
                return false;
            }
            itd.map = map_path;
        } else if (keyword == "path" && values.size() == 3 && line_number == 1) {
            itd.path = parseColor(values);
        } else if (keyword == "in" && values.size() == 3 && line_number == 2){
            itd.in = parseColor(values);
            if (itd.in == itd.path) {
                std::cerr << "In color is the same as path color" << std::endl;
                return false;
            }
        } else if (keyword == "out" && values.size() == 3 && line_number == 3) {
            itd.out = parseColor(values);
            if (itd.out == itd.path || itd.out == itd.in) {
                std::cerr << "Out color is the same as path or in color" << std::endl;
                return false;
            }
        } else if (keyword == "graph" && values.size() == 1 && line_number == 4) {
            int nb_nodes = std::stoi(values[0]);
            if (nb_nodes < 0) {
                std::cerr << "Invalid number of nodes: " << values[0] << std::endl;
                return false;
            }
            itd.graphSize = nb_nodes;
            itd.list_adjacency.resize(nb_nodes);
        } else if (keyword == "node" && values.size() >= 3) {
            if (line_number < 5) {
                std::cerr << "Node defined before graph size" << std::endl;
                return false;
            }
            parseNode(values, itd.list_adjacency, itd.graphSize);
        } else {
            std::cerr << "Invalid keyword or number of values: " << keyword << std::endl;
            return false;
        }
        line_number++;
    }
    file.close();
    return true;
}

int getWeight(const std::vector<Node>& nodes, int from, int to) {
    Node fromNode = nodes[from];
    Node toNode = nodes[to];
    int dx = fromNode.x - toNode.x;
    int dy = fromNode.y - toNode.y;
    return std::sqrt(dx * dx + dy * dy);
}

std::unordered_map<int, std::pair<int, int>> getCoord(const std::vector<Node>& nodes){
    std::unordered_map<int, std::pair<int, int>> coord;
    for (const Node& node : nodes) {
        coord[node.id] = {node.x, node.y};
    }
    return coord;
}


