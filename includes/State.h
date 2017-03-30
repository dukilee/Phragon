#ifndef STATE_H
#define STATE_H

#include "Move.h"
#include <cstdio>
#include <iostream>

class State{
public:
	int val[10][10];
	int di[6], dj[6], ni, nj;
	State(){
	    di[0] = 1; di[1] = -1; di[2] =  0; di[3] = 0; di[4] =  1; di[5] = 1;
	    dj[0] = 0; dj[1] =  0; dj[2] = -1; dj[3] = 1; dj[4] = -1; dj[5] = 1;
		for(int i = 0; i<7; i++){
			for(int j = 0; j<7; j++){
				val[i][j] = 0;
			}
		}
		val[0][0] = 1;
		val[5][6] = 1;
		val[5][0] = val[0][6] = 2;
		score[1] = score[2] = 2;
	}
	State(bool b){
	    di[0] = 1; di[1] = -1; di[2] =  0; di[3] = 0; di[4] =  1; di[5] = 1;
	    dj[0] = 0; dj[1] =  0; dj[2] = -1; dj[3] = 1; dj[4] = -1; dj[5] = 1;
		score[1] = score[2] = 2;
	}

	void print(){
		for(int i = 0; i<6; i++){
			for(int j = 0; j<7; j++){
				if(val[i][j]==0) printf("_ "); else
				printf("%d ", val[i][j]);
			}
			printf("\n");
		}
		// printf("\t\t%dx%d\n", score[1], score[2]);
	}

	int makeMove(Move& jogada, int id, int adv){
		if(jogada.first<0 || val[jogada.first][jogada.second]!=0) return -1;
		val[jogada.first][jogada.second] = id;
		
		if(jogada.type==2) val[jogada.of][jogada.os] = 0;
		else score[id]++;
		
		int v = -1, ni, nj;
		int ans = 0;
		if(jogada.second & 1) v*=-1;
		for(int g = 0; g<6; g++){
			ni = jogada.first + di[g]*v;
			nj = jogada.second + dj[g];
			if(ni>=0 && ni<6 && nj>=0 && nj<7 && val[ni][nj]==adv){
				val[ni][nj] = id;
				ans++;
				score[id]++;
				score[adv]--;
			} 
		}
		return ans;
	}
	int score[3];


};
#endif