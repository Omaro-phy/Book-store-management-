#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "book/book.h"
#include "customer/customer.h"
#include "sales/sales.h"

#define CUSTOMERS_DATA_FILE "data/customers.csv"
#define MAX_BOOKS 100
#define MAX_SALES 100

Book books[MAX_BOOKS];
int numBooks = 0;
Customer customers[MAX_CUSTOMERS];
int numCustomers = 0;
Sale sales[MAX_SALES];
int numSales = 0;
pthread_mutex_t dataMutex = PTHREAD_MUTEX_INITIALIZER;

// Book Management Menu Function
void bookManagementMenu(Book *books, int *numBooks, pthread_mutex_t *dataMutex) {
    int choice, ISBN;
    char title[MAX_TITLE_LENGTH];

    do {
        printf("\nBook Management\n");
        printf("1. Add Book\n");
        printf("2. Edit Book\n");
        printf("3. Delete Book\n");
        printf("4. Search Book by ISBN\n");
        printf("5. Search Book by Title\n");
        printf("6. Display All Books\n");
        printf("0. Back\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            fprintf(stderr, "Error: Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        switch (choice) {
            case 1: // Add Book
                pthread_mutex_lock(dataMutex);
                addBook(books, numBooks);
                pthread_mutex_unlock(dataMutex);
                break;
            case 2: // Edit Book
                pthread_mutex_lock(dataMutex);
                printf("Enter ISBN of book to edit: ");
                if (scanf("%d", &ISBN) != 1) {
                    fprintf(stderr, "Error: Invalid ISBN input.\n");
                    while (getchar() != '\n');
                } else {
                    editBook(books, *numBooks);
                }
                pthread_mutex_unlock(dataMutex);
                break;
            case 3: // Delete Book
                pthread_mutex_lock(dataMutex);
                printf("Enter ISBN of book to delete: ");
                if (scanf("%d", &ISBN) != 1) {
                    fprintf(stderr, "Error: Invalid ISBN input.\n");
                    while (getchar() != '\n');
                } else {
                    deleteBook(books, numBooks);
                }
                pthread_mutex_unlock(dataMutex);
                break;
            case 4: // Search Book by ISBN
                pthread_mutex_lock(dataMutex);
                printf("Enter ISBN to search: ");
                if (scanf("%d", &ISBN) != 1) {
                    fprintf(stderr, "Error: Invalid ISBN input.\n");
                    while (getchar() != '\n');
                } else {
                    Book *foundBook = searchBookByISBN(books, *numBooks, ISBN);
                    if (foundBook) {
                        displayBook(foundBook);
                    } else {
                        printf("Book not found.\n");
                    }
                }
                pthread_mutex_unlock(dataMutex);
                break;
            case 5: // Search Book by Title
                pthread_mutex_lock(dataMutex);
                printf("Enter title to search: ");
                scanf("%s", title); // Assuming title doesn't have spaces
                Book *foundBooks = searchBookByTitle(books, *numBooks, title);
                if (foundBooks) {
                    for (int i = 0; foundBooks[i].ISBN != 0; i++) {
                        displayBook(&foundBooks[i]);
                    }
                    free(foundBooks); // Free the memory allocated in searchBookByTitle
                } else {
                    printf("Book not found.\n");
                }
                pthread_mutex_unlock(dataMutex);
                break;
            case 6: // Display All Books
                pthread_mutex_lock(dataMutex);
                displayAllBooks(books, *numBooks);
                pthread_mutex_unlock(dataMutex);
                break;
            case 0: // Back
                printf("Back to main menu.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
}


// Customer Management Menu Function (Similar to Book Management)
void customerManagementMenu(Customer *customers, int *numCustomers, pthread_mutex_t *dataMutex) {
    int choice, customerID;
    char name[MAX_NAME_LENGTH]; // Use the global MAX_NAME_LENGTH from customer.h

    do {
            printf("\nCustomer Management\n");
            printf("1. Add Customer\n");
            printf("2. Edit Customer\n");
            printf("3. Delete Customer\n");
            printf("4. Search Customer by ID\n");
            printf("5. Search Customers by Name\n");
            printf("6. Display All Customers\n");
            printf("0. Back\n");
            printf("Enter your choice: ");

            if (scanf("%d", &choice) != 1) {
                fprintf(stderr, "Error: Invalid input. Please enter a number.\n");
                while (getchar() != '\n'); // Clear input buffer
                continue;
            }

            switch (choice) {
                case 1: // Add Customer
                    pthread_mutex_lock(dataMutex);
                    addCustomer(customers, numCustomers);
                    pthread_mutex_unlock(dataMutex);
                    break;

                case 2: // Edit Customer
                    pthread_mutex_lock(dataMutex);
                    printf("Enter customer ID to edit: ");
                    if (scanf("%d", &customerID) != 1) {
                        fprintf(stderr, "Error: Invalid customer ID input.\n");
                        while (getchar() != '\n');
                    } else {
                        editCustomer(customers, *numCustomers, customerID);
                    }
                    pthread_mutex_unlock(dataMutex);
                    break;

                case 3: // Delete Customer
                    pthread_mutex_lock(dataMutex);
                    printf("Enter customer ID to delete: ");
                    if (scanf("%d", &customerID) != 1) {
                        fprintf(stderr, "Error: Invalid customer ID input.\n");
                        while (getchar() != '\n');
                    } else {
                        deleteCustomer(customers, numCustomers, customerID);
                    }
                    pthread_mutex_unlock(dataMutex);
                    break;

                case 4: // Search Customer by ID
                    pthread_mutex_lock(dataMutex);
                    printf("Enter customer ID to search: ");
                    if (scanf("%d", &customerID) != 1) {
                        fprintf(stderr, "Error: Invalid customer ID input.\n");
                        while (getchar() != '\n');
                    } else {
                        Customer *foundCustomer = searchCustomerByID(customers, *numCustomers, customerID);
                        if (foundCustomer) {
                            displayCustomer(foundCustomer);
                        } else {
                            printf("Customer not found.\n");
                        }
                    }
                    pthread_mutex_unlock(dataMutex);
                    break;

                case 5: // Search Customers by Name
                    pthread_mutex_lock(dataMutex);
                    printf("Enter name to search: ");
                    scanf("%s", name);
                    searchCustomerByName(customers, *numCustomers, name); // Assuming this function handles multiple results
                    pthread_mutex_unlock(dataMutex);
                    break;

                case 6: // Display All Customers
                    pthread_mutex_lock(dataMutex);
                    displayAllCustomers(customers, *numCustomers);
                    pthread_mutex_unlock(dataMutex);
                    break;

                case 0: // Back
                    printf("Back to main menu.\n");
                    break;

                default:
                    printf("Invalid choice. Please try again.\n");
            }
        } while (choice != 0);
    }


// Enhanced Sales Report Function
void displaySalesReport(const Sale *sales, int numSales) {
    if (numSales == 0) {
        printf("No sales records found.\n");
        return;
    }

    printf("\nSales Report:\n");

    float totalRevenue = 0.0;
    for (int i = 0; i < numSales; i++) {
        displaySale(&sales[i]);
        totalRevenue += sales[i].totalPrice;
    }

    // Additional Analysis
    printf("\nSales by Book:\n");
    for (int i = 0; i < numBooks; i++) {
        int bookSalesCount = 0;
        float bookRevenue = 0.0;

        // Iterate through sales to find sales for this book
        for (int j = 0; j < numSales; j++) {
            if (sales[j].ISBN == books[i].ISBN) {
                bookSalesCount += sales[j].quantity;
                bookRevenue += sales[j].totalPrice;
            }
        }

        // Display if there were sales for this book
        if (bookSalesCount > 0) {
            printf("ISBN: %d, Title: %s, Copies Sold: %d, Revenue: %.2f\n",
                   books[i].ISBN, books[i].title, bookSalesCount, bookRevenue);
        }
    }
    printf("\nSales by Customer:\n");

    for (int i = 0; i < numCustomers; i++) {
        int customerSalesCount = 0;
        float customerRevenue = 0.0;

        // Iterate through sales to find sales for this customer
        for (int j = 0; j < numSales; j++) {
            if (sales[j].customerID == customers[i].customerID) {
                customerSalesCount++;
                customerRevenue += sales[j].totalPrice;
            }
        }

        // Display if there were sales for this customer
        if (customerSalesCount > 0) {
            printf("Customer ID: %d, Name: %s, Number of Purchases: %d, Total Spent: %.2f\n",
                   customers[i].customerID, customers[i].name, customerSalesCount, customerRevenue);
        }
    }
}



int main() {
    // Load initial data from files with thread safety
    pthread_mutex_lock(&dataMutex);
    loadBooks(books, &numBooks);
    loadCustomers(customers, &numCustomers);
    loadSales(sales, &numSales);
    pthread_mutex_unlock(&dataMutex);

    int choice;
    do {
        // Main Menu (Clear & Informative)
        printf("\n\n----- Bookstore Management System -----\n");
        printf("1. Manage Books (Add, Edit, Delete, Search, List)\n");
        printf("2. Manage Customers (Add, Edit, Delete, Search, List)\n");
        printf("3. Process Sale\n");
        printf("4. View Sales Report\n"); // Enhanced report option
        printf("0. Exit\n");
        printf("---------------------------------------\n");
        printf("Your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while(getchar() != '\n'); // Clear input buffer
            continue;
        }

        switch (choice) {
            case 1:
                bookManagementMenu(books, &numBooks, &dataMutex); // Modularize menu
                break;
            case 2:
                customerManagementMenu(customers, &numCustomers, &dataMutex);
                break;
            case 3:
                pthread_mutex_lock(&dataMutex); // Lock before sale
                processSale(books, numBooks, customers, numCustomers, sales, &numSales); // Corrected line
                pthread_mutex_unlock(&dataMutex); // Unlock after sale
                break;
            case 4:
                pthread_mutex_lock(&dataMutex); // Lock before display
                displaySalesReport(sales, numSales); // Implement this function
                pthread_mutex_unlock(&dataMutex);
                break;
            case 0:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);

    pthread_mutex_destroy(&dataMutex);
    return 0;
}
// ...

