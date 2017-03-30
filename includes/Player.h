#ifndef PLAYER_H
#define PLAYER_H

#include "State.h"
#include "Move.h"
#include <iostream>
#include <vector>
class Player{
public:
    Player();
 
    void calculatePossibleMoves(std::vector<Move> &, State*, int);
    double stateValue(State&);
 
    Move play();
    double play(State&, int, int);
    State* state;
 
    int di[6], dj[6], id, opp, ddi[12], ddj[12];
    void setGenes(double x, int i){
        genetic[i] = x;
    }
    double genetic[10];
    double features[13];
    int NUM_GENES;
    int MAX_ANALYSED;
    int MAX_SONS;
    int MIN_DEPTH;
    int MAX_DEPTH;
    double fitness;
    int generation;
    int matches;
    int code;
    int actualAnalysed;
    int population;
    int deposit;
    int tame;
    std::pair<int, int> score;
    std::vector<Move > possibleMoves;
private:
    int maior;
    int minimum[20];
};

#endif