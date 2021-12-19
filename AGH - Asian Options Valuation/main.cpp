#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include "PathDepOption.h"
#include <vector>
#include "BSModel.h"

using namespace std;

int main()
{
    srand(time(NULL));

    // Parameters setup:
    double r = 0.05;    // risk-free-rate
    int k = 135, m = 252;  // lattice for simulation: 0 = t0 < tk < tm = T
    double S_tk = 100, A_k = 100, K = 110, sigma = 0.7,  T = 2.0; // Option parameters setup

    // Simulation controls:
    long N = 10000;             // MC repetitions
    double epsilon = 0.0001;    // dt step for delta approximation

    // Market, and Asian Option Classes initialization:
    BSModel model(r, sigma, S_tk);
    ArithmeticAsianCall Acall(T,m,K);
    ArithmeticAsianPut Aput(T,m,K);

    // MC pricing:
    Acall.PriceByMC(model, N, epsilon, k, S_tk, A_k);
    Aput.PriceByMC(model, N, epsilon, k, S_tk, A_k);

    cout <<"====================================="<<endl;
    cout <<"      MC Asian arithmetic call:"<<endl;
    cout <<"====================================="<<endl;

    cout <<"          Price : "<< Acall.get_Price()<<endl;
    cout <<"       Accuracy : "<< Acall.get_standardError()<<endl;
    cout <<"          Delta : "<< Acall.get_delta()<<endl;
    cout <<" -----------------------------------"<<endl;
    cout <<"        Replicating strategy: "<<endl;
    cout <<"          x(tk) : "<< Acall.get_xt() <<endl;
    cout <<"          y(tk) : "<< Acall.get_yt() <<endl;
    cout << endl;

    cout <<"====================================="<<endl;
    cout <<"      MC Asian arithmetic put :"<<endl;
    cout <<"====================================="<<endl;

    cout <<"          Price : "<< Aput.get_Price()<<endl;
    cout <<"       Accuracy : "<< Aput.get_standardError()<<endl;
    cout <<"          Delta : "<< Aput.get_delta()<<endl;
    cout <<" -----------------------------------"<<endl;
    cout <<"        Replicating strategy: "<<endl;
    cout <<"          x(tk) : "<< Aput.get_xt() <<endl;
    cout <<"          y(tk) : "<< Aput.get_yt() <<endl;

    return 0;
}
