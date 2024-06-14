#pragma once
#include <iostream>
#include "entity/Enemy.hpp"
#include "entity/Tower.hpp"

#include <vector>
#include "other/file.hpp"
#include <img/img.hpp>
#include "other/graph.hpp"
#include "other/utils.hpp"
#include "entity/Projectile.hpp"

#define TOTAL_WAVES 5

enum TileType {
    Empty = 0,
    Path = 1,
    Input = 2,
    Output = 3,
    TowerSlow = 4,
    TowerShortRange = 5,
    TowerLongRange = 6,
};

struct Data{
    std::vector<TileType> grid; // 0 = empty, 1 = path, 2 = input, 3 = output  | Liste 1D de la grille
    std::vector<Enemy> enemies;
    std::vector<Tower> towers;
    std::vector<Projectile> projectiles;
    Graph::WeightedGraph graph;
    std::unordered_map<int, std::pair<int, int>> coordNodes;
    std::unordered_map<int, std::vector<int>> shortestPaths;
    std::vector<int> entries;
    int exit;
    int spawnIndex {0}; //utilisé pour alterner les spawns
    bool spawning = true;
    int waveCount {0};

    int width; // Largeur de la grille
    int height; // Hauteur de la grille
    glm::u8vec3 start; // Couleur du début du chemin
    glm::u8vec3 end; // Couleur de la fin du chemin
    glm::u8vec3 path; // Couleur du chemin
    int cardSelected = -1;
    int money = 200000;


    bool isAlive = true;

    TileType  getCell(int x, int y) const; // Récupère la valeur de la cellule à la position (x, y)
    void setCell(int x, int y, TileType value) ; // Modifie la valeur de la cellule à la position (x, y)
    void loadFromITD(std::filesystem::path const& pathFile) ; // Charge les données à partir d'un fichier ITD
    void initGrid(int width, int height) ; // Initialise la grille avec des cases vides
    void printGrid() const; // Affiche la grille
    bool isEverythingValid() const; // Vérifie si les données sont valides
    bool isCardSelected() const; // Vérifie si une carte est sélectionnée
    void selectCard(int index); // Sélectionne une carte
    void unselectCard(); // Désélectionne une carte
    bool placeCard(int x, int y); // Place une carte sur la grille

    int getNodeWithCoord(int x, int y); // Récupère le noeud à la position (x, y)
    std::pair<int,int> getCoordWithNode(int node); // Récupère les coordonnées du noeud donné
    std::vector<int> getShortestPath(int entry) const; // Récupère le chemin le plus court
    void putShortestPaths();

    void addTower(int x, int y , int cardSelected); // Ajoute une tour
    TileType getCardType(int index); // Récupère le type de la carte à l'index donné
    Tower getTower(int x, int y) const; // Récupère une tour

    void addEnemy(int spawnIndex, float x, float y, int hp, float speed, int reward); // Ajoute un ennemi
    void addEnemies(); // Ajoute des ennemis
    void alternateSpawn(double currentTime);


    void killEnemy(Enemy const& enemy); // Tue un ennemi
    void moveEnemy(Enemy &enemy, std::vector<int> const& pathList, float time); // Déplace un ennemi
    void moveEnemies(float time); // Déplace tous les ennemis

    void attackEnemies(float currentTime); // Attaque les ennemis
    void moveProjectiles(float time);


};



