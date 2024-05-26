#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "../book/book.h"
#include "../sales/sales.h" // Include sales.h to ensure Customer type is recognized

#define MAX_CUSTOMERS 100
#define MAX_NAME_LENGTH 50
#define MAX_PHONE_LENGTH 20

// Define the Customer structure
typedef struct {
    int customerID;
    char name[MAX_NAME_LENGTH];
    char phone[MAX_PHONE_LENGTH];
} Customer;

// Function prototypes
void addCustomer(Customer *customers, int *numCustomers);
void editCustomer(Customer *customers, int numCustomers, int customerID);
void deleteCustomer(Customer *customers, int *numCustomers, int customerID);
Customer* searchCustomerByID(const Customer *customers, int numCustomers, int customerID);
void searchCustomerByName(const Customer *customers, int numCustomers, const char *name);
void displayCustomer(const Customer *customer);
void saveCustomers(const Customer *customers, int numCustomers);
void loadCustomers(Customer *customers, int *numCustomers);
void displayAllCustomers(const Customer *customers, int numCustomers);

#endif // CUSTOMER_H
