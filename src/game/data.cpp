#include "data.hpp"


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

void Data::setCell(int x, int y, TileType value) {
    grid[y * width + x] = value;
}

TileType Data::getCell(int x, int y) const {
    return grid[y * width + x];
}

void Data::initGrid(int width, int height) {
    this->width = width;
    this->height = height;
    grid.resize(width * height, TileType::Empty);
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
    graph = Graph::build_from_adjacency_list(itd.list_adjacency);
    coordNodes = getCoord(itd.list_adjacency);

    for (int y = 0; y < image_map.height(); ++y)
    {
        for (unsigned int x = 0; x < image_map.width(); ++x)
        {
            auto const& pixel {pixels[y * image_map.width() + x]};
            if (pixel == Data::path){
                setCell(x, convertY(y, image_map.height()), TileType::Path);
            } else if (pixel == Data::start) {
                setCell(x, convertY(y, image_map.height()), TileType::Input);
                int nodeIndex = getNodeWithCoord(x, convertY(y, image_map.height()));
                if (nodeIndex != -1) {
                    entries.push_back(nodeIndex);
                } else {
                    throw std::runtime_error{"Invalid ITD file"};
                }
            } else if (pixel == Data::end) {
                setCell(x, convertY(y, image_map.height()), TileType::Output);
                exit = getNodeWithCoord(x, convertY(y, image_map.height()));
                if (exit == -1) {
                    throw std::runtime_error{"Invalid ITD file"};
                }
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

bool Data::isEverythingValid() const {
    for (const auto& node : coordNodes) {
        int nodeIndex = node.first;
        int nodeX = node.second.first;
        int nodeY = node.second.second;
        TileType gridIndex = getCell(nodeX, nodeY);
        if (gridIndex != TileType::Path && gridIndex != TileType::Input && gridIndex != TileType::Output) { // on vérif si c'est un path, un input ou un output
            return false;
        }
    }
    return true;
}

bool Data::isCardSelected() const {
    return cardSelected != -1;
}

void Data::selectCard(int index) {
    cardSelected = index;
}

void Data::unselectCard() {
    cardSelected = -1;
}

bool Data::placeCard(int x, int y) {
    if (cardSelected == -1) {
        return false;
    }
    if (getCell(x, y) == TileType::Empty){
        setCell(x, y, getCardType(cardSelected));
        return true;
    } else {
        return false;
    }
}


TileType Data::getCardType(int index) {
    switch (index) {
        case 0:
            return TileType::TowerSlow;
        case 1:
            return TileType::TowerShortRange;
        case 2:
            return TileType::TowerLongRange;
        default:
            return TileType::Empty;
    }
}

void Data::addTowerLongRange(int x, int y) {
    towers.push_back(getTowerLongRange(x, y));
}

void Data::addTowerShortRange(int x, int y) {
    towers.push_back(getTowerShortRange(x, y));
}

void Data::addTowerSlow(int x, int y) {
    towers.push_back(getTowerSlow(x, y));
}

void Data::addTower(int x , int y, int cardSelected) {
    if (cardSelected == -1) {
        return;
    }
    switch (cardSelected) {
        case 0:
            addTowerSlow(x, y);
            break;
        case 1:
            addTowerShortRange(x, y);
            break;
        case 2:
            addTowerLongRange(x, y);
            break;
        default:
            break;
    }
}

void Data::addEnemy(Enemy enemy) {
    enemies.push_back(enemy);
}

int Data::getNodeWithCoord(int x, int y) {
    for (const auto& node : coordNodes) {
        if (node.second.first == x && node.second.second == y) {
            return node.first;
        }
    }
    return -1;
}

std::pair<int,int> Data::getCoordWithNode(int node) {
    return coordNodes[node];
}

//A FAIRE : pour chaque entrée, le chemin n'est pas le même pour chaque entrée...
std::vector<int> Data::getShortestPath(int entry) const{
    std::unordered_map<int, std::pair<float, int>> dijkstraTemp;
    dijkstraTemp = dijkstra(graph,entry, exit);
    std::vector<int> path;
    int currentNode = exit;
    while (currentNode != entry){
        path.push_back(currentNode);
        currentNode = dijkstraTemp[currentNode].second;
    }
    path.push_back(entry);
    std::reverse(path.begin(), path.end());
    return path;
}

void Data::putShortestPaths(){
    for (int entry : entries) {
        shortestPaths[entry] = getShortestPath(entry);
    }
}

void Data::moveEnemy(Enemy &enemy, std::vector<int> const& pathList, float time) {
    std::pair<int, int> nextStepCoords = coordNodes[pathList[enemy.currentPathIndex + 1]]; //on va chercher les coordonnées du prochain noeud

    // vecteur normalisé pour uniquement la direction
    float length = std::sqrt(std::pow(nextStepCoords.first - enemy.x, 2) + std::pow(nextStepCoords.second - enemy.y, 2));
    float directionX = (nextStepCoords.first - enemy.x) / length;
    float directionY = (nextStepCoords.second - enemy.y) / length;


    //on déplace l'ennemi selon la direction et la vitesse
    enemy.moveX(directionX * enemy.speed * time);
    enemy.moveY(directionY * enemy.speed * time);

    //distance entre l'ennemi et le prochain noeud comme dans le workshop
    if (glm::distance(glm::vec2(enemy.x, enemy.y), glm::vec2(nextStepCoords.first, nextStepCoords.second)) < 0.03f) { //0.03f = tolérance pour pas que l'ennemi se tp d'une case à l'autre à la fin
        enemy.currentPathIndex++;
        enemy.x = nextStepCoords.first;
        enemy.y = nextStepCoords.second;
    }

    if (enemy.currentPathIndex == pathList.size() - 1) { // a changer l'action ici (l'ennemi ne meurt pas mais c'est gameover)
        isAlive = false;
    }
}

void Data::killEnemy(Enemy const& enemy) {
    enemies.erase(std::remove(enemies.begin(), enemies.end(), enemy), enemies.end());
}

void Data::moveEnemies(float time) { //on bouge tous les ennemis
    for (Enemy &enemy: enemies) {
        moveEnemy(enemy, shortestPaths.at(enemy.spawnIndex), time);
        if (enemy.hp <= 0) {
            killEnemy(enemy);
        }
    }
}

void Data::attackEnemies(float currentTime){
    for (Tower &tower : towers) {
        tower.attack(enemies, currentTime, projectiles);
    }
}

void Data::moveProjectiles(float time) {
    for (Projectile &projectile : projectiles) {
        projectile.move(time);
        if (projectile.isArrived()) {
            killProjectile(projectile);
        }
    }
}

void Data::killProjectile(Projectile const& projectile) {
    projectiles.erase(std::remove(projectiles.begin(), projectiles.end(), projectile), projectiles.end());
}
