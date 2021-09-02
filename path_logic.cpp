#include <utility>
#include <vector>
#include <iostream>
#include "pathfinding.hpp"

using namespace std;

GameSituation::GameSituation (int compassDir, int * windsockIndices, nodeField nodes, unordered_map<Segment, float, SegmentHasher> distances, float avgSpeed){
    this->compassDir = compassDir;
    this->windsockIndices = windsockIndices;
    this->nodes = nodes;
    this->distances = distances;
    this->avgSpeed = avgSpeed;
}

//assumes node is not consumed, computes new points given context (cups in robot and ports) 
int GameSituation::nodePoints (int currentNode, cupTracker &cups, bool consumed []) {

    char nodeType = this->nodes.types[currentNode];

    if(nodeType == 'L'){
        return 13;
    } else if(nodeType == 'W'){
        int otherWindsockIndex = otherWindsockIdx(currentNode, this->windsockIndices);
        if( consumed[otherWindsockIndex] ){
            return 10;
        } else {
            return 5;
        }
    } else if(nodeType == 'P'){
        int total = 0;
        int channelSlotsRed = cups.pChannelLimit - cups.pRed;
        int channelSlotsGreen = cups.pChannelLimit - cups.pGreen;

        if(channelSlotsRed != 0){
            int singleGreen = max(0, cups.pGreen - cups.pRed);
            int placeInChannel = min(cups.heldRed, channelSlotsRed);
            int newPairs = min(singleGreen, placeInChannel);

            total += (2*placeInChannel + 2*newPairs);
            cups.heldRed -= placeInChannel;
            cups.pRed += placeInChannel;
        } 
        
        if(cups.heldRed != 0){
            //no bonus, cups placed in port but not channel
            total += cups.heldRed;
            cups.heldRed = 0;
        }

        if(channelSlotsGreen != 0){
            int singleRed = max(0, cups.pRed - cups.pGreen);
            int placeInChannel = min(cups.heldGreen, channelSlotsGreen);
            int newPairs = min(singleRed, placeInChannel);

            total += (2*placeInChannel + 2*newPairs);
            cups.heldGreen -= placeInChannel;
            cups.pGreen += placeInChannel;
        } 
        
        if(cups.heldGreen != 0){
            //no bonus, cups placed in port but not channel
            total += cups.heldGreen;
            cups.heldGreen = 0;
        }

        return total;

    } else if(nodeType == 'H'){

        int total = 0;
        int channelSlotsRed = cups.hChannelLimit - cups.hRed;
        int channelSlotsGreen = cups.hChannelLimit - cups.hGreen;

        if(channelSlotsRed != 0){
            int singleGreen = max(0, cups.hGreen - cups.hRed);
            int placeInChannel = min(cups.heldRed, channelSlotsRed);
            int newPairs = min(singleGreen, placeInChannel);

            total += (2*placeInChannel + 2*newPairs);
            cups.heldRed -= placeInChannel;
            cups.hRed += placeInChannel;
        } 
        
        if(cups.heldRed != 0){
            //no bonus, cups placed in port but not channel
            total += cups.heldRed;
            cups.heldRed = 0;
        }

        if(channelSlotsGreen != 0){
            int singleRed = max(0, cups.hRed - cups.hGreen);
            int placeInChannel = min(cups.heldGreen, channelSlotsGreen);
            int newPairs = min(singleRed, placeInChannel);

            total += (2*placeInChannel + 2*newPairs);
            cups.heldGreen -= placeInChannel;
            cups.hGreen += placeInChannel;
        }
        
        
        if(cups.heldGreen != 0){
            //no bonus, cups placed in port but not channel
            total += cups.heldGreen;
            cups.heldGreen = 0;
        }


        return total;

    } else {
        return 0;
    }

}

//determine next nodes for recursive solver calls
vector<int> GameSituation::recursionDirector (int currentNode, float remainingTime, bool consumed []){

    vector<int> validNext;
    float pathLength;

    for(int i=0; i<nodes.nodeCount; i++){

        if(i != currentNode && !consumed[i] && i != this->N && i != this->S){

            if( distances.at({currentNode, i}) <= 810 ){

                if(nodes.types[i] == 'C'){

                    //P, H, N, S are expected to be at indices 0,1,2,3
                    float pathLengthP = this->distances.at({currentNode, i}) + this->distances.at({i, this->P}) + this->distances.at({this->P, this->compassDir});
                    float pathLengthH = this->distances.at({currentNode, i}) + this->distances.at({i, this->S}) + this->distances.at({this->S, this->compassDir});

                    pathLength = min(pathLengthP, pathLengthH);
                } else {
                    pathLength = this->distances.at( {currentNode, i} ) + this->distances.at( {i, this->compassDir} );
                }

                if (pathLength <= remainingTime * this->avgSpeed){
                    validNext.push_back(i);
                } 
            }

        }
            
    }

    return validNext;
}

//P, H, N, S are expected to be at indices 0,1,2,3
solution GameSituation::solver(int currentNode, float remainingTime, cupTracker cups, bool consumedIn []){
    
    cups.heldGreen += nodes.greenCups[currentNode];
    cups.heldRed += nodes.redCups[currentNode];

    int nodeScore = this->nodePoints(currentNode, cups, consumedIn);

    bool consumed [this->nodes.nodeCount];

    memcpy(consumed, consumedIn, this->nodes.nodeCount * sizeof(bool));

    if(this->nodes.types[currentNode] == 'L'){
        remainingTime -= GLOBAL_LH_COST;
    } else if (this->nodes.types[currentNode] == 'W'){
        remainingTime -= GLOBAL_W_COST;
    } else if (this->nodes.types[currentNode] == 'C'){
        remainingTime -= GLOBAL_PICKUP_COST * (nodes.greenCups[currentNode] + nodes.redCups[currentNode]);
    } else {
        remainingTime -= GLOBAL_DROPOFF_COST * (cups.heldGreen + cups.heldRed);
    }

    consumed[currentNode] = true;

    vector<int> nextNodes = this->recursionDirector(currentNode, remainingTime, consumed);

    int numOptions = nextNodes.size();
    if(numOptions == 0){
        vector<int> traversedNodes;
        float travelDistance;
        //add points from port
        if(currentNode != this->P){

            if( distances.at({currentNode, this->P}) + this->distances.at({this->P, this->compassDir}) <= remainingTime * this->avgSpeed ){
                traversedNodes.push_back(currentNode);
                traversedNodes.push_back(this->P);
                nodeScore += nodePoints(this->P, cups, consumed);
                traversedNodes.push_back(this->compassDir);
                travelDistance = this->distances.at({currentNode, this->P}) + distances.at({this->P, this->compassDir});
            } else {
                traversedNodes.push_back(currentNode);
                traversedNodes.push_back(this->compassDir);
                travelDistance = this->distances.at({currentNode, this->compassDir});
            }
        } else {
            traversedNodes.push_back(currentNode);
            traversedNodes.push_back(this->compassDir);
            travelDistance = this->distances.at({currentNode, this->compassDir}); 
        }
        
        return {nodeScore, traversedNodes, travelDistance};

    } else {

        solution topSolution = {0, {}, 0};
        solution newSolution;
        float timeAfter;
        int option;
        int retainedOption;
        
        for(int i=0; i<numOptions; i++){
            option = nextNodes[i];
            timeAfter = remainingTime - this->distances.at({currentNode, option}) /this->avgSpeed;
            newSolution = solver(option, timeAfter, cups, consumed);
            newSolution.distance += this->distances.at({currentNode, option});

            if(newSolution.points > topSolution.points 
            || newSolution.points == topSolution.points && newSolution.distance < topSolution.distance
            || topSolution.points == 0 ){
                topSolution = newSolution;
                retainedOption = option;
            }
        }

        topSolution.points += nodeScore;
        topSolution.nodes.insert(topSolution.nodes.begin(), currentNode);

        return topSolution;
    }

}