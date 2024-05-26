#ifndef SALES_H
#define SALES_H

#include "../book/book.h"
#include "../customer/customer.h"

// Structure to represent a sale
typedef struct {
    int saleID;
    int customerID;
    int ISBN;
    int quantity;
    float totalPrice;
    // You can add a timestamp (date and time) here if needed
} Sale;

// Function prototypes (declarations)
void processSale(const Book *books, int numBooks, const Customer *customers, int numCustomers, Sale *sales, int *numSales);
void displaySale(const Sale *sale);
void displayAllSales(const Sale *sales, int numSales);
void loadSales(Sale *sales, int *numSales);
void saveSales(const Sale *sales, int numSales);

// Add more function prototypes for sales reports, calculations, etc. as needed

#endif
