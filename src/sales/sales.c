#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "sales.h"
#include "../book/book.h"
#include "../customer/customer.h"

#define BOOKS_DATA_FILE "data/books.csv"
#define CUSTOMERS_DATA_FILE "data/customers.csv"
#define SALES_DATA_FILE "data/sales.csv"


// Function to load sales data from file
void loadSales(Sale *sales, int *numSales) {
    FILE *file = fopen(SALES_DATA_FILE, "r");
    if (!file) {
        *numSales = 0;
        return;
    }

    *numSales = 0;
    while (fscanf(file, "%d %d %d %d %f", &sales[*numSales].saleID, &sales[*numSales].customerID,
                  &sales[*numSales].ISBN, &sales[*numSales].quantity, &sales[*numSales].totalPrice) == 5) {
        (*numSales)++;
    }
    fclose(file);
}

// Function to save sales data to file
void saveSales(const Sale *sales, int numSales) {
    FILE *file = fopen(SALES_DATA_FILE, "w");
    if (!file) {
        perror("Error opening file for writing");
        return;
    }

    for (int i = 0; i < numSales; i++) {
        fprintf(file, "%d %d %d %d %.2f\n", sales[i].saleID, sales[i].customerID,
                sales[i].ISBN, sales[i].quantity, sales[i].totalPrice);
    }
    fclose(file);
}

// Function to process a sale
void processSale(const Book *books, int numBooks, const Customer *customers, int numCustomers, Sale *sales, int *numSales) {
    Sale newSale;

    // Input and Validation (customer ID, ISBN, quantity, etc.)
    // (Same as provided in previous responses)
    // ...

    // Generate Sale ID (assuming IDs are simply sequential)
    time_t t;
    srand((unsigned) time(&t)); // Seed random number generator
    newSale.saleID = rand(); // Generate a random sale ID
    newSale.saleID = (*numSales > 0) ? sales[*numSales - 1].saleID + 1 : 1;

    // Store the sale:
    sales[*numSales] = newSale;
    (*numSales)++;
    saveSales(sales, *numSales);

    printf("Sale processed successfully! Total: %.2f\n", newSale.totalPrice);
}

// Function to display a single sale
void displaySale(const Sale *sale) {
    printf("Sale ID: %d\n", sale->saleID);
    printf("Customer ID: %d\n", sale->customerID);
    printf("ISBN: %d\n", sale->ISBN);
    printf("Quantity: %d\n", sale->quantity);
    printf("Total Price: %.2f\n", sale->totalPrice);
    printf("--------------------\n");
}

// Function to display all sales
void displayAllSales(const Sale *sales, int numSales) {
    if (numSales == 0) {
        printf("No sales records found.\n");
        return;
    }

    printf("All Sales:\n");
    for (int i = 0; i < numSales; i++) {
        displaySale(&sales[i]);
    }
}
