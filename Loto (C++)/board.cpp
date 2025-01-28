#include "board.hpp"
#include <fstream>
#include <stdexcept>
#include <random>
#include <ctime>

//-------------

Board::Board(){
    this->board_addr = "";
    for(auto i=0; i<this->sizeN; i++){
        this->num[i] = 0;
        this->num_row[i] = 0;
    }
}

Board::~Board(){

}

bool Board::loadFile(const std::string& in_f){
    this->board_addr = in_f;
    std::ifstream inp(in_f);
    if(!inp){
        throw std::invalid_argument("Board::loadFile 1.\n");
        return false;
    }

    // input numbers
    for(size_t i=0; i<Board::sizeN; i++){
        unsigned int temp;
        inp >> temp;
        this->num[i] = temp;
    }
    for(size_t i=0, k=0; i<Board::sizeY; i++){
        for(size_t j=0; j<Board::sizeX; j++, k++){
            this->num_row[k] = i;
        }
    }
    inp.close();

    // sort numbers
    this->sortNum(0, Board::sizeN-1);
    return true;
}

int Board::getRow(unsigned char k){
    int l = 0, h = Board::sizeN-1;
    int id = -1;
    while(l <= h){
        int m = (l + h) / 2;
        if(this->num[m] == k){
            id = m;
            break;
        }
        else if(this->num[m] > k){
            h = m-1;
        }
        else{
            l = m+1;
        }
    }
    return (id == -1)? -1 : this->num_row[id];
}

void Board::memcpy_num(unsigned char* dst, int n){
    if(!dst){
        throw std::invalid_argument("Board::memcpy_num 1.\n");
    }
    n = std::min(n, Board::sizeN);
    for(auto i=0; i<n; i++){
        dst[i] = this->num[i];
    }
}
void Board::memcpy_numrow(unsigned char* dst, int n){
    if(!dst){
        throw std::invalid_argument("Board::memcpy_numrow 1.\n");
    }
    n = std::min(n, Board::sizeN);
    for(auto i=0; i<n; i++){
        dst[i] = this->num_row[i];
    }
}

void Board::swapIndex(int a, int b){
    auto c = this->num[a];
    this->num[a] = this->num[b];
    this->num[b] = c;

    c = this->num_row[a];
    this->num_row[a] = this->num_row[b];
    this->num_row[b] = c;
}

void Board::sortNum(int l, int h){
    #define arr this->num
    if(l < h){
        int pi = this->sortPartition(l, h);

        this->sortNum(l, pi-1);
        this->sortNum(pi + 1, h);
    }
    #undef arr
}

int Board::sortPartition(int l, int h){
    #define arr this->num
    auto pivot = arr[h];
    auto i = l - 1;

    for(auto j=l; j<h; j++){
        if(arr[j] <= pivot){
            i++;
            this->swapIndex(i, j);
        }
    }

    this->swapIndex(i+1, h);

    return i+1;
    #undef arr
}

void Board::printNum(std::ostream& out){
    out << "[";
    for(auto i=0; i<Board::sizeN-1; i++){
        out << (int) this->num[i] << ", ";
    }
    out << (int) this->num[Board::sizeN-1] << "]\n";
}

void Board::printNumRow(std::ostream& out){
    out << "[";
    for(auto i=0; i<Board::sizeN-1; i++){
        out << (int) this->num_row[i] << ", ";
    }
    out << (int) this->num_row[Board::sizeN-1] << "]\n";
}

//------------------

BoardPlayer::BoardPlayer():Board(){
    this->winState = false;
    for(auto i=0; i<Board::sizeY; i++)
        this->prog[i] = 0;
}

BoardPlayer::~BoardPlayer(){

}

void BoardPlayer::resetProg(){
    for(auto i=0; i<Board::sizeY; i++)
        this->BoardPlayer::prog[i] = 0;
    this->BoardPlayer::winState = false;
}

void BoardPlayer::updateNextNum(unsigned char num){
    int k = this->Board::getRow(num);
    if(k == -1){
        // do nothing
        return;
    }
    else if(k >= this->Board::sizeY){
        throw std::invalid_argument("BoardPlayer::updateNextNum 1.\n");
    }

    this->BoardPlayer::prog[k]++;
    if(this->BoardPlayer::prog[k] == this->Board::sizeX)
        this->BoardPlayer::winState = true;
}

bool BoardPlayer::isWon(){
    return this->BoardPlayer::winState;
}

void BoardPlayer::memcpy_prog(unsigned char* dst, int n){
    if(!dst){
        std::invalid_argument("BoardPlayer::memcpy_prog 1.\n");
    }
    n = std::min(n, Board::sizeY);
    for(auto i=0; i<n; i++){
        dst[i] = this->prog[i];
    }
}

void BoardPlayer::printProg(std::ostream& out){
    out << "[";
    for(auto i=0; i<this->Board::sizeY-1; i++){
        out << (int) this->prog[i] << ", ";
    }
    out << (int)this->prog[this->Board::sizeY-1] << "]\n";
}

// -------------

Player::Player(){

}

void Player::setCredentials(const std::string& name, const std::string& board_addr){
    this->name = name;
    this->board.loadFile(board_addr);
}

std::string Player::getName(){
    return this->name;
}

void Player::playNextNum(unsigned char num){
    this->board.updateNextNum(num);
}

void Player::newGame(){
    this->board.resetProg();
}

bool Player::isWon(){
    return this->board.isWon();
}

void Player::printProg(std::ostream& out){
    out << "Player: " << this->name << "\n";
    this->board.printProg(out);
}

// --------
NumberDeck::NumberDeck(){
    srand(time(NULL));
    this->count = 0;
    this->reset();
}

NumberDeck::~NumberDeck(){

}

void NumberDeck::reset(){
    for(auto i=0; i<NumberDeck::sizeN; i++)
        this->deck[i] = i+1;
    this->count = 0;
}

void NumberDeck::shuffle(){
    for(auto i=0; i<1000; i++){
        unsigned char a = rand() % NumberDeck::sizeN;
        unsigned char b = rand() % NumberDeck::sizeN;

        if(a == b){
            if(b == NumberDeck::sizeN-1)
                b = 0;
            else
                b++;
        }
        auto c = this->deck[a];
        this->deck[a] = this->deck[b];
        this->deck[b] = c;
    }
}

int NumberDeck::getNext(){
    if(this->count >= NumberDeck::sizeN){
        throw std::invalid_argument("NumberDeck::getNext 1.\n");
    }
    return this->deck[this->count++];
}

void NumberDeck::memcpy_deck(unsigned char* dst, int n){
    if(!dst){
        throw std::invalid_argument("NumberDeck::memcpy_deck 1.\n");
    }
    n = std::min(n, NumberDeck::sizeN);
    for(auto i=0; i<n; i++){
        dst[i] = this->deck[i];
    }
}

void NumberDeck::printDeck(std::ostream& out){
    out << "[";
    for(auto i=0; i<NumberDeck::sizeN-1; i++){
        out << (int) this->deck[i] << ", ";
    }
    out << (int) this->deck[NumberDeck::sizeN-1] << "]\n";
}

// -------------- 

GameMaster* GameMaster::instance_ = NULL;
GameMaster::GameMaster(){

}

GameMaster* GameMaster::getInstance(){
    if(!GameMaster::instance_){
        GameMaster::instance_ = new GameMaster;
    }

    return GameMaster::instance_;
}

/*void GameMaster::loadJson(const std::string& in_f){
    std::ifstream inp(in_f);
    json inp_data = nlohmann::json::parse(inp);
    this->n_players = inp_data["n_players"];
    
    this->scores.resize(this->n_players);
    this->scores.resize(this->n_players);
    
    for(auto i=0; i<this->n_players; i++){
        std::string name = inp_data["players"][i]["name"];
        std::string board_addr = inp_data["players"][i]["board_addr"];

        Player player;
        player.setCredentials(name, board_addr);
        this->players.push_back(player);
    }
    inp.close();
}*/

void GameMaster::loadTextFile(const std::string& in_f){
    std::ifstream inp(in_f);
    if(!inp){
        throw std::invalid_argument("GameMaster::loadTextFile 1.\n");
    }

    inp >> this->n_players;
    for(auto i=0; i<this->n_players; i++){
        std::string name, addr;
        inp >> name >> addr;
        Player temp;
        temp.setCredentials(name, addr);

        this->players.push_back(temp);
        this->scores.push_back(0);
    }
    inp.close();
}

void GameMaster::saveScores(const std::string& out_f){
    std::ofstream out(out_f);
    if(!out){
        throw std::invalid_argument("GameMaster::saveScores 1.\n");
    }

    for(auto i=0; i<this->n_players; i++){
        out << this->players[i].getName() << " ";
        out << this->scores[i] << "\n";
    }
    out.close();
}

void GameMaster::resetScores(){
    for(auto i=0; i<this->n_players; i++){
        this->scores[i] = 0;
    }
}

void GameMaster::autoPlay(int n_games){
    if(this->n_players == 0){
        throw std::invalid_argument("GameMaster::autoPlay 1.\n");
    }

    for(auto i=0; i<n_games; i++){
        // reset
        deck.reset();
        deck.shuffle();
        for(auto j=0; j<this->n_players; j++){
            this->players[j].newGame();
        }

        // play
        bool winFlag = false;
        while(!winFlag){
            std::vector<unsigned char> winner_index;

            auto nextNum = deck.getNext();
            
            for(auto j=0; j<this->n_players; j++){
                this->players[j].playNextNum(nextNum);
                if(this->players[j].isWon()){
                    winFlag = true;
                    winner_index.push_back(j);
                }
            }

            this->updateScores(winner_index);
        }
    }
}

void GameMaster::updateScores(std::vector<unsigned char> winner_index){
    float bonus = 1.0f / (float) winner_index.size();
    for(auto i=0; i<winner_index.size(); i++){
        this->scores[winner_index[i]] += bonus;
    }
}

void GameMaster::printScores(std::ostream& out){
    for(auto i=0; i<this->n_players; i++){
        out << this->players[i].getName() << " ";
        out << this->scores[i] << "\n";
    }
}

void GameMaster::printScoresSorted(std::ostream& out, bool ascending, unsigned int crop){
    std::vector<unsigned char> index;
    index.resize(this->n_players);
    for(auto i=0; i<this->n_players; i++){
        index[i] = i;
    }

    for(auto i=0; i<this->n_players-1; i++){
        for(auto j=i+1; j<this->n_players; j++){
            if((this->scores[index[i]] > this->scores[index[j]]) != ascending){
                auto c = index[i];
                index[i] = index[j];
                index[j] = c;
            }
        }
    }

    for(auto i=0; i<std::min(this->n_players, crop); i++){
        out << this->players[index[i]].getName() << " ";
        out << this->scores[index[i]] << "\n";
    }

    out << "(Showing top " << std::min(this->n_players, crop) << "/" << this->n_players << ")\n";
}