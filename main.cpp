#include <iostream>
#include <string>
#include <cctype>
using namespace std;

int currentIndex = 0;

bool check_limit(string circuit);
float calculate_req(string circuit);
float calculate_series(string circuit, int& currentIndex);
float calculate_parallel(string circuit, int& currentIndex);

int main() {
    string circuit;
    getline(cin, circuit); // get circuit input from user
    if (check_limit(circuit)) { // check for max limit m < 20
        float Req = calculate_req(circuit); // calculate total resistance function call
        cout << "The total resistance = " << Req << endl;
    }
    else {
        cout << "Number of resistors can't exceed 20." << endl;
    }
    return 0;
}

bool check_limit(string circuit) {
    int m = 0;
    for (int i = 0; i < circuit.length(); i++) {
        if (isdigit(circuit[i])) { // check for digits (resistors) number must be <= 20
            m++;
        }
    }
    if (m <= 20) return true;
    else return false;
}

float calculate_req(string circuit) {
    float result = 0;
    for (; currentIndex < circuit.length(); currentIndex++) { // loop through all char's
        char current = circuit[currentIndex];
        if (current == 'S' || current == 's') { // check for series 
            result += calculate_series(circuit, currentIndex);
        }
        else if (current == 'P' || current == 'p') { // check for parallel
            result += calculate_parallel(circuit, currentIndex);
        }
        else if (current == ' ' || current == ',') { // skip spaces and commas
            currentIndex++;
            continue;
        }
    }
    return result;
}

float calculate_series(string circuit, int& currentIndex) {
    int resistors = 0; // check that 1 resistor or more for series
    float series_result = 0;
    string number = "";
    for (int i = currentIndex + 1; i < circuit.length(); i++) {
        char current = circuit[i];
        if (isdigit(current) || current == '.') { // check for digits that represent resistors
            number += current; // if there is a digit or a point decimal add it to number string until we encounter a space or comma
        }
        else if (current == 'P' || current == 'p') { // check for parallel connections
            series_result += calculate_parallel(circuit, i);
            resistors++;
        }
        else if (current == 'S' || current == 's') {
            series_result += calculate_series(circuit, i);
            resistors++;
        }
        else if (current == ' ' || current == ',') { // skip spaces and commas and store the added numbers
            if (!number.empty()) { // if number string not empty
                series_result += stof(number); // convert string to float and add it to result
                number = ""; // empty the number string for future numbers we encounter
                resistors++; // inc. number of resistors
            }
        }
        else if (current == 'e') { // end the connection
            if (resistors == 0) { // make sure series connection has more than 1 resistor
                cout << "Incorrect Input" << endl;
                exit(0);
            }
            else {
                currentIndex = i + 1;
                return series_result;
            }
            
        } 
        else { // check for wrong input
            cout << "Wrong Description" << endl;
            exit(0);
        }

    }
    return 0;
}

float calculate_parallel(string circuit, int& currentIndex) {
    int resistors = 0;
    float parallel_result = 0;
    string number = "";
    for (int i = currentIndex + 1; i < circuit.length(); i++) {
        char current = circuit[i];
        if (isdigit(current) || current == '.') { // check for digits that represent resistors
            number += current; // if there is a digit or a point decimal add it to number string until we encounter a space or comma
        }
        else if (current == 'S' || current == 's') { // check for series connections
            parallel_result += (1 / calculate_series(circuit, i));
            resistors++;
        }
        else if (current == 'P' || current == 'p') {
            parallel_result += (1 / calculate_parallel(circuit, i));
            resistors++;
        }
        else if (current == ' ' || current == ',') { // skip spaces and commas and store the added numbers
            if (!number.empty()) { // if number string not empty
                parallel_result += ((float)1 / stof(number)); // convert string to float and add it to result
                number = ""; // empty the number string for future numbers we encounter
                resistors++;
            }
        }
        else if (current == 'e') { // end the connection
            if (resistors <= 1) { // make sure parallel connection has more than 1 resistor
                cout << "Incorrect Input" << endl;
                exit(0);
            }
            else {
                currentIndex = i + 1;
                return (1 / parallel_result);
            }
        }
        else { // check for wrong input
            cout << "Wrong Description" << endl;
            exit(0);
        }
        
    }
    return 0; 
}
