// Jay Eichmuller
// Based off the Shut The Box game

#include <iostream>
#include <map>
#include <ctime>
#include <string>
#include <vector>
#include <cctype>
#include <numeric>
#include <sstream>
#include <algorithm>

using std::cout; using std::cin; using std::endl;
using std::map;
using std::string;
using std::vector;
using std::isdigit;
using std::accumulate;
using std::pair; using std::make_pair;

int diceRoll();
vector<int> getInput(string);
bool checkBoard(const map<int, bool> &,const vector<int> &);
void shut(map<int, bool> &, const vector<int> &);
bool getMove( map<int, bool> &board, const string &str, int roll);
void print(const map<int, bool> &board);
bool possibleMove(const map<int, bool> &, int );
bool findComb(const vector<int> &remainingInt, int roll, int sum, size_t start);
pair<bool, int> win(const map<int, bool> &board);

int main(){

    srand(time(nullptr));
    

    /*
    // debug
    map<int, bool> board = {
        {1, false}, {2, false}, {3, true}, {4, false}, {5, false}, {6, true},
        {7, true}, {8, true}, {9, true}
    };

    int roll = 7;
    if (possibleMove(board, roll)) {
        cout << "A valid move exists for roll " << roll << endl;
    } else {
        cout << "No valid move exists for roll " << roll << endl;
    }
    */
    vector<int> scores;
    bool again = false; 
    do{
        again = false;


        int dice1, dice2;
        dice1 = diceRoll();
        dice2 = diceRoll();


        map<int, bool> board = {{1, false}, {2, false}, {3, false}, {4, false}, {5, false}, {6, false}, {7, false}, {8, false}, {9, false}};
        

        bool game = true;
        pair<bool, int> p;
        while(game){
            dice1 = diceRoll();
            dice2 = diceRoll();

            int roll = dice1 + dice2;

            print(board);

            if(!possibleMove(board, roll)){
                cout << "You rolled " << dice1 << " and " << dice2 << endl;
                p = win(board);
                cout << "You lose! You had a score of " << p.second << endl;
                break;
            }

            cout << "You rolled " << dice1 << " and " << dice2 << ". What numbers would you like to put down? " << endl;
            cout << "Roll: " << roll << endl;

            int total = 0;
            bool valid = true;
            do{
                string str; 
                getline(cin, str);
                if(!getMove(board, str, roll)){
                    cout << "Give new numbers, needs to equal " << roll << endl;
                    valid = false;
                }else {
                    valid = true;
                    p = win(board);
                    if(p.first){
                        cout << "Congratulations you won!" << endl;
                        cout << "Score of " << p.second << endl;
                        game = false;
                        scores.push_back(p.second);
                        break;
                    }
                }
            }while(!valid);
            cout << endl;
        }
        if(!p.first){
            cout << "You lost with a score of " << p.second << endl;
            game = false;
            scores.push_back(p.second);
        }


        cout << "Would you like to play again? (y/n) ";
        string playAgain;
        getline(cin, playAgain);
        if(playAgain[0] == 'y' || playAgain[0] == 'Y'){
            again = true;
        }
        cout << endl;
    }while(again);

    std::sort(scores.begin(), scores.end());
    cout << "Your highscores were: " << endl;
    for(int i = 0; i < scores.size() - 10; ++i){
        cout << i + 1 << ": " << scores[i] <<endl;
    }

}

// Checks if there are any moves the user can currently make with a given roll
bool possibleMove(const map<int, bool> &board, int roll){
    vector<int> remainingInt;

    for(auto e: board){
        if(!e.second){
            remainingInt.push_back(e.first);
        }
    }

    return findComb(remainingInt, roll, 0, 0);
}

// Checks if theres a combination of remaining integers that can equal the roll
bool findComb(const vector<int> &remainingInt, int roll, int sum = 0, size_t start = 0){

    if (sum == roll) {
        return true;
    }

    for (size_t i = start; i < remainingInt.size(); ++i) {
        if (sum + remainingInt[i] <= roll) {
            if (findComb(remainingInt, roll, sum + remainingInt[i], i + 1)) {
                return true;
            }
        }
    }

    return false;
}

// Rolls dice, 1-6
int diceRoll() {
    return rand() % 6 + 1;
}

// Takes users string input and breaks it up to integers
vector<int> getInput(string str){
    vector<int> numbers;
    std::istringstream stream(str);
    int num;
    while (stream >> num) {
        numbers.push_back(num);
    }
    return numbers;
}

// Determines whether given move is valid in the board
bool checkBoard(const map<int, bool> &board, const vector<int> &numbs){
    for (auto num: numbs){
        if(board.find(num)->second){
            return true;
        }
    }
    return false;
}

// Returns true if all elements of board are true
pair<bool, int> win(const map<int, bool> &board){
    return make_pair(accumulate(board.begin(), board.end(), true, [](bool result, const std::pair<const int, bool>& p) {
        return result && p.second; 
    }), accumulate(board.begin(), board.end(), 0, [](int total, const std::pair<const int, bool> &p){
        if(!p.second) return total += p.first;
        else return total;
    }));
}

// Puts the selected numbers down
void shut(map<int, bool> &board, const vector<int> &numbs){
    for(auto num: numbs){
        board.find(num)->second = true;
    }    
}

// Takes players move and checks whether it is legal or not, if legal will invoke shut(),
// else will return false allowing user to give new move
bool getMove(map<int, bool> &board, const string &str, int roll){
    int total = 0;
    vector<int> numbers = getInput(str);

    // Checking for duplicate entries
    // Ex: roll of 8, cannot be satisfied by "4 4", since theres only one 4 tile
    std::map<int, int> countMap;
    for(int num : numbers){
        ++countMap[num];
        if (num > 9) {
            cout << "You cannot input numbers greater than 9!" << endl;
            return false;
        }
    }
    // If any number is used more than once, return false
    for (const auto& entry : countMap) {
        if (entry.second > 1) {
            cout << "You cannot use any numbers more than once!" << endl;
            return false;
        }
    }

    // Get total
    for (int num : numbers) {
        total += num;
    }

    // If move total doesn't equal the roll total, not valid
    if(total != roll){
        cout << "You're input does not equal the roll (roll: " << roll << ", total you entered: " << total << ")." << endl;
        return false; 
    }
    // If the move cannot be done using the board
    if(checkBoard(board, numbers)){
        cout << "You're move is not valid!" << endl;
        return false;
    }else{
        shut(board, numbers);
        return true;
    }

}

// Prints the board
void print(const map<int, bool> &board){
    for (auto e: board){
        cout << e.first << ", ";
        e.second ? cout << "Down" : cout << "Up";
        cout << endl;
    }
}