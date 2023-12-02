#include "Graph.hpp"

double getBound(Graph *graph, int finalVertice) {
  double cost = 0;
  for (int i = 0; i < graph->getNumVertices(); ++i) {
    cost += graph->minPath[i][0];
    cost += graph->minPath[i][1];
  }
  return cost / 2.0;
}

double getBound(Graph *graph, int finalVertice, Node* prevNode) {
  double cost = graph->getWeight(prevNode->path.back(), finalVertice);
  double bound = prevNode->bound * 2.0;

  if (cost >= graph->minPath[finalVertice][1]) {
    bound -= graph->minPath[finalVertice][1];
    bound += cost;
  }

  if (cost >= graph->minPath[prevNode->path.back()][1]) {
    bound -= graph->minPath[prevNode->path.back()][1];
    bound += cost;
  }

  return bound / 2.0;
}

Graph::Graph(int numVertices) : numVertices(numVertices) {
  adjMatrix.resize(numVertices, std::vector<double>(numVertices, std::numeric_limits<double>::infinity()));
  minPath.resize(numVertices, std::vector<double>(2, std::numeric_limits<double>::infinity()));
}

void Graph::addEdge(int src, int dest, double weight) {
  adjMatrix[src][dest] = weight;
  adjMatrix[dest][src] = weight;
}

double Graph::getWeight(int src, int dest) {
  return adjMatrix[src][dest];
}

int Graph::getNumVertices() {
  return numVertices;
}

void Graph::printGraph() {
  std::cout << "num vertices " << numVertices << std::endl;
  for (int i = 0; i < numVertices; ++i) {
    for (int j = 0; j < numVertices; ++j) {
      std::cout << adjMatrix[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

std::vector<std::pair<int, double>> Graph::getNeighbors(int vertex) {
  std::vector<std::pair<int, double>> neighbors;
  for (int i = 0; i < numVertices; ++i) {
    if (adjMatrix[vertex][i] != std::numeric_limits<double>::infinity()) {
      neighbors.emplace_back(i, adjMatrix[vertex][i]);
    }
  }
  return neighbors;
}

void Graph::computeMinPath() {
  for (int i = 0; i < numVertices; ++i) {
    std::vector<std::pair<int, double>> neighbors = getNeighbors(i);
    std::vector<double> edgeWeights;

    double minEdge = std::numeric_limits<double>::infinity();
    double secondMinEdge = std::numeric_limits<double>::infinity();

    for (int j = 0; j < numVertices; ++j) {
      if (neighbors[j].second < minEdge) {
        secondMinEdge = minEdge;
        minEdge = neighbors[j].second;
      } else if (neighbors[j].second < secondMinEdge) {
        secondMinEdge = neighbors[j].second;
      }
    }

    minPath[i][0] = minEdge;
    minPath[i][1] = secondMinEdge;
  }
}

std::pair<std::vector<int>, double> Graph::getOnePath() {
  std::vector<int> path = {0};
  double cost = 0;

  for (int i = 1; i < numVertices; ++i) {
    cost += adjMatrix[path.back()][i];
    path.push_back(i);
  }

  cost += adjMatrix[path.back()][0];
  path.push_back(0);

  return {path, cost};
}

std::vector<int> Graph::getUnvisitedVertices(std::vector<int>& path) {
  std::vector<int> unvisited;
  for (int i = 0; i < numVertices; ++i) {
    if (std::find(path.begin(), path.end(), i) == path.end()) {
      unvisited.push_back(i);
    }
  }
  return unvisited;
}

Node::Node(Graph *graph, Node* prevNode, int finalVertice) {
  if (prevNode == NULL) {
    level = 1;
    path = {0};
    cost = 0;
    bound = getBound(graph, 0);
    return;
  }

  level = prevNode->level + 1;
  path = prevNode->path;
  path.push_back(finalVertice);
  
  if (prevNode->path.empty()) {
    cost = 0;
  } else {
    cost = prevNode->cost + graph->getWeight(prevNode->path.back(), finalVertice);
  }

  bound = getBound(graph, finalVertice, prevNode);
}

bool Node::operator<(const Node& other) const {
  return bound < other.bound;
}

bool Node::operator>(const Node& other) const {
  return bound > other.bound;
}

bool Node::operator==(const Node& other) const {
  return bound == other.bound;
}

