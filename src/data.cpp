#include "include/data.hpp"
#include "include/Enemy.hpp"
#include "include/Tower.hpp"
#include "include/file.hpp"
#include "include/GLHelpers.hpp"

#include <img/img.hpp>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "include/utils.hpp"

/*struct Data{
    std::vector<int> grid; // 0 = empty, 1 = path, 2 = input, 3 = output  | Liste 2D de la grille
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

void Data::loadFromITD(std::filesystem::path const& pathFile, img::Image &image_map) {
    ITD itd;
    if (!isValidITD(pathFile, itd)) {
        throw std::runtime_error{"Invalid ITD file"};
    }
    start = itd.in;
    end = itd.out;
    path = itd.path;

    image_map = {img::load(make_absolute_path("images/" + itd.map , true), 3, true)};
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
