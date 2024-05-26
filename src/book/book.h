#ifndef BOOK_H
#define BOOK_H

#include "../book/book.h"
#include "../customer/customer.h"

#define MAX_TITLE_LENGTH 100
#define MAX_AUTHOR_LENGTH 50

// Structure to represent a book
typedef struct {
    int ISBN;
    char title[MAX_TITLE_LENGTH];
    char author[MAX_AUTHOR_LENGTH];
    float price;
    int quantity;
} Book;

// Function prototypes (declarations)
void addBook(Book *books, int *numBooks);
void editBook(Book *books, int numBooks, int ISBN); // Updated prototype
void deleteBook(Book *books, int *numBooks, int ISBN);
Book* searchBookByISBN(const Book *books, int numBooks, int ISBN);
Book* searchBookByTitle(const Book *books, int numBooks, const char *title);
void displayBook(const Book *book);
void displayAllBooks(const Book *books, int numBooks); // Added numBooks parameter
void loadBooks(Book *books, int *numBooks);

#endif // BOOK_H
