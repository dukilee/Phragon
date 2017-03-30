#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>

#include "Judge.h"
#include "Player.h"
#include "State.h"
#include <cmath>
#include <vector>

using namespace std;

bool comp(Player a, Player b){
    return a.fitness>b.fitness;
}

int tempo;
int dead;
int code = 1;
const int NUM_GENERATIONS = 10100;
const int NUM_GENES = 4;
const int NUM_POPULATIONS = 1;
const int NUM_PLAYERS = NUM_POPULATIONS*12;
const int NUM_PLAYERS_2 = NUM_PLAYERS/2;
const int MATCHES_PER_GENERATION = 4*NUM_PLAYERS;
const double CHANCE_CROSS = 0.1*RAND_MAX;
const double CHANCE_MUTATE = 0.01*RAND_MAX;

double randomGene(){    return 1.0*rand()/RAND_MAX;
}

double mutateGene(double a){
    double x = 2.0*rand()/RAND_MAX - 1;
    a =  a + x/100;
    if(a>1)a = 1;
    if(a<0)a = 0;
    return a;
}
void reprodute(Player &a, Player &b, int pos){
    int x;
    double p = 1;
    if(b.matches<50) p = 0.5;
    double chanceToDie = (2-b.fitness/1000.0)*p;
    
    x = rand();
    if(x>chanceToDie*RAND_MAX){
        return;
    }
    dead++;
    for(int i = 0; i<NUM_GENES; i++){
        x = rand();
        if(x<CHANCE_CROSS){
            b.genetic[i] = randomGene();
        }else b.genetic[i] = a.genetic[i];
    }
    b.fitness = 1500;
    b.matches = 0;
    b.generation = pos;
    b.code = code++;
    b.population = a.population;
    x = rand();
    if(x<CHANCE_MUTATE) b.population = rand()%NUM_POPULATIONS;
}

char read(){
    char ch;
    fflush(stdin);
    scanf(" %c", &ch);
    while(!isalpha(ch)){
        fflush(stdin);
        scanf(" %c", &ch);       
    }
    if(isupper(ch)) ch +=32;
    if(ch=='b') tempo = 100;
    else if(ch=='m') tempo = 10;
    else if(ch=='g') tempo = 1000;
    else if(ch=='a'){cin>>tempo;};
    return ch;
}

void printLogo(){
    printf(" ______________________________________________________\n");
    printf("| _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _  |\n");
    printf("|    ______________________________________________    |\n");
    printf("| | |  __          __    ___   ___   ___           | | |\n");
    printf("|   | |  \\  |   | |  \\  |   | |     |   | |\\     | |   |\n");
    printf("| | | |   | |   | |   | |   | |     |   | | \\    | | | |\n");
    printf("|   | |__/  |___| |__/  |___| | ___ |   | |  \\   | |   |\n");
    printf("| | | |     |   | |\\    |   | |   | |   | |   \\  | | | |\n");
    printf("|   | |     |   | | \\   |   | |   | |   | |    \\ | |   |\n");
    printf("| | | |     |   | |  \\  |   | |___| |___| |     \\| | | |\n");
    printf("|   |______________________________________________|   |\n");
    printf("| | _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _  | |\n");
    printf("|______________________________________________________|\n");
}

int main(){
    printLogo();
    double aux1[] = {0.6359, 0.6319, 0.4059, 0.76249};

    double aux2[] = {0.8418,0.3978,0.2164,0.7853};
    double aux3[] = {0.8418,0.8297,0.16041,0.7853 };
    double aux4[] = {0, 0, 0, 1};;
    double aux5[] = {0, 0, 0, 0};
    Player mine, yours;

    for(int i = 0; i<NUM_GENES; i++){
        mine.genetic[i] = aux1[i];
        yours.genetic[i] = aux2[i];
    }


    Judge judge(&mine, &yours);
    judge.run(true);









    Player jogadores[NUM_PLAYERS];
    int membersPopJog[NUM_POPULATIONS+1] = {0};
    // double geneticJog[10100];
    for(int i = 0; i<NUM_PLAYERS; i++){
        jogadores[i].id = 1;
        jogadores[i].opp = 2;
        jogadores[i].fitness = 1500;
        jogadores[i].matches = 0;
        jogadores[i].code = code++;
        jogadores[i].generation = 0;
        jogadores[i].population = i*NUM_POPULATIONS/NUM_PLAYERS;
        for(int j = 0; j<NUM_GENES; j++){
            double x = randomGene();
            jogadores[i].setGenes(x, j);
        }
    }

    FILE * saidaF;
    FILE * saidaG;
    FILE * saidaP;
    saidaF = fopen("fitness.txt", "w");
    saidaG = fopen("genetic.txt", "w");
    saidaP = fopen("population.txt", "w");
    char ch;
    double media = 1500;


    for(int i = 0; i<NUM_GENERATIONS; i++){
        for(int j = 0; j<NUM_POPULATIONS; j++){
            membersPopJog[j] = 0;
        }
        for(int j = 0; j<NUM_PLAYERS; j++){
            membersPopJog[jogadores[j].population]++;
        }

        for(int j = 0; j<NUM_GENES; j++){
            fprintf(saidaG, "%.6f ", jogadores[0].genetic[j]);
        }
        fprintf(saidaG, "\n");

        for(int j = 0; j<NUM_POPULATIONS; j++){
            printf("%d ", membersPopJog[j]);
            fprintf(saidaP, "%d ", membersPopJog[j]);
        }
        printf("\n");
        fprintf(saidaP, "\n");

        fprintf(saidaF, "%.2f %.2f %.2f\n", jogadores[0].fitness,
            jogadores[NUM_PLAYERS_2].fitness, jogadores[NUM_PLAYERS-1].fitness);


        media = 0;
        if(tempo==0){
            ch = read();
            if(ch=='q') break;
        }else tempo--;

        for(int j = 0; j<MATCHES_PER_GENERATION; j++){
            int ida = rand()%NUM_PLAYERS;
            int idb = rand()%NUM_PLAYERS;
            while(idb==ida) idb = rand()%NUM_PLAYERS;
            Judge judge(&jogadores[ida], &jogadores[idb]);
            if(ch=='s'){
                judge.run(true);
                ch = read();
            }else judge.run(false);
        }

        media /= NUM_PLAYERS;
        sort(jogadores, jogadores+NUM_PLAYERS, comp);
   
        cout<<endl<<"GENERATION: "<<i+1<<" ("<<tempo<<")"<<endl;
        printf("(%d:%d, %3d, %.2f, %5d), (%d:%d, %3d, %.2f, %5d), (%d:%d, %3d, %.2f, %5d)\n",
            jogadores[0].generation, jogadores[0].population, jogadores[0].matches,
            jogadores[0].fitness, jogadores[0].code,
            jogadores[NUM_PLAYERS_2].generation, jogadores[NUM_PLAYERS_2].population, jogadores[NUM_PLAYERS_2].matches,
            jogadores[NUM_PLAYERS_2].fitness, jogadores[NUM_PLAYERS_2].code, 
            jogadores[NUM_PLAYERS-1].generation, jogadores[NUM_PLAYERS-1].population, jogadores[NUM_PLAYERS-1].matches,
            jogadores[NUM_PLAYERS-1].fitness, jogadores[NUM_PLAYERS-1].code);
        
        dead = 0;
        for(int g = -1+NUM_PLAYERS_2; g>=0; g--){
            reprodute(jogadores[g], jogadores[NUM_PLAYERS-g-1], i+1);
        }

        cout<<"double aux[] = {";
        for(int j = 0; j<NUM_GENES-1; j++){
            cout<<jogadores[0].genetic[j]<< ",";
            if(j == 4) printf("\n");
        }
        cout<<jogadores[0].genetic[NUM_GENES-1]<<"};\n";
        
    }

    fclose(saidaF);
    fclose(saidaG);
    fclose(saidaP);

}