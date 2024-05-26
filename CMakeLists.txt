cmake_minimum_required(VERSION 3.1)

project(OS2Project C)  # project's name


# source files
add_executable(OS2Project src/main.c src/book/book.c src/customer/customer.c
        src/book/book.c
        src/customer/customer.c
        src/book/book.h
        src/customer/customer.h
        src/sales/sales.h
        src/sales/sales.c)

# Link libraries (if needed)
target_link_libraries(OS2Project pthread) # For multi-threading
