#ifndef JUDGE_H
#define JUDGE_H

#include "Player.h"
#include "State.h"

typedef std::pair<int, int> ii;

class Judge{
public:

    Judge(Player* p1, Player* p2);

    void run(bool);
    double play(bool, Player*, Player*);
    void makePlay(ii jogada, int player, int adv);

private:
    State state;
    Player *p1, *p2;
    int score[4];
};

#endif