// APP_Assignment.cpp : This file contains the 'main' function. Program execution begins and ends there.
// 
// THIS IS HARRY WESSON'S (28969299) APPLIED PROGRAMMING PARADIGMS ASSINGMENT 2 PROJECT.
//

//Functionalities:
// 
//------[CORE]------
// Read in a set of txt files to populate a data structure (books, journals, conference). Assign each a unique ID (string).
// Read in a set of txt files to populate a data structure (students, staff, libstaff). Assign each a unique ID (int).
// Allow borrowing of resources, updating records. (MAX) students: 1, staff: 2, libstaff: 0.
// Books, journals borrowed once max. conferences cannot be borrowed.
// Allow the returning of resources, updating records.
// List all resources available for lending *
// Produce a report of all resources currently loaned out **
// Produce a report of all users that have borrowed a resource ***
// ------[EXTENDED]------
// * Allow the user to sort asc/desc by title or author
// ** Allow the user to sort asc/desc by title or author
// *** allow them to save the report to a txt file
// Implement a search mechanism that allows users to search for keywords in title/author/acronym. Print to console in alphabetical order
// An ordered (by occurrence) list of all borrowing/returning activity should be stored, and displayed on the console if desired by the user.
// ------[END OF LIST]------
// 
// List of classes on brief that NEED to be implemented.
//

#include <iostream>
#include <fstream>
#include <vector>

#include "resources.cpp"
#include "users.cpp"
#include "utils.cpp"

using namespace std;

void loadAllFiles(vector<unique_ptr<resource>>& library, vector<unique_ptr<person>>& users);
void loadResources(vector<unique_ptr<resource>>& library);
void loadUsers(vector<unique_ptr<person>>& users);
bool menu();

int main()
{
    // Load in the text files, present the options
    vector<unique_ptr<resource>> library;
    vector<unique_ptr<person>> users;

    loadAllFiles(library, users);

    bool finished{ true };

    while (!finished) {
        cout << "Please Select an Option";
        menu();
    }

}

void loadAllFiles(vector<unique_ptr<resource>>& library, vector<unique_ptr<person>>& users) {
    cout << "Loading resources..... \n";
    loadResources(library);
    cout << &library[1];
    cout << "Resources loaded!\n";
    cout << "Loading users.....\n";
    loadUsers(users);
    cout << "Users loaded!\n";
}

void loadResources(vector<unique_ptr<resource>>& library) {
    ifstream Resources("A2ResourceList.txt");

    // skip the opening 6 lines
    string temp;
    int lineNum = 0;

    while (lineNum < 6) {
        getline(Resources, temp);
        lineNum++;
    }

    // reads the books and adds them to the library vector
    int id{1};

    while (lineNum < 36) {
        string author, title, yearStr;
        
        getline(Resources, author);
        getline(Resources, title);
        getline(Resources, yearStr);

        lineNum += 3;

        int year = stoi(yearStr);
        library.push_back(make_unique<book>(author, title, year, id));
        id++;
    }

    getline(Resources, temp);
    lineNum++;

    // read the journals and add them to the library vector

    while (lineNum < 45) {
        string journalName;
        string editionTemp{"temp"};
        vector<string> editions;

        getline(Resources, journalName);
        lineNum += 1;

        while (editionTemp != "!") {
            getline(Resources, editionTemp);
            if (editionTemp != "!") { editions.push_back(editionTemp); }
            lineNum += 1;
        }

        library.push_back(make_unique<journal>(journalName, id, editions));
        id++;
    }
    
    getline(Resources, temp);
    lineNum++;

    // read the conferences and add them to the library vector

    while (lineNum < 73) {
        string conferenceName;
        string yearTemp{"temp"};
        vector<string> years;

        getline(Resources, conferenceName);
        lineNum += 1;

        int pos = conferenceName.find(" ");
        string acronym = conferenceName.substr(0, pos);

        while (yearTemp != "!") {
            getline(Resources, yearTemp);
            if (yearTemp != "!") { years.push_back(yearTemp); }
            lineNum += 1;
        }

        library.push_back(make_unique<conference>(conferenceName, acronym, id, years));
        id++;
    }

}

void loadUsers(vector<unique_ptr<person>>& users) { cout << "fun"; }
bool menu() { return true; }