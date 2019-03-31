#include <fstream>
#include <random>
#include <chrono>
#include <iostream>

using namespace std;

int main() {
    ofstream out("10_8.txt");
    ofstream outB("10_8.bin", ofstream::binary);
    unsigned int n = 1000 * 1000 * 100;
    out << n << endl;
    outB.write((const char *)&n, sizeof(n));

    mt19937 generator(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    uniform_int_distribution<unsigned int> int_distribution;
    unsigned int x = int_distribution(generator);

    out << x;
    outB.write((const char *)&x, sizeof(x));
    while (--n) {
        x = int_distribution(generator);
        out << " " << x;
        outB.write((const char *)&x, sizeof(x));
    }
}
