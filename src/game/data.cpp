#include "data.hpp"

//GETTERS&SETTERS&PRINT-------------------------------------------------------------------------------------------------------
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

bool Data::isCardSelected() const {
    return cardSelected != -1;
}

void Data::selectCard(int index) {
    cardSelected = index;
}

void Data::unselectCard() {
    cardSelected = -1;
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

//ITD----------------------------------------------------------------------------------------------------------------
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

//GRAPH&DIJKSTRA&PATH-------------------------------------------------------------------------------------------------------
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
//TOWER----------------------------------------------------------------------------------------------------------------
bool Data::placeCard(int x, int y) {
    if (cardSelected == -1) {
        return false;
    }
    if (getCell(x, y) == TileType::Empty){
        Tower newTower = getTower(x, y);
        if (money >= newTower.cost) {
            towers.push_back(newTower);
            money -= newTower.cost;
            setCell(x, y, getCardType(cardSelected));
            return true;
        }
    }
    return false;
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

Tower Data::getTower(int x, int y) const {
    switch (cardSelected) {
        case 0:
            return getTowerSlow(x, y);
        case 1:
            return getTowerShortRange(x, y);
        case 2:
            return getTowerLongRange(x, y);
        default:
            return Tower{};
    }
}

//PROJECTILE-----------------------------------------------------------------------------------------------------------
void Data::moveProjectiles(float time) {
    for (int i = 0; i < projectiles.size();) {
        if (!projectiles[i].isTargetAlive(enemies)) {
            projectiles.erase(projectiles.begin() + i);  // Supprimer le projectile si la cible est morte
        } else if (projectiles[i].isArrived(enemies)) {
            for (int j = 0; j < enemies.size(); ++j) {
                if (enemies[j].uniqueId == projectiles[i].targetId) {
                    enemies[j].hp -= projectiles[i].damage;
                    break;
                }
            }
            projectiles.erase(projectiles.begin() + i);  // Supprimer le projectile après avoir infligé des dégâts
        } else {
            projectiles[i].move(time, enemies);
            ++i;
        }
    }
}


//ENEMY----------------------------------------------------------------------------------------------------------------
void Data::addEnemy(int spawnIndex, float x, float y, int hp, float speed, int reward) {
    enemies.push_back(Enemy(spawnIndex, x, y, hp, speed, reward));
}

void Data::addEnemies() {
    int entry = entries[spawnIndex];
    if (spawnIndex % 2 == 0) {
        addEnemy(entry, coordNodes[entry].first, coordNodes[entry].second, 25, 1.5, 20);
    } else {
        addEnemy(entry, coordNodes[entry].first, coordNodes[entry].second, 15, 0.8, 50);
    }
    spawnIndex = (spawnIndex + 1) % entries.size(); //on alterne les spawns
}

void Data::alternateSpawn(double currentTime) {
    static double lastTime = 0.0;
    static double spawnCounter = 0.0;

    if (waveCount < TOTAL_WAVES) {
        if (spawning) {
            if (spawnCounter < 20.0 && currentTime - lastTime > 2.0) {
                lastTime = currentTime;
                spawnCounter += 2.0;
                addEnemies();
            } else if (spawnCounter >= 20.0) {
                spawning = false;
                spawnCounter = 0.0;
                lastTime = currentTime;
            }
        } else {
            if (currentTime - lastTime > 10.0) {
                spawning = true;
                waveCount++;
                lastTime = currentTime;
            }
        }
    }
}

void Data::killEnemy(Enemy const& enemy) {
    enemies.erase(std::remove(enemies.begin(), enemies.end(), enemy), enemies.end());
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

void Data::moveEnemies(float time) { //on bouge tous les ennemis
    for (Enemy &enemy: enemies) {
        moveEnemy(enemy, shortestPaths.at(enemy.spawnIndex), time);
        if (enemy.hp <= 0) {
            money += enemy.reward;
            killEnemy(enemy);
            std::cout << money << std::endl;
        }
    }
}

void Data::attackEnemies(float currentTime){
    for (Tower &tower : towers) {
        tower.attack(enemies, currentTime, projectiles);
    }
}