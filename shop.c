#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> 


// A struct is used to create a structure and group data together. It is a simple way in C of combining together pieces of 
// data into one data type to group them together. You can also contain a struct within a struct.
// It can be accessed via a single pointer or by the struct declared name which returns the same address.


// struct for product information
struct Product {
    char* name;     // Pointer * allows it to expand in memory 
    double price;   // Double allows for a decimal number
};
// struct for stock information
struct ProductStock {
    struct Product product;
    int quantity;   // Quantity as an integer
};
// struct for shop information
struct Shop {
    double cash;   // Shop Balance 
    struct ProductStock stock[30];  // Allows for 30 items to be in stock
    int index;   // Keeps track of how many items have been added and read in from file    
};
// struct for customer information
struct Customer {
    char* name; 
    double budget;  
    struct ProductStock shoppingList[20];   // Maximum number of items on shoppinglist is 20  
    int index;  // keeps track of how many elements have been stored in customers shoppingList
};

// Void functions are created and used just like value-returning functions except they do not return a value after the function executes.
// A void function performs a task and then control returns back to the caller but it does not return a value.
// A good utilization of a void function would be to print to a screen or file.

  
// Print out product information
void printProduct(struct Product p)
{
    printf("PRODUCT: %s \nPRICE: %.2f\n", p.name, p.price);   
}

// Print customer information
void printCustomer(struct Customer c)
{   // Print customer name and budget
    printf("\nCUSTOMER: %s \nBUDGET: %.2f\n", c.name, c.budget); 
    printf("\n-----------------------\n");

    double cost = 0.0;
    // loop through items in customer shoppinglist 
    for(int i = 0; i < c.index; i++)
    {   
        // Print name, cost and quantity
        printProduct(c.shoppingList[i].product);
        printf("QUANTITY ORDERED: %d\n", c.shoppingList[i].quantity);
        
        // Find the total cost
        double itemcost = c.shoppingList[i].quantity * c.shoppingList[i].product.price;
        
        // Print the total cost
        printf("COST: €%.2f\n", itemcost);
        printf("-----------------------\n");

        // adds the itemcost to the cost
        cost += itemcost;
    }

    printf("\n%s total cost: €%.2f\n", c.name, cost);
    printf("-----------------------\n");
}

// Create shop from CSV file
struct Shop createandStockShop()
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("./stock.csv", "r");
    // If file does not exist exit the program
    if (fp == NULL)
        exit(EXIT_FAILURE);
        
    // Read first line (which is the shop cash)
    read = getline(&line, &len, fp);
	double cash = atof(line);
    
    struct Shop shop = { cash };

    // Loop through the rest of the stock file line by line
    while ((read = getline(&line, &len, fp)) != -1) {

        // Strtok used to split string according to delimiters 
        // Get name, product and quantity. Seperated by ",".
        char *n = strtok(line, ",");
        char *p = strtok(NULL, ",");
        char *q = strtok(NULL, ",");
        
        int quantity = atoi(q); // atoi converts string to integer
        double price = atof(p); // atof converts string to floating point number
        char *name = malloc(sizeof(char) * 50); // manually allocate memory
		strcpy(name, n); // takes name from n and stores in allocated memory

        struct Product product = {name, price}; // Write in product
        struct ProductStock stockitem = {product, quantity}; // Store the quantity
        
        shop.stock[shop.index++] = stockitem; // shop stock is updated with stock item      
    }

    return shop;   
};
// Find a product in the shop and return it
struct Product findProduct(struct Shop s, char* pname)
{
    struct Product p;
    for (int i = 0; i < s.index; i++){
        if(strcmp(s.stock[i].product.name,pname)==0){
            p = s.stock[i].product;
        }
    }
    return p;

};

// Create customer and order from CSV file
struct Customer customerOrder(struct Shop s, char* csvfile)
{
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    // read csv file
    fp = fopen(csvfile, "r");
    // If file does not exist exit the program
    if (fp == NULL)
        exit(EXIT_FAILURE);

    // Get name and budget from first line
    getline(&line, &len, fp);
    char *cn = strtok(line, ","); // customer name
    char *cb = strtok(NULL, ","); // customer budget

    char *cname = malloc(sizeof(char) * 50); // manually allocate memory
    strcpy(cname, cn);

    double cbudget = atof(cb); // converts the string into a float

    struct Customer customer = {cname, cbudget}; // struct used for customer and budget


    // while loop to read file and create shopping list 
    while ((read = getline(&line, &len, fp)) != -1) {
        
        char *pn = strtok(line, ","); 
        char *qt = strtok(NULL, ",");

        int orderQuantity = atoi(qt);
        char *name = malloc(sizeof(char) * 50); 
        strcpy(name, pn);

        struct Product product = {name, findProduct(s, name).price}; 
        struct ProductStock listItem = {product, orderQuantity};

        // Create shoppinglist from list items
        customer.shoppingList[customer.index++] = listItem;

    }
    return customer;

};
// Check for product in the shop stock
char* getProduct(struct Shop* s, char *n){
    // Loop through the shop stock
    for(int j = 0; j < s->index; j++){
        // Check if the item is in stock
        if(strcmp(s->stock[j].product.name,n)==0){
            // Return product
            return s->stock[j].product.name;
        }
    }
    return "NULL";
};

// Print shop information
void printShop(struct Shop s)
{   // Print shop balance
    printf("-----------------------\n");
    printf("SHOP BALANCE: %.2f\n", s.cash);
    printf("-----------------------\n");
    // Loop through each product in shop stock
    for (int i = 0; i < s.index; i++)
    {   
        // Print product name, price and quantity
        printProduct(s.stock[i].product);
        printf("QUANTITY: %d\n", s.stock[i].quantity);
        printf("-----------------------\n");
    }
}

// Process and carry out orders
void processOrder(struct Shop* s, struct Customer* c) {

    double orderCost = 0; // Variable to calculate the cost of order
	double shortAmount = 0; // Variable to calculate the amount a customer is short
	double shortStock = 0; // Variable to calculate the stock

	
	printf("\n---------------------------------------------------\n");
	printf("\n-------------------PROCESS ORDER-------------------\n");
	printf("\n---------------------------------------------------\n");
    
    // Loop through customer shopping list and calculate the total cost of the order 
    for(int i = 0; i < c->index; i++){ 
        orderCost += (c->shoppingList[i].quantity * c->shoppingList[i].product.price); 
    }

    // If the customer budget is less than total cost of the products the transaction will be cancelled
    // Find the amount the customer is short by and return appropriate error	
	if(orderCost > c->budget){
		shortAmount = (orderCost - c->budget);  
		 
		printf("\nUNABLE TO PROCESS ORDER\n");
        printf("\nYour budget is too low");
		printf("\nYou are €%.2f short \n", shortAmount);
        printf("\nPlease make another order\n");
        printf("\n---------------------------------------------------\n");
      	return;
    }
    // If the product is not stocked in the shop return error
	for(int i = 0; i < c->index; i++){	
		if (strcmp(getProduct(s, c->shoppingList[i].product.name),"NULL")==0){
            printf("\nUNABLE TO PROCESS ORDER\n");
			printf("\nSorry %s is currently not in stock\n", c->shoppingList[i].product.name);
			printf("\nPlease make another order\n");
            printf("\n-----------------------------------------------------------\n");
            return;
		}

	}
    // If the shop stock is less than the quantity ordered return appropraite error

    // Loop through customer shopping list 
    for(int i = 0; i < c->index; i++) {
        // Loop through the shop stock
        for(int j = 0; j < s->index; j++){
        
            // Check if the items are the same 
            if(strcmp(c->shoppingList[i].product.name, s->stock[j].product.name)==0) {
                // if the shop stock is less than the order quantity return error
                if(c->shoppingList[i].quantity > s->stock[j].quantity) {
					shortStock = (c->shoppingList[i].quantity -  s->stock[j].quantity);
                    printf("\nUNABLE TO PROCESS ORDER\n");
                    printf("\nSorry %s quantity is currently not in stock\n", c->shoppingList[i].product.name);   
					printf("\nPlease make another order\n");
                    printf("\n-----------------------------------------------------------\n");
					
					return;
                } 
                
                // Update the shop stock after the customer purchase 
                s->stock[j].quantity = s->stock[j].quantity - c->shoppingList[i].quantity;

                // Update the shop balance
                s->cash = s->cash + (c->shoppingList[i].product.price * c->shoppingList[i].quantity);
                
                // Update the customer budget after making a purchase from the shop
                c->budget = c->budget - (c->shoppingList[i].product.price * c->shoppingList[i].quantity); 	

				printf("\nPRODUCT: %s\n", c->shoppingList[i].product.name);
				printf("PRICE: €%.2f\n", (c->shoppingList[i].product.price * c->shoppingList[i].quantity));
				printf("QUANTITY: %.d\n", s->stock[j].quantity);				
				//printf("%s's budget: € %.2f\n", c->name, c->budget);
	            printf("SHOP BALANCE: €%.2f", s->cash);
			    printf("\n-----------------------------------------------------------\n");


        	}
		
        }
	
    }
   
}
// Struct for a live customer
struct liveCustomer 
{
    double budget;
    struct ProductStock* shoppinglist; // pointer used for list as we do not know lenght
    int index; // keeps track of items on shoppinglist
};

// create live customer
struct liveCustomer createLiveCustomer(struct Shop s) 
{
    // struct for live order
    struct liveCustomer order;
    order.shoppinglist = malloc(sizeof(struct ProductStock) * 20);


        printf("\nLIVE MODE\n");

		// User input - allows the user to enter their detais
        
		printf("\nEnter name: ");
  		char *name = malloc(sizeof(char) * 50);
		scanf("%s", name);

		printf("Enter budget: ");
		double budget;
		scanf("%3f", &budget);

    order.index = 0;
    char allowOrder;

    // Create a loop that continues until the customer selects no(n)
    // add text
    while(strcmp(&allowOrder, "n") != 0)
    {
        printf("Enter Product: ");
        char* item = malloc(sizeof(char)*30);
        scanf("\n%[^\n]%*c",item);

        printf("Enter Quantity: ");
        int qty;
        scanf("%d",&qty);


        // create a product and product stock from the user input
		struct Product product = {item, findProduct(s, item).price};
		struct ProductStock listItem = {product, qty};

        // put the stock item in to the stock array
		order.shoppinglist[order.index] = listItem;
		order.index++;
      

		printf("Would you like another item? (y/n)");
		scanf("\n%s", &allowOrder);
    }
    return order;
}

void processLiveOrder(struct Shop* s, struct liveCustomer lc)
{
    // Create the following variables:

    double orderCost = 0;             // Variable to  calculate the amount due for products stocked in shop
    double shortAmount = 0;           // Variable to calculate amount the customer is short when paying for items
    double shortStock = 0;            // Variable to calculate the shortage in stock in the shop vs what the customer ordered

    printf("\n---------------------------------------------------\n");
	printf("\n-------------------PROCESS ORDER-------------------\n");
	printf("\n---------------------------------------------------\n");

	 
	// Loop through customer shopping list
	for(int i = 0; i <= lc.index;i++){
        // Calculate the order cost
        orderCost += (lc.shoppinglist[i].quantity * lc.shoppinglist[i].product.price);
    
        // If customer budget is less than order cost return error and short amount
        //for(int i = 0; i < lc.index;i++){
        if(orderCost < lc.budget){
	        shortAmount = (orderCost - lc.budget);
            printf("\nUNABLE TO PROCESS ORDER\n"); 
	        printf("Your budget is too low");
            printf("\nYou are short by €%.2f\n", shortAmount);
            printf("\nPlease make another order\n");
            printf("\n---------------------------------------------------\n");			
            return;
        }    
        //}
    }
    
	// If product entered is not in stock return error
	for(int i = 0; i < lc.index;i++){
		if (strcmp(getProduct(s, lc.shoppinglist[i].product.name),"NULL")==0){
			printf("\nUNABLE TO PROCESS ORDER\n");
			printf("Sorry %s is currently not in stock\n", lc.shoppinglist[i].product.name);
			printf("\nPlease make another order\n");
            printf("\n---------------------------------------------------\n");
            return;
		}
	}


// If the shop stock is less than the quantity the customer orders return error

    // Loops through customer shopping list 
    for(int i = 0; i < lc.index; i++) {

        // Loop through the shop stock
        for(int j = 0; j < s->index; j++){

            // Find if the items and check if the strings are the same 
            if(strcmp(lc.shoppinglist[i].product.name, s->stock[j].product.name)==0) {
            
                // if the shop stock quantity is less than the customer order quantity return error
                if(lc.shoppinglist[i].quantity > s->stock[j].quantity) {
                    shortStock = (lc.shoppinglist[i].quantity -  s->stock[j].quantity);
                    printf("\nUNABLE TO PROCESS ORDER\n");
                    printf("\nSorry %s quantity is currently not in stock\n", lc.shoppinglist[i].product.name);
				    printf("\nPlease make another order\n");
                    printf("\n---------------------------------------------------\n");

					return;
                    
            }   

             // Update the shop stock quantity to reflect the customer purchasing products
            s->stock[j].quantity = s->stock[j].quantity - lc.shoppinglist[i].quantity;
            
            // Update the shop balance to reflect the customer purchasing products
            s->cash = s->cash + (lc.shoppinglist[i].product.price * lc.shoppinglist[i].quantity);

            // Update the customer budget to reflect the purchase of products from the shop
            lc.budget = lc.budget - (lc.shoppinglist[i].product.price * lc.shoppinglist[i].quantity); 
	
		printf("\nPRODUCT: %s\n", lc.shoppinglist[i].product.name);
		printf("PRICE: €%.2f\n", lc.shoppinglist[i].product.price);
		printf("QUANTITY: %.d\n", s->stock[j].quantity);				
        printf("SHOP BALANCE: € %.2f\n", s->cash);
		printf("\n-----------------------------------------------------------\n");
        

        	}
        }
    }

}

// Create options menu for customer 
void optionsMenu(struct Shop s) 
{
    int choice = 0;
    char file;

    do {
        printf("\n-------------------------------------------------------- \n");
        printf("----------------WELCOME TO THE SHOP---------------------");
        printf("\n-------------------------------------------------------- \n");
        printf("\n");
        printf("Please select an option");
        printf("\n---------------------------------------------------------\n");
        printf("\n");
        printf("\n1 - View Shop Items\n");
        printf("\n2 - Order From CSV File\n");
        printf("\n3 - Order Live\n");
        printf("\n4 - Exit\n");
        printf("\n");
        printf("\nEnter Number: ");
        scanf("%d",&choice);
        switch (choice) 
        {
            case 1:{
					printShop(s);
					optionsMenu(s);
			}

			case 2:{
					printf("CSV FILE NAME: \n");
      				scanf("%s", &file);
					struct Customer customer = customerOrder(s, &file); 
					printCustomer(customer);
                    processOrder(&s, &customer); 
					optionsMenu(s);

			}

			case 3:{
					struct liveCustomer live = createLiveCustomer(s);
                    processLiveOrder(&s, live);
					//printShop(s);
					optionsMenu(s);
			}

			case 4:{
					
                    printf("----------------GOODBYE----------------------");		
	    		    exit(0);
			}

			default:	
					
                    printf("ERROR");                    		
					optionsMenu(s);

        }
    
    }
    while (choice != 4);
    return;

}

int main(void)
{
    struct Shop shop = createandStockShop();
    optionsMenu(shop);
    return 0;
}

/* 
References:

https://www.programiz.com/c-programming/c-structures
https://www.cs.fsu.edu/~cop3014p/lectures/ch7/index.html
https://syntaxdb.com/ref/c/while
https://www.studytonight.com/c/programs/misc/menu-driven-program
https://www.w3resource.com/c-programming-exercises/conditional-statement/c-conditional-statement-exercises-25.php
https://www.geeksforgeeks.org/menu-driven-program-using-switch-case-c/
https://www.linkedin.com/learning/learning-c-5/welcome?u=94919786
https://stackoverflow.com/questions/52141857/how-to-make-return-char-function-in-c-programming/52141903
https://csharp.net-tutorials.com/data-types/the-char-type/
https://www.geeksforgeeks.org/strtok-strtok_r-functions-c-examples/
https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
https://www.thoughtco.com/definition-of-void-958182
https://www.programiz.com/c-programming/c-pointers
https://www.programiz.com/c-programming/c-structures
https://syntaxdb.com/ref/c/do-while-loop
https://www.studytonight.com/c/programs/misc/menu-driven-program#
https://www.geeksforgeeks.org/menu-driven-program-using-switch-case-c/

*/






