#include "board.hpp"

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char** argv){
    ofstream out("results/3.txt");
    GameMaster* M = GameMaster::getInstance();
    M->loadTextFile("gamemaster.txt");

    M->autoPlay(1e7);
    M->printScoresSorted(out, 1, 16);

    out.close();
    return 0;
}