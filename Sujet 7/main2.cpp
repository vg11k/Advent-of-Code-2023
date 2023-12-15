#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
using namespace std;


struct Hand {
    string cards;
    int bid;
    string svalue;
    int value;
    vector<int> cardValues;
};

void setHandType(Hand& hand, map<string, int> cardToValue, map<string, int> handTypeToValue) {

    int carsCounter[13] = { 0 };
    for (int cardIndex = 0; cardIndex < hand.cards.length(); cardIndex++) {
        string charac = hand.cards.substr(cardIndex, 1);
        int val = cardToValue.find(charac)->second;
        carsCounter[val]++;
        hand.cardValues.push_back(val);
    }

    int nbJokers = carsCounter[0];

    string result = "high";

    for (int i = 0; i < 13; i++) {
        int n = carsCounter[i];
        if (n == 5) result = "five";
        else if (n == 4) result = "four";
        else if (n == 3 && result == "high") result = "three";
        else if ((n == 2 && result == "three") || (n == 3 && result == "one")) result = "full";
        else if (n == 2 && result == "one") result = "two";
        else if (n == 2 && result == "high") result = "one";
    }


    if (nbJokers == 1) {
        if (result == "four") result = "five";
        else if (result == "three") result = "four";
        else if (result == "two") result = "full";
        else if (result == "one") result = "three";
        else if (result == "high") result = "one";
    }
    else if (nbJokers == 2) {
        if (result == "full") result = "five";
        else if (result == "two") result = "four";
        else if (result == "one") result = "three";
    }
    else if (nbJokers == 3) {
        if (result == "full") result = "five";
        else if (result == "three") result = "four";
    }
    else if (nbJokers == 4) {
        if (result == "four") result = "five";
    }

    hand.svalue = result;
    hand.value = handTypeToValue.find(result)->second;
}

vector<Hand> computeSortedHands(vector<Hand> hands, map<string, int> handTypeToValue, map<string, int> cardToValue) {

    vector<Hand> result;
    for (int handIndex = 0; handIndex < hands.size(); handIndex++) {

        int position = 0;
        Hand handToInsert = hands.at(handIndex);


        bool found = false;
        for (int resultIndex = 0; resultIndex < result.size() && !found; resultIndex++) {

            Hand handToCompare = result.at(resultIndex);

            if (handToCompare.value < handToInsert.value) {
                position = resultIndex;
                found = true;
            }
            else if (handToCompare.value == handToInsert.value) {

                bool equals = true;
                for (int cardIndex = 0; cardIndex < handToInsert.cardValues.size() && !found && equals; cardIndex++) {

                    if (handToCompare.cardValues.at(cardIndex) < handToInsert.cardValues.at(cardIndex) && !found) {
                        position = resultIndex;
                        found = true;
                    }
                    if (handToCompare.cardValues.at(cardIndex) > handToInsert.cardValues.at(cardIndex)) {
                        equals = false;
                    }
                }
            }
        }

        if (found)result.insert(result.begin() + position, handToInsert);
        else result.push_back(handToInsert);
    }
    return result;
}

int main(char* args[]) {

    cout << "Hello World !" << endl;

    clock_t start, end;

    ifstream infile("input.txt");
    string line;
    long long result = 0;
    vector<string> allLines;

    map<string, int> cardToValue;
    cardToValue["A"] = 12;
    cardToValue["K"] = 11;
    cardToValue["Q"] = 10;
    cardToValue["T"] = 9;
    cardToValue["9"] = 8;
    cardToValue["8"] = 7;
    cardToValue["7"] = 6;
    cardToValue["6"] = 5;
    cardToValue["5"] = 4;
    cardToValue["4"] = 3;
    cardToValue["3"] = 2;
    cardToValue["2"] = 1;
    cardToValue["J"] = 0;

    map<string, int> handTypeToValue;
    handTypeToValue["five"] = 6;
    handTypeToValue["four"] = 5;
    handTypeToValue["full"] = 4;
    handTypeToValue["three"] = 3;
    handTypeToValue["two"] = 2;
    handTypeToValue["one"] = 1;
    handTypeToValue["high"] = 0;

    start = clock();

    while (getline(infile, line)) {
        allLines.push_back(line);
    }

    vector<Hand> hands;


    for (int lineIndex = 0; lineIndex < allLines.size(); lineIndex++) {

        istringstream iss(allLines.at(lineIndex));
        string s;
        Hand h;

        getline(iss, s, ' ');
        h.cards = s;
        getline(iss, s, ' ');
        h.bid = stoi(s);

        setHandType(h, cardToValue, handTypeToValue);

        hands.push_back(h);
    }


    vector<Hand> sortedHandIndexes = computeSortedHands(hands, handTypeToValue, cardToValue);

    for (int handIndex = 0; handIndex < sortedHandIndexes.size(); handIndex++) {
        result += (sortedHandIndexes.size() - handIndex) * sortedHandIndexes.at(handIndex).bid;
    }


    end = clock();

    cout << "result : " << result << endl;
    cout << end - start << " ms to compute." << endl;

    cout << "Bye World !" << endl;
    return 0;
}