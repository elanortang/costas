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


void countVecOccurrence(map<pii, int>& vec_coord_freqs, vector<pii>& coords, pii& vec) {
    for (int dot1 = 0; dot1 < coords.size() - 1; dot1++) {
        for (int dot2 = dot1 + 1; dot2 < coords.size(); dot2++) {
            int dx = coords[dot2].first - coords[dot1].first;
            int dy = coords[dot2].second - coords[dot1].second;

            int dotWithSmallerXCoord = dot1;
            if (dx < 0) {
                dx = -dx;
                dy = -dy;
                dotWithSmallerXCoord = dot2;
            }

            if (dx == vec.first && dy == vec.second) {
                ++vec_coord_freqs[coords[dotWithSmallerXCoord]];
            }
        }
    }
}

bool countVecFreq(map<pii, int>& freqs, vector<pii>& coords) {
    for (int dot1 = 0; dot1 < coords.size() - 1; dot1++) {
        for (int dot2 = dot1 + 1; dot2 < coords.size(); dot2++) {
            int dx = coords[dot2].first - coords[dot1].first;
            int dy = coords[dot2].second - coords[dot1].second;
            if (dx > 0){
                ++freqs[make_pair(dx,dy)];
            } else if (dx < 0) {
                ++freqs[make_pair(-dx,-dy)];
            } else {
                cout << "x displacement is 0\n";
                return false;
            }
        }
    }
    return true;
}

int main(int argc, char* argv[]) {
    // Open the file specified by the first command-line argument
    ifstream file(argv[1]);
    if (!file) {
        cerr << "Error opening file: " << argv[1] << endl;
        return 1;
    }
    int dx0 = atoi(argv[3]);
    int dy0 = atoi(argv[4]);

    pii vec = make_pair(dx0, dy0);

    int n;
    int numSolutions;
    string line;

    // Read first line
    getline(file, line); 
    std::stringstream ss(line);
    ss >> n >> numSolutions;

    int i = 1; //Solution number
    map<pii, int> freqs; //Map to store frequencies of vectors
    map<pii, int> vec_coord_freqs;

    // Read each line from the file
    while (getline(file, line)) {
        // Use a stringstream to parse numbers from the line
        std::stringstream ss(line);
        int y;
        int x = 0;
        vector<pii> coords; // Vector to store the dots

        bool interested = false;

        while (ss >> y) {
            coords.push_back(make_pair(x, y));
            ++x;
            if (x == 0 && y == 11) {
                interested = true;
            }
        }
        
        if (coords.size() != n) {
            cout << "Solution " << i << " does not have " << n << " coordinates\n";
        }

        if (interested) {
            cout << "Solution " << i << " has dot at (0, 11)\n";
        }

        if (!countVecFreq(freqs, coords)) {
            cout << "Solution " << i << " does not have distinct vectors\n";
        }
        countVecOccurrence(vec_coord_freqs, coords, vec);

        ++i;
    }
    if (i - 1 != numSolutions) {
        cout << "Expected " << numSolutions << " solutions but found " << i << " instead\n";
    }

    ofstream csv(argv[2]);
    csv << "x-coord of first dot, y-coord of first dot, freq\n";

    int total = 0;

    for (auto it = vec_coord_freqs.begin(); it != vec_coord_freqs.end(); ++it) {
        csv << it->first.first << ", " << it->first.second << ", " << it->second << ",\n";
        total += it->second;
    }

    // for (int x = 0; x < n; ++x) {
    //     for (int y = 0; y < n; ++y) {
    //         if (vec_coord_freqs.find(make_pair(x, y)) == vec_coord_freqs.end()) {
    //             cout << x << " " << y << "\n";
    //         }
    //     }
    // }

    cout << "Total frequency count: " << total << "\n";
    cout << "Expected frequency count: " << freqs[vec] << "\n";

    return 0;
}