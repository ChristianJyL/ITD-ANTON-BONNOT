#include "other/graph.hpp"
#include "other/file.hpp"
#include <iostream>
#include <vector>
#include <stack>
#include <queue>

namespace Graph{

    void WeightedGraph::add_vertex(int const id) {
        auto it { adjacency_list.find(id)};
        if (it == adjacency_list.end()){ //pas dans la list
            adjacency_list[id];
        }
    }

    void WeightedGraph::add_directed_edge(int const from, int const to, int const weight){
        add_vertex(from);
        add_vertex(to);
        adjacency_list[from].push_back({to, weight});
    }

    void WeightedGraph::add_undirected_edge(int const from, int const to, int const weight){
        add_directed_edge(from, to, weight);
        add_directed_edge(to, from, weight);
    }

    WeightedGraph build_from_adjacency_list(std::vector<Node> const& adjacency_list){
        WeightedGraph result ;
        for (Node const& node : adjacency_list){
            for (int const& voisin : adjacency_list[node.id].neighbours){
                result.add_directed_edge(node.id, voisin, getWeight(adjacency_list, node.id, voisin));
                //std::cout << "from : " << node.id << " to : " << voisin << " weight : " << getWeight(adjacency_list, node.id, voisin) << std::endl;
            }
        }
        return result;
    }


    void WeightedGraph::print_DFS(int const start) const{
        std::vector<bool> visited ;
        for (int i {0}; i < adjacency_list.size(); i++){
            visited.push_back(false);
        }
        std::stack<int> stack;
        stack.push(start);
        while (!stack.empty()){ //tant que la pile n'est pas vide
            int current {stack.top()}; //on dépile
            stack.pop();
            if (!visited[current]){ //si on a pas encore visité
                std::cout << current << " "; //on affiche
                visited[current] = true; //on dit qu'on a visité
                for (WeightedGraphEdge const& voisin : adjacency_list.at(current)){ //on regarde les voisins
                    if (!visited[voisin.to]){ //si on a pas encore visité
                        stack.push(voisin.to); //on empile
                    }
                }
            }

        }
        std::cout << std::endl;
    }

    void WeightedGraph::print_BFS(int const start) const{
        std::vector<bool> visited ;
        for (int i {0}; i < adjacency_list.size(); i++){
            visited.push_back(false);
        }
        std::queue<int> queue;
        queue.push(start);
        visited[start] = true;
        while(!queue.empty()){
            int current {queue.front()};
            queue.pop();
            std::cout << current << " "; //on affiche
            for (WeightedGraphEdge const& voisin : adjacency_list.at(current)){ //on regarde les voisins
                if (!visited[voisin.to]){
                    queue.push(voisin.to);
                    visited[voisin.to] = true;
                }
            }
        }
        std::cout << std::endl;
    }

    std::unordered_map<int, std::pair<float, int>> dijkstra(WeightedGraph const& graph, int const& start, int const end) {
        // On crée un tableau associatif pour stocker les distances les plus courtes connues pour aller du sommet de départ à chaque sommet visité
        // La clé est l'identifiant du sommet et la valeur est un pair (distance, sommet précédent)
        std::unordered_map<int, std::pair<float, int>> distances {};

        // On crée une file de priorité pour stocker les sommets à visiter
        // la pair contient la distance pour aller jusqu'au sommet et l'identifiant du sommet

        // Ce type compliqué permet d'indiquer que l'on souhaite trier les éléments par ordre croissant (std::greater) et donc les éléments les plus petits seront au début de la file (top) (Min heap)
        std::priority_queue<std::pair<float, int>, std::vector<std::pair<float, int>>, std::greater<std::pair<float, int>>> to_visit {};

        // 1. On ajoute le sommet de départ à la liste des sommets à visiter avec une distance de 0 (on est déjà sur le sommet de départ)
        to_visit.push({0.0f, start});

        // Tant qu'il reste des sommets à visiter
        while (!to_visit.empty()) {
            // 2. On récupère le sommet le plus proche du sommet de départ dans la liste de priorité to_visit
            int current_vertex {to_visit.top().second};
            float current_distance {to_visit.top().first};
            to_visit.pop();

            // 3.Si on atteins le point d'arrivé, on s'arrête
            if (current_vertex== end) {
                return distances;
            }
            // 3. On parcoure la liste des voisins (grâce à la liste d'adjacence) du nœud courant
            for (auto voisin: graph.adjacency_list.at(current_vertex)) {
                // 4. on regarde si le nœud existe dans le tableau associatif (si oui il a déjà été visité)

                auto find_node {distances.find(voisin.to)};
                bool const visited {find_node != distances.end()};

                if (!visited) {
                    // 5. Si le nœud n'a pas été visité, on l'ajoute au tableau associatif en calculant la distance pour aller jusqu'à ce nœud
                    // la distance actuelle + le point de l'arrête)
                    distances[voisin.to] = { current_distance + voisin.weight , current_vertex} ;

                    // 6. On ajout également le nœud de destination à la liste des nœud à visité (avec la distance également pour prioriser les nœuds les plus proches)
                    to_visit.push({current_distance + voisin.weight, voisin.to});
                }else {
                    // 7. Si il a déjà été visité, On test si la distance dans le tableau associatif est plus grande
                    // Si c'est le cas on à trouvé un plus court chemin, on met à jour le tableau associatif et on ajoute de nouveau le sommet de destination dans la liste à visité
                    if ( current_distance + voisin.weight < distances[voisin.to].first) {
                        distances[voisin.to] = { current_distance + voisin.weight , current_vertex} ;
                        to_visit.push({current_distance + voisin.weight, voisin.to});
                    }
                }
            }
        }

        return distances;
    }
} // namespace Graph
