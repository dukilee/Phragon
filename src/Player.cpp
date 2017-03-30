#include "Player.h"
#include "Move.h"
#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

 

 
Player::Player(){
    di[0] = 1; di[1] = -1; di[2] =  0; di[3] = 0; di[4] =  1; di[5] = 1;
    dj[0] = 0; dj[1] =  0; dj[2] = -1; dj[3] = 1; dj[4] = -1; dj[5] = 1;
    int aux1[] = {-1, 0, 1, -1, 2, -2, 2, -1, 2, -1, 0, 1};
    int aux2[] = {-2, -2, -2, -1, -1, 0, 0, 1, 1, 2, 2, 2};
    for(int i = 0; i<12; i++){
        ddi[i] = aux1[i];
        ddj[i] = aux2[i];
    }
    NUM_GENES = 4;
    MAX_ANALYSED = 5000;
    MAX_SONS = 3;
    MIN_DEPTH = 3;
    MAX_DEPTH = 7;
 
    int aux = 1;
 
    minimum[MIN_DEPTH] = 1;
    for(int i = MIN_DEPTH-1; i>=0; i--){
        aux = aux*MAX_SONS;
        minimum[i] = aux + minimum[i+1];
    }
 
}
 
bool comp(Move a, Move b){
    if(a.first!=b.first) return a.first<b.first;
    if(a.second!=b.second) return a.second<b.second;
    return a.type<b.type;
}
 
bool compInc(Move a, Move b){
    return a.rating<b.rating;
}
bool compDec(Move a, Move b){
    return a.rating>b.rating;
}
 
void Player::calculatePossibleMoves(std::vector<Move>& resp, State* s, int id){
    resp.clear();
    std::vector<Move> aux;
    int ni, nj, v, nni, nnj;
    for(int i = 0; i<6; i++){
        for(int j = 0; j<7; j++){
            if(s->val[i][j]!=id) continue;
            v = -1;
            if(j & 1) v = 1;
            for(int g = 0; g<6; g++){
                ni = i + di[g]*v;
                nj = j + dj[g];
                if(ni>=0 && ni<6 && nj>=0 && nj<7 && s->val[ni][nj]==0){
                    aux.push_back(Move(ni, nj, 1, i, j));
                }
            }
            for(int g = 0; g<12; g++){
                ni = i + ddi[g]*v;
                nj = j + ddj[g];
                if(ni>=0 && ni<6 && nj>=0 && nj<7 && s->val[ni][nj]==0){
                    aux.push_back(Move(ni, nj, 2, i, j));
                }
            }
        }
    }
    if(aux.size()==0) return;
    sort(aux.begin(), aux.end(), comp);
    resp.push_back(aux[0]);
    for(int i = 1; i<aux.size(); i++){
        if(aux[i].first!=aux[i-1].first || aux[i].second!=aux[i-1].second
            || aux[i].type!=aux[i-1].type || aux[i].type==2){
            resp.push_back(aux[i]);
        }
    }
}
double Player::stateValue(State& s){
    features[0] = s.score[id] - state->score[id];
    features[1] = - s.score[opp] + state->score[opp];
    features[2] = s.score[id];
    features[3] = - s.score[opp];
 
    double result = 0;
    for(int i = 0; i<NUM_GENES; i++){
        result += features[i]*genetic[i];
    }
    return result;
}
 
 
double Player::play(State &s, int depth, int maxAnalysed){
    if(depth + tame>100){
        double ans;
        if(s.score[1]>s.score[2]) ans =  99999;
        else if(s.score[1]<s.score[2]) ans = -99999;
        else ans = 0;
        return ans;
    }

    int delta = maxAnalysed - actualAnalysed -1;
    if(depth<MIN_DEPTH && delta<=minimum[depth]){
        if(depth & 1) return -99999;
        return 99999;
     
    }
    int eachSon = delta/MAX_SONS;
    actualAnalysed++;
    int myId = actualAnalysed + delta - eachSon*MAX_SONS;
    // s.print();
    // getchar();
    if(eachSon<1 || depth>MAX_DEPTH){
        depth++;
        double aux = stateValue(s);
        // solvre(depth);
        // cout<<"Como jah eh a maior possivel, retornando"<<aux<<endl;
        return aux;
    
    }
    vector<Move> possibilities;
    if(depth & 1){
        // solvre(depth+1);
        // cout<<"Calculando as jogada para o jogador "<<opp<<" ...\n";
        calculatePossibleMoves(possibilities, &s, opp);
    }else{
        // solvre(depth+1);
        // cout<<"Calculando as jogada para o jogador "<<id<<" ...\n";
        calculatePossibleMoves(possibilities, &s, id);
    }
 
    if(possibilities.size()==0){
        // solvre(depth+1);
        // cout<<"Nao ha movimentos possiveis\n";
        double ans;
        if(s.score[1]>s.score[2]) ans =  99999-depth;
        else if(s.score[1]<s.score[2]) ans = -99999+depth;
        else ans = 0;
        return ans;
    } 
    State next(true);
    double ans, actual;
 
    int tam = possibilities.size();
    if(depth & 1){
        depth++;
        // solvre(depth);
        // cout<<"Vamos Retornar o minimo\n";
        ans = 999977;
        for(int i = 0; i<possibilities.size(); i++){
            next = s;
            next.makeMove(possibilities[i], opp, id);
            possibilities[i].rating = stateValue(next);
        }
        sort(possibilities.begin(), possibilities.end(), compInc);
        int i = 0;
        while(actualAnalysed<maxAnalysed && i<tam){
        // for(int i = 0; i<tam; i++){
            next = s;
            next.makeMove(possibilities[i], opp, id);
            maior = min(myId + (i+1)*eachSon, maxAnalysed);
            actual = play(next, depth, maior);
            if(actual<ans) ans = actual;
            i++;
        }
    }else{
        depth++;
        // solvre(depth);
        // cout<<"Vamos Retornar o maximo\n";
        ans = -999977;
        for(int i = 0; i<possibilities.size(); i++){
            next = s;
            next.makeMove(possibilities[i], id, opp);
            possibilities[i].rating = stateValue(next);
        }
        sort(possibilities.begin(), possibilities.end(), compDec);
        int i = 0;
        while(actualAnalysed<maxAnalysed && i<tam){
        // for(int i = 0; i<tam; i++){
            next = s;
            next.makeMove(possibilities[i], id, opp);
            maior = min(myId + (i+1)*eachSon, maxAnalysed);
            actual = play(next, depth, maior);
            if(actual>ans) ans = actual;
            i++;
        }
    
    }
    return ans;
}
 
Move Player::play(){
    actualAnalysed = 0;
    int eachSon = MAX_ANALYSED/MAX_SONS;
    int maior;
    int delta = MAX_ANALYSED - MAX_SONS*eachSon;
    calculatePossibleMoves(possibleMoves, state, id);
    if(possibleMoves.size()==0){
        return Move(-1, 0, 0);
    }
    State s(true);
    Move best = possibleMoves[0];
    double bestValue = -1000000;
    double actualValue;
    for(int i = 0; i<possibleMoves.size(); i++){
        s = *state;
        s.makeMove(possibleMoves[i], id, opp);
        possibleMoves[i].rating = stateValue(s);
    }
    sort(possibleMoves.begin(), possibleMoves.end(), compDec);
    int tam = possibleMoves.size();
    int i = 0;
    while(actualAnalysed<MAX_ANALYSED && i<tam){
        s = *state;
 
        s.makeMove(possibleMoves[i], id, opp);
        maior = min(delta+(i+1)*eachSon, MAX_ANALYSED);
        actualValue = play(s, 1, maior);
        if(actualValue>bestValue){
            bestValue = actualValue;
            best = possibleMoves[i];
        }
        i++;
    }
    return best;
}