#include "include/graph.hpp"
#include "include/file.hpp"
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
                std::cout << "from : " << node.id << " to : " << voisin << " weight : " << getWeight(adjacency_list, node.id, voisin) << std::endl;
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
} // namespace Graph
