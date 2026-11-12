// DORUDO - Doraemon-themed Ludo
// STEP 1 (Minahil): Token and Player
// STEP 2 (Fatima): Board Data

#include <iostream>
#include <string>
using namespace std;

const int TOKENS_PER_PLAYER = 4;   
const int LAST_TRACK_STEP = 51;
const int FINAL_STEP = 57;

enum Character { DORAEMON, NOBITA, SHIZUKA, GIAN };

struct Token {
    int stepsTaken;      
};

struct Player {
    string name;                       
    Character character;               
    Token tokens[TOKENS_PER_PLAYER];   
    bool finished;                     
    int rank;                          
};

Token createToken() {
    Token t;
    t.stepsTaken = 0;
    return t;
}

Player createPlayer(string name, Character character) {
    Player p;
    p.name = name;
    p.character = character;
    for (int i = 0; i < TOKENS_PER_PLAYER; i++) {
        p.tokens[i] = createToken();
    }
    p.finished = false;
    p.rank = 0;
    return p;
}

bool isInYard(Token t) {
    return t.stepsTaken == 0;
}

bool isOnTrack(Token t) {
    return t.stepsTaken >= 1 && t.stepsTaken <= LAST_TRACK_STEP;
}

bool isInHomeColumn(Token t) {
    return t.stepsTaken > LAST_TRACK_STEP && t.stepsTaken < FINAL_STEP;
}

bool isCompleted(Token t) {
    return t.stepsTaken == FINAL_STEP;
}

int countCompletedTokens(Player p) {
    int count = 0;
    for (int i = 0; i < TOKENS_PER_PLAYER; i++) {
        if (isCompleted(p.tokens[i])) {
            count++;
        }
    }
    return count;
}

string characterName(Character c) {
    if (c == DORAEMON) return "Doraemon";
    else if (c == NOBITA) return "Nobita";
    else if (c == SHIZUKA) return "Shizuka";
    else return "Gian";
}

string characterColour(Character c) {
    if (c == DORAEMON) return "Blue";
    else if (c == NOBITA) return "Yellow";
    else if (c == SHIZUKA) return "Pink";
    else return "Green";
}

string tokenStatus(Token t) {
    if (isInYard(t)) return "in the home yard";
    else if (isOnTrack(t)) return "on the track";
    else if (isInHomeColumn(t)) return "in the home column";
    else return "completed";
}

void printPlayer(Player p) {
    cout << "Player: " << p.name << endl;
    cout << "Character: " << characterName(p.character)
        << " (" << characterColour(p.character) << ")" << endl;

    for (int i = 0; i < TOKENS_PER_PLAYER; i++) {
        cout << "  Token " << i + 1 << ": step " << p.tokens[i].stepsTaken
            << " - " << tokenStatus(p.tokens[i]) << endl;
    }

    if (p.finished) {
        cout << "  Finished: yes" << endl;
    }
    else {
        cout << "  Finished: no" << endl;
    }

    if (p.rank == 0) {
        cout << "  Rank: not ranked yet" << endl;
    }
    else {
        cout << "  Rank: " << p.rank << endl;
    }
    cout << endl;
}

int main() {
    Player p1 = createPlayer("Minahil", DORAEMON);
    Player p2 = createPlayer("Fatima", NOBITA);

    cout << "=== Two new players ===" << endl;
    printPlayer(p1);
    printPlayer(p2);

    p1.tokens[0].stepsTaken = 1;
    p1.tokens[1].stepsTaken = 30;
    p1.tokens[2].stepsTaken = 54;
    p1.tokens[3].stepsTaken = FINAL_STEP;

    cout << "=== After moving Minahil's tokens ===" << endl;
    printPlayer(p1);
    cout << "Completed tokens: " << countCompletedTokens(p1) << endl;

    return 0;
}