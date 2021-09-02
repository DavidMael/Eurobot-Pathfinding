#include "../pathfinding.hpp"
#include <iostream>
#include <cassert>

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

    cupTracker cups = {0, 0, 0, 0, 0, 0, 0, 0};

    bool consumed [lines];

    nodeField nodes = {lines, red_cups, green_cups, node_types};

    unordered_map<Segment, float, SegmentHasher> distances = computeDistances(x_coord, y_coord, lines);

    float avgSpeed = 500;

    GameSituation game = GameSituation(2, windsocks, nodes, distances, avgSpeed);

    //lighthouse
    int points = game.nodePoints (6, cups, consumed);
    assert(points == 13);
    
    //test windsocks
    points = game.nodePoints (4, cups, consumed);
    assert(points == 5);
    points = game.nodePoints (5, cups, consumed);
    assert(points == 5);

    consumed [4] = true;
    points = game.nodePoints (5, cups, consumed);
    assert(points == 10);

    consumed [4] = false;
    consumed [5] = true;
    points = game.nodePoints (4, cups, consumed);
    assert(points == 10);

    //test cups
    points = game.nodePoints (0, cups, consumed);
    assert(points == 0);

    cupTracker cupsa = {2, 2, 0, 0, 0, 0};
    points = game.nodePoints (0, cupsa, consumed);
    assert(points == 12);

    cupTracker cupsb = {2, 1, 0, 0, 0, 0};
    points = game.nodePoints (0, cupsb, consumed);
    assert(points == 8);

    cupTracker cupsC = {2, 2, 0, 0, 0, 0};
    points = game.nodePoints (1, cupsC, consumed);
    assert(points == 12);

    cupTracker cupsD = {2, 1, 0, 0, 0, 0};
    points = game.nodePoints (1, cupsD, consumed);
    assert(points == 8);

        //cups already in ports

    cupTracker cupsE = {2, 2, 4, 4, 0, 0};
    points = game.nodePoints (0, cupsE, consumed);
    assert(points == 8);

    cupTracker cupsF = {2, 1, 4, 4, 0, 0};
    points = game.nodePoints (0, cupsF, consumed);
    assert(points == 7);

    cupTracker cupsG = {2, 2, 0, 0, 0, 3};
    points = game.nodePoints (1, cupsG, consumed);
    assert(points == 11);

    cupTracker cupsH = {2, 1, 0, 0, 0, 3};
    points = game.nodePoints (1, cupsH, consumed);
    assert(points == 10);

        //cup persistence
    assert(cupsG.heldGreen == 0);

    assert(cupsG.heldRed == 0);

    assert(cupsG.hGreen == 4);

    assert(cupsG.hRed == 2);

    cupsG.heldGreen += 1;
    points = game.nodePoints (0, cupsG, consumed);
    assert(points == 2);

    //specific scenario test
    cupTracker cupsT = {5, 4, 0, 0, 0, 0};
    points = game.nodePoints (0, cupsT, consumed);
    assert(points == 26);

}