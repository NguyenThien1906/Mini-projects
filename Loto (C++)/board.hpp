#ifndef BOARD_HPP
#define BOARD_HPP

#define DEBUG_FLAG

#include <string>
#include <iostream>
#include <vector>

class Board{
// func
public:
    Board();
    ~Board();
    bool loadFile(const std::string& in_f);
    int getRow(unsigned char num);

private:
    void sortNum(int l, int r);
    int sortPartition(int l, int r);
    void swapIndex(int a, int b);

#ifdef DEBUG_FLAG 
public: 
#else
private:
#endif
    void printNum(std::ostream& out);
    void printNumRow(std::ostream& out);

// var
public:
    static const unsigned char sizeX = 5, sizeY = 9, sizeN = 45;

private:
    unsigned char num[sizeN];
    unsigned char num_row[sizeN];
    std::string board_addr;
};

class BoardPlayer : public Board{
// func
public:
    BoardPlayer();
    ~BoardPlayer();

    bool loadBoard(const std::string& b_addr);
    void resetProg();
    void updateNextNum(unsigned char num);
    bool isWon();
#ifdef DEBUG_FLAG 
public: 
#else
private:
#endif
    void printProg(std::ostream& out);

// var
private:
    bool winState;
    unsigned char prog[sizeY];
};

class Player{
// func
public:
    Player();

    void setCredentials(const std::string& name, const std::string& board_addr);
    std::string getName();
    
    void playNextNum(unsigned char num);
    void newGame();
    bool isWon();
#ifdef DEBUG_FLAG 
public: 
#else
private:
#endif
    void printProg(std::ostream& out);
// var
private:
    BoardPlayer board;
    std::string name;
};

class NumberDeck{
// func
public:
    NumberDeck();
    ~NumberDeck();

    void reset();
    void shuffle();
    int getNext();
#ifdef DEBUG_FLAG 
public: 
#else
private:
#endif
    void printDeck(std::ostream& out);

// var
private:
    static const unsigned char sizeN = 90;
    unsigned char count;
    unsigned char deck[sizeN];
};

class GameMaster{
// func
public:
    static GameMaster* getInstance();
    //void loadJson(const std::string& in_f);
    void loadTextFile(const std::string& in_f);
    void saveScores(const std::string& out_f);
    void resetScores();

    void autoPlay(int n_games = 1);
    void autoPlay_GPU(int n_games = 1);
    void printScores(std::ostream& out);
    void printScoresSorted(std::ostream& out, bool ascending = true, unsigned int crop = 5);
protected:
    GameMaster();
private:
    void updateScores(std::vector<unsigned char> winner_index);

// var
private:
    static GameMaster* instance_;
    std::vector<Player> players;
    std::vector<float> scores;
    NumberDeck deck;
    unsigned int n_players = 0;
};

#endif