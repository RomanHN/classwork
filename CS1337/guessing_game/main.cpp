#include "Romans_toolbox.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::ofstream;
using std::ifstream;
using std::ostream;

// This is the setup for an animal guessing game, but you can change it.
const string GUESSING_NOUN = "animal";
const string DATABASE_FILE_NAME = "animals.txt";

typedef struct node {
    string inquiry;
    node *ifYes;
    node *ifNo;
} node;

bool isGuess(node theInquiry);

void askGuess(node &theGuess);

// "Inquiry" refers to either a guess or a question, depending on the type of node.
void askInquiry(node &theInquiry);

bool yesOrNo();

void wrongGuess(node &thisGuess);

void saveDatabase(ostream &saveFile, node* database, int indent);
void loadDatabase(ifstream &saveFile, node* database);

string tabs(int size);

// For example, transforms "lizard" to "a lizard" and "emu" to "an emu".
string a_an(string noun);

int main() {
    node firstInquiry{"", nullptr, nullptr};

    cout << "Loading knowledge database from \"" << DATABASE_FILE_NAME << "\"." << endl;
    ifstream loadFile(DATABASE_FILE_NAME);
    ensure(loadFile.is_open());
    loadDatabase(loadFile, &firstInquiry);
    loadFile.close();

    cout << "Think of " << a_an(GUESSING_NOUN) << ", then press enter to continue." << endl;

    // Only advances if the user presses enter.
    cin.get();

    do {
        askInquiry(firstInquiry);
        cout << "Do you want to play again? [y/n] ";
    } while (yesOrNo());

    ofstream saveFile(DATABASE_FILE_NAME);
    saveDatabase(saveFile, &firstInquiry, 0);
    saveFile.close();

    //cout << endl << "Updated database:" << endl;
    //saveDatabase(cout, &firstInquiry, 0);
    cout << endl;

    cout << "Thanks for playing! Goodbye :)" << endl;

    return (0);
}

void askInquiry(node &theInquiry) {
    if (isGuess(theInquiry)) {

        // Take the guess.
        askGuess(theInquiry);

    } else {

        // Ask the question.
        cout << theInquiry.inquiry << " [y/n] ";

        // Recursively move to the "yes" or "no" nodes depending on user's answer.
        askInquiry(*(yesOrNo() ? theInquiry.ifYes : theInquiry.ifNo));
    }
}

bool isGuess(node theInquiry) {
    return (theInquiry.ifYes == nullptr && theInquiry.ifNo == nullptr);
}

void askGuess(node &theGuess) {
    cout << "Are you thinking of " << a_an(theGuess.inquiry) << "? [y/n] ";
    if (yesOrNo()) {
        cout << "GG ez 😎" << endl;
    } else {
        wrongGuess(theGuess);
    }
}

void wrongGuess(node &thisGuess) {
    cout << "Darn! I couldn't think of your " << GUESSING_NOUN << ". What was it?" << endl;
    string correctAnswer;
    getline(cin, correctAnswer);
    cout << "Ohhh, tricky! What's a good yes-or-no question I could use to differentiate " << a_an(correctAnswer)
         << " from " << a_an(thisGuess.inquiry) << "?" << endl;
    string differentiatingQuestion;
    getline(cin, differentiatingQuestion);
    cout << "And what is the answer to \"" << differentiatingQuestion << "\" for " << a_an(correctAnswer) << "? [y/n] ";

    // Create two leaves in the tree: one for a "yes" answer and one for a "no".
    node *newAnimal = new node{correctAnswer, nullptr, nullptr};
    node *movedThisGuess = new node{thisGuess.inquiry, nullptr, nullptr};
    if (yesOrNo()) {
        thisGuess.ifYes = newAnimal;
        thisGuess.ifNo = movedThisGuess;
    } else {
        thisGuess.ifYes = movedThisGuess;
        thisGuess.ifNo = newAnimal;
    }

    // The current node is now a question node, and we use the user's provided question.
    thisGuess.inquiry = differentiatingQuestion;
}

string a_an(string noun) {
    string output = "a";
    std::vector<char> vowels {'a', 'e', 'i', 'o', 'u'};
    if (charInVector(noun[0], vowels)) {
        output += "n";
    }
    output += " ";
    output += noun;
    return(output);
}

void saveDatabase(ostream &databaseFile, node* database, int indent) {
    if (isGuess(*database)) {
        databaseFile << tabs(indent) << "G " << database->inquiry << endl;
    } else {
        databaseFile << tabs(indent) << "Q " << database->inquiry << endl;
	indent++;
        saveDatabase(databaseFile, database->ifYes, indent);
        saveDatabase(databaseFile, database->ifNo, indent);
    }
}

string tabs(int size) {
	string output = "";
	for (int i = 0; i < size; i++) {
		output += '\t';
	}
	return(output);
}

void loadDatabase(ifstream &databaseFile, node* database) {
	char inquiryType;
	databaseFile >> inquiryType;
	switch (inquiryType) {
		case 'G':
			databaseFile >> std::ws;
			getline(databaseFile, database->inquiry);
			break;
		case 'Q':
			databaseFile >> std::ws;
			getline(databaseFile, database->inquiry);
			database->ifYes = (new node {"", nullptr, nullptr});
			database->ifNo  = (new node {"", nullptr, nullptr});
			loadDatabase(databaseFile, database->ifYes);
			loadDatabase(databaseFile, database->ifNo);
			break;
		default:
			cout << "Loading database failed." << endl;
			exit(1);
	}
}
