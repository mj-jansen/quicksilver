//
// Created by Nikolay Yakovets on 2018-02-01.
//

#ifndef QS_SIMPLEESTIMATOR_H
#define QS_SIMPLEESTIMATOR_H

#include "Estimator.h"
#include "SimpleGraph.h"

class SimpleEstimator : public Estimator {

    std::shared_ptr<SimpleGraph> graph;
    int* num_of_edges;
    int* missing_out_vertices;

    int* num_of_edges2;
    int* missing_in_vertices;

    int* edges_previous;
    int* edges_previous2;

public:
    explicit SimpleEstimator(std::shared_ptr<SimpleGraph> &g);
    ~SimpleEstimator();

    void prepare() override ;
    cardStat estimate(RPQTree *q) override ;
};


#endif //QS_SIMPLEESTIMATOR_H
