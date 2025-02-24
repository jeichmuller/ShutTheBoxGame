// Jay Eichmuller
// Based off the Shut The Box game

#include <iostream>
#include <map>
#include <ctime>
#include <string>
#include <vector>
#include <ctype.h>



using std::cout; using std::cin; using std::endl;
using std::map;
using std::string;
using std::vector;

int diceRoll();
vector<int> getInput(string);
bool checkBoard(const map<int, bool> &,const vector<int> &);
void shut(map<int, bool> &, const vector<int> &);

int main(){

    srand(time(nullptr));
    

    int dice1, dice2;
    dice1 = diceRoll();
    dice2 = diceRoll();

    // cout << "dice1: " << dice1 << ", dice2: " << dice2 << endl;

    map<int, bool> board = {{1, false}, {2, false}, {3, false}, {4, false}, {5, false}, {6, false}, {7, false}, {8, false}, {9, false}};
    
    // for(auto e = board.begin(); e != board.end(); ++e) cout << e->first << ", " << e->second << endl;

    bool game = true;

    while(game){

        dice1 = diceRoll();
        dice2 = diceRoll();

        int roll = dice1 + dice2;
        int total = 0;

        for (auto e: board){
            cout << e.first << ", " << e.second << endl << endl;
        }
        cout << "You rolled " << dice1 << " and " << dice2 << ". What numbers would you like to put down? " << endl;
        cout << "Roll: " << roll << endl;
        bool valid = true;
        do{
            string str; 
            getline(cin, str);

            valid = true;
            total = 0;

            vector<int> numbers;
            int i = 0;
            for(auto ints: str){
                if(i == 0 || i % 2 == 0){
                    numbers.push_back(int(ints) - 48); // convert to using ascii
                    total += numbers.back();
                }
                ++i;
            }
            

            if(total != roll){
                cout << "Your roll is " << roll << " which does not equal the sum of numbers you input. Try again" << endl;
                continue;
            }

            if(checkBoard(board, numbers)){
                cout << "Try again!" << endl;
                continue;
            }else{
                shut(board, numbers);
            }
        }while(total != roll);
        //cout << "numbers: " << str << endl;
        
    }








}





int diceRoll() {
    return rand() % 6 + 1;
}


vector<int> getInput(string str){
    vector<int> numbers;
    for (int i = 0; i < str.length(); ++i){
        if(isdigit(str[i])){
            int num = int(str[i]) - 48;
            numbers.push_back(num);
        }
    }
    return numbers;
}

bool checkBoard(const map<int, bool> &board,const vector<int> &numbs){
    for (auto num: numbs){
        if(board.find(num)->second){
            return true;
        }
    }
    return false;
}

void shut(map<int, bool> &board, const vector<int> &numbs){
    for(auto num: numbs){
        board.find(num)->second = true;
    }
}