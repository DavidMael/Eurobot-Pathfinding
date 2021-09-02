#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <unordered_map>
#include <math.h>
#include "pathfinding.hpp"

using namespace std;

#define max_windsocks 2

//the first 4 values of the 5 first arrays should correspond to nodes P, H, N, S
void processNodes(char node_types [], float x_coord [], float y_coord [], int red_cups [], int green_cups [], int windsock_indices [], int lines, const char nodeFile []){

    //read type, coordinates and cup content of each node
    //produce array of distances between each node

    //indices of the two windsock nodes
    int idx = 0;

    FILE *description = fopen(nodeFile, "r");

    for(int i=0; i<lines; i++){
      fscanf(description, "%c", &node_types[i]);
      fscanf(description, ",");
      fscanf(description, "%f", &x_coord[i]);
      fscanf(description, ",");
      fscanf(description, "%f", &y_coord[i]);
      fscanf(description, ",");
      fscanf(description, "%d", &red_cups[i]);
      fscanf(description, ",");
      fscanf(description, "%d", &green_cups[i]);
      fscanf(description, "\n");

      if (node_types[i] == 'W'){
        if(idx < max_windsocks){
          windsock_indices[idx] = i;
        }
        idx++;
      }
    }

    assert(idx == max_windsocks && "unexpected number of windsocks");

}

//map the length of every path, including equivalents [a, b] and [b, a]
//this is unlikely to provide any performance benefit but simplifies sorting out of code accessing the map
unordered_map<Segment, float, SegmentHasher> computeDistances(float x_coords [], float y_coords [], int lines){

    unordered_map<Segment, float, SegmentHasher> distances;

    for(int a=0; a<lines; a++){
      for(int b=0; b<lines; b++){
        if(a != b){
          Segment arrayKey = {a, b};
          distances.insert( { arrayKey, sqrt( pow(x_coords[a]-x_coords[b], 2) + pow(y_coords[a]-y_coords[b], 2) ) } );
        }
      }
    }

    return distances;
}

//used to determine which is the other index, may not be worth a dedicated function
int otherWindsockIdx(int idx, int indices []){
  assert(idx == indices[0] || idx == indices[1]);

  if(idx == indices[0] ){
    return indices[1];
  } else {
    return indices[0];
  }

}