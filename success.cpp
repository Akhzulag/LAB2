#include <iostream>
#include <cmath>

void theor( double m, double t)
{
    double N = pow(2,32);
//     m = pow(2,10);
//    t = pow(2,10);
    double sum = 0;
    std::cout<<"K "<<m << " L"<<t<<'\n';
    for (int i = 1; i <= m; ++i) {
        double temp = (1 - i * t / N);
        double temp1 = (1 - i * t / N);
        for (int j = 0; j < t; ++j) {
            sum += temp;
            temp *= temp1;
        }
        sum/=N;
    }

    std::cout << "final" << std::endl;
    std::cout << std::endl;
    std::cout <<100*sum <<"\\\%"  << std::endl;

    std::cout<<"for K tables:\n";
    std::cout<<(1 - pow(1-sum/N,t))*100<<"\\\%"<<"\n\n";
}

int main() {

    theor(pow(2,20), pow(2,12));
    theor(pow(2,22), pow(2,12));
    theor(pow(2,24), pow(2,12));
    return 0;
}

