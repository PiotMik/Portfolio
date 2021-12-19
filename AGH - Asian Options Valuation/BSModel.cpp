#include "BSmodel.h"
#include<cmath>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>


double Gauss()
{
    double U1 = (rand()+1.0)/(RAND_MAX+1.0);
    double U2 = (rand()+1.0)/(RAND_MAX+1.0);
    return sqrt(-2.0*log(U1))*sin(2.0*M_PI*U2);
}


BSModel::BSModel(double r_, double sigma_, double S0_)
{
    r=r_; S0=S0_; sigma = sigma_;
}


void BSModel::generateSamplePath(samplePath &S, double T)
{
    int m=S.size();
    double dt = T/m;
    S[0]= S0*exp((r-0.5*sigma*sigma)*dt+sigma*sqrt(dt)*Gauss());
    for(int i=1; i<m; i++) S[i]= S[i-1]*exp((r-0.5*sigma*sigma)*dt+sigma*sqrt(dt)*Gauss());
}

// GBM simulation, but with overriden starting price
void BSModel::generateSamplePath(samplePath &S, double S0_, double T)
{
    int m=S.size();
    double dt = T/m;
    S[0]= S0_*exp((r-0.5*sigma*sigma)*dt+sigma*sqrt(dt)*Gauss());
    for(int i=1; i<m; i++) S[i]= S[i-1]*exp((r-0.5*sigma*sigma)*dt+sigma*sqrt(dt)*Gauss());
}
