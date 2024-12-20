#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <set>
#include <assert.h>
#include <map>
#include <utility>
#include <algorithm>
#include <sstream>
using namespace std;
typedef pair<int,int> pii;

int arr_var (int n, int i, int j) {
    return n*i + j + 1;
}

void countCoordFreq(map<pii, int>& coord_freqs, vector<pii>& coords) {
    for (int i = 0; i < coords.size(); i++) {
        ++coord_freqs[coords[i]];
    }
}

int main(int argc, char* argv[]) {
    // Open the file specified by the first command-line argument
    ifstream file(argv[1]);
    if (!file) {
        cerr << "Error opening file: " << argv[1] << endl;
        return 1;
    }

    int n;
    int numSolutions;
    string line;

    // Read first line
    getline(file, line); 
    std::stringstream ss(line);
    ss >> n >> numSolutions;

    int i = 1; //Solution number
    map<pii, int> coord_freqs;
    vector< set<pii> > sol_coords;

    // Read each line from the file
    while (getline(file, line)) {
        // Use a stringstream to parse numbers from the line
        std::stringstream ss(line);
        int y;
        int x = 0;
        vector<pii> coords; // Vector to store the dots

        while (ss >> y) {
            coords.push_back(make_pair(x, y));
            ++x;
        }
        
        if (coords.size() != n) {
            cout << "Solution " << i << " does not have " << n << " coordinates\n";
        }

        countCoordFreq(coord_freqs, coords);

        set<pii> s;
        for (int i = 0; i < coords.size(); ++i) {
            pii p = make_pair(coords[i].first, coords[i].second);
            s.insert(p);
        }
        sol_coords.push_back(s);
        ++i;
    }
    if (i - 1 != numSolutions) {
        cout << "Expected " << numSolutions << " solutions but found " << i << " instead\n";
    }

    ofstream txt2(argv[2]);
    ofstream txt3(argv[3]);
    ofstream txt4(argv[4]);
    ofstream txt5(argv[5]);
    ofstream csv(argv[2]);
    csv << "freq, num coords, num solutions, product,\n";

    set<pii> coords_freq2;
    set<pii> coords_freq3;
    set<pii> coords_freq4;
    set<pii> coords_freq5;

    for (auto it = coord_freqs.begin(); it != coord_freqs.end(); ++it) {
        if (it->second == 2){
            coords_freq2.insert(make_pair(it->first.first, it->first.second));
        }
        if (it->second == 3){
            coords_freq3.insert(make_pair(it->first.first, it->first.second));
        }
        if (it->second == 4){
            coords_freq4.insert(make_pair(it->first.first, it->first.second));
        }
        if (it->second == 5){
            coords_freq5.insert(make_pair(it->first.first, it->first.second));
        }
    }

    int num_without2 = 0; //number of solutions not present in any solution
    int num_without23 = 0;
    int num_without234 = 0;
    int num_without2345 = 0;

    for (int i = 0; i < numSolutions; ++i) {
        bool is_without2 = true;
        for (auto it = coords_freq2.begin(); it != coords_freq2.end(); ++it){
            if (sol_coords[i].find(*it) != sol_coords[i].end()){
                is_without2 = false;
            }
        }

        bool is_without23 = is_without2;
        for (auto it = coords_freq3.begin(); it != coords_freq3.end(); ++it){
            if (sol_coords[i].find(*it) != sol_coords[i].end()){
                is_without23 = false;
            }
        }

        bool is_without234 = is_without23;
        for (auto it = coords_freq4.begin(); it != coords_freq4.end(); ++it){
            if (sol_coords[i].find(*it) != sol_coords[i].end()){
                is_without234 = false;
            }
        }

        bool is_without2345 = is_without234;
        for (auto it = coords_freq5.begin(); it != coords_freq5.end(); ++it){
            if (sol_coords[i].find(*it) != sol_coords[i].end()){
                is_without2345 = false;
            }
        }

        if (is_without2) {
            ++num_without2;
        }
        if (is_without23) {
            ++num_without23;
        }        
        if (is_without234) {
            ++num_without234;
        }  
        if (is_without2345) {
            ++num_without2345;
        }  
    }

    for (auto it = coords_freq2.begin(); it != coords_freq2.end(); ++it){
        txt2 << -arr_var(24, it->first, it->second) << " 0\n";
    }

    for (auto it = coords_freq3.begin(); it != coords_freq3.end(); ++it){
        txt3 << -arr_var(24, it->first, it->second) << " 0\n";
    }

    for (auto it = coords_freq4.begin(); it != coords_freq4.end(); ++it){
        txt4 << -arr_var(24, it->first, it->second) << " 0\n";
    }

    for (auto it = coords_freq5.begin(); it != coords_freq5.end(); ++it){
        txt5 << -arr_var(24, it->first, it->second) << " 0\n";
    }

    int num_points2 = coords_freq2.size();
    int num_points23 = coords_freq2.size() + coords_freq3.size();
    int num_points234 = coords_freq2.size() + coords_freq3.size() + coords_freq4.size();
    int num_points2345 = coords_freq2.size() + coords_freq3.size() + coords_freq4.size() + coords_freq5.size();

    csv << "2, " << num_points2 << ", " << num_without2 << ", " << num_points2*num_without2 <<  ",\n";
    csv << "3, " << num_points23 << ", " << num_without23 << ", " << num_points23*num_without23 << ",\n";
    csv << "4, " << num_points234 << ", " << num_without234 << ", " << num_points234*num_without234 << ",\n";
    csv << "5, " << num_points2345 << ", " << num_without2345 << ", " << num_points2345*num_without2345 << ",\n";

    return 0;
}