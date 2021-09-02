#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <stdlib.h>
#include <unordered_map>
#include <vector>

using namespace std;

const int GLOBAL_LH_COST = 0;
const int GLOBAL_W_COST = 0;
const int GLOBAL_PICKUP_COST = 0;
const int GLOBAL_DROPOFF_COST = 0;

const int GLOBAL_P_CHANNEL_CAPACITY = 5;
const int GLOBAL_H_CHANNEL_CAPACITY = 4;

struct Segment{

  int start;
  int end;

  bool operator==(const Segment &other ) const{
    return start == other.start && end == other.end;
  }

};

//used by the unordered_map to hash segment between nodes
struct SegmentHasher{
  size_t operator()(const Segment (&segment)) const{

        return ( hash<int>()(segment.start) ^ hash<int>()(segment.end) << 1 );
  }
};

//don't know about this handling of constants, may want to put them in a constants class or the solution class idk
struct cupTracker{
    int heldRed;
    int heldGreen;
    int pRed;
    int pGreen;
    int hRed;
    int hGreen;
    const int pChannelLimit = 5;
    const int hChannelLimit = 4; 
};

struct solution{
    int points;
    vector<int> nodes;
    float distance;
};

//wrapper for arrays describing the unchanging qualities of the nodes
struct nodeField{
    int nodeCount;
    int * redCups;
    int * greenCups;
    char * types;
};

class GameSituation{

    const int P = 0;
    const int H = 1;
    const int N = 2;
    const int S = 3;

    int compassDir;
    int * windsockIndices;
    nodeField nodes;
    unordered_map<Segment, float, SegmentHasher> distances;

    //in mm/s
    float avgSpeed;

    public:
      GameSituation(int compassDir, int * windsockIndices, nodeField nodes, unordered_map<Segment, float, SegmentHasher> distances, float avgSpeed);

      int nodePoints (int currentNode, cupTracker &cups, bool consumed []);
      
      //remaining time in s
      vector<int> recursionDirector (int currentNode, float remainingTime, bool consumed []);

      solution solver(int currentNode, float remainingTime, cupTracker cups, bool consumedIn []);

};

//the first 4 values of the 5 first arrays should correspond to nodes P, H, N, S
void processNodes(char node_types [], float x_coord [], float y_coord [], int red_cups [], int green_cups [], int windsock_indices [], int lines, const char nodeFile []);

//map the length of every path, including equivalents [a, b] and [b, a]
//this provides no performance benefit but simplifies sorting out of code accessing the map
unordered_map<Segment, float, SegmentHasher> computeDistances(float x_coords [], float y_coords [], int lines);

//used to determine which is the other index
int otherWindsockIdx(int idx, int indices []);

#endif