#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "book.h"

#define BOOKS_DATA_FILE "data/books.csv"
#define MAX_BOOKS 100

// Global data and mutex
Book books[MAX_BOOKS];
int numBooks = 0;
pthread_mutex_t bookMutex = PTHREAD_MUTEX_INITIALIZER;

void loadBooks(Book *books, int *numBooks) {
    pthread_mutex_lock(&bookMutex);

    FILE *file = fopen(BOOKS_DATA_FILE, "r");
    if (!file) {
        perror("Error opening books file for reading");
        *numBooks = 0;
        pthread_mutex_unlock(&bookMutex);
        return;
    }

    // Skip header row if it exists
    char buffer[256];
    fgets(buffer, sizeof(buffer), file);

    *numBooks = 0;
    while (fscanf(file, "%d,%99[^,],%49[^,],%f,%d", &books[*numBooks].ISBN, books[*numBooks].title,
                  books[*numBooks].author, &books[*numBooks].price, &books[*numBooks].quantity) == 5) {
        (*numBooks)++;
        if (*numBooks >= MAX_BOOKS) {
            fprintf(stderr, "Error: Maximum number of books reached.\n");
            break;
        }
    }

    if (ferror(file)) {
        perror("Error reading books file");
    }

    fclose(file);
    pthread_mutex_unlock(&bookMutex);
}



void saveBooks(const Book *books, int numBooks) {
    FILE *file = fopen(BOOKS_DATA_FILE, "w");
    if (!file) {
        perror("Error opening file for writing");
        return;
    }

    for (int i = 0; i < numBooks; i++) {
        fprintf(file, "%d %s %s %.2f %d\n", books[i].ISBN, books[i].title,
                books[i].author, books[i].price, books[i].quantity);
    }
    fclose(file);
}

// Book Operations

void addBook(Book *books, int *numBooks) {
    pthread_mutex_lock(&bookMutex);

    if (*numBooks >= MAX_BOOKS) {
        printf("Error: Maximum number of books reached.\n");
        pthread_mutex_unlock(&bookMutex);
        return;
    }

    Book newBook;

    printf("Enter ISBN: ");
    if (scanf("%d", &newBook.ISBN) != 1) {
        fprintf(stderr, "Error: Invalid ISBN input.\n");
        while (getchar() != '\n'); // Clear input buffer
        pthread_mutex_unlock(&bookMutex);
        return;
    }

    printf("Enter title: ");
    scanf("%99s", newBook.title); // Limit input to prevent buffer overflow

    printf("Enter author: ");
    scanf("%49s", newBook.author);

    printf("Enter price: ");
    if (scanf("%f", &newBook.price) != 1 || newBook.price <= 0) {
        fprintf(stderr, "Error: Invalid price input.\n");
        while (getchar() != '\n');
        pthread_mutex_unlock(&bookMutex);
        return;
    }

    printf("Enter quantity: ");
    if (scanf("%d", &newBook.quantity) != 1 || newBook.quantity <= 0) {
        fprintf(stderr, "Error: Invalid quantity input.\n");
        while (getchar() != '\n');
        pthread_mutex_unlock(&bookMutex);
        return;
    }

    books[*numBooks] = newBook;
    (*numBooks)++;
    saveBooks(books, *numBooks);
    printf("Book added successfully!\n");

    pthread_mutex_unlock(&bookMutex);
}

void editBook(Book *books, int numBooks, int ISBN) {
    pthread_mutex_lock(&bookMutex); // Acquire lock

    int found = 0;
    int index = -1;
    for (int i = 0; i < numBooks; i++) {
        if (books[i].ISBN == ISBN) {
            found = 1;
            index = i;
            break;
        }
    }

    if (!found) {
        printf("Book with ISBN %d not found.\n", ISBN);
        pthread_mutex_unlock(&bookMutex);
        return;
    }

    Book *bookToEdit = &books[index];

    printf("\nCurrent Book Details:\n");
    displayBook(bookToEdit);

    printf("\nEnter new details (leave blank to keep current value):\n");

    char inputBuffer[100]; // Buffer for user input

    printf("Title: ");
    fgets(inputBuffer, sizeof(inputBuffer), stdin);
    inputBuffer[strcspn(inputBuffer, "\n")] = 0; // Remove trailing newline
    if (strlen(inputBuffer) > 0) {
        strncpy(bookToEdit->title, inputBuffer, MAX_TITLE_LENGTH - 1);
        bookToEdit->title[MAX_TITLE_LENGTH - 1] = '\0'; // Ensure null-termination
    }

    printf("Author: ");
    fgets(inputBuffer, sizeof(inputBuffer), stdin);
    inputBuffer[strcspn(inputBuffer, "\n")] = 0;
    if (strlen(inputBuffer) > 0) {
        strncpy(bookToEdit->author, inputBuffer, MAX_AUTHOR_LENGTH - 1);
        bookToEdit->author[MAX_AUTHOR_LENGTH - 1] = '\0';
    }

    printf("Price: ");
    fgets(inputBuffer, sizeof(inputBuffer), stdin);
    inputBuffer[strcspn(inputBuffer, "\n")] = 0;
    if (strlen(inputBuffer) > 0) {
        if (sscanf(inputBuffer, "%f", &bookToEdit->price) != 1 || bookToEdit->price <= 0) {
            fprintf(stderr, "Error: Invalid price input.\n");
        }
    }

    printf("Quantity: ");
    fgets(inputBuffer, sizeof(inputBuffer), stdin);
    inputBuffer[strcspn(inputBuffer, "\n")] = 0;
    if (strlen(inputBuffer) > 0) {
        if (sscanf(inputBuffer, "%d", &bookToEdit->quantity) != 1 || bookToEdit->quantity <= 0) {
            fprintf(stderr, "Error: Invalid quantity input.\n");
        }
    }

    saveBooks(books, numBooks);
    printf("Book edited successfully!\n");

    pthread_mutex_unlock(&bookMutex); // Release lock
}

void deleteBook(Book *books, int *numBooks, int ISBN) {
    pthread_mutex_lock(&bookMutex); // Acquire lock

    int found = 0;
    for (int i = 0; i < *numBooks; i++) {
        if (books[i].ISBN == ISBN) {
            found = 1;
            // Shift remaining books to fill the gap
            for (int j = i; j < *numBooks - 1; j++) {
                books[j] = books[j + 1];
            }
            (*numBooks)--; // Decrement the book count
            saveBooks(books, *numBooks); // Save the updated book data
            printf("Book with ISBN %d deleted successfully.\n", ISBN);
            break; // Exit loop since the book is found and deleted
        }
    }

    if (!found) {
        printf("Book with ISBN %d not found.\n", ISBN);
    }

    pthread_mutex_unlock(&bookMutex); // Release lock
}

Book* searchBookByISBN(const Book *books, int numBooks, int ISBN) {
    // ... (Implementation for searching a book by ISBN) ...
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // for tolower()
#include "book.h"

// ... (other includes and declarations) ...

Book* searchBookByTitle(const Book *books, int numBooks, const char *title) {
    pthread_mutex_lock(&bookMutex);

    Book *foundBooks = NULL;
    int count = 0;

    // Allocate memory for potential matches (worst case: all books match)
    foundBooks = malloc(numBooks * sizeof(Book));
    if (!foundBooks) {
        perror("Memory allocation failed");
        pthread_mutex_unlock(&bookMutex);
        return NULL;
    }

    // Convert search title to lowercase for case-insensitive comparison
    char lowercaseTitle[MAX_TITLE_LENGTH];
    strncpy(lowercaseTitle, title, MAX_TITLE_LENGTH - 1);
    lowercaseTitle[MAX_TITLE_LENGTH - 1] = '\0'; // Ensure null-termination
    for (int i = 0; lowercaseTitle[i]; i++) {
        lowercaseTitle[i] = tolower(lowercaseTitle[i]);
    }

    // Search for books with matching titles
    for (int i = 0; i < numBooks; i++) {
        char lowercaseBookTitle[MAX_TITLE_LENGTH];
        strncpy(lowercaseBookTitle, books[i].title, MAX_TITLE_LENGTH - 1);
        lowercaseBookTitle[MAX_TITLE_LENGTH - 1] = '\0';
        for (int j = 0; lowercaseBookTitle[j]; j++) {
            lowercaseBookTitle[j] = tolower(lowercaseBookTitle[j]);
        }

        if (strstr(lowercaseBookTitle, lowercaseTitle) != NULL) {
            foundBooks[count++] = books[i];
        }
    }

    // If no books found, free the allocated memory and return NULL
    if (count == 0) {
        free(foundBooks);
        foundBooks = NULL;
    } else {
        // Add a sentinel value to mark the end of the foundBooks array
        foundBooks[count].ISBN = 0;
    }

    pthread_mutex_unlock(&bookMutex);
    return foundBooks;
}

void displayBook(const Book *book) {
    // ... (Implementation for displaying a book's details) ...
}

void displayAllBooks(const Book *books, int numBooks) {
    // ... (Implementation for displaying all books) ...
}
