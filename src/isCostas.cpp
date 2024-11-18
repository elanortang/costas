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
#include <unordered_map>
using namespace std;
typedef pair<int,int> pii;
typedef vector<int> vi;
typedef long double ld;
typedef long long ll;


bool isPermutationArray(vector<pii> vec, int n) {
    if (vec.size() != n) {
        return false;
    }
    unordered_set<int> rowsWithDots;
    unordered_set<int> colsWithDots;
    for (int i = 0; i < n; i++) {
        int dotx = vec[i].first;
        int doty = vec[i].second;
        if (rowsWithDots.find(dotx) != rowsWithDots.end() || colsWithDots.find(doty) != colsWithDots.end()) {
            return false;
        }
        rowsWithDots.insert(dotx);
        colsWithDots.insert(doty);
    }
    return true;
}

bool hasDistinctVectors(vector<pii> vec) {
    map<pii,pii> displacementMap; //map displacement vector to row
    for (int dot1 = 0; dot1 < vec.size() - 1; dot1++) {
        for (int dot2 = dot1 + 1; dot2 < vec.size(); dot2++) {
            int x = vec[dot2].first - vec[dot1].first;
            int y = vec[dot2].second - vec[dot1].second;
            if (displacementMap.find(make_pair(x,y)) != displacementMap.end()) {
                cout << "collision!" << endl;
                cout << "displacement vector: " << endl;
                cout << "x: "<< x << ", y: "<< y << endl;
                cout << "collisions: " << endl;
                cout << "first dot: " << displacementMap[make_pair(x,y)].first << ", "<< displacementMap[make_pair(x,y)].second << endl;
                cout << "second dot: " << vec[dot1].first << ", " << vec[dot1].second << endl;
                return false;
            } else if (displacementMap.find(make_pair(-x,-y)) != displacementMap.end()) {
                cout << "collision!" << endl;
                cout << "displacement vector: " << endl;
                cout << "x: "<< x << ", y: "<< y << endl;
                cout << "collisions: " << endl;
                cout << "first row: " << displacementMap[make_pair(-x,-y)].first << endl;
                cout << "second row: " << vec[dot1].first << endl;
                return false;
            } else {
                displacementMap[make_pair(x,y)] = vec[dot1];
            }
        }
    }
    return true;
}

pii var_arr(int n, int k) {
    for (int i = n-1; i >= 0; i--) {
        if (n*i < k) {
            int j = k-1-n*i;
            return make_pair(i,j);
        }
    }
    cout << "failure to make pair!" << endl;
    return make_pair(-1,-1);
}
//first argument: file name. 
//second argument: n
int main(int argc, char* argv[]) {
    
    // Check if a filename was provided
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    // Open the file specified by the first command-line argument
    ifstream file(argv[1]);
    if (!file) {
        cerr << "Error opening file: " << argv[1] << endl;
        return 1;
    }

    int n = stoi(argv[2]);
    vector<int> numbers; // Vector to store the parsed integers
    string line;

    // Read each line from the file
    while (std::getline(file, line)) {
        // Use a stringstream to parse numbers from the line
        std::stringstream ss(line);
        int number;
        char v;
        ss >> v;
        while (ss >> number) {
            numbers.push_back(number);
        }
    }
    file.close();

    vector<pii> vec; // Vector to store the dots
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (numbers[i*n + j] > 0) {
                vec.push_back(var_arr(n,numbers[i*n + j])); //only store number if it is a dot
                cout << "1 "; 
            } else {
                cout << "0 ";
            }
        }
        cout << endl;
    }

    if (!isPermutationArray(vec,n)){
        cout << "input is not a costas array: fails permutation array test" << endl;
    } else if (!hasDistinctVectors(vec)) {
        cout << "input is not a costas array: fails distinct vectors test" << endl;
    } else {
        cout << "input is a costas array" << endl;
    }
    return 0;
}