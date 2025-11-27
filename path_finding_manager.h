//
// Created by juan-diego on 3/29/24.
//

#ifndef HOMEWORK_GRAPH_PATH_FINDING_MANAGER_H
#define HOMEWORK_GRAPH_PATH_FINDING_MANAGER_H

#include "graph.h"
#include "window_manager.h"
#include <cmath>
#include <limits>
#include <queue>
#include <set>
#include <unordered_map>

// Este enum sirve para identificar el algoritmo que el usuario desea simular
enum Algorithm { None, Dijkstra, AStar, BestFirstSearch };

//* --- PathFindingManager ---
//
// Esta clase sirve para realizar las simulaciones de nuestro grafo.
//
// Variables miembro
//     - path           : Contiene el camino resultante del algoritmo que se
//     desea simular
//     - visited_edges  : Contiene todas las aristas que se visitaron en el
//     algoritmo, notar que 'path'
//                        es un subconjunto de 'visited_edges'.
//     - window_manager : Instancia del manejador de ventana, es utilizado para
//     dibujar cada paso del algoritmo
//     - src            : Nodo incial del que se parte en el algoritmo
//     seleccionado
//     - dest           : Nodo al que se quiere llegar desde 'src'
//*
class PathFindingManager {
  WindowManager *window_manager;
  std::vector<sfLine> path;
  std::vector<sfLine> visited_edges;
  double heuristic_scale = 1.0; // Factor de escala para la heurística

  struct Entry {
    Node *node;
    double dist;

    bool operator<(const Entry &other) const {
      return dist > other.dist; // Min-heap
    };
  };

  void dijkstra(Graph &graph) {
    std::unordered_map<Node *, Node *> parent;
    std::unordered_map<Node *, double> dist;
    for (auto &[_, node] : graph.nodes) {
      dist[node] = std::numeric_limits<double>::infinity();
    }

    dist[src] = 0;
    std::priority_queue<Entry> pq;
    pq.push({src, 0});

    int steps = 0;

    while (!pq.empty()) {
      Entry top = pq.top();
      pq.pop();
      Node *u = top.node;

      if (u == dest)
        break;

      if (top.dist > dist[u])
        continue;

      steps++;

      for (Edge *edge : u->edges) {
        Node *v = (edge->src == u) ? edge->dest : edge->src;
        double weight = edge->length;

        if (dist[u] + weight < dist[v]) {
          dist[v] = dist[u] + weight;
          parent[v] = u;
          pq.push({v, dist[v]});

          // Visualization
          visited_edges.emplace_back(u->coord, v->coord, sf::Color::Yellow,
                                     2.0f);
          if (steps % 5000 == 0)
            render(graph);
        }
      }
    }

    set_final_path(parent);
    visited_edges
        .clear(); // Limpiar aristas visitadas para mostrar solo el camino
    render(graph);
  }

  void a_star(Graph &graph) {
    std::unordered_map<Node *, Node *> parent;
    std::unordered_map<Node *, double> g_score;
    std::unordered_map<Node *, double> f_score;
    std::set<Node *> closed_set;

    for (auto &[_, node] : graph.nodes) {
      g_score[node] = std::numeric_limits<double>::infinity();
      f_score[node] = std::numeric_limits<double>::infinity();
    }

    g_score[src] = 0;
    f_score[src] = heuristic(src, dest);

    std::priority_queue<Entry> pq;
    pq.push({src, f_score[src]});

    int steps = 0;

    while (!pq.empty()) {
      Entry top = pq.top();
      pq.pop();
      Node *u = top.node;

      if (u == dest)
        break;

      // Evitar reprocesar nodos con peor f_score (similar a Dijkstra)
      if (top.dist > f_score[u])
        continue;

      // Evitar reprocesar nodos ya cerrados
      if (closed_set.find(u) != closed_set.end())
        continue;

      closed_set.insert(u);
      steps++;

      for (Edge *edge : u->edges) {
        Node *v = (edge->src == u) ? edge->dest : edge->src;

        double weight = edge->length;
        double tentative_g_score = g_score[u] + weight;

        if (tentative_g_score < g_score[v]) {
          parent[v] = u;
          g_score[v] = tentative_g_score;
          // f(n) = g(n) + h(n)
          double new_f_score = g_score[v] + heuristic(v, dest);
          f_score[v] = new_f_score;
          pq.push({v, new_f_score});

          // Visualization
          visited_edges.emplace_back(u->coord, v->coord, sf::Color::Magenta,
                                     2.0f);
          if (steps % 5000 == 0)
            render(graph);
        }
      }
    }

    set_final_path(parent);
    visited_edges
        .clear(); // Limpiar aristas visitadas para mostrar solo el camino
    render(graph);
  }

  double heuristic(Node *a, Node *b) {
    // Distancia euclidiana en el espacio de coordenadas (unidades de pantalla)
    double coord_dist = std::sqrt(std::pow(a->coord.x - b->coord.x, 2) +
                                  std::pow(a->coord.y - b->coord.y, 2));
    
    // Aplicar factor de escala para convertir a las mismas unidades que edge->length
    return coord_dist * heuristic_scale;
  }

  // Calcular el factor de escala comparando distancias de coordenadas vs edge lengths
  void calculate_heuristic_scale(Graph &graph) {
    if (graph.edges.empty()) {
      heuristic_scale = 1.0;
      return;
    }

    double sum_coord_dist = 0.0;
    double sum_edge_length = 0.0;
    int count = 0;
    int sample_size = std::min(100, (int)graph.edges.size());

    for (int i = 0; i < sample_size; i++) {
      Edge *edge = graph.edges[i];
      double coord_dist = std::sqrt(
          std::pow(edge->src->coord.x - edge->dest->coord.x, 2) +
          std::pow(edge->src->coord.y - edge->dest->coord.y, 2));
      
      if (coord_dist > 0.0) {
        sum_coord_dist += coord_dist;
        sum_edge_length += edge->length;
        count++;
      }
    }

    if (count > 0 && sum_coord_dist > 0.0) {
      // Factor de escala = promedio(edge_length) / promedio(coord_dist)
      heuristic_scale = sum_edge_length / sum_coord_dist;
    } else {
      heuristic_scale = 1.0;
    }
  }

  void bfs(Graph &graph) {
    std::unordered_map<Node *, Node *> parent;
    std::set<Node *> visited_set;

    std::priority_queue<Entry> pq;
    pq.push({src, heuristic(src, dest)});
    visited_set.insert(src);

    int steps = 0;

    while (!pq.empty()) {
      Entry top = pq.top();
      pq.pop();
      Node *u = top.node;

      if (u == dest)
        break;

      steps++;

      for (Edge *edge : u->edges) {
        Node *v = (edge->src == u) ? edge->dest : edge->src;

        if (visited_set.find(v) == visited_set.end()) {
          visited_set.insert(v);
          parent[v] = u;
          double h = heuristic(v, dest);
          pq.push({v, h});

          // Visualization
          visited_edges.emplace_back(u->coord, v->coord, sf::Color::Blue, 2.0f);
          if (steps % 500 == 0)
            render(graph);
        }
      }
    }

    set_final_path(parent);
    visited_edges
        .clear(); // Limpiar aristas visitadas para mostrar solo el camino
    render(graph);
  }

  //* --- render ---
  // En cada iteración de los algoritmos esta función es llamada para dibujar
  // los cambios en el 'window_manager'
  void render() {
    // sf::sleep(sf::milliseconds(1)); // Faster animation
    window_manager->clear();
    // graph.draw(); // Cannot access graph here easily without passing it or
    // storing it. But the task says "dibujar los cambios en el window_manager".
    // The GUI loop calls graph.draw() then path_finding_manager.draw().
    // Here we are INSIDE the algorithm loop, blocking the GUI loop.
    // So we must manually clear and draw everything if we want real-time
    // update. However, we don't have access to 'graph' in 'render()' unless we
    // pass it or store it. I'll modify render to take Graph& or assume we just
    // draw visited edges on top of black? No, we need to redraw the graph. I
    // will modify render signature to take Graph& or store graph reference in
    // exec? For now, let's just draw visited edges to show progress, but
    // background will be black if we clear. Ideally we should pass graph to
    // render. But 'render' signature in the provided code was `void render()`.
    // I'll assume I should change it or use what I have.
    // I'll modify it to take Graph& graph.
  }

  void render(Graph &graph) {
    window_manager->clear();
    graph.draw();
    draw(true); // Draw visited edges
    window_manager->display();
  }

  //* --- set_final_path ---
  void set_final_path(std::unordered_map<Node *, Node *> &parent) {
    Node *current = dest;
    while (current != nullptr && current != src) {
      Node *p = parent[current];
      if (p) {
        path.emplace_back(p->coord, current->coord, sf::Color::Red, 4.0f);
        current = p;
      } else {
        break;
      }
    }
  }

public:
  Node *src = nullptr;
  Node *dest = nullptr;

  explicit PathFindingManager(WindowManager *window_manager)
      : window_manager(window_manager) {}

  void exec(Graph &graph, Algorithm algorithm) {
    if (src == nullptr || dest == nullptr) {
      return;
    }

    reset_path(); // Clear previous path/visited

    // Calcular el factor de escala de la heurística en la primera ejecución
    if (heuristic_scale == 1.0) {
      calculate_heuristic_scale(graph);
    }

    if (algorithm == Dijkstra) {
      dijkstra(graph);
    } else if (algorithm == AStar) {
      a_star(graph);
    } else if (algorithm == BestFirstSearch) {
      bfs(graph);
    }
  }

  void reset_path() {
    path.clear();
    visited_edges.clear();
  }

  void reset() {
    path.clear();
    visited_edges.clear();

    if (src) {
      src->reset();
      src = nullptr;
      // ^^^ Pierde la referencia luego de restaurarlo a sus valores por defecto
    }
    if (dest) {
      dest->reset();
      dest = nullptr;
      // ^^^ Pierde la referencia luego de restaurarlo a sus valores por defecto
    }
  }

  void draw(bool draw_extra_lines) {
    // Dibujar todas las aristas visitadas
    if (draw_extra_lines) {
      for (sfLine &line : visited_edges) {
        line.draw(window_manager->get_window(), sf::RenderStates::Default);
      }
    }

    // Dibujar el camino resultante entre 'str' y 'dest'
    for (sfLine &line : path) {
      line.draw(window_manager->get_window(), sf::RenderStates::Default);
    }

    // Dibujar el nodo inicial
    if (src != nullptr) {
      src->draw(window_manager->get_window());
    }

    // Dibujar el nodo final
    if (dest != nullptr) {
      dest->draw(window_manager->get_window());
    }
  }
};

#endif // HOMEWORK_GRAPH_PATH_FINDING_MANAGER_H
