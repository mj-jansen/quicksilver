//
// Created by Nikolay Yakovets on 2018-01-31.
//

#ifndef QS_SIMPLEGRAPH_H
#define QS_SIMPLEGRAPH_H

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <regex>
#include <fstream>
#include "Graph.h"

class SimpleGraph : public Graph {
public:
    std::vector<std::vector<std::pair<int,int>>> adj;
    std::vector<std::vector<std::pair<int,int>>> reverse_adj; // vertex adjacency list
protected:
    int V;
    int L;

public:

    SimpleGraph() : V(0), L(0) {};
    ~SimpleGraph() = default;
    explicit SimpleGraph(int n);

    int getNoVertices() const override ;
    int getNoEdges() const override ;
    int getNoLabels() const override ;

    void addEdge(int from, int to, int edgeLabel) override ;
    void readFromContiguousFile(const std::string &fileName) override ;

    void setNoVertices(int n);
    void setNoLabels(int noLabels);

};

#endif //QS_SIMPLEGRAPH_H
