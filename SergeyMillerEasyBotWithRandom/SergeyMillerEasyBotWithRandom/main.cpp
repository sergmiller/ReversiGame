//
//  main.cpp
//  SergeyMillerEasyBotWithRandom
//
//  Created by Сергей Миллер on 03.12.15.
//  Copyright © 2015 Сергей Миллер. All rights reserved.
//

#include <iostream>
#include <string>
#include <map>
using namespace std;

#define loop(i,n) for(int i = 0;i < n;++i)
#define Turn pair <int,int>

enum id {
    BLACK,
    WHITE,
    EMPTY
};

/***********************GLOBAL**********************************/
bool color, endFlag;

id reverseColor(id color) {
    return (color == BLACK ? WHITE : BLACK);
}
/***************************************************************/
class Desk {
public:
    Desk(id myColor);
    id desk[8][8];
    id myColor;
    id rivalColor;
    map <id, int> count;
    //void addRivalTurn(Turn);
    void addTurn(Turn turn, id color);
    int countCoveredCounters(int i, int j, Turn turn, id color);
    void updateDirection(int i, int j, Turn turn, id color);
    Turn generateMyRandomTurn();
};

Desk::Desk(id myColor): myColor(myColor), rivalColor(reverseColor(myColor)) {
    count[BLACK] = 2;
    count[WHITE] = 2;
    
    loop(i,8) {
        loop(j,8) {
            desk[i][j] = EMPTY;
        }
    }
    
    desk[3][3] = WHITE;
    desk[4][4] = WHITE;
    desk[3][4] = BLACK;
    desk[4][3] = BLACK;
}

void Desk::addTurn(Turn turn, id color) {
    desk[turn.first][turn.second] = color;
    for(int i = -1; i <= 1; ++i) {
        for(int j = -1; j <= 1; ++j) {
            if(!i && !j) {
                continue;
            }
            updateDirection(i, j, turn, color);
        }
    }
}

Turn Desk::generateMyRandomTurn() {
    loop(x, 8) {
        loop(y,8) {
            if(desk[x][y] == EMPTY) {
                for(int i = -1; i <= 1; ++i) {
                    for(int j = -1; j <= 1; ++j) {
                        if(!i && !j) {
                            continue;
                        }
                        if(countCoveredCounters(i, j, make_pair(x,y), myColor) > 0) {
                            return make_pair(x, y);
                        }
                    }
                }
            }
        }
    }
    return make_pair(0, 0);
}

int Desk::countCoveredCounters(int i, int j, Turn turn, id color) {
    id rColor = reverseColor(color);
    Turn start = turn;
    turn.first += i;
    turn.second += j;
    bool changeColor;
    bool existRivalCounters = false;
    
    while(turn.first >= 0 && turn.first < 8 && turn.second >= 0 && turn.second < 8) {
        if(desk[turn.first][turn.second] == color) {
            changeColor = true;
            return existRivalCounters;
        }
        
        if(desk[turn.first][turn.second] == rColor) {
            existRivalCounters = true;
        }
        
        if(desk[turn.first][turn.second] == EMPTY) {
            changeColor = false;
            return 0;
        }
        turn.first += i;
        turn.second += j;
    }
    
    return 0;
}

void Desk::updateDirection(int i, int j, Turn turn, id color) {
    id rColor = reverseColor(color);
    Turn start = turn;
    turn.first += i;
    turn.second += j;
    bool changeColor;
    
    while(turn.first >= 0 && turn.first < 8 && turn.second >= 0 && turn.second < 8) {
        if(desk[turn.first][turn.second] == color) {
            changeColor = true;
            break;
        }
        
        if(desk[turn.first][turn.second] == EMPTY) {
            changeColor = false;
            break;
        }
        turn.first += i;
        turn.second += j;
    }

    
    if(changeColor) {
        int quntChange = 0;
        
        for(;start != turn; start.first += i, start.second += j) {
            desk[start.first][start.second] = color;
            ++quntChange;
        }
        
        --quntChange;
        
        count[color] += quntChange;
        count[rColor] -= quntChange;
    }
}

Turn scanAction();
id initColor();
//void initDesk();
void sendMyTurn(Turn turn);


int main() {
    // insert code here...
    //std::cout << "Hello, World!\n";
//    char c1,c2,c3;
//    cin >> c1 >> c2 >> c3;
//    cout << c1 << " " << c2 << " " << c3;
    endFlag = false;
    Desk desk(initColor());
    Turn rivalTurn;
    Turn myTurn;
    if(desk.myColor == WHITE) {
        rivalTurn = scanAction();
        desk.addTurn(rivalTurn, desk.rivalColor);
    }
    
    string dummy;
    while(!endFlag) {
        cin >> dummy;
        
        myTurn = desk.generateMyRandomTurn();
        
        desk.addTurn(myTurn, desk.myColor);
        
        sendMyTurn(myTurn);
        
        rivalTurn = scanAction();
        
        if(rivalTurn == make_pair(-1, -1)) {
            endFlag = true;
        }
        
        desk.addTurn(rivalTurn, desk.rivalColor);
    }
    return 0;
}


id initColor() {
    string s;
    cin >> s;
    cin >> s;
    return (s == "black" ? BLACK : WHITE);
}

Turn scanAction() {
    string s;
    cin >> s;
    if(s != "move") {
        return make_pair(-1, -1);
    }
    char l1, l2;
    cin >> l1 >> l2;
    return make_pair((l1 - 'a'), (l2 - '1'));
}

void sendMyTurn(Turn turn) {
    cout << "move " << (char)('a' + turn.first) << " " << turn.second + 1 << endl;
}

