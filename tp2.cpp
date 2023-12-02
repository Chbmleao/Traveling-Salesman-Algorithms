#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <fstream>
#include <string>
#include <tuple>
#include <cmath>
#include "Graph.hpp"
using namespace std; 

pair<vector<int>, double> branchBoundTravelingSalesman(Graph *graph) {
  graph->computeMinPath();

  Node root(graph);
  priority_queue<Node, vector<Node>, greater<Node>> queue;
  queue.push(root);

  auto [solution, best] = graph->getOnePath();
  int numVertices = graph->getNumVertices();

  while (!queue.empty()) {
    Node node = queue.top();
    queue.pop();

    if (node.level >= numVertices) {
      Node finalNode(graph, &node, 0);
      if (finalNode.cost < best) {
        best = finalNode.cost;
        solution = finalNode.path;
      }
    } else if (node.bound < best) {
      if (node.level <= numVertices) {
        vector<int> unvisited = graph->getUnvisitedVertices(node.path);
        for (int k : unvisited) {
          Node newNode(graph, &node, k);
          if (newNode.bound < best && graph->getWeight(node.path.back(), k) != numeric_limits<double>::infinity()) {
            queue.push(newNode);
          }
        }
      }
    }
  }

  return {solution, best};
}


vector<tuple<int, double, double>> readCoordinatesFromFile(string filename) {
  ifstream file(filename);

  vector<tuple<int, double, double>> coordinates;

  string line;
  while (getline(file, line)) {
    if (line.find("NODE_COORD_SECTION") != string::npos) {
      break;
    }
  }

  int id;
  double x, y;
  while (file >> id >> x >> y) {
    coordinates.emplace_back(id-1, x, y);
  }

  file.close();

  return coordinates;
}

double calculateEuclideanDistance(tuple<int, double, double> point1, tuple<int, double, double> point2) {
  double x1 = get<1>(point1);
  double y1 = get<2>(point1);
  double x2 = get<1>(point2);
  double y2 = get<2>(point2);

  return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

Graph* getGraphFromFile(string filename) {
  vector<tuple<int, double, double>> coordinates = readCoordinatesFromFile(filename);

  int numPoints = coordinates.size();
  Graph *graph = new Graph(numPoints);
  
  for (int i = 0; i < numPoints; ++i) {
    for (int j = i + 1; j < numPoints; ++j) {
      double distance = calculateEuclideanDistance(coordinates[i], coordinates[j]);
      graph->addEdge(get<0>(coordinates[i]), get<0>(coordinates[j]), distance);
    }
  }
  return graph;
}

void test1() {
  cout << "----------- TEST 1 -----------" << endl;
  Graph *g = new Graph(5);
  g->addEdge(0, 1, 3);
  g->addEdge(0, 2, 1);
  g->addEdge(0, 3, 5);
  g->addEdge(0, 4, 8);
  g->addEdge(1, 2, 6);
  g->addEdge(1, 3, 7);
  g->addEdge(1, 4, 9);
  g->addEdge(2, 3, 4);
  g->addEdge(2, 4, 2);
  g->addEdge(3, 4, 3);

  g->printGraph();

  pair<vector<int>, double> solution = branchBoundTravelingSalesman(g);
  cout << "Solution: " << solution.second << endl << endl;
}

void test2() {
  cout << "----------- TEST 2 -----------" << endl;
  Graph *g = new Graph(4);
  g->adjMatrix = vector<vector<double>>({
      {0, 10, 15, 20},
      {10, 0, 35, 25},
      {15, 35, 0, 30},
      {20, 25, 30, 0}});

  pair<vector<int>, double> solution = branchBoundTravelingSalesman(g);
  cout << "Solution: " << solution.second << endl << endl;
}

void test3() {
  cout << "----------- TEST 3 -----------" << endl;
  Graph *g = getGraphFromFile("data/berlin52.tsp");

  g->printGraph();

  pair<vector<int>, double> solution = branchBoundTravelingSalesman(g);
  cout << "Solution: " << solution.second << endl << endl;
}

int main() {
  test1();  
  test2();

  test3();
  return 0;
}
