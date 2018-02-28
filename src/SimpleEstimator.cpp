//
// Created by Nikolay Yakovets on 2018-02-01.
//

#include "SimpleGraph.h"
#include "SimpleEstimator.h"

SimpleEstimator::SimpleEstimator(std::shared_ptr<SimpleGraph> &g){

    // works only with SimpleGraph
    graph = g;
}

void SimpleEstimator::prepare() {

    // do your prep here

}

cardStat SimpleEstimator::estimate(RPQTree *q) {
    uint32_t s = 0;
    uint32_t t = 0;
    //ending
    uint32_t end = 3;
    for(auto edges: graph->reverse_adj){
        auto it = std::find_if(edges.begin(), edges.end(), [end](const std::pair<uint32_t,uint32_t> & element){return element.first == end;});
        if(it != edges.end()){
            t++;
        }
    }
    //begin
    uint32_t start = 0;
    for(auto edges: graph->adj){
        auto it = std::find_if(edges.begin(), edges.end(), [start](const std::pair<uint32_t,uint32_t> & element){return element.first == start;});
        if(it != edges.end()){
            s++;
        }
    }
    return cardStat {s, 0, t};
}