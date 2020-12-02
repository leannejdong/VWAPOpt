//
// Created by leanne on 11/21/20.
//

#ifndef VWAPOPT_VWAP_H
#define VWAPOPT_VWAP_H

#include <iostream>
#include <fstream>
#include <ctime>
#include <cmath>
#include <random>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <boost/timer/timer.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/moment.hpp>
#include <boost/accumulators/statistics/variance.hpp>


//using namespace boost::accumulators;


class VWAPOption {

public:
    template <typename... Args>
    using accumulator_set = boost::accumulators::accumulator_set<Args...>;

    template <typename... Args>
    using stats = boost::accumulators::stats<Args...>;

    template <int N>
    using tag_moment = boost::accumulators::tag::moment<N>;

    using tag_mean = boost::accumulators::tag::mean;
    using tag_variance = boost::accumulators::tag::variance;
    using immediate = boost::accumulators::immediate;

    VWAPOption(
            int NPaths,
            int numIncrements,
            double T,
            double lambda,
            double a,
            double sigmaVol,
            double mu,
            double sigmaPrice,
            double X0,
            double S0,
            double K,
            double interestRate)
            : NPaths_(NPaths),
              numIncrements_(numIncrements),
              lambda_(lambda),
              a_(a),
              sigmaVol_(sigmaVol),
              mu_(mu),
              sigmaPrice_(sigmaPrice),
              X0_(X0),
              S0_(S0),
              K_(K),
              interestRate_(interestRate) {

        delta_ = T/numIncrements;
        // rng_   = rng;
    }

    void computefirst2Moments(std::ofstream& file);


    void computeMoment3(std::ofstream& file);

    void computePrice(std::ofstream& file);
    void computeMoment3AndPrice(std::ofstream& file);

    void computeParameters(std::ofstream& file);
private:
    double GetNormDistrDouble(std::mt19937 &gen);


    //return a realisation of VWAP_T
    double simulateSinglePath();
    int NPaths_;
    int numIncrements_;
    double lambda_,
            a_,
            sigmaVol_,
            mu_,
            sigmaPrice_,
            X0_,
            S0_,
            delta_,
            K_,
            interestRate_; //strike price

    accumulator_set<double, stats<tag_moment<1>, tag_moment<2>, tag_moment<3> > > VWAPMoments_;
    accumulator_set<double, stats<tag_mean, tag_variance(immediate) > > callOptionPayoffs_;
};
#endif //VWAPOPT_VWAP_H
