#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int arr_var (int n, int i, int j) {
    return n*i + j + 1;
}

int main(int argc, char* argv[]) {
    string path(argv[1]);
    int n = atoi(argv[2]);

    if (n % 2 != 0) {
        cout << "n is not odd\n";
        return 1;
    }

    for (int x = 0; x <= 4; x += 2) {
        for (int y = 0; y <= 2; ++y) {
            ostringstream ss;
            ss << path << "/sym_n" << n << "_x" << x << "_y" << y << "_top.cnf";
            ofstream cnf(ss.str());
            for (int i = 0; i <= n - 1 - x; ++i) {
                for (int j = 0; j <= n/2 - 1 - y; ++j) {
                    cnf << arr_var(n, i, j) << " " << -arr_var(n, n-1-x-i, j + n/2-y) << " 0\n";
                    cnf << -arr_var(n, i, j) << " " << arr_var(n, n-1-x-i, j + n/2-y) << " 0\n";
                }
            }
        }
    }

    for (int x = 0; x <= 4; x += 2) {
        for (int y = 1; y <= 2; ++y) {
            if (x == 0 && y == 1) {
                continue;
            }
            ostringstream ss;
            ss << path << "/sym_n" << n << "_x" << x << "_y" << y << "_edge.cnf";
            ofstream cnf(ss.str());
            for (int i = 0; i <= n - 1 - x; ++i) {
                for (int j = y; j <= n/2 - 1; ++j) {
                    cnf << arr_var(n, i, j) << " " << -arr_var(n, n-1-x-i, j + n/2-y) << " 0\n";
                    cnf << -arr_var(n, i, j) << " " << arr_var(n, n-1-x-i, j + n/2-y) << " 0\n";
                }
            }
        }
    }   
}