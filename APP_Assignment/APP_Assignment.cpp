// APP_Assignment.cpp : This file contains the 'main' function. Program execution begins and ends there.
// 
// THIS IS HARRY WESSON'S (28969299) APPLIED PROGRAMMING PARADIGMS ASSINGMENT 2 PROJECT.
//

//Functionalities:
// 
// ------[EXTENDED]------
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

void loadAllFiles(vector<unique_ptr<resource>>& library, vector<unique_ptr<person>>& users, resourceList& rA);
void loadResources(vector<unique_ptr<resource>>& library, resourceList& rA);
void loadUsers(vector<unique_ptr<person>>& users);
void borrow(vector<unique_ptr<loan>>& loans, vector<unique_ptr<resource>>& library, vector<unique_ptr<person>>& users, userList& uB, resourceList& rl, resourceList& rA);
void returnresource(vector<unique_ptr<loan>>& loans, vector<unique_ptr<resource>>& library, vector<unique_ptr<person>>& users, userList& uB, resourceList& rL, resourceList& rA);
void listResources(vector<unique_ptr<resource>>& library, resourceList& rL);
void listUsers(vector<unique_ptr<person>>& users, userList& uL);
int pickID(string fetching);
int getChoice(int max);
bool validate(int userID, int bookID, vector<unique_ptr<resource>>& library, vector<unique_ptr<person>>& users);
bool menu(vector<unique_ptr<resource>>& library, vector<unique_ptr<person>>& users, vector<unique_ptr<loan>>& loans, userList& uB, resourceList& rL, resourceList& rA);
resourceList sortList(vector<unique_ptr<resource>>& library, resourceList& rL);

int main()
{
    // Load in the text files, present the options
    vector<unique_ptr<resource>> library;
    vector<unique_ptr<person>> users;
    vector<unique_ptr<loan>> loans;
    userList usersBorrowing;
    resourceList resourcesLent;
    resourceList resourcesAvailable;


    loadAllFiles(library, users, resourcesAvailable);

    bool finished{ false };
    cout << "----- Welcome to UniLib: The University Library ----- \n";

    while (!finished) {
        cout << "\nPlease Select an Option \n";
        finished = menu(library, users, loans, usersBorrowing, resourcesLent, resourcesAvailable);
    }

}

void loadAllFiles(vector<unique_ptr<resource>>& library, vector<unique_ptr<person>>& users, resourceList& rA) {
    cout << "Loading resources..... \n";
    loadResources(library, rA);
    cout << "Resources loaded!\n";
    cout << "Loading users.....\n";
    loadUsers(users);
    cout << "Users loaded!\n\n ";
}

void loadResources(vector<unique_ptr<resource>>& library, resourceList& rA) {
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

    for (int i = 0; i < id; i++) {
        rA.addItem(i);
    }
}

void loadUsers(vector<unique_ptr<person>>& users) {
    ifstream Users("A2UserList.txt");
    
    int end{12};
    int lineNum{ 0 };
    int id{ 1 };

    while (lineNum < end) {
        string userTemp;

        getline(Users, userTemp);
        string type{ userTemp[0] };
        int choice{ 0 };
        choice = stoi(type);

        switch (choice) {
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

        id++;
        lineNum++;
    }
}

void borrow(vector<unique_ptr<loan>>& loans, vector<unique_ptr<resource>>& library, vector<unique_ptr<person>>& users, userList& uB, resourceList& rL, resourceList& rA) {
    int userID{ 0 };
    int bookID{ 0 };
    bool bookBorrowed{ false };

    // get the user and book id
    userID = pickID(" user ");
    bookID = pickID(" resource ");

    // if the book and person exist
    if (validate(userID, bookID, library, users)) {

        // find the book
        for (auto& ptr : library) {
            if (ptr->getID() == bookID) {
                // if its not borrowed then borrow
                if (ptr->checkIfCanLend()) {
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
            uB.addItem(userID);
            rL.addItem(bookID);
            rA.removeItem(bookID);
        }
    }
}

void returnresource(vector<unique_ptr<loan>>& loans, vector<unique_ptr<resource>>& library, vector<unique_ptr<person>>& users, userList& uB, resourceList& rL, resourceList& rA) {
    // this contains multiple validation incase of external interferernce with the txt or the loan vector being untrue.
    int userID{ 0 };
    int bookID{ 0 };
    int pos0{ 0 };
    int pos1{ 0 };
    bool bookreturned{ false };
    bool correctLoan{ false };

    // get the user and book id
    userID = pickID(" user ");
    bookID = pickID(" resource ");

    // find the record of the loan
    for (auto& ptr : loans) {
        if ((ptr->getPersonID() == userID) && (ptr->getResourceID() == bookID)) {
            correctLoan = true;
            pos1 = pos0;
        }
        pos0++;
    }

    // delete the record of the loan, regardless if the book or person actually exist.
    if (correctLoan) { loans.erase(loans.begin() + pos1); }

    // if the book and person exist and it has been loaned by that person
    if ((validate(userID, bookID, library, users)) && correctLoan) {

        // find the book
        for (auto& ptr : library) {
            if (ptr->getID() == bookID) {
                // if its borrowed then return
                if (ptr->checkIfBorrowed()) {
                    ptr->setBorrowed(false);
                    cout << "returned!\n";
                    bookreturned = true;
                }
                else {
                    cout << "This resource is already returned.\n";
                }
            }
        }

        // if we returned
        if (bookreturned) {
            // find the user
            for (auto& ptr : users) {
                // and give a token for borrowing
                if (ptr->getID() == userID) {
                    { ptr->incBorrow(); }
                }
            }
            uB.removeItem(userID);
            rL.removeItem(bookID);
            rA.addItem(bookID);
        }
    }
}

void listResources(vector<unique_ptr<resource>>& library, resourceList& rL) {

    vector<int>list = sortList(library, rL).getList();

    cout << "\nList of resources: \n";

    for (auto& ptr : library) {
        for (int id : list) {
            if (ptr->getID() == id) {
                cout << id << " : " << ptr->getTitle() << "\n";
            }
        }
    }

    cout << "\n";
}

void listUsers(vector<unique_ptr<person>>& users, userList& uL) {
    vector<int> list = uL.getList();

    cout << "List of users: \n";

    for (auto& ptr : users) {
        for (int id : list) {
            if (ptr->getID() == id) {
                cout << id << " : " << ptr->getName() << "\n";
            }
        }
    }

    cout << "\n";
}

int pickID(string fetching) {
    int x;

    cout << "Please input the" << fetching << "ID : ";
    cin >> x;

    return x;
}

int getChoice(int max) {

    int choice{ 0 };
    cout << "Your Choice: ";
    cin >> choice;

    while (choice < 1 && choice > max) {
        cout << "\nInvalid choice, please pick again: ";
        cin >> choice;
    }

    return choice;
}

bool validate(int userID, int bookID, vector<unique_ptr<resource>>& library, vector<unique_ptr<person>>& users) {

    bool bookExists{ false };
    bool personExists{ false };

    // does the book exist?
    for (auto& ptr : library) {
        if (ptr->getID() == bookID) {
            bookExists = true;
        }
    }

    if (!bookExists) { cout << "BOOK NO EXISTS"; }

    // does the person exist?
    for (auto& ptr : users) {
        if (ptr->getID() == userID) {
            personExists = true;
        }
    }

    if (!personExists) { cout << "person NO EXISTS"; }

    return (bookExists && personExists);
}

resourceList sortList(vector<unique_ptr<resource>>& library, resourceList& rL) {
    int size = library.size();
    resourceList sorted{};

    cout << "\n1. Title Ascending? \n";
    cout << "2. Title Descending? \n";
    cout << "3. Author Ascending? \n";
    cout << "4. Author Descending? \n\n";

    int choice = getChoice(4);
    
    switch (choice) {
    case 1:
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - 1; j++) {
                if ((library[j]->getTitle()) > library[j + 1]->getTitle())
                    swap(library[j], library[j + 1]);
            }
        }
        break;
    case 2:
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - 1; j++) {
                if ((library[j]->getTitle()) < library[j + 1]->getTitle())
                    swap(library[j], library[j + 1]);
            }
        }
        break;
    case 3:
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - 1; j++) {
                if ((library[j]->asString()) > library[j + 1]->asString())
                    swap(library[j], library[j + 1]);
            }
        }
        break;
    case 4:
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - 1; j++) {
                if ((library[j]->asString()) < library[j + 1]->asString())
                    swap(library[j], library[j + 1]);
            }
        }
        break;
    default:
        cout << "Error!";
        break;
    }

    
    for (auto& ptr : library) {
        for (int id : rL.getList()) {
            if (ptr->getID() == id) {
                sorted.addItem(ptr->getID());
            }
        }
    }

    return sorted;
}

bool menu(vector<unique_ptr<resource>>& library, vector<unique_ptr<person>>& users, vector<unique_ptr<loan>>& loans, userList& uB, resourceList& rL, resourceList& rA) {
    

    cout << "1. Borrow \n";
    cout << "2. Return \n";
    cout << "3. List all available resources \n";
    cout << "4. Report borrowed resources \n";
    cout << "5. Report borrowing users \n";
    cout << "6. Search NOT IMPLEMENTED \n";
    cout << "7. History NOT IMPLEMENTED \n";
    cout << "8. Exit \n \n";

    int choice = getChoice(8);

    switch (choice) {
    case 1:
        borrow(loans, library, users, uB, rL, rA);
        break;
    case 2:
        returnresource(loans, library, users, uB, rL, rA);
        break;
    case 3:
        listResources(library, rA);
        break;
    case 4:
        listResources(library, rL);
        break;
    case 5:
        listUsers(users, uB);
        break;
    case 6:
        cout << "no";
        break;
    case 7: 
        cout << "no";
        break;
    case 8:
        return true;
        break;
    default:
        cout << "error!";
        break;
    }
    return false;
}