#include <fstream>
#include <map>
#include <set>
#include <utility>
#include <algorithm>
#include <iostream>
using namespace std;

/*
Variables:
- For each cell (i, j) in the grid, the variable a_{i, j} 
signifies that a dot is in cell (i, j)
  - 0 <= i,j < n
  - Number of variables: n^2
- For each distinct pair of points (i1, j1) and (i2, j2),
  v_{(i1,j1),(i2,j2)} to represent the vector between them
  - Number of variables: n^2 (n^2 - 1) / 2

Clauses:
- Each row has exactly one dot
  - For all i: 
        AtMostOne(a_{i, 0}, ..., a_{i, n-1})
        AtLeastOne(a_{i, 0}, ..., a_{i, n-1})
  - n * (3 * n - 5) clauses
  - n * (n-3)/2 new variables
- Each column has at most one of each number
  - For all j: 
        AtMostOne(a_{0, j}, ..., a_{n-1, j})
        AtLeastOne(a_{0, j}, ..., a_{n-1, j})
  - n * (3 * n - 5) clauses
  - n * (n-3)/2 new variables
- Define vector variables
  - For all distinct pairs (i1, j1) and (i2, j2):
        v_{(i1,j1),(i2,j2)} is true iff a_{i1,j1} and a_{i2, j2} are true
  - 3 * n^2 (n^2 - 1) / 2 clauses
- Among all vectors with direction (x,y), at most one of them is true
  - Bounds on x and y:
        0 <= x <= n - 1, x != 0
        -(n - 1) <= y <= n - 1
        excluding when x == 0 && y <= 0
  - Bounds on the starting point (i,j) of the vector:
        0 <= i < n - x
        max(0, -y) <= j < min(n, n-y)
  - Too annoying to figure out the number of new clauses or auxiliary variables beforehand

Variable Mapping to DIMACS
- a_{i, j, k} is variable number n*i + j
- E.g. the order goes
  a_{0, 1}  a_{0, 2} ... a_{0, n}
  a_{1, 1}  a_{1, 2} ... a_{1, n}
  ...
  a_{n-1, 1}  a_{n-1, 2} ... a_{n-1, n}
- Everything else is assigned during runtime
 */


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

int main (int argc, char** argv) {
    ofstream cnf(argv[1]);
    int n = atoi (argv[2]);

    map<pair<pair<int, int>, pair<int, int> >, int> vec_vars;
    int var_counter = n*n + 1;
    int clause_counter = 0;

    // (i1, j1) iterate over all starting points in the array
    for (int i1 = 0; i1 < n; ++i1) {
        for (int j1 = 0; j1 < n; ++j1) {
            // We start the second point at row i1 to avoid double-counting a pair of points
            for (int i2 = i1; i2 < n; ++i2) {
                // If (i2, j2) is in the same row as (i1, j1), start j2 at j1 + 1. 
                // If (i2, j2) is in a lower row, start j2 at 0.
                int jstart = (i1 == i2) ? j1 + 1 : 0;
                for (int j2 = jstart; j2 < n; ++j2) {

                    // auto it1 = vec_vars.find(make_pair(make_pair(i1, j1), make_pair(i2, j2)));
                    // auto it2 = vec_vars.find(make_pair(make_pair(i2, j2), make_pair(i1, j1)));
                    // if (it1 == vec_vars.end() && it2 == vec_vars.end()){
                        // cout << "Added (" << i1 << "," << j1 << "), (" << i2 << "," << j2 << ") with direction " << i2 - i1 << " " << j2 - j1 << "\n";
                    // }

                    // v_{(i1,j1),(i2,j2)} is true iff a_{i1,j1} and a_{i2, j2} are true
                    cnf << -arr_var(n, i1, j1) << " " << -arr_var(n, i2, j2) << " " << var_counter << " 0\n";
                    cnf << arr_var(n, i1, j1) << " " << -var_counter << " 0\n";
                    cnf << arr_var(n, i2, j2) << " " << -var_counter << " 0\n";

                    vec_vars[make_pair(make_pair(i1, j1), make_pair(i2, j2))] = var_counter;
                    ++var_counter;
                    clause_counter += 3;
                }
            }
        }
    }

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
    for (int x = 0; x < n; ++x) {
        for (int y = -(n-1); y < n; ++y) {
            if (x == 0 && y <= 0) {
                continue;
            }

            int lbi = 0;                //lower bound on the possible x-coordinates of the starting point
            int upi = n-x;              //upper bound on the possible x-coordinates of the starting point
            int lbj = max(0, -y);       //lower bound on the possible y-coordinates of the starting point
            int upj = min(n, n-y);      //upper bound on the possible y-coordinates of the starting point
            int num = (upi - lbi)*(upj - lbj);      //total number of vectors with this direction

            // cout << "\nx y: " << x << ", " << y << "\n";
            // cout << "i bounds: " << lbi << ", " << upi << "\n";
            // cout << "j bounds: " << lbj << ", " << upj << "\n";
            // cout << "num: " << num << "\n";

            int arr[num];
            int idx = 0;
            // Iterate through all possible starting points and add their DIMACS numbers to a vector
            for (int i = lbi; i < upi; ++i) {
                for (int j = lbj; j < upj; ++j) {
                    auto it = vec_vars.find(make_pair(make_pair(i, j), make_pair(i+x, j+y)));
                    // cout << "x y: " << x << ", " << y << "\n";
                    // cout << "i j:" << i << ", " << j << "\n";

                    if (it != vec_vars.end()){
                        // cout << it->second << "\n";
                        arr[idx] = it->second;
                        ++idx;
                    } else {
                        cout << "unexpected not found: (" << i << ", " << j << "), (" << i+x << ", " << j+y << ")\n";
                    }
                    
                    // v_{(i,j),(x,y)} is true iff a_{i,j} and a_{i+x, j+y} are true
                    // cnf << " -" << arr_var(n, i, j) << " -" << arr_var(n, i+x, j+x) << " -" << vec_var(n, i, j, i+x, j+x) << " 0\n";
                    // cnf << " " << arr_var(n, i, j) << " -" << vec_var(n, i, j, i+x, j+x) << " 0\n";
                    // cnf << " " << arr_var(n, i+x, j+x) << " -" << vec_var(n, i, j, i+x, j+x) << " 0\n";

                    // clause_counter += 3;
                }
            }
            at_most_one(cnf, arr, num, &var_counter);

            if (num <= 2){
                clause_counter += num - 1;
            } else {
                clause_counter += 3*num - 6;
            }
        }
    }

    cnf << "p cnf " << var_counter - 1 << ' ' << clause_counter << '\n';
    // It was too annoying to figure out how to store everything and then output it after this line
    // so I just manually move this last line to the first line before I run cadical
}