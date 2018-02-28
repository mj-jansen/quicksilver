//
// Created by Nikolay Yakovets on 2018-02-01.
//

#include "SimpleGraph.h"
#include "SimpleEstimator.h"
#include <cmath>

double pZero(double n, double z, int l);
double pDubl(double n, double z, int l);
int factorial(int n);

int highestOutDegree = 0;
int noOutEdges = 0;

SimpleEstimator::SimpleEstimator(std::shared_ptr<SimpleGraph> &g){

    // works only with SimpleGraph
    graph = g;
}

void SimpleEstimator::prepare() {

    //calculate highest out degree
    for(const auto edges: graph->adj){
        if(edges.size() > highestOutDegree)
        {
            highestOutDegree = edges.size();
        }
        if(edges.size() == 0){
            //calculate number of nodes without any outgoing edge
            noOutEdges++;
        }
    }


}

cardStat SimpleEstimator::estimate(RPQTree *q) {
    //finding s,l
    int l = 1;
    auto root = q;
    while(root->left){
        root = root->left;
        ++l;
    }
    std::string sRaw = root->data;
    uint32_t s = 0;
    uint32_t s1 = (uint32_t) atoi(sRaw.substr(0,1).data());
    if(sRaw[1] == '+'){
        for(auto edges: this->graph->adj){
            for(std::pair<uint32_t,uint32_t> edge: edges){
                if(edge.first == s1){
                    ++s;
                    break;
                }
            }
        }
    }

    //finding t
    root = q;
    while(root->right){
        root = root->right;
    }
    std::string tRaw = root->data;
    uint32_t t = 0;
    uint32_t t1 = (uint32_t) atoi(tRaw.substr(0,1).data());
    if(tRaw[1] == '+'){
        for(auto edges: this->graph->adj){
            for(std::pair<uint32_t,uint32_t> edge: edges){
                if(edge.first == t1){
                    ++t;
                    break;
                }
            }
        }
    }

    //finding n
    double n = graph->getNoVertices();

    //finding m
    double m = graph->getNoEdges();

    //finding x
    double x = highestOutDegree;

    //finding z
    double z = noOutEdges;

    //estimating the stuff
    //cardpaths
    double sum = 0;
    double mn = m/n;
    for(int i = 1; i <= l; i++){
        sum += (l+1)*pow(mn + (x-mn)/16.,i)*(1.-pZero(n,z,l))*(1.-pDubl(n,x,l));
    }
    sum = sum*s*t;
    return cardStat {t, static_cast<uint32_t>(sum), s};
}

double pZero(double n, double z, int l){
    double sum = 0;
    double facl = factorial(l);
    for(int i = 1; i <=l; ++i){
        sum += facl/(factorial(i)*factorial(l-i))*pow(z/n,i)*pow((n-z)/n,l-i);
    }
    return sum;
}

double pDubl(double n, double x, int l){
    double sum = 0;
    double facl = factorial(l);
    for(int i = 2; i <=l; ++i){
        sum += facl/(factorial(i)*factorial(l-i))*pow(x/n,i)*pow((n-x)/n,l-i);
    }
    return sum;
}

int factorial(int n)
{
    return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}