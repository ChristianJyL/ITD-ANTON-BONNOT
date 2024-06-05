#include "data.hpp"
#include "entity/Enemy.hpp"
#include "entity/Tower.hpp"
#include "other/file.hpp"
#include "model.hpp"

#include <img/img.hpp>
#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "other/utils.hpp"

/*struct Data{
    std::vector<int> grid; // 0 = empty, 1 = path, 2 = input, 3 = output  | Liste 2D de la grille
    Graph::WeightedGraph graph;
    std::unordered_map<int, std::pair<int, int>> coordNodes;

    int width; // Largeur de la grille
    int height; // Hauteur de la grille
    glm::u8vec3 start; // Couleur du début du chemin
    glm::u8vec3 end; // Couleur de la fin du chemin
    glm::u8vec3 path; // Couleur du chemin

*/

void Data::setCell(int x, int y, int value) {
    grid[y * width + x] = value;
}

int Data::getCell(int x, int y) const {
    return grid[y * width + x];
}

void Data::initGrid(int width, int height) {
    this->width = width;
    this->height = height;
    grid.resize(width * height, 0);
}

void Data::loadFromITD(std::filesystem::path const& pathFile) {
    ITD itd;
    if (!isValidITD(pathFile, itd)) {
        throw std::runtime_error{"Invalid ITD file"};
    }
    start = itd.in;
    end = itd.out;
    path = itd.path;

    img::Image image_map = {img::load(make_absolute_path("images/" + itd.map , true), 3, true)};
    std::vector<glm::u8vec3> pixels {pixels_as_vec3(image_map)};
    initGrid(image_map.width(), image_map.height());
    for (int y = 0; y < image_map.height(); ++y)
    {
        for (unsigned int x = 0; x < image_map.width(); ++x)
        {
            auto const& pixel {pixels[y * image_map.width() + x]};
            if (pixel == Data::path){
                setCell(x, convertY(y, image_map.height()), 1);
            } else if (pixel == Data::start) {
                setCell(x, convertY(y, image_map.height()), 2);
            } else if (pixel == Data::end) {
                setCell(x, convertY(y, image_map.height()), 3);
            }
        }
    }

    graph = Graph::build_from_adjacency_list(itd.list_adjacency);
    coordNodes = getCoord(itd.list_adjacency);
}

void Data::printGrid() const {
    for (int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            std::cout << getCell(x, y);
        }
        std::cout << std::endl;
    }
}

bool Data::isEverythingValid() const {
    for (const auto& node : coordNodes) {
        int nodeIndex = node.first;
        int nodeX = node.second.first;
        int nodeY = node.second.second;
        int gridIndex = getCell(nodeX, nodeY);

        if (gridIndex != 1 && gridIndex != 2 && gridIndex != 3) { // on vérif si c'est un path, un input ou un output
            return false;
        }
    }
    return true;
}
