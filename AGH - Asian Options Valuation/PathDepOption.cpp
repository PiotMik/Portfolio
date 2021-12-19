#include "PathDepOption.h"
#include <iostream>
#include <cmath>

samplePath operator*(double a, samplePath S)
{
    int n=S.size();
    for(int i=0; i<n; i++) S[i]=a*S[i];
    return S;
}

/// Standard Monte Carlo with delta estimation, pricing at tk
double PathDepOption::PriceByMC(BSModel Model, long N, double epsilon, int k, double S_tk, double A_k)
{

    if(m == k){                                           // If k = m, then we're already at expiry
        Price = Payoff(A_k);
        standardError = 0;
        delta = 0;
        xt = delta;
        yt = 0;
        return 0;
    }

    samplePath S(m-k);                                  // simulate only the 'remaining' points of the path
    double Av  = 0.0;                                   // for avg payoff
    double Av2 = 0.0;                                   // for avg of payoff^2 (for SE)
    double Av3 = 0.0;                                   // for delta estimation
    double payoff, payoff3;
    double frac = ((m-k)*1.0)/m;                        // fraction of remaining time, so that: T*frac = T-tk
    double disc_factor = exp(-Model.get_r()*T*frac);    // discounting from T to tk

    for(long n=0; n<N; n++)                             // main MC loop
    {

        Model.generateSamplePath(S, S_tk, T*frac);      // GBM path from tk to T, starting at S_tk
        payoff = Payoff(S, k, A_k);                     // Payoff, taking into account price history up to tk

        Av=(n*Av+payoff)/(n+1.0);
        Av2 = (n*Av2 + payoff*payoff)/(n+1.0);


        S=(1.0+epsilon)*S;                              // option delta approximation
        payoff3 = Payoff(S, k, A_k, S_tk*epsilon/(1.0*k));
        Av3 = (n*Av3+payoff3)/(n+1.0);

    }

    standardError = disc_factor*sqrt((Av2 - Av*Av)/(N-1.0));
    Price= disc_factor*Av;
    delta= disc_factor*(Av3-Av)/(epsilon*S_tk);
    xt   = delta;
    yt   = (Price - delta*S_tk)/exp(Model.get_r()*(T-T*frac));      // assuming risk free asset was worth 1 at t0 = 0.

    return Price;
}


/// Asian Option methods:

// Standard Asian Payoff tk = 0
double ArithmeticAsianCall::Payoff(double Av)
{
    if(Av<K) return 0.0;
    return Av-K;
}
double ArithmeticAsianPut::Payoff(double Av)
{
    if(Av>K) return 0.0;
    return K-Av;
}


// Asian payoffs, taking into account known part of the price trajectory (up to tk)
double ArithmeticAsianCall::Payoff(const samplePath &S, int k, double A_k)
{
    int m=S.size(); // here S contains only a part of the path: (t_k;T]
    double Av = A_k;

    for(long i=0; i<m;i++)
    {
        Av=((i+k)*Av+S[i])/(i+k+1.0); // index offset by +k, since we start with already running average A(tk)
    }
    if(Av<K) return 0.0;
    return Av-K;

}
double ArithmeticAsianPut::Payoff(const samplePath &S, int k, double A_k)
{
    int m=S.size(); // here S contains only a part of the path: (t_k;T]
    double Av = A_k;

    for(long i=0; i<m;i++)
    {
        Av=((i+k)*Av+S[i])/(i+k+1.0);
    }
    if(Av>K) return 0.0;
    return K-Av;
}


// For delta estimation: when multiplying S*(1+epsilon), the "S_tk" point will not get adjusted.
// The below functions manually accounts for that:
double ArithmeticAsianCall::Payoff(const samplePath &S, int k, double A_k, double S_tk_adj)
{
    int m=S.size(); // here S contains only a part of the path: (t_k;T]
    double Av = A_k + S_tk_adj; // S_tk_adj = S_tk*epsilon

    for(long i=0; i<m;i++)
    {
        Av=((i+k)*Av+S[i])/(i+k+1.0); // index offset by +k, since we start with already running average A(tk)
    }
    if(Av<K) return 0.0;
    return Av-K;

}
double ArithmeticAsianPut::Payoff(const samplePath &S, int k, double A_k, double S_tk_adj)
{
    int m=S.size();
    double Av = A_k + S_tk_adj;

    for(long i=0; i<m;i++)
    {
        Av=((i+k)*Av+S[i])/(i+k+1.0);
    }
    if(Av>K) return 0.0;
    return K-Av;
}

