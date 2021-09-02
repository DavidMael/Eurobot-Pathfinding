#include "../pathfinding.hpp"
#include <iostream>
#include <chrono>

using namespace std;

int main(){

    int lines = 22;
    char node_types [lines];
    float x_coord [lines];
    float y_coord [lines];
    int red_cups [lines];
    int green_cups [lines];
    
    int windsocks [2];

    string fileName = "test/node_info.txt";

    processNodes(node_types, x_coord, y_coord, red_cups, green_cups, windsocks, lines, fileName.c_str());

    //start at P
    int startNode = 0;

    //0.5 m/s over 100s
    float totalTime = 100;
    float avgSpeed = 500;

    unordered_map<Segment, float, SegmentHasher> distances = computeDistances(x_coord, y_coord, lines);

    cupTracker cups = {0, 0, 0, 0, 0, 0};

    bool consumed [lines] = {0};
    nodeField nodes = {lines, red_cups, green_cups, node_types};

    //finish at N
    int endpoint = 2;
    GameSituation game = GameSituation(endpoint, windsocks, nodes, distances, avgSpeed);

    clock_t start, end;
    start = clock();
    solution sol = game.solver(startNode, totalTime, cups, consumed);
    end = clock();
    cout<<"execution time: "<<(float(end-start))/CLOCKS_PER_SEC<<" s"<<endl;

    cout<<"points: "<<sol.points<<", distance: "<<sol.distance<<" mm\nnodes traversed: ";

    for(int i=0; i<sol.nodes.size(); i++){
        cout<<sol.nodes[i]+1<<" ";
    }
    cout<<endl;
}