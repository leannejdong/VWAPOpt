#include "VWAP.h"

#include <iostream>

int main() {
    int NPaths = 1000; int numIncrements = 500; double T = 1.0;
    double lambda = 5; double a = 1.0; double sigmaVol = 0.1; double mu = 0.1;
    double sigmaPrice = 0.1; double X0 = 100; double S0 = 100;
    double K = 100; double interestRate = 0.25; //double delta = T/numIncrements;

    VWAPOption opt1{NPaths,numIncrements,T,lambda, a, sigmaVol, mu, sigmaPrice, X0, S0, K, interestRate};


    std::ofstream file1;

    opt1.computefirst2Moments(file1);
    opt1.computeMoment3(file1);
    opt1.computeMoment3AndPrice(file1);
    opt1.computeParameters(file1);

    return 0;

}
