// DORUDO - Doraemon-themed Ludo
// STEP 1 (Minahil): the basic data - Token and Player
//
// This step does not draw anything and does not play the game.
// It only creates the "boxes" (structures) that store the information
// the game needs, plus a few small functions to read that information.
// The cout lines in main() are only for testing. They will be removed
// when we build the GUI.

#include <iostream>
#include <string>
using namespace std;

// ---------------------------------------------------------------
// 1. CONSTANTS (numbers that the whole game uses)
// ---------------------------------------------------------------

const int TOKENS_PER_PLAYER = 4;   // every player has 4 tokens

// The position of a token is stored as "steps taken".
//   0         -> still in the home yard
//   1 to 51   -> moving on the shared track
//   52 to 56  -> inside its own colour's home column
//   57        -> reached the final home square (completed)
// Fatima can adjust these numbers in Step 2 if the board needs it.
const int LAST_TRACK_STEP = 51;
const int FINAL_STEP = 57;

// ---------------------------------------------------------------
// 2. CHARACTERS
// An enum is a list of named choices. DORAEMON is stored as 0,
// NOBITA as 1, SHIZUKA as 2 and GIAN as 3, but we use the names
// so the code is easy to read.
// ---------------------------------------------------------------

enum Character { DORAEMON, NOBITA, SHIZUKA, GIAN };

// ---------------------------------------------------------------
// 3. STRUCTURES
// A structure groups related information into one "box".
// ---------------------------------------------------------------

// One token (a piece on the board)
struct Token {
    int stepsTaken;      // 0 = in yard ... 57 = completed
};

// One player
struct Player {
    string name;                       // the name the player types in
    Character character;               // Doraemon, Nobita, Shizuka or Gian
    Token tokens[TOKENS_PER_PLAYER];   // an array of 4 tokens
    bool finished;                     // true when all 4 tokens are completed
    int rank;                          // 1, 2, 3... (0 means no rank yet)
};

// ---------------------------------------------------------------
// 4. FUNCTIONS THAT CREATE THINGS
// ---------------------------------------------------------------

// Makes a new token that starts in the home yard
Token createToken() {
    Token t;
    t.stepsTaken = 0;
    return t;
}

// Makes a new player with 4 tokens, all in the home yard
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

// ---------------------------------------------------------------
// 5. FUNCTIONS THAT ANSWER QUESTIONS ABOUT A TOKEN
// ---------------------------------------------------------------

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

// Counts how many of a player's tokens are completed
int countCompletedTokens(Player p) {
    int count = 0;
    for (int i = 0; i < TOKENS_PER_PLAYER; i++) {
        if (isCompleted(p.tokens[i])) {
            count++;
        }
    }
    return count;
}

// ---------------------------------------------------------------
// 6. FUNCTIONS THAT GIVE TEXT (used for testing and later for the screen)
// ---------------------------------------------------------------

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

// Prints everything about one player (for testing only)
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

// ---------------------------------------------------------------
// 7. MAIN - a small test to check that everything works
// ---------------------------------------------------------------

int main() {
    // Make two players
    Player p1 = createPlayer("Minahil", DORAEMON);
    Player p2 = createPlayer("Fatima", NOBITA);

    cout << "=== Two new players ===" << endl;
    printPlayer(p1);
    printPlayer(p2);

    // Test: pretend Minahil's tokens have moved different amounts
    p1.tokens[0].stepsTaken = 1;
    p1.tokens[1].stepsTaken = 30;
    p1.tokens[2].stepsTaken = 54;
    p1.tokens[3].stepsTaken = FINAL_STEP;

    cout << "=== After moving Minahil's tokens ===" << endl;
    printPlayer(p1);
    cout << "Completed tokens: " << countCompletedTokens(p1) << endl;

    return 0;
}