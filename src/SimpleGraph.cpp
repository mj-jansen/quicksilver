//
// Created by Nikolay Yakovets on 2018-01-31.
//

#include "SimpleGraph.h"

SimpleGraph::SimpleGraph(int n)   {
    setNoVertices(n);
}

int SimpleGraph::getNoVertices() const {
    return V;
}

void SimpleGraph::setNoVertices(int n) {
    if(n < 0) throw "Negative number of vertices!";
    V = n;
    adj.resize(V);
    reverse_adj.resize(V);
}

int SimpleGraph::getNoEdges() const {
    int sum = 0;
    for (const auto & l : adj)
        sum += l.size();
    return sum;
};

int SimpleGraph::getNoLabels() const {
    return L;
}

void SimpleGraph::setNoLabels(int noLabels) {
    if(noLabels < 0) throw "Negative number of labels!";
    L = noLabels;
}

void SimpleGraph::addEdge(int from, int to, int edgeLabel) {
    if(from < 0 || from >= V ||  to < 0 || to >= V || edgeLabel < 0 || edgeLabel >= L)
        throw std::runtime_error(std::string("Edge data out of bounds: ") +
                                         "(" + std::to_string(from) + "," + std::to_string(to) + "," +
                                         std::to_string(edgeLabel) + ")");
    adj[from].emplace_back(std::make_pair(edgeLabel, to));
    reverse_adj[to].emplace_back(std::make_pair(edgeLabel, from));
}

void SimpleGraph::readFromContiguousFile(const std::string &fileName) {

    std::string line;
    std::ifstream graphFile { fileName };

    std::regex edgePat (R"((\d+)\s(\d+)\s(\d+)\s\.)"); // subject predicate object .
    std::regex headerPat (R"((\d+),(\d+),(\d+))"); // noNodes,noEdges,noLabels

    // parse the header (1st line)
    std::getline(graphFile, line);
    std::smatch matches;
    if(std::regex_search(line, matches, headerPat)) {
        int noNodes = std::stoi(matches[1]);
        int noLabels = std::stoi(matches[3]);

        setNoVertices(noNodes);
        setNoLabels(noLabels);
    } else {
        throw std::runtime_error(std::string("Invalid graph header!"));
    }

    // parse edge data
    while(std::getline(graphFile, line)) {

        if(std::regex_search(line, matches, edgePat)) {
            int subject = std::stoi(matches[1]);
            int predicate = std::stoi(matches[2]);
            int object = std::stoi(matches[3]);

            addEdge(subject, object, predicate);
        }
    }

    graphFile.close();

}