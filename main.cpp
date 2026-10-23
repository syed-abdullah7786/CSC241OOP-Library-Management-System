/*
COMSATS University Islamabad - Department of Electrical Engineering
CSC241 Object Oriented Programming - Fall 2026
Semester Project: Library Management System
Team Members: [Enter 2-4 team members and registration numbers]
Instructor: Dr. Riaz Hussain
Language: C++17
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include <limits>

using namespace std;

// Abstract base class demonstrates abstraction and polymorphism.
class User {
protected:
    int id;
    string name;

public:
    User(int userId, const string& userName) : id(userId), name(userName) {}
    virtual ~User() = default;

    int getId() const { return id; }
    string getName() const { return name; }
    virtual string getRole() const = 0;
    virtual void display() const {
        cout << "ID: " << id << " | Name: " << name
             << " | Role: " << getRole() << '\n';
    }
};

class Member : public User {
private:
    int borrowedBookId;

public:
    Member(int memberId, const string& memberName, int borrowedId = -1)
        : User(memberId, memberName), borrowedBookId(borrowedId) {}

    string getRole() const override { return "Member"; }
    int getBorrowedBookId() const { return borrowedBookId; }
    void setBorrowedBookId(int bookId) { borrowedBookId = bookId; }
};

class Librarian : public User {
public:
    Librarian(int staffId, const string& staffName) : User(staffId, staffName) {}
    string getRole() const override { return "Librarian"; }
};

class Book {
private:
    int id;
    string title;
    string author;
    bool available;

public:
    Book(int bookId, const string& bookTitle, const string& bookAuthor,
         bool isAvailable = true)
        : id(bookId), title(bookTitle), author(bookAuthor), available(isAvailable) {}

    int getId() const { return id; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    bool isAvailable() const { return available; }
    void setAvailable(bool value) { available = value; }

    void display() const {
        cout << "Book ID: " << id << " | Title: " << title
             << " | Author: " << author
             << " | Status: " << (available ? "Available" : "Borrowed") << '\n';
    }
};

class Loan {
private:
    int bookId;
    int memberId;
    bool active;

public:
    Loan(int bId, int mId, bool isActive = true)
        : bookId(bId), memberId(mId), active(isActive) {}

    int getBookId() const { return bookId; }
    int getMemberId() const { return memberId; }
    bool isActive() const { return active; }
    void setActive(bool value) { active = value; }
};

class Library {
private:
    vector<Book> books;
    vector<Member> members;
    vector<Loan> loans;
    vector<unique_ptr<User>> users; // Composition/ownership of user objects.

    Book* findBook(int bookId) {
        for (auto& book : books)
            if (book.getId() == bookId) return &book;
        return nullptr;
    }

    Member* findMember(int memberId) {
        for (auto& member : members)
            if (member.getId() == memberId) return &member;
        return nullptr;
    }

public:
    Library() {
        // A librarian object is stored through a base-class pointer to demonstrate polymorphism.
        users.push_back(make_unique<Librarian>(1, "Main Librarian"));
        loadBooks();
        loadMembers();
        loadLoans();
        syncAvailability();
    }

    void addBook() {
        int id;
        string title, author;
        cout << "Enter book ID: ";
        cin >> id;
        if (findBook(id)) throw runtime_error("A book with this ID already exists.");
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter title: ";
        getline(cin, title);
        cout << "Enter author: ";
        getline(cin, author);
        if (title.empty() || author.empty()) throw runtime_error("Title and author cannot be empty.");
        books.emplace_back(id, title, author);
        saveBooks();
        cout << "Book added successfully.\n";
    }

    void showBooks() const {
        if (books.empty()) {
            cout << "No books are currently saved.\n";
            return;
        }
        for (const auto& book : books) book.display();
    }

    void registerMember() {
        int id;
        string name;
        cout << "Enter member ID: ";
        cin >> id;
        if (findMember(id)) throw runtime_error("A member with this ID already exists.");
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter member name: ";
        getline(cin, name);
        if (name.empty()) throw runtime_error("Member name cannot be empty.");
        members.emplace_back(id, name);
        saveMembers();
        cout << "Member registered successfully.\n";
    }

    void showMembers() const {
        if (members.empty()) {
            cout << "No members are currently registered.\n";
            return;
        }
        for (const auto& member : members) member.display();
    }

    void borrowBook() {
        int bookId, memberId;
        cout << "Enter book ID: ";
        cin >> bookId;
        cout << "Enter member ID: ";
        cin >> memberId;

        Book* book = findBook(bookId);
        Member* member = findMember(memberId);
        if (!book) throw runtime_error("Book ID was not found.");
        if (!member) throw runtime_error("Member ID was not found.");
        if (!book->isAvailable()) throw runtime_error("This book is already borrowed.");

        book->setAvailable(false);
        loans.emplace_back(bookId, memberId, true);
        member->setBorrowedBookId(bookId);
        saveBooks();
        saveMembers();
        saveLoans();
        cout << "Book borrowed successfully.\n";
    }

    void returnBook() {
        int bookId, memberId;
        cout << "Enter book ID: ";
        cin >> bookId;
        cout << "Enter member ID: ";
        cin >> memberId;

        Book* book = findBook(bookId);
        Member* member = findMember(memberId);
        if (!book) throw runtime_error("Book ID was not found.");
        if (!member) throw runtime_error("Member ID was not found.");

        bool found = false;
        for (auto& loan : loans) {
            if (loan.getBookId() == bookId && loan.getMemberId() == memberId && loan.isActive()) {
                loan.setActive(false);
                found = true;
                break;
            }
        }
        if (!found) throw runtime_error("No active loan matches these IDs.");

        book->setAvailable(true);
        member->setBorrowedBookId(-1);
        saveBooks();
        saveMembers();
        saveLoans();
        cout << "Book returned successfully.\n";
    }

    void showUsers() const {
        cout << "\nUsers and roles:\n";
        for (const auto& user : users) user->display();
        for (const auto& member : members) member.display();
    }

    void showActiveLoans() const {
        bool any = false;
        for (const auto& loan : loans) {
            if (loan.isActive()) {
                cout << "Book ID: " << loan.getBookId()
                     << " | Member ID: " << loan.getMemberId() << '\n';
                any = true;
            }
        }
        if (!any) cout << "There are no active loans.\n";
    }

    void saveBooks() const {
        ofstream file("books.txt");
        if (!file) throw runtime_error("Could not write books.txt");
        for (const auto& b : books)
            file << b.getId() << '|' << b.getTitle() << '|'
                 << b.getAuthor() << '|' << b.isAvailable() << '\n';
    }

    void saveMembers() const {
        ofstream file("members.txt");
        if (!file) throw runtime_error("Could not write members.txt");
        for (const auto& m : members)
            file << m.getId() << '|' << m.getName() << '|'
                 << m.getBorrowedBookId() << '\n';
    }

    void saveLoans() const {
        ofstream file("loans.txt");
        if (!file) throw runtime_error("Could not write loans.txt");
        for (const auto& l : loans)
            file << l.getBookId() << '|' << l.getMemberId() << '|'
                 << l.isActive() << '\n';
    }

    void loadBooks() {
        ifstream file("books.txt");
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string idText, title, author, availableText;
            if (getline(ss, idText, '|') && getline(ss, title, '|') &&
                getline(ss, author, '|') && getline(ss, availableText)) {
                try {
                    books.emplace_back(stoi(idText), title, author, availableText == "1");
                } catch (...) { /* Ignore malformed data lines. */ }
            }
        }
    }

    void loadMembers() {
        ifstream file("members.txt");
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string idText, name, borrowedText;
            if (getline(ss, idText, '|') && getline(ss, name, '|') &&
                getline(ss, borrowedText)) {
                try {
                    members.emplace_back(stoi(idText), name, stoi(borrowedText));
                } catch (...) { /* Ignore malformed data lines. */ }
            }
        }
    }

    void loadLoans() {
        ifstream file("loans.txt");
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string bookText, memberText, activeText;
            if (getline(ss, bookText, '|') && getline(ss, memberText, '|') &&
                getline(ss, activeText)) {
                try {
                    loans.emplace_back(stoi(bookText), stoi(memberText), activeText == "1");
                } catch (...) { /* Ignore malformed data lines. */ }
            }
        }
    }

    void syncAvailability() {
        for (auto& book : books) book.setAvailable(true);
        for (const auto& loan : loans)
            if (loan.isActive()) {
                Book* book = findBook(loan.getBookId());
                if (book) book->setAvailable(false);
            }
    }
};

int main() {
    Library library;
    int choice;

    do {
        cout << "\n===== LIBRARY MANAGEMENT SYSTEM =====\n"
             << "1. Add book\n"
             << "2. View books\n"
             << "3. Register member\n"
             << "4. View members\n"
             << "5. Borrow book\n"
             << "6. Return book\n"
             << "7. View active loans\n"
             << "8. View users and roles\n"
             << "0. Exit\n"
             << "Choose an option: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a valid number.\n";
            continue;
        }

        try {
            switch (choice) {
                case 1: library.addBook(); break;
                case 2: library.showBooks(); break;
                case 3: library.registerMember(); break;
                case 4: library.showMembers(); break;
                case 5: library.borrowBook(); break;
                case 6: library.returnBook(); break;
                case 7: library.showActiveLoans(); break;
                case 8: library.showUsers(); break;
                case 0: cout << "Goodbye. Records are saved in text files.\n"; break;
                default: cout << "Invalid option. Try again.\n";
            }
        } catch (const exception& e) {
            cout << "Error: " << e.what() << '\n';
        }
    } while (choice != 0);

    return 0;
}
