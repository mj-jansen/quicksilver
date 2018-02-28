//
// Created by Nikolay Yakovets on 2018-02-01.
//

#include "SimpleGraph.h"
#include "SimpleEstimator.h"

using namespace std;

SimpleEstimator::SimpleEstimator(std::shared_ptr<SimpleGraph> &g){

    // works only with SimpleGraph
    graph = g;
}

void SimpleEstimator::prepare() {

    // do your prep here
    int noLabels = graph->getNoLabels();
    int noVertices = graph->getNoVertices();

    int num_of_edges [noLabels] = {};
    int edges_previous [noLabels] = {};
    int missing_out_vertices [noLabels] = {};

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
    int num_of_edges2 [noLabels] = {};
    int edges_previous2 [noLabels] = {};
    int missing_in_vertices [noLabels] = {};

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
        cout << j << "th label: " << num_of_edges2[j] << '\n';
        cout << j << "th vertices: " << noVertices - missing_in_vertices[j] << '\n';
        cout << j << "th average per vertice: " << (double)num_of_edges2[j]/(noVertices - missing_in_vertices[j]) << '\n';
    }
}

cardStat SimpleEstimator::estimate(RPQTree *q) {

    // perform your estimation here
    //int noOut;
    //int noPaths;
    //int noIn;

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
            cout << "label: " << label << '\n';
            inverse = false;
        } else if(std::regex_search(q->data, matches, inverseLabel)) {
            label = (uint32_t) std::stoul(matches[1]);
            inverse = true;
        } else {
            std::cerr << "Label parsing failed!" << std::endl;
            return cardStat{0, 0, 0};
            //return nullptr;
        }
        return cardStat{0, 0, 0};
    }

    if(q->isConcat()) {

        // evaluate the children
        cout << "left\n";
        auto leftGraph = SimpleEstimator::estimate(q->left);
        cout << "right\n";
        auto rightGraph = SimpleEstimator::estimate(q->right);
        cout << "concat\n";
        return cardStat{0, 0, 0};

        // join left with right
    }

    return cardStat {0, 0, 0};
}