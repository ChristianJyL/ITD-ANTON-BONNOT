#pragma once
#include <vector>
#include "include/file.hpp"
#include <img/img.hpp>
#include "include/graph.hpp"

enum class TileType {
    Empty = 0,
    Path = 1,
    Input = 2,
    Output = 3,
    Tower = 4,
};

struct Data{
    std::vector<int> grid; // 0 = empty, 1 = path, 2 = input, 3 = output  | Liste 2D de la grille
    Graph::WeightedGraph graph;
    std::unordered_map<int, std::pair<int, int>> coordNodes;

    int width; // Largeur de la grille
    int height; // Hauteur de la grille
    glm::u8vec3 start; // Couleur du début du chemin
    glm::u8vec3 end; // Couleur de la fin du chemin
    glm::u8vec3 path; // Couleur du chemin


    int  getCell(int x, int y) const; // Récupère la valeur de la cellule à la position (x, y)
    void setCell(int x, int y, int value) ; // Modifie la valeur de la cellule à la position (x, y)
    void loadFromITD(std::filesystem::path const& pathFile) ; // Charge les données à partir d'un fichier ITD
    void initGrid(int width, int height) ; // Initialise la grille avec des cases vides
    void printGrid() const; // Affiche la grille
};



