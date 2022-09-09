#include "iostream"
using namespace std;

class Landmark{
public:
    Landmark(string nome, double x, double y): nome(nome), x(x), y(y){}
    string nome;
    double x;
    double y;
};