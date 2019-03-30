#include <iostream>

using namespace std;

void help(const char *arg0) {
    cout << "Usage: " << arg0 << " <file> [<binary file>]" << endl;
    cout << endl;
    cout << "<file>\tfile to read integers" << endl;
    cout << "<binary file>\tfile with same integers but in binary format" << endl;
}

int main(int argc, char *argv[]) {
    if (argc != 2 && argc != 3) {
        help(argv[0]);
        return 0;
    }
}
