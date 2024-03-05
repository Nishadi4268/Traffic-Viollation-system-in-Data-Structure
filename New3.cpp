#pragma warning(disable : 4996)
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <Windows.h>
#include "Header.h"
using namespace std;

int main()
{
    VehicleHashTable Vehicles;
    ifstream file1("VehicleNumbers.txt");
    string line1;
    while (getline(file1, line1)) Vehicles.insertVehicle(line1);
    file1.close();

    ViolationArray Violations;
    ifstream file2("TrafficLawViolations.txt");
    string line2{};
    while (getline(file2, line2)) {
        string des{};
        int i{}, n{};
        int in{}, fine{};
        for (int space = 0; space < 2; space++) {
            string temp = "";
            while (line2[i] != ' ') {
                i++;
            }
            if (space == 0) in = stoi(temp.append(line2, n, i - n));
            else fine = stoi(temp.append(line2, n, i - n));
            i++;
            n = i;
        }
        i = line2.length();
        des.append(line2, n, i - n);
        Violations.insert(in, fine, des);
    }
    file2.close();

    ifstream file3("History.txt");
    string vnum{};
    int code{};
    while (file3 >> vnum >> code) {
        Vehicles.insertViolationToVehicle(vnum, code, time(NULL), &Violations);
        Sleep(1);
    }
    file3.close();

    while (true) {
        cout << "Enter 'F' to generate the Fine for a Vehicle for Violating a Traffic Law" << endl;
        cout << "Enter 'C' to check the previous Traffic Law Violations of a Vehicle" << endl;
        cout << "Enter 'V' to view the code and the description of Traffic Law Violations" << endl;
        cout << "Enter 'A' to analyze the number of violations of a Traffic Law till now" << endl;
        char m;
        cin >> m;
        cout << endl;
        if (m == 'F') {
            cout << "Enter the Registration Number of the Vehicle : ";
            string vnum{};
            cin >> vnum;
            cout << "Enter the Code of the Traffic Law : ";
            int code{};
            cin >> code;
            cout << endl;
            cout << "The Vehicle '" << vnum << "' has committed the Traffic Law Violation of '" << Violations.arrayViolations[code].violation << "'." << endl;
            cout << "The Fine for the Violation is Rs." << Vehicles.insertViolationToVehicle(vnum, code, time(NULL), &Violations) << endl << endl;
        }
        else if (m == 'C') {
            cout << "Enter the Registration Number of the Vehicle : ";
            string vnum{};
            cin >> vnum;
            cout << endl;
            cout << "The History of the Traffic Law Violations of the Vehicle '" << vnum << "' is," << endl;
            Vehicles.getViolations(vnum, &Violations);
            cout << endl;
        }
        else if (m == 'V') {
            Violations.printViolations();
            cout << endl;
        }
        else if (m == 'A') {
            cout << "Enter the Code of the Traffic Law Violation : ";
            int h{};
            cin >> h;
            cout << "The Incidents of Traffic Law Violation of '" << Violations.arrayViolations[h].violation << "' are," << endl;
            Violations.analyzeViolations(h);
            cout << endl;
        }
        else {
            cout << "Error:Invalid Input" << endl << endl;
        }
    }
}
