//
// Created by Nikolay Yakovets on 2018-02-03.
//

#ifndef QS_GRAPH_H
#define QS_GRAPH_H

#include <unordered_map>

class Graph {

public:

    virtual int getNoVertices() const = 0;
    virtual int getNoEdges() const = 0;
    virtual int getNoLabels() const = 0;

    virtual void addEdge(int from, int to, int edgeLabel) = 0;
    virtual void readFromContiguousFile(const std::string &fileName) = 0;

};


#endif //QS_GRAPH_H
