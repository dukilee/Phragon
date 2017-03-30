#ifndef MOVE_H
#define MOVE_H

class Move{
public:
	Move(){
		first = second = type = 0;	
	}
	Move(int f, int s, int t, int a = 0, int b = 0){
		first = f;
		second = s;
		type = t;
		of = a;
		os = b;
	}

	int first, second;
	int of, os;
	int type;
	double rating;

};

#endif