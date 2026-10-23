# Implementation Plan and Test Checklist

## Milestones
1. **Design:** confirm requirements, identify classes, relationships, menu actions, and text-file formats.
2. **Core classes:** implement `User`, `Member`, `Librarian`, `Book`, and `Loan`.
3. **Library operations:** implement add/list/register/borrow/return and active-loan display.
4. **Persistence and errors:** implement load/save functions and exception handling.
5. **Testing:** run the scenarios below; fix defects and review class responsibilities.
6. **Release:** add UML and README, commit each team member's own work to GitHub, prepare a demonstration.

## Test scenarios
1. Add a new book and view the book list.
2. Add a member and view the member list.
3. Borrow an available book using valid IDs.
4. Attempt to borrow the same book again; expect a clear error.
5. Return a book with matching active loan IDs.
6. Attempt to return a book without an active loan; expect a clear error.
7. Exit and restart; verify records remain available from text files.
8. Enter a non-number at the menu; verify the program recovers and asks again.
9. Enter duplicate book/member IDs; expect a clear error.
10. View active loans and verify returned loans are not shown.

## Known scope/limitations
- Console interface; SFML is not used because the selected Library Management System is an alternative project option.
- Records use simple pipe-delimited text files; avoid entering `|` in titles or names.
- This is an educational prototype, not a production library database.
- Team must run tests on the final version and record actual results before submission.
