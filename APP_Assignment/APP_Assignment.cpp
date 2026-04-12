// APP_Assignment.cpp : This file contains the 'main' function. Program execution begins and ends there.
// 
// THIS IS HARRY WESSON'S (28969299) APPLIED PROGRAMMING PARADIGMS ASSINGMENT 2 PROJECT.
//

//Functionalities:
// 
//------[CORE]------
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
void borrow(vector<unique_ptr<loan>>& loans, vector<unique_ptr<resource>>& library, vector<unique_ptr<person>>& users);
void returnresource(vector<unique_ptr<loan>>& loans, vector<unique_ptr<resource>>& library, vector<unique_ptr<person>>& users);
bool validate(int userID, int bookID, vector<unique_ptr<resource>>& library, vector<unique_ptr<person>>& users);
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
    for (auto& ptr : library) {
        cout << ptr->getID() << "\n";
    }
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

void loadUsers(vector<unique_ptr<person>>& users) {
    ifstream Users("A2UserList.txt");
    
    int end{12};
    int lineNum{ 0 };
    int id{ 0 };

    while (lineNum < end) {
        string userTemp;

        getline(Users, userTemp);
        string type{ userTemp[1] };

        switch (stoi(type)) {
        case 1:
            users.push_back(make_unique<student>(userTemp, id));
            break;
        case 2:
            users.push_back(make_unique<staff>(userTemp, id));
            break;
        case 3:
            users.push_back(make_unique<libstaff>(userTemp, id));
            break;
        default:
            break;
        }

        lineNum++;
    }
}

bool menu() { return true; }

bool validate(int userID, int bookID, vector<unique_ptr<resource>>& library, vector<unique_ptr<person>>& users) {

    bool bookExists{ false };
    bool personExists{ false };

    // does the book exist?
    for (auto& ptr : library) {
        if (ptr->getID() == bookID) {
            bookExists = true;
        }
    }

    // does the person exist?
    for (auto& ptr : users) {
        if (ptr->getID() == userID) {
            personExists = true;
        }
    }

    return (bookExists && personExists);
}

void borrow(vector<unique_ptr<loan>>& loans, vector<unique_ptr<resource>>& library, vector<unique_ptr<person>>& users) {
    int userID;
    int bookID;
    bool bookBorrowed{ false };

    // if the book and person exist
    if (validate(userID, bookID, library, users)) {

        // find the book
        for (auto& ptr : library) {
            if (ptr->getID() == bookID) {
                // if its not borrowed then borrow
                if (!(ptr->checkIfBorrowed())) {
                    ptr->setBorrowed(true);
                    cout << "Borrowed!\n";
                    loans.push_back(make_unique<loan>(userID, bookID));
                    bookBorrowed = true;
                }
                else {
                    cout << "This resource can not be borrowed.\n";
                }
            }
        }

        // if we borrowed
        if (bookBorrowed) {
            // find the user
            for (auto& ptr : users) {
                // and take away a token for borrowing
                if (ptr->getID() == userID) {
                    if (ptr->getBorrowCount() > 0) { ptr->decBorrow(); }
                }
            }
        }
    }
}

void returnresource(vector<unique_ptr<loan>>& loans, vector<unique_ptr<resource>>& library, vector<unique_ptr<person>>& users) {

}