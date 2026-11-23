// DORUDO - Doraemon-themed Ludo
// STEP 1 (Minahil): Token and Player
// STEP 2 (Fatima): Board Data
// STEP 3 (Minahil): Dice and Game Setup
// STEP 4 (Fatima): Token Movement Rules
// STEP 5 (Minahil): Turn Flow
// Step 6 (Fatima): captures
// Step 7 (Minahil): Ranking, Game End and Results
// Step 8 (Both): testing and fixes
// Step 9 (Fatima): Board and Token drawings
// Step 10 (Minahil): Roll Button, Dice Animation and Turn Indicator
// Step 11 (Fatima): Allowing players click a token to move it and linking to step  4 to 6 
// Step 12(Minahil) : Make the screens intro, main menu, instructions and setup screen
// Step 13 (Fatima) : Add background music, winner and loser sounds and token sounds
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
using namespace std;
sf::Music backgroundMusic;
// Sound buffers
sf::SoundBuffer diceBuffer;
sf::SoundBuffer gadgetBuffer;
sf::SoundBuffer gianBuffer;
sf::SoundBuffer tokenReplaceBuffer;

// Sound objects
sf::Sound diceSound(diceBuffer);
sf::Sound gadgetSound(gadgetBuffer);
sf::Sound gianSound(gianBuffer);
sf::Sound tokenReplaceSound(tokenReplaceBuffer);
const int TOKENS_PER_PLAYER = 4;                           //(MINAHIL STEP 1)

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
}                                                              // END OF STEP 1

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

const int MAX_PLAYERS = 4;                                             //(MINAHIL STEP 3)

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
}                                                                  // END OF STEP 3

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

int countTokensAt(Player player, int position)
{
    int count = 0;

    for (int j = 0; j < TOKENS_PER_PLAYER; j++)
    {
        if (isOnTrack(player.tokens[j]))
        {
            int p = getTrackPosition(player.character, player.tokens[j].stepsTaken);

            if (p == position)
                count++;
        }
    }

    return count;
}

void captureTokens(Player players[], int currentPlayer,
    int numberOfPlayers, int tokenIndex)
{
    Token myToken = players[currentPlayer].tokens[tokenIndex];

    if (!isOnTrack(myToken))
        return;

    int position = getTrackPosition(
        players[currentPlayer].character,
        myToken.stepsTaken
    );

    if (!isSafe(position))
    {
        bool captured = false;

        for (int i = 0; i < numberOfPlayers; i++)
        {
            if (i != currentPlayer)
            {
                for (int j = 0; j < TOKENS_PER_PLAYER; j++)
                {
                    Token& opponentToken = players[i].tokens[j];

                    if (isOnTrack(opponentToken))
                    {
                        int opponentPosition = getTrackPosition(
                            players[i].character,
                            opponentToken.stepsTaken
                        );

                        if (position == opponentPosition &&
                            countTokensAt(players[i], position) == 1)
                        {
                            opponentToken.stepsTaken = 0;
                            captured = true;

                            cout << players[i].name
                                << "'s Token " << j + 1
                                << " has been captured!" << endl;
                        }
                    }
                }
            }
        }

        if (captured)
            tokenReplaceSound.play();
    }
}
bool hasLegalMove(Player player, int dice)                         //(MINAHIL STEP 5)
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

void playTurn(Player players[], int numberOfPlayers, int currentPlayer)
{
    int sixesInRow = 0;
    bool again = true;

    while (again)
    {
        int dice = rollDice();

        cout << players[currentPlayer].name
            << " rolled " << dice << endl;

        if (dice == 6)
            sixesInRow++;

        if (sixesInRow == 3)
        {
            cout << "Three 6s in a row. Turn cancelled." << endl;
            return;
        }

        if (hasLegalMove(players[currentPlayer], dice))
        {
            int t = chooseToken(players[currentPlayer], dice);

            moveToken(players[currentPlayer].tokens[t], dice);

            cout << "Token " << t + 1
                << " is now on step "
                << players[currentPlayer].tokens[t].stepsTaken
                << endl;

            captureTokens(players, currentPlayer, numberOfPlayers, t);
            if (countCompletedTokens(players[currentPlayer]) == TOKENS_PER_PLAYER)
                return;
        }
        else
        {
            cout << "No token can move." << endl;
        }

        if (dice != 6)
            again = false;
    }
}                                                           // END OF STEP 5

int countFinishedPlayers(Player players[], int numberOfPlayers)
{
    int count = 0;

    for (int i = 0; i < numberOfPlayers; i++)
    {
        if (players[i].finished)
            count++;
    }

    return count;
}

void updateRanks(Player players[], int numberOfPlayers)
{
    for (int i = 0; i < numberOfPlayers; i++)
    {
        if (!players[i].finished && countCompletedTokens(players[i]) == TOKENS_PER_PLAYER)
        {
            players[i].finished = true;
            players[i].rank = countFinishedPlayers(players, numberOfPlayers);

            cout << players[i].name << " finished in place "
                << players[i].rank << "!" << endl;
        }
    }
}

bool isGameOver(Player players[], int numberOfPlayers)
{
    return countFinishedPlayers(players, numberOfPlayers) == numberOfPlayers - 1;
}

void giveLastRank(Player players[], int numberOfPlayers)
{
    for (int i = 0; i < numberOfPlayers; i++)
    {
        if (!players[i].finished)
        {
            players[i].finished = true;
            players[i].rank = numberOfPlayers;
        }
    }
}

string rewardFor(int rank, int numberOfPlayers)
{
    if (rank == numberOfPlayers)
        return "Gian's song";
    else if (rank == 1)
        return "Gourmet Table Cloth";
    else if (rank == 2)
        return "Memory Bread";
    else
        return "Translation Konjac";
}

void printResults(Player players[], int numberOfPlayers)
{
    cout << endl << "=== FINAL RESULTS ===" << endl;

    for (int rank = 1; rank <= numberOfPlayers; rank++)
    {
        for (int i = 0; i < numberOfPlayers; i++)
        {
            if (players[i].rank == rank)
            {
                cout << "Rank " << rank << ": " << players[i].name
                    << " (" << characterName(players[i].character) << ") - "
                    << rewardFor(rank, numberOfPlayers) << endl;
            }
        }
    }
}

drawing code :



// STEP 9 (Fatima) 
const int WINDOW_SIZE = 720;
const float CELL = 42.f;
const float ORIGIN = 45.f;
const float TOKEN_RADIUS = 14.f;

sf::Color BLUE(35, 145, 235);
sf::Color YELLOW(255, 205, 45);
sf::Color PINK(245, 125, 185);
sf::Color GREEN(65, 180, 100);

sf::Color LIGHT_BLUE(190, 225, 250);
sf::Color LIGHT_YELLOW(255, 240, 170);
sf::Color LIGHT_PINK(255, 215, 235);
sf::Color LIGHT_GREEN(195, 235, 205);

sf::Color getCharacterColor(Character character)
{
    switch (character)
    {
    case DORAEMON: return BLUE;
    case NOBITA: return YELLOW;
    case SHIZUKA: return PINK;
    case GIAN: return GREEN;
    }

    return sf::Color::White;
}

sf::Color getLightColor(Character character)
{
    switch (character)
    {
    case DORAEMON: return LIGHT_BLUE;
    case NOBITA: return LIGHT_YELLOW;
    case SHIZUKA: return LIGHT_PINK;
    case GIAN: return LIGHT_GREEN;
    }

    return sf::Color::White;
}

sf::Vector2f cellPosition(int row, int col)
{
    return {
        ORIGIN + col * CELL,
        ORIGIN + row * CELL
    };
}

sf::Vector2f cellCenter(int row, int col)
{
    return {
        ORIGIN + col * CELL + CELL / 2,
        ORIGIN + row * CELL + CELL / 2
    };
}

void drawCell(sf::RenderWindow& window, int row, int col,
    sf::Color color)
{
    sf::RectangleShape square({ CELL - 2, CELL - 2 });
    square.setPosition(cellPosition(row, col));
    square.setFillColor(color);
    square.setOutlineColor(sf::Color(90, 90, 90));
    square.setOutlineThickness(1);
    window.draw(square);
}

sf::Vector2i trackCells[52] =
{
    { 6,1 },{ 6,2 },{ 6,3 },{ 6,4 },{ 6,5 },
    { 5,6 },{ 4,6 },{ 3,6 },{ 2,6 },{ 1,6 },
    { 0,6 },{ 0,7 },{ 0,8 },

    { 1,8 },{ 2,8 },{ 3,8 },{ 4,8 },{ 5,8 },
    { 6,9 },{ 6,10 },{ 6,11 },{ 6,12 },{ 6,13 },
    { 6,14 },{ 7,14 },{ 8,14 },

    { 8,13 },{ 8,12 },{ 8,11 },{ 8,10 },{ 8,9 },
    { 9,8 },{ 10,8 },{ 11,8 },{ 12,8 },{ 13,8 },
    { 14,8 },{ 14,7 },{ 14,6 },

    { 13,6 },{ 12,6 },{ 11,6 },{ 10,6 },{ 9,6 },
    { 8,5 },{ 8,4 },{ 8,3 },{ 8,2 },{ 8,1 },
    { 8,0 },{ 7,0 },{ 6,0 }
};

sf::Vector2i homeCells[4][5] =
{
    { { 13,7 },{ 12,7 },{ 11,7 },{ 10,7 },{ 9,7 } },
    { { 7,13 },{ 7,12 },{ 7,11 },{ 7,10 },{ 7,9 } },
    { { 1,7 },{ 2,7 },{ 3,7 },{ 4,7 },{ 5,7 } },
    { { 7,1 },{ 7,2 },{ 7,3 },{ 7,4 },{ 7,5 } }
};

sf::Vector2i yardCenters[4][4] =
{
    { { 10,2 },{ 10,4 },{ 12,2 },{ 12,4 } },
    { { 10,10 },{ 10,12 },{ 12,10 },{ 12,12 } },
    { { 2,10 },{ 2,12 },{ 4,10 },{ 4,12 } },
    { { 2,2 },{ 2,4 },{ 4,2 },{ 4,4 } }
};

void drawYards(sf::RenderWindow& window)
{
    int yardRows[4] = { 9, 9, 0, 0 };
    int yardCols[4] = { 0, 9, 9, 0 };

    for (int i = 0; i < 4; i++)
    {
        Character character = static_cast<Character>(i);

        sf::RectangleShape yard({ CELL * 6, CELL * 6 });
        yard.setPosition(cellPosition(yardRows[i], yardCols[i]));
        yard.setFillColor(getLightColor(character));
        yard.setOutlineColor(getCharacterColor(character));
        yard.setOutlineThickness(4);

        window.draw(yard);

        for (int j = 0; j < 4; j++)
        {
            sf::Vector2i position = yardCenters[i][j];
            sf::Vector2f center = cellCenter(position.x, position.y);

            sf::CircleShape token(TOKEN_RADIUS + 3);
            token.setOrigin({ TOKEN_RADIUS + 3, TOKEN_RADIUS + 3 });
            token.setPosition(center);
            token.setFillColor(sf::Color::White);
            token.setOutlineColor(getCharacterColor(character));
            token.setOutlineThickness(3);

            window.draw(token);
        }
    }
}

void drawTrack(sf::RenderWindow& window)
{
    for (int i = 0; i < 52; i++)
    {
        int row = trackCells[i].x;
        int col = trackCells[i].y;

        sf::Color color(245, 245, 245);

        if (i == 0 || i == 13 || i == 26 || i == 39)
        {
            int characterIndex = 0;                // i == 39 is Doraemon
            if (i == 0) characterIndex = 3;        // Gian
            else if (i == 13) characterIndex = 2;  // Shizuka
            else if (i == 26) characterIndex = 1;  // Nobita
            color = getLightColor(static_cast<Character>(characterIndex));
        }

        drawCell(window, row, col, color);

        if (i == 0 || i == 8 || i == 13 || i == 21 ||
            i == 26 || i == 34 || i == 39 || i == 47)
        {
            sf::CircleShape safeMark(5);
            safeMark.setOrigin({ 5, 5 });
            safeMark.setPosition(cellCenter(row, col));
            safeMark.setFillColor(sf::Color(230, 175, 35));
            window.draw(safeMark);
        }
    }
}

void drawHomeColumns(sf::RenderWindow& window)
{
    for (int character = 0; character < 4; character++)
    {
        for (int i = 0; i < 5; i++)
        {
            int row = homeCells[character][i].x;
            int col = homeCells[character][i].y;

            drawCell(
                window,
                row,
                col,
                getLightColor(static_cast<Character>(character))
            );
        }
    }
}

void drawCenter(sf::RenderWindow& window)
{
    sf::RectangleShape center({ CELL * 3, CELL * 3 });
    center.setPosition(cellPosition(6, 6));
    center.setFillColor(sf::Color(235, 235, 235));
    center.setOutlineColor(sf::Color::Black);
    center.setOutlineThickness(2);

    window.draw(center);

    sf::CircleShape centerMark(25);
    centerMark.setOrigin({ 25, 25 });
    centerMark.setPosition(cellCenter(7, 7));
    centerMark.setFillColor(BLUE);
    centerMark.setOutlineColor(sf::Color::White);
    centerMark.setOutlineThickness(3);

    window.draw(centerMark);
}

void drawBoard(sf::RenderWindow& window)
{
    sf::RectangleShape background({ 630, 630 });
    background.setPosition({ ORIGIN, ORIGIN });
    background.setFillColor(sf::Color::White);
    background.setOutlineColor(sf::Color::Black);
    background.setOutlineThickness(3);

    window.draw(background);

    drawYards(window);
    drawTrack(window);
    drawHomeColumns(window);
    drawCenter(window);
}

void drawToken(sf::RenderWindow& window,
    sf::Vector2f center,
    Character character,
    int tokenIndex)
{
    sf::CircleShape token(TOKEN_RADIUS);
    token.setOrigin({ TOKEN_RADIUS, TOKEN_RADIUS });

    float offsetsX[4] = { -5, 5, -5, 5 };
    float offsetsY[4] = { -5, -5, 5, 5 };

    center.x += offsetsX[tokenIndex];
    center.y += offsetsY[tokenIndex];

    token.setPosition(center);
    token.setFillColor(getCharacterColor(character));
    token.setOutlineColor(sf::Color::White);
    token.setOutlineThickness(2);

    window.draw(token);
}

void drawTokens(sf::RenderWindow& window,
    Player players[],
    int numberOfPlayers)
{
    for (int p = 0; p < numberOfPlayers; p++)
    {
        Character character = players[p].character;

        for (int t = 0; t < TOKENS_PER_PLAYER; t++)
        {
            int position = getBoardPosition(
                character,
                players[p].tokens[t].stepsTaken
            );

            if (position == -1)
            {
                sf::Vector2i yard = yardCenters[character][t];

                drawToken(
                    window,
                    cellCenter(yard.x, yard.y),
                    character,
                    t
                );
            }
            else if (position >= 0 && position < 52)
            {
                sf::Vector2i track = trackCells[position];

                drawToken(
                    window,
                    cellCenter(track.x, track.y),
                    character,
                    t
                );
            }
            else if (players[p].tokens[t].stepsTaken >= 52 &&
                players[p].tokens[t].stepsTaken <= 56)
            {
                int homeIndex = players[p].tokens[t].stepsTaken - 52;
                sf::Vector2i home = homeCells[character][homeIndex];

                drawToken(
                    window,
                    cellCenter(home.x, home.y),
                    character,
                    t
                );
            }
        }
    }
}

// STEP 10 (Minahil)
const float BUTTON_X = 780.f;
const float BUTTON_Y = 560.f;
const float BUTTON_W = 160.f;
const float BUTTON_H = 60.f;


struct DiceState
{
    bool rolling;   
    int frames;     
    int shown;      
    int result;     
};

DiceState createDice()
{
    DiceState d;
    d.rolling = false;
    d.frames = 0;
    d.shown = 1;
    d.result = 0;
    return d;
}


void startRoll(DiceState& d)
{
    d.rolling = true;
    d.frames = 0;
    d.result = rollDice();   
}


bool updateDice(DiceState& d)
{
    if (!d.rolling)
        return false;

    d.frames++;

    
    if (d.frames % 6 == 0)
        d.shown = rollDice();

    
    if (d.frames >= 60)
    {
        d.rolling = false;
        d.shown = d.result;
        return true;
    }

    return false;
}


bool clickedRollButton(float mouseX, float mouseY)
{
    return mouseX >= BUTTON_X && mouseX <= BUTTON_X + BUTTON_W &&
        mouseY >= BUTTON_Y && mouseY <= BUTTON_Y + BUTTON_H;
}


void drawText(sf::RenderWindow& window, sf::Font& font, string words,
    unsigned int size, float x, float y, sf::Color colour)
{
    sf::Text text(font, words, size);
    text.setFillColor(colour);

    sf::FloatRect box = text.getLocalBounds();
    text.setOrigin({ box.position.x + box.size.x / 2,
                    box.position.y + box.size.y / 2 });
    text.setPosition({ x, y });

    window.draw(text);
}


void drawPanel(sf::RenderWindow& window)
{
    sf::RectangleShape panel({ 280.f, 720.f });
    panel.setPosition({ 720.f, 0.f });
    panel.setFillColor(sf::Color(235, 230, 215));
    window.draw(panel);
}

void drawRollButton(sf::RenderWindow& window, sf::Font& font, bool rolling)
{
    sf::RectangleShape button({ BUTTON_W, BUTTON_H });
    button.setPosition({ BUTTON_X, BUTTON_Y });

    if (rolling)
        button.setFillColor(sf::Color(160, 160, 160));   
    else
        button.setFillColor(sf::Color(35, 145, 235));    

    button.setOutlineColor(sf::Color::White);
    button.setOutlineThickness(3);
    window.draw(button);

    drawText(window, font, "ROLL", 28,
        BUTTON_X + BUTTON_W / 2, BUTTON_Y + BUTTON_H / 2,
        sf::Color::White);
}

void drawDice(sf::RenderWindow& window, sf::Font& font, DiceState d)
{
    sf::RectangleShape box({ 120.f, 120.f });
    box.setOrigin({ 60.f, 60.f });
    box.setPosition({ 860.f, 430.f });
    box.setFillColor(sf::Color::White);
    box.setOutlineColor(sf::Color::Black);
    box.setOutlineThickness(4);
    window.draw(box);

    drawText(window, font, to_string(d.shown), 70, 860.f, 425.f,
        sf::Color::Black);
}


void drawTurnIndicator(sf::RenderWindow& window, sf::Font& font, Player player)
{
    drawText(window, font, "Turn", 28, 860.f, 50.f, sf::Color::Black);

    sf::CircleShape face(60.f);
    face.setOrigin({ 60.f, 60.f });
    face.setPosition({ 860.f, 160.f });
    face.setFillColor(getCharacterColor(player.character));
    face.setOutlineColor(sf::Color::White);
    face.setOutlineThickness(4);
    window.draw(face);

    drawText(window, font, characterName(player.character), 26,
        860.f, 250.f, sf::Color::Black);
    drawText(window, font, player.name, 22,
        860.f, 285.f, sf::Color(90, 90, 90));
}
//STEP 11 (Fatima)

struct MoveAnimation
{
    bool active;
    int playerIndex;
    int tokenIndex;
    int targetStep;
    sf::Clock timer;
};

MoveAnimation movement;

int sixesInRow = 0;
bool rollProcessed = false;

sf::Vector2f getTokenPosition(Player players[],
    int playerIndex, int tokenIndex)
{
    Character character = players[playerIndex].character;

    int steps = players[playerIndex].tokens[tokenIndex].stepsTaken;

    int position = getBoardPosition(character, steps);

    sf::Vector2f center;

    if (position == -1)
    {
        sf::Vector2i yard = yardCenters[character][tokenIndex];
        center = cellCenter(yard.x, yard.y);
    }
    else if (position >= 0 && position < TRACK_SIZE)
    {
        sf::Vector2i track = trackCells[position];
        center = cellCenter(track.x, track.y);
    }
    else if (steps >= 52 && steps <= 56)
    {
        int index = steps - 52;
        sf::Vector2i home = homeCells[character][index];
        center = cellCenter(home.x, home.y);
    }
    else
    {
        return { -1000, -1000 };
    }

    float offsetsX[4] = { -5, 5, -5, 5 };
    float offsetsY[4] = { -5, -5, 5, 5 };

    center.x += offsetsX[tokenIndex];
    center.y += offsetsY[tokenIndex];

    return center;
}

int getClickedToken(sf::RenderWindow& window,
    Player players[], int currentPlayer)
{
    sf::Vector2i mouse = sf::Mouse::getPosition(window);

    for (int i = 0; i < TOKENS_PER_PLAYER; i++)
    {
        sf::Vector2f position =
            getTokenPosition(players, currentPlayer, i);

        float dx = mouse.x - position.x;
        float dy = mouse.y - position.y;

        if (dx * dx + dy * dy <= 20 * 20)
            return i;
    }

    return -1;
}

bool startTokenMovement(Player players[],
    int currentPlayer, int tokenIndex, int diceResult)
{
    if (!canTokenMove(
        players[currentPlayer].tokens[tokenIndex],
        diceResult))
    {
        return false;
    }

    movement.active = true;
    movement.playerIndex = currentPlayer;
    movement.tokenIndex = tokenIndex;

    int currentStep =
        players[currentPlayer].tokens[tokenIndex].stepsTaken;

    if (currentStep == 0)
        movement.targetStep = 1;
    else
        movement.targetStep = currentStep + diceResult;

    movement.timer.restart();

    return true;
}

bool updateTokenMovement(Player players[],
    int numberOfPlayers)
{
    if (!movement.active)
        return false;

    if (movement.timer.getElapsedTime().asMilliseconds() < 120)
        return false;

    movement.timer.restart();

    int p = movement.playerIndex;
    int t = movement.tokenIndex;

    players[p].tokens[t].stepsTaken++;

    if (players[p].tokens[t].stepsTaken >= movement.targetStep)
    {
        movement.active = false;

        captureTokens(players, p, numberOfPlayers, t);

        return true;
    }

    return false;
}

bool finishPlayerTurn(Player players[],
    int numberOfPlayers, int& currentPlayer, int diceResult)
{
    updateRanks(players, numberOfPlayers);

    if (isGameOver(players, numberOfPlayers))
    {
        giveLastRank(players, numberOfPlayers);
        printResults(players, numberOfPlayers);

        for (int i = 0; i < numberOfPlayers; i++)
        {
            if (players[i].rank == 1)
                gadgetSound.play();

            if (players[i].rank == numberOfPlayers)
                gianSound.play();
        }

        return true;
    }

    if (diceResult != 6 || players[currentPlayer].finished)
    {
        sixesInRow = 0;

        currentPlayer = nextPlayer(
            players, numberOfPlayers, currentPlayer
        );
    }

    return false;
}

// STEP 12 (Minahil)
enum Screen { INTRO, MENU, INSTRUCTIONS, SETUP, REVEAL, PLAYING };


bool isInside(float mx, float my, float x, float y, float w, float h)
{
    return mx >= x && mx <= x + w && my >= y && my <= y + h;
}


void drawButton(sf::RenderWindow& window, sf::Font& font, string label,
    float x, float y, float w, float h, sf::Color colour)
{
    sf::RectangleShape box({ w, h });
    box.setPosition({ x, y });
    box.setFillColor(colour);
    box.setOutlineColor(sf::Color::White);
    box.setOutlineThickness(3);
    window.draw(box);

    drawText(window, font, label, 28, x + w / 2, y + h / 2, sf::Color::White);
}


void drawIntro(sf::RenderWindow& window, sf::Font& font, int frames)
{
    window.clear(sf::Color::Black);

    int alpha1 = frames * 3;            
    if (alpha1 > 255)
        alpha1 = 255;

    int alpha2 = (frames - 60) * 3;     
    if (alpha2 < 0)
        alpha2 = 0;
    if (alpha2 > 255)
        alpha2 = 255;

    drawText(window, font, "DORUDO", 110, 500.f, 310.f,
        sf::Color(35, 145, 235, (unsigned char)alpha1));
    drawText(window, font, "Doraemon based", 36, 500.f, 410.f,
        sf::Color(255, 255, 255, (unsigned char)alpha2));
}


void drawMenu(sf::RenderWindow& window, sf::Font& font)
{
    window.clear(sf::Color(250, 248, 240));

    drawText(window, font, "DORUDO", 90, 500.f, 140.f, sf::Color(35, 145, 235));

    drawButton(window, font, "START GAME", 350.f, 300.f, 300.f, 70.f,
        sf::Color(35, 145, 235));
    drawButton(window, font, "INSTRUCTIONS", 350.f, 400.f, 300.f, 70.f,
        sf::Color(245, 125, 185));
    drawButton(window, font, "EXIT GAME", 350.f, 500.f, 300.f, 70.f,
        sf::Color(65, 180, 100));
}


void drawInstructions(sf::RenderWindow& window, sf::Font& font, bool afterStart)
{
    window.clear(sf::Color(250, 248, 240));

    sf::RectangleShape popup({ 800.f, 520.f });
    popup.setPosition({ 100.f, 50.f });
    popup.setFillColor(sf::Color::White);
    popup.setOutlineColor(sf::Color(35, 145, 235));
    popup.setOutlineThickness(5);
    window.draw(popup);

    drawText(window, font, "How to play", 40, 500.f, 95.f, sf::Color(35, 145, 235));

    string lines[8] =
    {
        "1. Click ROLL to roll the dice.",
        "2. You need a 6 to bring a token out of the yard.",
        "3. A 6 gives another turn. Three 6s in a row cancel it.",
        "4. After rolling, click the token you want to move.",
        "5. Landing on one opponent token captures it.",
        "    (Safe squares and blocks cannot be captured.)",
        "6. You need the exact number to reach home.",
        "7. The last player left has to listen to Gian's song!"
    };

    for (int i = 0; i < 8; i++)
    {
        drawText(window, font, lines[i], 24, 500.f, 160.f + i * 48.f,
            sf::Color::Black);
    }

    if (afterStart)
        drawButton(window, font, "CONTINUE", 350.f, 600.f, 300.f, 70.f,
            sf::Color(35, 145, 235));
    else
        drawButton(window, font, "BACK", 350.f, 600.f, 300.f, 70.f,
            sf::Color(35, 145, 235));
}

void drawSetup(sf::RenderWindow& window, sf::Font& font,
    int numberOfPlayers, string names[], int activeBox)
{
    window.clear(sf::Color(250, 248, 240));

    drawText(window, font, "How many players?", 40, 500.f, 60.f,
        sf::Color(35, 145, 235));

    
    for (int k = 0; k < 3; k++)
    {
        sf::Color colour(160, 160, 160);        
        if (numberOfPlayers == k + 2)
            colour = sf::Color(35, 145, 235);   

        drawButton(window, font, to_string(k + 2), 300.f + k * 130.f, 110.f,
            110.f, 60.f, colour);
    }

    drawText(window, font, "Click a box and type the name (Enter = next box)",
        22, 500.f, 205.f, sf::Color(90, 90, 90));

    
    for (int i = 0; i < numberOfPlayers; i++)
    {
        sf::RectangleShape box({ 400.f, 55.f });
        box.setPosition({ 300.f, 235.f + i * 75.f });
        box.setFillColor(sf::Color::White);

        if (i == activeBox)
        {
            box.setOutlineColor(sf::Color(35, 145, 235));
            box.setOutlineThickness(4);
        }
        else
        {
            box.setOutlineColor(sf::Color(150, 150, 150));
            box.setOutlineThickness(2);
        }

        window.draw(box);

        string shown = names[i];
        sf::Color textColour = sf::Color::Black;

        if (shown == "" && i != activeBox)
        {
            shown = "Player " + to_string(i + 1);   
            textColour = sf::Color(170, 170, 170);
        }

        if (i == activeBox)
            shown = shown + "|";                    

        drawText(window, font, shown, 28, 500.f, 235.f + i * 75.f + 27.f,
            textColour);
    }

    drawButton(window, font, "START", 350.f, 600.f, 300.f, 70.f,
        sf::Color(65, 180, 100));
}


void drawReveal(sf::RenderWindow& window, sf::Font& font,
    Player players[], int numberOfPlayers, int frames)
{
    window.clear(sf::Color(250, 248, 240));

    drawText(window, font, "Your characters", 48, 500.f, 70.f,
        sf::Color(35, 145, 235));

    int shown = frames / 45 + 1;        
    if (shown > numberOfPlayers)
        shown = numberOfPlayers;

    for (int i = 0; i < shown; i++)
    {
        float y = 190.f + i * 100.f;

        sf::CircleShape face(35.f);
        face.setOrigin({ 35.f, 35.f });
        face.setPosition({ 300.f, y });
        face.setFillColor(getCharacterColor(players[i].character));
        face.setOutlineColor(sf::Color::White);
        face.setOutlineThickness(4);
        window.draw(face);

        drawText(window, font,
            players[i].name + " is " + characterName(players[i].character),
            34, 620.f, y, sf::Color::Black);
    }

    
    if (frames >= numberOfPlayers * 45)
        drawButton(window, font, "PLAY", 350.f, 620.f, 300.f, 70.f,
            sf::Color(65, 180, 100));
}
int consoleMain()
{
    srand(time(0));

    int numberOfPlayers = 2;

    string names[MAX_PLAYERS] = { "Minahil", "Fatima", "Player 3", "Player 4" };
    Player players[MAX_PLAYERS];
    setupGame(players, names, numberOfPlayers);

    int current = 0;

    while (!isGameOver(players, numberOfPlayers))
    {
        cout << endl << players[current].name << "'s turn" << endl;
        playTurn(players, numberOfPlayers, current);
        updateRanks(players, numberOfPlayers);
        current = nextPlayer(players, numberOfPlayers, current);
    }

    giveLastRank(players, numberOfPlayers);
    printResults(players, numberOfPlayers);

    return 0;
}
int main()
{
    srand(time(0));

    sf::RenderWindow window(sf::VideoMode({ 1000, 720 }), "DORUDO");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf"))
        return 1;

    // Load background music
    if (!backgroundMusic.openFromFile("Audio/background.ogg"))
    {
        cout << "Background music could not be loaded!" << endl;
    }
    else
    {
        backgroundMusic.setLooping(true);
        backgroundMusic.setVolume(30);
        backgroundMusic.play();
    }

    // Load sound effects
    if (!diceBuffer.loadFromFile("Audio/diceroll.wav"))
        cout << "Dice sound not loaded!" << endl;

    if (!gadgetBuffer.loadFromFile("Audio/gadget.wav"))
        cout << "Gadget sound not loaded!" << endl;

    if (!gianBuffer.loadFromFile("Audio/gian.wav"))
        cout << "Gian sound not loaded!" << endl;

    if (!tokenReplaceBuffer.loadFromFile("Audio/tokenreplace.wav"))
        cout << "Token replacement sound not loaded!" << endl;

    // Screen information
    Screen screen = INTRO;
    int frames = 0;
    bool afterStart = false;

    // Setup information
    int numberOfPlayers = 2;
    string names[MAX_PLAYERS] = { "", "", "", "" };
    int activeBox = 0;
    Player players[MAX_PLAYERS];

    // Game information
    DiceState dice = createDice();
    int currentPlayer = 0;
    bool gameFinished = false;

    movement.active = false;
    movement.playerIndex = 0;
    movement.tokenIndex = 0;
    movement.targetStep = 0;

    while (window.isOpen())
    {
        // 1. EVENTS
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            // Typing player names
            if (screen == SETUP)
            {
                if (const auto* typed = event->getIf<sf::Event::TextEntered>())
                {
                    char32_t c = typed->unicode;

                    if (c == 8)
                    {
                        if (!names[activeBox].empty())
                            names[activeBox].pop_back();
                    }
                    else if (c == 13)
                    {
                        activeBox = (activeBox + 1) % numberOfPlayers;
                    }
                    else if (c >= 32 && c < 127 &&
                        names[activeBox].size() < 12)
                    {
                        names[activeBox] += (char)c;
                    }
                }
            }

            // Mouse clicks
            if (const auto* click =
                event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (click->button != sf::Mouse::Button::Left)
                    continue;

                float mx = (float)click->position.x;
                float my = (float)click->position.y;

                if (screen == INTRO)
                {
                    screen = MENU;
                    frames = 0;
                }
                else if (screen == MENU)
                {
                    if (isInside(mx, my, 350, 300, 300, 70))
                    {
                        afterStart = true;
                        screen = INSTRUCTIONS;
                        frames = 0;
                    }
                    else if (isInside(mx, my, 350, 400, 300, 70))
                    {
                        afterStart = false;
                        screen = INSTRUCTIONS;
                        frames = 0;
                    }
                    else if (isInside(mx, my, 350, 500, 300, 70))
                    {
                        window.close();
                    }
                }
                else if (screen == INSTRUCTIONS)
                {
                    if (isInside(mx, my, 350, 600, 300, 70))
                    {
                        if (afterStart)
                            screen = SETUP;
                        else
                            screen = MENU;

                        frames = 0;
                    }
                }
                else if (screen == SETUP)
                {
                    // Number of players buttons
                    for (int k = 0; k < 3; k++)
                    {
                        if (isInside(mx, my,
                            300.f + k * 130.f, 110, 110, 60))
                        {
                            numberOfPlayers = k + 2;
                            activeBox = 0;
                        }
                    }

                    // Name boxes
                    for (int i = 0; i < numberOfPlayers; i++)
                    {
                        if (isInside(mx, my,
                            300, 235.f + i * 75.f, 400, 55))
                        {
                            activeBox = i;
                        }
                    }

                    // Start button
                    if (isInside(mx, my, 350, 600, 300, 70))
                    {
                        for (int i = 0; i < numberOfPlayers; i++)
                        {
                            if (names[i] == "")
                                names[i] = "Player " + to_string(i + 1);
                        }

                        setupGame(players, names, numberOfPlayers);

                        screen = REVEAL;
                        frames = 0;
                    }
                }
                else if (screen == REVEAL)
                {
                    if (frames >= numberOfPlayers * 45 &&
                        isInside(mx, my, 350, 620, 300, 70))
                    {
                        dice = createDice();
                        currentPlayer = 0;
                        gameFinished = false;

                        movement.active = false;
                        sixesInRow = 0;
                        rollProcessed = false;

                        screen = PLAYING;
                        frames = 0;
                    }
                }
                else if (screen == PLAYING &&
                    !gameFinished && !movement.active)
                {
                    // ROLL button
                    if (clickedRollButton(mx, my))
                    {
                        if (!dice.rolling && dice.result == 0)
                        {
                            diceSound.play();

                            startRoll(dice);
                            rollProcessed = false;
                        }
                    }

                    // Token click
                    else if (dice.result > 0 && rollProcessed)
                    {
                        int tokenIndex =
                            getClickedToken(window, players, currentPlayer);

                        if (tokenIndex != -1)
                        {
                            if (!startTokenMovement(
                                players,
                                currentPlayer,
                                tokenIndex,
                                dice.result))
                            {
                                cout << "This token cannot move!" << endl;
                            }
                        }
                    }
                }
            }
        }

        if (!window.isOpen())
            break;

        frames++;

        // 2. INTRO SCREEN
        if (screen == INTRO && frames > 240)
        {
            screen = MENU;
            frames = 0;
        }

        // 3. GAME LOGIC
        if (screen == PLAYING)
        {
            updateDice(dice);

            // Dice has stopped
            if (!gameFinished && !dice.rolling &&
                dice.result > 0 && !rollProcessed)
            {
                rollProcessed = true;

                if (dice.result == 6)
                    sixesInRow++;

                if (sixesInRow == 3)
                {
                    cout << "Three 6s in a row. Turn cancelled!" << endl;

                    dice.result = 0;
                    sixesInRow = 0;

                    currentPlayer = nextPlayer(
                        players, numberOfPlayers, currentPlayer
                    );
                }
                else if (!hasLegalMove(
                    players[currentPlayer], dice.result))
                {
                    cout << "No legal moves!" << endl;

                    int result = dice.result;
                    dice.result = 0;

                    if (result != 6)
                    {
                        sixesInRow = 0;

                        currentPlayer = nextPlayer(
                            players, numberOfPlayers, currentPlayer
                        );
                    }
                }
            }

            // Token movement
            if (!gameFinished && movement.active)
            {
                bool moveFinished =
                    updateTokenMovement(players, numberOfPlayers);

                if (moveFinished)
                {
                    int result = dice.result;
                    dice.result = 0;

                    gameFinished = finishPlayerTurn(
                        players,
                        numberOfPlayers,
                        currentPlayer,
                        result
                    );
                }
            }
        }

        // 4. DRAW SCREENS
        if (screen == INTRO)
        {
            drawIntro(window, font, frames);
        }
        else if (screen == MENU)
        {
            drawMenu(window, font);
        }
        else if (screen == INSTRUCTIONS)
        {
            drawInstructions(window, font, afterStart);
        }
        else if (screen == SETUP)
        {
            drawSetup(
                window, font, numberOfPlayers, names, activeBox
            );
        }
        else if (screen == REVEAL)
        {
            drawReveal(window, font, players, numberOfPlayers, frames);
        }
        else if (screen == PLAYING)
        {
            window.clear(sf::Color(250, 248, 240));

            drawBoard(window);
            drawTokens(window, players, numberOfPlayers);
            drawPanel(window);

            if (!gameFinished)
            {
                drawTurnIndicator(
                    window, font, players[currentPlayer]
                );

                drawDice(window, font, dice);
                drawRollButton(window, font, dice.rolling);
            }
        }

        window.display();
    }

    backgroundMusic.stop();

    return 0;
}