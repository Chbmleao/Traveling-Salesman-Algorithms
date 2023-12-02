#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include "Graph.hpp"

std::pair<std::vector<int>, double> branchBoundTravelingSalesman(Graph *graph) {
  graph->computeMinPath();

  Node root(graph);
  std::priority_queue<Node, std::vector<Node>, std::greater<Node>> queue;
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
        std::vector<int> unvisited = graph->getUnvisitedVertices(node.path);
        for (int k : unvisited) {
          Node newNode(graph, &node, k);
          if (newNode.bound < best && graph->getWeight(node.path.back(), k) != std::numeric_limits<double>::infinity()) {
            queue.push(newNode);
          }
        }
      }
    }
  }

  return {solution, best};
}

void test1() {
  std::cout << "----------- TEST 1 -----------" << std::endl;
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

  std::pair<std::vector<int>, double> solution = branchBoundTravelingSalesman(g);
  std::cout << "Solution: " << solution.second << std::endl << std::endl;
}

void test2() {
  std::cout << "----------- TEST 2 -----------" << std::endl;
  Graph *g = new Graph(4);
  g->adjMatrix = std::vector<std::vector<double>>({
      {0, 10, 15, 20},
      {10, 0, 35, 25},
      {15, 35, 0, 30},
      {20, 25, 30, 0}});
  
  g->printGraph();

  std::pair<std::vector<int>, double> solution = branchBoundTravelingSalesman(g);
  std::cout << "Solution: " << solution.second << std::endl << std::endl;
}

int main() {
  test1();  
  test2();

  return 0;
}
