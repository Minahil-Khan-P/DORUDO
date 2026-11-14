// DORUDO - Doraemon-themed Ludo
// STEP 1 (Minahil): Token and Player
// STEP 2 (Fatima): Board Data
// STEP 3 (Minahil): Dice and Game Setup

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
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

const int TRACK_SIZE = 52;
const int HOME_COLUMN_SIZE = 5;

int startpos[4] = { 39, 26, 13, 0 };

int safepos[8] = { 0, 8, 13, 21, 26, 34, 39, 47 };

int homePos[4][5] =
{
    {52, 53, 54, 55, 56},
    {57, 58, 59, 60, 61},
    {62, 63, 64, 65, 66},
    {67, 68, 69, 70, 71}
};

bool isSafe(int pos)
{
    for (int i = 0; i < 8; i++)
    {
        if (safepos[i] == pos)
            return true;
    }

    return false;
}

int getTrackPosition(Character character, int stepsTaken)
{
    return (startpos[character] + stepsTaken - 1) % TRACK_SIZE;
}

int getHomeIndex(int stepsTaken)
{
    return stepsTaken - 52;
}

int getBoardPosition(Character character, int stepsTaken)
{
    if (stepsTaken == 0)
        return -1;

    else if (stepsTaken >= 1 && stepsTaken <= LAST_TRACK_STEP)
        return getTrackPosition(character, stepsTaken);

    else if (stepsTaken > LAST_TRACK_STEP && stepsTaken < FINAL_STEP)
    {
        int index = getHomeIndex(stepsTaken);
        return homePos[character][index];
    }

    else if (stepsTaken == FINAL_STEP)
        return -2;

    return -3;
}

const int MAX_PLAYERS = 4;

int rollDice()
{
    return rand() % 6 + 1;
}

Character pickRandomCharacter(bool used[])
{
    int number = rand() % 4;

    while (used[number])
    {
        number = rand() % 4;
    }

    used[number] = true;
    return (Character)number;
}

void setupGame(Player players[], string names[], int numberOfPlayers)
{
    bool used[4] = { false, false, false, false };

    for (int i = 0; i < numberOfPlayers; i++)
    {
        Character c = pickRandomCharacter(used);
        players[i] = createPlayer(names[i], c);
    }
}

int nextPlayer(Player players[], int numberOfPlayers, int currentPlayer)
{
    int next = currentPlayer;

    for (int i = 0; i < numberOfPlayers; i++)
    {
        next = (next + 1) % numberOfPlayers;

        if (!players[next].finished)
            return next;
    }

    return currentPlayer;
}

bool canTokenMove(Token token, int dice)
{
    if (dice < 1 || dice > 6)
        return false;

    if (token.stepsTaken == 0)
        return dice == 6;

    if (token.stepsTaken >= FINAL_STEP)
        return false;

    if (token.stepsTaken + dice > FINAL_STEP)
        return false;

    return true;
}

bool moveToken(Token& token, int dice){
    if (!canTokenMove(token, dice))
        return false;

    if (token.stepsTaken == 0)
        token.stepsTaken = 1;
    else
        token.stepsTaken += dice;

    return true;
}

bool hasLegalMove(Player player, int dice)
{
    for (int i = 0; i < TOKENS_PER_PLAYER; i++)
    {
        if (canTokenMove(player.tokens[i], dice))
            return true;
    }

    return false;
}

int chooseToken(Player player, int dice)
{
    int count = 0;
    int onlyOne = 0;

    for (int i = 0; i < TOKENS_PER_PLAYER; i++)
    {
        if (canTokenMove(player.tokens[i], dice))
        {
            count++;
            onlyOne = i;
        }
    }

    if (count == 1)
        return onlyOne;

    int choice = 0;

    while (choice < 1 || choice > 4 || !canTokenMove(player.tokens[choice - 1], dice))
    {
        cout << "Which token do you want to move (1 to 4)? ";
        cin >> choice;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            choice = 0;
        }
    }

    return choice - 1;
}

void playTurn(Player& player)
{
    int sixesInRow = 0;
    bool again = true;

    while (again)
    {
        int dice = rollDice();
        cout << player.name << " rolled " << dice << endl;

        if (dice == 6)
            sixesInRow++;

        if (sixesInRow == 3)
        {
            cout << "Three 6s in a row. Turn cancelled." << endl;
            return;
        }

        if (hasLegalMove(player, dice))
        {
            int t = chooseToken(player, dice);
            moveToken(player.tokens[t], dice);
            cout << "Token " << t + 1 << " is now on step "
                << player.tokens[t].stepsTaken << endl;
        }
        else
        {
            cout << "No token can move." << endl;
        }

        if (dice != 6)
            again = false;
    }
}
int main()
{
    srand(time(0));

    string names[MAX_PLAYERS] = { "Minahil", "Fatima", "", "" };
    Player players[MAX_PLAYERS];
    setupGame(players, names, 2);

    int current = 0;

    for (int turn = 1; turn <= 20; turn++)
    {
        cout << endl << "Turn " << turn << ": " << players[current].name << endl;
        playTurn(players[current]);
        current = nextPlayer(players, 2, current);
    }

    return 0;
}