#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h> // for isdigit()
#include "customer.h"
#include "../book/book.h"
#include "../sales/sales.h"

#define CUSTOMERS_DATA_FILE "data/customers.csv"
#define MAX_CUSTOMERS 100
#define MAX_SALES 100
#define MAX_NAME_LENGTH 50
#define MAX_PHONE_LENGTH 20
#define MAX_BOOKS 100 // Define MAX_BOOKS here

Book books[MAX_BOOKS];       // Array to store book data
int numBooks = 0;            // Number of books currently in the system
Customer customers[MAX_CUSTOMERS]; // Array to store customer data
int numCustomers = 0;        // Number of customers currently in the system
Sale sales[MAX_SALES];       // Array to store sales data
int numSales = 0;            // Number of sales made

// Mutex for Thread Synchronization
pthread_mutex_t dataMutex = PTHREAD_MUTEX_INITIALIZER; // Initialize the mutex

// Thread function for adding a customer
void *addCustomerThread(void *arg) {
    pthread_mutex_lock(&dataMutex); // Lock the mutex to ensure exclusive access to shared data.

    Customer *newCustomer = (Customer *)arg;

    // Input validation for ID (ensure it's unique)
    int id;
    do {
        printf("Enter customer ID: ");
        if (scanf("%d", &id) != 1) {
            printf("Invalid input. Please enter a number for ID.\n");
            while(getchar() != '\n'); // Clear input buffer
            continue;
        } else {
            if (searchCustomerByID(customers, numCustomers, id)) {
                printf("Customer ID already exists. Please enter a different ID.\n");
            } else {
                newCustomer->customerID = id;
                break;
            }
        }
    } while (1);

    // Input validation for name (only letters and spaces)
    printf("Enter name: ");
    if (scanf("%49[a-zA-Z ]", newCustomer->name) != 1) {
        printf("Invalid name. Please use only letters and spaces.\n");
        while (getchar() != '\n');
        pthread_mutex_unlock(&dataMutex);
        pthread_exit(NULL);
    }

    // Input validation for phone (only digits)
    printf("Enter phone: ");
    if (scanf("%19s", newCustomer->phone) != 1) {
        printf("Invalid phone number. Please use only digits.\n");
        while (getchar() != '\n');
        pthread_mutex_unlock(&dataMutex);
        pthread_exit(NULL);
    }
    for (int i = 0; newCustomer->phone[i]; i++) {
        if (!isdigit(newCustomer->phone[i])) {
            printf("Invalid phone number. Please use only digits.\n");
            pthread_mutex_unlock(&dataMutex);
            pthread_exit(NULL);
        }
    }

    if (numCustomers < MAX_CUSTOMERS) {
        customers[numCustomers] = *newCustomer;
        numCustomers++;
        saveCustomers(customers, numCustomers);
        printf("Customer added successfully in a separate thread!\n");
    } else {
        printf("Error: Maximum number of customers reached.\n");
    }

    pthread_mutex_unlock(&dataMutex); // Release the mutex after finishing with the shared data.
    pthread_exit(NULL); // Exit the thread.
}

// Function to load customer data from file (with error handling)
void loadCustomers(Customer *customers, int *numCustomers) {
    pthread_mutex_lock(&dataMutex);

    FILE *file = fopen(CUSTOMERS_DATA_FILE, "r");
    if (!file) {
        perror("Error opening customers file for reading");
        *numCustomers = 0;
        pthread_mutex_unlock(&dataMutex);
        return;
    }

    *numCustomers = 0;
    while (fscanf(file, "%d,%49[^,],%19s", &customers[*numCustomers].customerID, customers[*numCustomers].name, customers[*numCustomers].phone) == 3) {
        (*numCustomers)++;
        if (*numCustomers >= MAX_CUSTOMERS) {
            fprintf(stderr, "Error: Maximum number of customers reached.\n");
            break;
        }
    }

    if (ferror(file)) {
        perror("Error reading customers file");
    }

    fclose(file);
    pthread_mutex_unlock(&dataMutex);
}

// Function to save customer data to file (with error handling)
void saveCustomers(const Customer *customers, int numCustomers) {
    pthread_mutex_lock(&dataMutex);

    FILE *file = fopen(CUSTOMERS_DATA_FILE, "w");
    if (!file) {
        perror("Error opening customers file for writing");
        pthread_mutex_unlock(&dataMutex);
        return;
    }

    for (int i = 0; i < numCustomers; i++) {
        fprintf(file, "%d,%s,%s\n", customers[i].customerID, customers[i].name, customers[i].phone);
    }

    if (ferror(file)) {
        perror("Error writing to customers file");
    }

    fclose(file);
    pthread_mutex_unlock(&dataMutex);
}

// Function to add a customer (with input validation)
void addCustomer(Customer *customers, int *numCustomers) {
    if (*numCustomers >= MAX_CUSTOMERS) {
        fprintf(stderr, "Error: Maximum number of customers reached.\n");
        return;
    }

    Customer newCustomer;
    int id, validId = 0;

    // Input validation for ID (ensure it's unique and positive)
    do {
        printf("Enter customer ID (positive integer): ");
        if (scanf("%d", &id) != 1 || id <= 0) {
            printf("Invalid input. Please enter a positive integer for ID.\n");
            while (getchar() != '\n'); // Clear input buffer
        } else {
            validId = 1;
            for (int i = 0; i < *numCustomers; i++) {
                if (customers[i].customerID == id) {
                    printf("Customer ID already exists. Please enter a different ID.\n");
                    validId = 0;
                    break;
                }
            }
        }
    } while (!validId);

    newCustomer.customerID = id;

    // Input validation for name (only letters and spaces)
    do {
        printf("Enter name: ");
        if (scanf("%49[a-zA-Z ]", newCustomer.name) != 1) {
            printf("Invalid name. Please use only letters and spaces.\n");
            while (getchar() != '\n'); // Clear input buffer
        } else {
            break;
        }
    } while (1);

    // Input validation for phone (only digits)
    do {
        printf("Enter phone (10 digits): ");
        if (scanf("%19s", newCustomer.phone) != 1 || strlen(newCustomer.phone) != 10) {
            printf("Invalid phone number. Please enter exactly 10 digits.\n");
            while (getchar() != '\n');
        } else {
            int allDigits = 1;
            for (int i = 0; newCustomer.phone[i]; i++) {
                if (!isdigit(newCustomer.phone[i])) {
                    allDigits = 0;
                    break;
                }
            }
            if (allDigits) break;
            else printf("Invalid phone number. Please use only digits.\n");
        }
    } while (1);

    // Add customer to the array and save to file
    customers[*numCustomers] = newCustomer;
    (*numCustomers)++;
    saveCustomers(customers, *numCustomers);
    printf("Customer added successfully!\n");
}

void editCustomer(Customer *customers, int numCustomers, int customerID) {
    pthread_mutex_lock(&dataMutex);

    int index = -1;
    for (int i = 0; i < numCustomers; i++) {
        if (customers[i].customerID == customerID) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Customer with ID %d not found.\n", customerID);
    } else {
        printf("Enter new name (leave empty to keep current): ");
        char newName[MAX_NAME_LENGTH];
        scanf("%49s", newName);
        if (strlen(newName) > 0) {
            strcpy(customers[index].name, newName);
        }

        printf("Enter new phone (10 digits, leave empty to keep current): ");
        char newPhone[MAX_PHONE_LENGTH];
        scanf("%19s", newPhone);
        if (strlen(newPhone) > 0 && strlen(newPhone) == 10) {
            strcpy(customers[index].phone, newPhone);
        }

        saveCustomers(customers, numCustomers);
        printf("Customer with ID %d edited successfully.\n", customerID);
    }

    pthread_mutex_unlock(&dataMutex);
}

void deleteCustomer(Customer *customers, int *numCustomers, int customerID) {
    pthread_mutex_lock(&dataMutex); // Acquire the lock

    int found = 0;
    for (int i = 0; i < *numCustomers; i++) {
        if (customers[i].customerID == customerID) {
            found = 1;

            // Shift remaining customers to fill the gap
            for (int j = i; j < *numCustomers - 1; j++) {
                customers[j] = customers[j + 1];
            }

            (*numCustomers)--; // Decrement the customer count
            saveCustomers(customers, *numCustomers); // Save the updated customer data
            printf("Customer with ID %d deleted successfully.\n", customerID);
            break; // Exit the loop since the customer is found and deleted
        }
    }

    if (!found) {
        printf("Customer with ID %d not found.\n", customerID);
    }

    pthread_mutex_unlock(&dataMutex); // Release the lock
}

Customer* searchCustomerByID(const Customer *customers, int numCustomers, int customerID) {
    pthread_mutex_lock(&dataMutex);

    for (int i = 0; i < numCustomers; i++) {
        if (customers[i].customerID == customerID) {
            pthread_mutex_unlock(&dataMutex);
            return (Customer*)&customers[i]; // Return a copy of the found customer
        }
    }

    pthread_mutex_unlock(&dataMutex);
    return NULL; // Customer not found
}

void searchCustomerByName(const Customer *customers, int numCustomers, const char *name) {
    pthread_mutex_lock(&dataMutex);

    int found = 0;
    printf("\nCustomers found with name '%s':\n", name);
    for (int i = 0; i < numCustomers; i++) {
        if (strcasecmp(customers[i].name, name) == 0) { // Case-insensitive comparison
            displayCustomer(&customers[i]);
            found = 1;
        }
    }
    if (!found) {
        printf("No customers found with the name '%s'.\n", name);
    }

    pthread_mutex_unlock(&dataMutex);
}

void displayCustomer(const Customer *customer) {
    printf("Customer ID: %d\n", customer->customerID);
    printf("Name: %s\n", customer->name);
    printf("Phone: %s\n", customer->phone);
    printf("--------------------\n");
}

void displayAllCustomers(const Customer *customers, int numCustomers) {
    pthread_mutex_lock(&dataMutex);

    if (numCustomers == 0) {
        printf("No customers found.\n");
    } else {
        printf("\nAll Customers:\n");
        for (int i = 0; i < numCustomers; i++) {
            displayCustomer(&customers[i]);
        }
    }
    pthread_mutex_unlock(&dataMutex);
}
