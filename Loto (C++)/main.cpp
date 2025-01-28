#include "board.hpp"

#include <iostream>

using namespace std;

int main(int argc, char** argv){
    GameMaster* M = GameMaster::getInstance();
    M->loadTextFile("gamemaster.txt");

    M->autoPlay(1e7);
    M->printScoresSorted(cout);
    return 0;
}