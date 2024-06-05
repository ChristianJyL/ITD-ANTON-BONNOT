#pragma once
#include <vector>
#include "other/file.hpp"
#include <img/img.hpp>
#include "other/graph.hpp"

enum class TileType {
    Empty = 0,
    Path = 1,
    Input = 2,
    Output = 3,
    Tower = 4,
};

struct Data{
    std::vector<int> grid; // 0 = empty, 1 = path, 2 = input, 3 = output  | Liste 1D de la grille
    Graph::WeightedGraph graph;
    std::unordered_map<int, std::pair<int, int>> coordNodes;

    int width; // Largeur de la grille
    int height; // Hauteur de la grille
    glm::u8vec3 start; // Couleur du début du chemin
    glm::u8vec3 end; // Couleur de la fin du chemin
    glm::u8vec3 path; // Couleur du chemin

    int CardSelected = -1;


    int  getCell(int x, int y) const; // Récupère la valeur de la cellule à la position (x, y)
    void setCell(int x, int y, int value) ; // Modifie la valeur de la cellule à la position (x, y)
    void loadFromITD(std::filesystem::path const& pathFile) ; // Charge les données à partir d'un fichier ITD
    void initGrid(int width, int height) ; // Initialise la grille avec des cases vides
    void printGrid() const; // Affiche la grille
    bool isEverythingValid() const; // Vérifie si les données sont valides
    bool isCardSelected() const; // Vérifie si une carte est sélectionnée
    void selectCard(int index); // Sélectionne une carte
    void unselectCard(); // Désélectionne une carte
};



