#ifndef BSModel_h
#define BSModel_h

#include <vector>
using namespace std;

double Gauss();

typedef vector<double> samplePath;

class BSModel
{
private:
    double r, sigma, S0;
public:
    BSModel(double r, double sigma, double S0);
    BSModel(){};

    void generateSamplePath(samplePath &S, double T);
    void generateSamplePath(samplePath &S, double S0_, double T);  // the same as above, but with overriden S0_

    double get_r(){return r;}
    double get_S0(){return S0;}
    double get_sigma(){return sigma;}
};

#endif // BSModel_h
