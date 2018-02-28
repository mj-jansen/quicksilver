//
// Created by Nikolay Yakovets on 2018-02-01.
//

#include "SimpleGraph.h"
#include "SimpleEstimator.h"

using namespace std;

SimpleEstimator::SimpleEstimator(std::shared_ptr<SimpleGraph> &g){

    // works only with SimpleGraph
    graph = g;
    num_of_edges = new int[graph->getNoLabels()] {};
    missing_out_vertices = new int[graph->getNoLabels()] {};

    num_of_edges2 = new int[graph->getNoLabels()] {};
    missing_in_vertices = new int[graph->getNoLabels()] {};
}

void SimpleEstimator::prepare() {

    // do your prep here
    int noLabels = graph->getNoLabels();
    int noVertices = graph->getNoVertices();

    int edges_previous [noLabels] = {};

    int sum = 0;
    for(int i = 0; i < noVertices; i++) {
        if(graph->reverse_adj[i].empty())
          sum++;
        for(auto labelTarget : graph->adj[i]) {
            num_of_edges[labelTarget.first]++;
        }
        for(int j = 0; j < noLabels; j++) {
            if(num_of_edges[j] == edges_previous[j])
                missing_out_vertices[j]++;
            else
                edges_previous[j] = num_of_edges[j];
        }
    }

    int edges_previous2 [noLabels] = {};

    int sum2 = 0;
    for(int i = 0; i < noVertices; i++) {
        if(graph->adj[i].empty())
            sum2++;
        for(auto labelTarget : graph->reverse_adj[i]) {
            num_of_edges2[labelTarget.first]++;
        }
        for(int j = 0; j < noLabels; j++) {
            if(num_of_edges2[j] == edges_previous2[j])
                missing_in_vertices[j]++;
            else
                edges_previous2[j] = num_of_edges2[j];
        }
    }

    std::cout << "Sum: " << sum << '\n' << std::endl;
    for(int j = 0; j < noLabels; j++) {
        cout << j << "th label: " << num_of_edges[j] << '\n';
        cout << j << "th vertices: " << noVertices - missing_out_vertices[j] << '\n';
        cout << j << "th average per vertice: " << (double)num_of_edges[j]/(noVertices - missing_out_vertices[j]) << '\n';
    }
}

cardStat SimpleEstimator::estimate(RPQTree *q) {

    // perform your estimation here

    // evaluate according to the AST bottom-up

    if(q->isLeaf()) {
        // project out the label in the AST
        std::regex directLabel (R"((\d+)\+)");
        std::regex inverseLabel (R"((\d+)\-)");

        std::smatch matches;

        uint32_t label;
        bool inverse;

        if(std::regex_search(q->data, matches, directLabel)) {
            label = (uint32_t) std::stoul(matches[1]);
            inverse = false;
            return cardStat{graph->getNoVertices() - missing_out_vertices[label], num_of_edges[label], graph->getNoVertices() - missing_in_vertices[label]};
        } else if(std::regex_search(q->data, matches, inverseLabel)) {
            label = (uint32_t) std::stoul(matches[1]);
            inverse = true;
            return cardStat{graph -> getNoVertices() - missing_in_vertices[label], num_of_edges[label], graph->getNoVertices() - missing_out_vertices[label]};
        } else {
            std::cerr << "Label parsing failed!" << std::endl;
            return cardStat{0, 0, 0};
        }
    }

    if(q->isConcat()) {

        // evaluate the children
        auto leftGraph = SimpleEstimator::estimate(q->left);
        auto rightGraph = SimpleEstimator::estimate(q->right);

        // join left with right
        cout << "ratio: " << (double)rightGraph.noOut/graph->getNoVertices() << "\n";
        //cout << "noOut: " << leftGraph

        return cardStat{0, 0, 0};
    }

    return cardStat {0, 0, 0};
}

SimpleEstimator::~SimpleEstimator() {
    delete[] num_of_edges;
    delete[] num_of_edges2;
    delete[] missing_in_vertices;
    delete[] missing_out_vertices;
}