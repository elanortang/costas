#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <deque>
#include <assert.h>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <stdio.h>
#include <string.h>
#include <utility>
#include <math.h>
#include <bitset>
#include <iomanip>
#include <complex>
#include <unordered_set>
using namespace std;
//#define int long long
typedef pair<int,int> pii;
typedef vector<int> vi;
typedef long double ld;
typedef long long ll;
// #define X first
// #define Y second
// #define all(o) o.begin(), o.end() // for vectors
// #define endl '\n'
// #define IOS ios::sync_with_stdio(0), cin.tie(0)// making cin and cout fast

// Maps from cell (i, j) to the DIMACS variable number
int arr_var (int n, int i, int j) {
    return n*i + j + 1;
}

// Direct encoding of AtMostOne of 3 variables
void at_most_one_three(ostream& cnf, int a, int b, int c){
    cnf << -a << " " << -b << " 0\n";
    cnf << -a << " " << -c << " 0\n";
    cnf << -b << " " << -c << " 0\n";
}

// Direct encoding of AtMostOne of 4 variables
void at_most_one_four(ostream& cnf, int a, int b, int c, int d){
    at_most_one_three(cnf, a, b, c);

    cnf << -a << " " << -d << " 0\n";
    cnf << -b << " " << -d << " 0\n";
    cnf << -c << " " << -d << " 0\n";
}

/*
Linear encoding of AtMostOne of all n variables in arr
Invariant (should hold on entry and exit): 
    var_counter_ptr holds the next available DIMACS 
    number to use as an auxiliary variable
*/
void at_most_one(ostream& cnf, int* arr, int n, int* var_counter_ptr){
    if (n <= 1) {
        // AtMostOne of one or fewer variables imposes no constraints
        return;
    }

    int idx = 0;
    if (n == 2) {
        cnf << -arr[idx] << " " << -arr[idx + 1] << " 0\n";
        return;
    }
    if (n == 3) {
        at_most_one_three(cnf, arr[idx], arr[idx + 1], arr[idx + 2]);
        return;
    } 
    if (n == 4) {
        at_most_one_four(cnf, arr[idx], arr[idx + 1], arr[idx + 2], arr[idx + 3]);
        return;
    } 
    
    at_most_one_four(cnf, arr[0], arr[1], arr[2], -(*var_counter_ptr));
    // Loop Invariant: idx points to the next variable in arr which hasn't been used
    // Loop Invariant (up until last iteration): 
    //      var_counter_ptr was used as a (negated) aux variable in a previous clause and 
    //      still needs to be used as an aux variable in the next clause
    idx = 3;
    while (idx < n){
        // only three left, will exit loop after this
        if (idx == n - 3) {
            at_most_one_four(cnf, *var_counter_ptr, arr[idx], arr[idx + 1], arr[idx + 2]);
            idx += 3;
        } else {
            // only two left, will exit loop after this
            if (idx == n - 2) {
                at_most_one_three(cnf, *var_counter_ptr, arr[idx], arr[idx + 1]);
            } 
            // at least four left (not possible to have only one left from previous if checks)
            else {
                at_most_one_four(cnf, *var_counter_ptr, arr[idx], arr[idx + 1], -(*var_counter_ptr + 1));
                ++(*var_counter_ptr); 
            }
            idx += 2;
        }
    }
    // at the end of the loop, var_counter_ptr points to the auxiliary variable which was most recently
    // used in the encoding. We increment it one more to maintain the invariant that it points to the 
    // next available DIMACS variable number.
    ++(*var_counter_ptr); 
}

int main (int argc, char** argv) { //g++ ord_gen_dimacs.cpp -o ord_gen_dimacs
    ofstream cnf(argv[1]);
    int n = atoi (argv[2]);
    int var_counter = n*n + 1;
    int clause_counter = 0;

    // each row has at most one of each number
    for (int i = 0; i < n; ++i) {
        int arr[n];
        for (int j = 0; j < n; ++j) {
            arr[j] = arr_var(n, i, j);
            cnf << arr[j] << " ";
        }
        cnf << "0\n";
        at_most_one(cnf, arr, n, &var_counter);
    }

    // each column has at most one of each number
    for (int j = 0; j < n; ++j) {
        int arr[n];
        for (int i = 0; i < n; ++i) {
            arr[i] = arr_var(n, i, j);
            cnf << arr[i] << " ";
        }
        cnf << "0\n";
        at_most_one(cnf, arr, n, &var_counter);
    }
    if (n <= 2){
        clause_counter += 2*n*n;
    } else {
        clause_counter += 2*n*(3*n - 5);
    }
    // Each direction (x, y) appears at most once, among all vectors with that direction
    // To avoid double-counting vectors, the x direction of the vector is non-negative,
    // and if the x direction is 0, the y direction is non-negative. Normally y starts at -(n-1).
    for (int x = 1; x < n; ++x) {
        for (int y = -(n-1); y < n; ++y) {
            if (y == 0) {
                continue;
            }

            int lbi = 0;                //lower bound on the possible x-coordinates of the starting point
            int upi = n-x;              //upper bound on the possible x-coordinates of the starting point
            int lbj = max(0, -y);       //lower bound on the possible y-coordinates of the starting point
            int upj = min(n, n-y);      //upper bound on the possible y-coordinates of the starting point
            
            int num = (upj - lbj);      //total number of vectors with this direction
            int arr[num];
            int idx = 0;

            // Iterate through all possible column starting points and add their DIMACS numbers to a vector
            for (int j = lbj; j < upj; ++j) {
                arr[idx] = var_counter;
                ++idx;

                // if a pair starting in column j has direction (x,y), then Z_{j, (x,y)} is true
                for (int i = lbi; i < upi; ++i) {
                    cnf << -arr_var(n,i,j) << " " << -arr_var(n,i+x,j+y) << " " << var_counter << " 0\n";
                    clause_counter++;
                    if (idx != 1) {
                        cnf << -arr_var(n,i,j) << " " << -arr_var(n,i+x,j+y) << " " << -var_counter+1 << " 0\n";
                        clause_counter++;
                    }
                }

                ++var_counter;
            }

            //Do order encoding over all variables in arr
            for (int k = 0; k < num - 1; k++) {
                cnf << -arr[k] << " " << arr[k+1] << " 0\n";
                clause_counter++;
            }

        }
    }
    cnf << "p cnf " << var_counter - 1 << ' ' << clause_counter << '\n';
    // It was too annoying to figure out how to store everything and then output it after this line
    // so I just manually move this last line to the first line before I run cadical
}