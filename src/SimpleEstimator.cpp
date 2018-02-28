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

    edges_previous = new int[graph->getNoLabels()] {};
    edges_previous2 = new int[graph->getNoLabels()] {};
}

void SimpleEstimator::prepare() {

    // do your prep here
    int noLabels = graph->getNoLabels();
    int noVertices = graph->getNoVertices();

    int sum = 0;
    for(int i = 0; i < noVertices; i++) {
        if (graph->reverse_adj[i].empty() && graph->adj[i].empty())
          sum++;

        for (auto labelTarget : graph->adj[i]) {
            num_of_edges[labelTarget.first]++;
        }

        for (auto labelTarget : graph->reverse_adj[i]) {
            num_of_edges2[labelTarget.first]++;
        }

        for (int j = 0; j < noLabels; j++) {
            if (num_of_edges[j] == edges_previous[j])
                missing_out_vertices[j]++;
            else
                edges_previous[j] = num_of_edges[j];

            if (num_of_edges2[j] == edges_previous2[j])
                missing_in_vertices[j]++;
            else
                edges_previous2[j] = num_of_edges2[j];
        }
    }




    std::cout << "Sum: " << sum << '\n' << std::endl;
    for(int j = 0; j < noLabels; j++) {
        cout << j << "th noOut: " << noVertices - missing_out_vertices[j] << '\n';
        cout << j << "th label: " << num_of_edges[j] << '\n';
        cout << j << "th noIn: " << noVertices - missing_in_vertices[j] << '\n';
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
            return cardStat{(uint32_t)(graph->getNoVertices() - missing_out_vertices[label]), (uint32_t)num_of_edges[label], (uint32_t)(graph->getNoVertices() - missing_in_vertices[label])};
        } else if(std::regex_search(q->data, matches, inverseLabel)) {
            label = (uint32_t) std::stoul(matches[1]);
            inverse = true;
            return cardStat{(uint32_t)(graph -> getNoVertices() - missing_in_vertices[label]), (uint32_t)num_of_edges[label], (uint32_t)(graph->getNoVertices() - missing_out_vertices[label])};
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
        //double ratio_out_left = (double)leftGraph.noPaths/leftGraph.noOut;
        //double ratio_in_left = (double)leftGraph.noPaths/leftGraph.noIn;
        double ratio_left_right = (double)rightGraph.noOut/graph->getNoVertices();
        double paths_per = (double)rightGraph.noPaths/rightGraph.noOut;

        uint32_t noOut =  (int)(ratio_left_right * leftGraph.noOut);
        uint32_t noIn = (int)(ratio_left_right * rightGraph.noIn);
        uint32_t noPaths = leftGraph.noPaths * ratio_left_right * paths_per;

        return cardStat{noOut, noPaths, noIn};
    }

    return cardStat {0, 0, 0};
}

SimpleEstimator::~SimpleEstimator() {
    delete[] num_of_edges;
    delete[] num_of_edges2;
    delete[] missing_in_vertices;
    delete[] missing_out_vertices;
    delete[] edges_previous;
    delete[] edges_previous2;
}