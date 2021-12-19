#ifndef PathDepOption_h
#define PathDepOption_h

#include "BSModel.h"
class PathDepOption
{
private:
    double T;
    int m;
    double standardError, Price, delta, xt, yt;

public:
    PathDepOption(double T_, int m_){T=T_; m=m_;}

    /// Payoff with known path history up to tk.
    virtual double Payoff(const samplePath &S, int k, double A_k) = 0;
    virtual double Payoff(const samplePath &S, int k, double A_k, double S_tk_adjustment_for_delta_estimation) = 0;
    virtual double Payoff(double additional_param) = 0;

    // Standard MC pricing
    double PriceByMC(BSModel Model, long N);
    double PriceByMC(BSModel Model, long N, double epsilon);

    /// Pricing at tk >= 0.
    double PriceByMC(BSModel Model, long N, double epsilon, int k, double S_tk, double A_k);

    // Access methods
    double get_standardError(){return standardError;}
    double get_Price(){return Price;}
    double get_delta(){return delta;}
    double get_T(){return T;}
    double get_m() {return m;}
    double get_xt() {return xt;}
    double get_yt() {return yt;}

};

class ArithmeticAsianCall: public PathDepOption
{
private:
    double K;
public:
    double Payoff(const samplePath &S);
    double Payoff(const samplePath &S, int k, double A_k);  // Payoff, when path until tk is known (Stk and Ak given)
    double Payoff(const samplePath &S, int k, double A_k, double S_tk_adj_for_delta_est);  // adjustment for S_tk in delta estimation
    double Payoff(double A_T);


    ArithmeticAsianCall(double T, int m, double K_): PathDepOption(T,m){K=K_;};
};

class ArithmeticAsianPut: public PathDepOption
{
private:
    double K;
public:
    double Payoff(const samplePath &S);
    double Payoff(const samplePath &S, int k, double A_k);  // Payoff, when path until tk is known (Stk and Ak given)
    double Payoff(const samplePath &S, int k, double A_k, double S_tk_adj_for_delta_est);  // adjustment for S_tk in delta estimation
    double Payoff(double A_T);
    ArithmeticAsianPut(double T, int m, double K_): PathDepOption(T,m){K=K_;};
};

#endif
