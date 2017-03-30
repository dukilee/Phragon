#include "Judge.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>

using namespace std;

typedef std::pair<int, int> ii;

Judge::Judge(Player* p1, Player* p2){
    this->p1 = p1;
    p1->state = &state;
    this->p2 = p2;
    p2->state = &state;
}
double Judge::play(bool show, Player* p1, Player* p2){
	show = false;
	printf(".");
    score[1] = score[2] = 2;
    State outro;
    state = outro;

	if(show){
		printf("(%d:%d, %.1f, %d): ", p1->generation, p1->population, p1->fitness, p1->id);
		for(int i = 0; i<4; i++){
			cout<<p1->genetic[i]<<" ";
		}
		cout<<endl;
		printf("(%d:%d, %.1f, %d): ", p2->generation, p2->population, p2->fitness, p2->id);
		for(int i = 0; i<4; i++){
			cout<<p2->genetic[i]<<" ";
		}
		cout<<endl;
	}


	ii jogada;
	int unfilled = 38;
	int x;
	Move aux;
	int tempo;
	for(int i = 0; i<50; i++){
		tempo = i;
		p1->tame = 2*i;
		aux = p1->play();
		x = state.makeMove(aux, 1, 2);
		if(x>=0){
			score[1] += x;
			score[2] -= x;
			if(aux.type == 1) {score[1]++; unfilled--;}
		} else break;
		if(unfilled<=0) break;
		if(show){
			printf("Jogada %d do P1: (%d, %d, %d, %d, %d) tendo efeito (%d)\n",
				i+1, aux.first, aux.second, aux.of, aux.os, aux.type, x);
			state.print();
			printf("\t\t %d X %d \n\n", score[1], score[2]);
			getchar();
		}
		p2->tame = 2*i+1;
		aux = p2->play();
		x = state.makeMove(aux, 2, 1);
		if(x>=0){
			score[2] += x;
			score[1] -= x;
			if(aux.type == 1) {
			score[2]++; unfilled--;}
		}else break;
		if(unfilled<=0) break;
		if(show){
			printf("Jogada %d do P2: (%d, %d, %d, %d, %d) tendo efeito (%d)\n",
				i+1, aux.first, aux.second, aux.of, aux.os, aux.type, x);
			state.print();
			printf("\t\t %d X %d\n\n", score[1], score[2]);
			getchar();
		}
	}
			printf("FINAL: \t\t %d X %d\n\n", score[1], score[2]);
	// cout<<score[1]<<"X"<<score[2]<<endl;
	if(score[1]>score[2]) return 0.5;
	if(score[1]==score[2]) return 0.25;
	return 0;


}

void Judge::run(bool show){
	p1->id = p2->opp = 1;
	p1->opp = p2->id = 2;
	double S = play(show, p1, p2);
	
	p1->id = p2->opp = 2;
	p1->opp = p2->id = 1;
	S = S + 0.5 - play(show, p2, p1);

	double k, a = p1->fitness, b = p2->fitness;
	double E1 = 1/(1 + pow(10, (p2->fitness - p1->fitness)/400));
	if(p1->matches<20) k = 50;
	else if(p1->fitness>2400) k = 10;
	else if(p1->fitness>2000) k = 20;
	else k = 30;
	p1->fitness += k*(S - E1);

	if(p2->matches<20) k = 50;
	else if(p1->fitness>2400) k = 10;
	else if(p1->fitness>2000) k = 20;
	else k = 30;
	p2->fitness -= k*(S - E1);
	p1->matches++;
	p2->matches++;

}
