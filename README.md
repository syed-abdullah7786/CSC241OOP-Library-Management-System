# Library Management System — CSC241 OOP Semester Project

**Course:** CSC241 Object Oriented Programming, Fall 2026  
**Institution:** COMSATS University Islamabad, Department of Electrical Engineering  
**Section:** BEE-3A  
**Instructor:** Dr. Riaz Hussain  
**Team:** Add 2–4 team members and registration numbers before submission.

## Overview
A C++17 console application to add and list books, register members, borrow and return books, and view active loans. Records are saved in `books.txt`, `members.txt`, and `loans.txt`.

## OOP concepts
- **Abstraction:** `User` is an abstract base class with a pure virtual `getRole()`.
- **Inheritance:** `Member` and `Librarian` inherit from `User`.
- **Polymorphism:** `getRole()` and `display()` are virtual; users are handled through base-class pointers.
- **Encapsulation:** class data is private/protected and accessed through methods.
- **Composition/ownership:** `Library` owns collections of books, members, loans, and user objects.
- **Constructors/destructors:** constructors initialize objects; `User` has a virtual destructor.
- **File handling:** data is loaded from and saved to text files.
- **Exception handling:** invalid operations and file-write errors are reported using exceptions.

## Build and run
```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o library
./library
```
On Windows, run `library.exe` after compiling. Keep the text data files in the program's working directory.

## Files
- `main.cpp` — complete source code
- `README.md` — project instructions
- `UML-Class-Diagram.puml` — editable PlantUML class diagram
- `Implementation-Plan.md` — milestones and testing plan

## Publishing
Create a GitHub repository under your account, upload these files, and add the repository URL to the final report. An external repository has not been published automatically by this package.
