#include "../pathfinding.hpp"
#include <iostream>

using namespace std;

int main(){

    int lines = 4;
    char node_types [lines];
    float x_coord [lines];
    float y_coord [lines];
    int red_cups [lines];
    int green_cups [lines];
    
    int windsocks [2];

    string fileName = "test/test_node_info.txt";

    processNodes(node_types, x_coord, y_coord, red_cups, green_cups, windsocks, lines, fileName.c_str());

    for(int i=0; i<lines; i++){
      cout<<node_types[i]<<" "<<x_coord[i]<<" "<<y_coord[i]<<" "<<red_cups[i]<<" "<<green_cups[i]<<endl;
    }

    cout<<endl;

    cout<<windsocks[0]<<" "<<windsocks[1]<<endl;

    cout<<endl;

    unordered_map<Segment, float, SegmentHasher> distances = computeDistances(x_coord, y_coord, lines);

    for (auto& x: distances)
        cout << x.first.start << "-" << x.first.end << ": " << x.second << std::endl;

}