#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

class Graph {
public:
    int numVertices;
    std::vector<std::vector<double>> adjMatrix;
    std::vector<std::vector<double>> minPath;

    Graph(int numVertices);

    void addEdge(int src, int dest, double weight);

    double getWeight(int src, int dest);

    int getNumVertices();

    void printGraph();

    std::vector<std::pair<int, double>> getNeighbors(int vertex);

    void computeMinPath();

    std::pair<std::vector<int>, double> getOnePath();

    std::vector<int> getUnvisitedVertices(std::vector<int>& path);
};

class Node {
public:
  int level;
  std::vector<int> path;
  double cost;
  double bound;

  Node(Graph *graph, Node* prevNode=NULL, int finalVertice=-1);

  bool operator<(const Node& other) const;
  bool operator>(const Node& other) const;
  bool operator==(const Node& other) const;
};

double getBound(Graph *graph, int finalVertice);

double getBound(Graph *graph, int finalVertice, Node* prevNode);

#endif
