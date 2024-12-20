#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>
#include <map>
#include <utility>
#include <algorithm>
#include <sstream>
using namespace std;
typedef pair<int,int> pii;


void countCoordFreq(map<pii, int>& coord_freqs, vector<pii>& coords) {
    for (int i = 0; i < coords.size(); i++) {
        ++coord_freqs[coords[i]];
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

//first argument: file name. 
//second argument: n
int main(int argc, char* argv[]) {
    
    // Check if a filename was provided
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <input filename> <output filename>" << endl;
        return 1;
    }

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
    map<pii, int> freqs; //Map to store frequencies of vectors
    map<pii, int> coord_freqs;

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

        if (!countVecFreq(freqs, coords)) {
            cout << "Solution " << i << " does not have distinct vectors\n";
        }

        countCoordFreq(coord_freqs, coords);
        ++i;
    }
    if (i - 1 != numSolutions) {
        cout << "Expected " << numSolutions << " solutions but found " << i << " instead\n";
    }

    ofstream csv(argv[2]);
    // csv << "dx, dy, freq\n";
    csv << "x-coord of first dot, y-coord of first dot, freq\n";
    // csv << "freq, num coords,\n";

    int total = 0;
    vector<int> num_per_freq(19, 0);

    for (auto it = coord_freqs.begin(); it != coord_freqs.end(); ++it) {
        csv << it->first.first << ", " << it->first.second << ", " << it->second << ",\n";
        ++num_per_freq[it->second];
        total += it->second;
    }
    
    // for (int i = 0; i < 19; ++i) {
    //     csv << i << ", " << num_per_freq[i] << ",\n";
    // }

    // Calculate missing vectors
    // for (int dx = 1; dx < n; ++dx) {
    //     for (int dy = -(n-1); dy < n; ++dy) {
    //         if (dy == 0) {
    //             continue;
    //         }

    //         if (freqs.find(make_pair(dx, dy)) == freqs.end()) {
    //             cout << dx << " " << dy << "\n";
    //         }
    //     }
    // }

    // Calculate missing coordinates
    // for (int x = 0; x < n; ++x) {
    //     for (int y = 0; y < n; ++y) {
    //         if (coord_freqs.find(make_pair(x, y)) == coord_freqs.end()) {
    //             cout << x << " " << y << "\n";
    //         }
    //     }
    // }

    cout << "Total frequency count: " << total << "\n";
    // cout << "Expected frequency count: " << numSolutions*n*(n-1)/2 << "\n";
    cout << "Expected frequency count: " << numSolutions*n << "\n";

    return 0;
}