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

using std::vector;
using std::cout;
using std::ofstream;
using namespace boost::accumulators;
using timer = boost::timer::auto_cpu_timer;
//using namespace boost::timer;


class VWAPOption {
public:
    VWAPOption(/*gsl_rng * rng,*/
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

    void computefirst2Moments(ofstream& file) {
        timer t;
        accumulator_set<double, stats<tag::moment<1>,tag::variance(immediate) > > moment1;
        accumulator_set<double, stats<tag::moment<2>,tag::variance(immediate) > > moment2;

        //compute estimate for 1st moment
        for (int i = 0; i < NPaths_; ++i) {
            double VWAP_T = simulateSinglePath();
            moment1(VWAP_T);
        }

        //compute estimate for 2nd moment
        for (int i = 0; i < NPaths_; ++i) {
            double VWAP_T = simulateSinglePath();
            moment2(VWAP_T);
        }

        cout << std::endl;
        cout << "computation time = " << t.format(3) << std::endl;
        cout << "sigmaPrice = " << sigmaPrice_ << std::endl;
        cout << "E(VWAP) = " << moment<1>(moment1) << std::endl;
        cout << "E(VWAP^2) = " << moment<2>(moment2) << std::endl;

        file << sigmaPrice_
             << ", "
             << moment<1>(moment1) //E(VWAP)
             << ", "
             << sqrt(variance(moment1)) //StdErr(VWAP)
             << ", "
             << moment<2>(moment2) //E(VWAP)
             << ", "
             << sqrt(variance(moment2)) //StdErr(VWAP^2)
             << std::endl;
    }


    void computeMoment3(ofstream& file) {
        timer t;
        accumulator_set<double, stats<tag::moment<3> > > moment3;

        //compute estimate for 3rd moment
        for (int i = 0; i < NPaths_; ++i) {
            double VWAP_T = simulateSinglePath();
            moment3(VWAP_T);
        }

        cout << std::endl;
        cout << "computation time = " << t.format(3) << std::endl;
        cout << "sigmaPrice = " << sigmaPrice_ << std::endl;
        cout << "E(VWAP^3) = " << moment<3>(moment3) << std::endl;

        file << sigmaPrice_
             << ", "
             << moment<3>(moment3) //E(VWAP^3)
             << std::endl;
    }

    void computePrice(ofstream& file) {
        timer t;
        accumulator_set<double, stats<tag::mean, tag::variance(immediate) > > price;

        //compute estimate for VWAP price
        for (int i = 0; i < NPaths_; ++i) {
            double VWAP_T = simulateSinglePath();
//            cout << "VWAP_T" << VWAP_T << "\n";
//            cout << "K_" << K_ << "\n";
            if((VWAP_T - K_ ) > 0)
            {
                price(VWAP_T - K_);
               // cout << "VWAP_T -K_: " << VWAP_T - K_ << "\n";
            }
            else
            {
                price(0.0);
            }
        }
        cout << std::endl;
        cout << "computation time = " << t.format(3) << std::endl;
        cout << "sigmaPrice = " << sigmaPrice_ << std::endl;
        cout << "price = " << mean(price) << std::endl;
        cout << "stdDevPrice = " << sqrt(variance(price)) << std::endl;

        file << sigmaPrice_
             << ", "
             << mean(price) //option price
             << ", "
             << sqrt(variance(price)) << std::endl;

    }
    void computeMoment3AndPrice(ofstream& file) {
        timer t;
        accumulator_set<double, stats<tag::moment<3> > > moment3;
        accumulator_set<double, stats<tag::mean, tag::variance(immediate) > > price;


        //compute estimate for 3rd moment
        for (int i = 0; i < NPaths_; ++i) {
            double VWAP_T = simulateSinglePath();
            moment3(VWAP_T);
        }

        //compute estimate for VWAP price
        for (int i = 0; i < NPaths_; ++i) {
            double VWAP_T = simulateSinglePath();
            if((VWAP_T - K_ ) > 0)
                price(VWAP_T - K_);
            else
                price(0.0);
        }

        cout << std::endl;
        cout << "computation time = " << t.format(2) << std::endl;
        cout << "sigmaPrice = " << sigmaPrice_ << std::endl;
        cout << "E(VWAP^3) = " << moment<3>(moment3) << std::endl;
        cout << "price = " << mean(price) << std::endl;
        cout << "stdDevPrice = " << sqrt(variance(price)) << std::endl;

        file << sigmaPrice_
             << ", "
             << moment<3>(moment3) //E(VWAP^3)
             << ", "
             << mean(price) //option price
             << ", "
             << sqrt(variance(price)) << std::endl;

    }

    void computeParameters(ofstream& file) {
        timer t;
        for (int i = 0; i < NPaths_; ++i) {

            double VWAP_T = simulateSinglePath();

            VWAPMoments_(VWAP_T);

            if((VWAP_T - K_ ) > 0)
                callOptionPayoffs_(VWAP_T - K_);
            else
                callOptionPayoffs_(0.0);
        }

        cout << std::endl;
        cout << "computation time = " << t.format(3) << std::endl;
        cout << "sigmaPrice = " << sigmaPrice_ << std::endl;
        cout << "Delta = " << delta_ << std::endl;
        cout << "E(VWAP) = " << moment<1>(VWAPMoments_) << std::endl;
        cout << "E(VWAP^2) = " << moment<2>(VWAPMoments_) << std::endl;
        cout << "E(VWAP^3) = " << moment<3>(VWAPMoments_) << std::endl;
        cout << "price = " << mean(callOptionPayoffs_) << std::endl;
        cout << "stdDev = " << sqrt(variance(callOptionPayoffs_)) << std::endl;


        file << sigmaPrice_
             << ", "
             << moment<1>(VWAPMoments_) //E(VWAP)
             << ", "
             << moment<2>(VWAPMoments_) //E(VWAP^2)
             << ", "
             << moment<3>(VWAPMoments_) //E(VWAP^3)
             << ", "
             << mean(callOptionPayoffs_) //option price
             << ", "
             << sqrt(variance(callOptionPayoffs_)) << std::endl;
    }
private:

    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution<> nd;

    float GetNormDistrFloat()
    {
        float val = -1;
//        do { val = nd(gen); }
//        while(val < 0.0f || val > 10.0f);
        val = nd(gen);
        return val;
    }

    //float val = nd(gen);


    //return a realisation of VWAP_T
    double simulateSinglePath() {
        double sumStUt = S0_*X0_*X0_;
        double sumUt   = X0_*X0_;

        double Sti = S0_;
        double Xti = X0_;
        double Uti = 0.0;
        const double muX = a_*(1-exp(-lambda_ * delta_));
        const double sigmaX2 = pow(sigmaVol_,2)/(2*lambda_)*(1-exp(-2*lambda_*delta_));

        //simulate path over [0,T]
        for (int i = 1; i <= numIncrements_; i++) {
            Sti = Sti*exp((mu_-0.5*sigmaPrice_*sigmaPrice_)*delta_
                          + sigmaPrice_*sqrt(delta_))*GetNormDistrFloat();

            Xti = exp(-lambda_*delta_)*Xti + muX + sqrt(sigmaX2)*GetNormDistrFloat();
            Uti = pow(Xti,2);

            sumStUt+= Sti*Uti;
            sumUt += Uti;
        }

        return (sumStUt/sumUt);

    }

    //gsl_rng * rng_;
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

    accumulator_set<double, stats<tag::moment<1>, tag::moment<2>, tag::moment<3> > > VWAPMoments_;
    accumulator_set<double, stats<tag::mean, tag::variance(immediate) > > callOptionPayoffs_;
};
#endif //VWAPOPT_VWAP_H
