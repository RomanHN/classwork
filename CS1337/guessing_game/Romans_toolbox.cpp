#include "Romans_toolbox.hpp"
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using std::cin;
using std::cout;
using std::string;
using std::endl;

void ensure(bool condition) {
	if (!condition) {
		exit(EXIT_FAILURE);
	}
}

bool charInVector(char charToTest, std::vector<char> list) {
    return(*find(list.begin(), list.end(), charToTest) == charToTest);
}

bool yesOrNo() {
    string buffer;
    std::vector<char> validChars {'y', 'n', 'Y', 'N'};
    getline(cin, buffer);
    //cout << buffer << endl;
    char firstChar;
    while (true) {
        firstChar = buffer[0];
        if (charInVector(firstChar, validChars)) {
            return(firstChar == 'y' || firstChar == 'Y');
        }
        cout << "Invalid response; try again." << endl;
        getline(cin, buffer);
    }
}
