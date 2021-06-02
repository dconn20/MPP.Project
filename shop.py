from dataclasses import dataclass, field
from typing import List
import csv


# A data class is a class whose main purpose is to store data without functionality. 
# This kind of class, also known as a data structure, is very common.

@dataclass
class Product:
    name: str
    price: float = 0

@dataclass 
class ProductStock:
    product: Product
    quantity: int

@dataclass 
class Shop:
    cash: float = 0
    stock: List[ProductStock] = field(default_factory=list)

@dataclass
class Customer:
    name: str
    budget: float = 0
    shopping_list: List[ProductStock] = field(default_factory=list)



# Once the data is stored in the data classes functions can then be used.
# Functions are a group of related statements that performs a specific task.
# Functions help break our programs into smaller and modular chunks. 
# As our program grows larger and larger, functions make it more organized and manageable.

# Create shop and stock from CSV file
def create_and_stock_shop():
    s = Shop()
    with open('./stock.csv') as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        first_row = next(csv_reader)
        s.cash = float(first_row[0])
    
        for row in csv_reader:
            p = Product(row[0], float(row[1]))
            ps = ProductStock(p, int(row[2]))
            s.stock.append(ps)
            #print(ps)
    return s

# Create customer from CSV file   
def read_customer(file_path):
    with open(file_path) as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        first_row = next(csv_reader)
        c = Customer(first_row[0], float(first_row[1]))
        for row in csv_reader:
            name = row[0]
            quantity = int(row[1])
            p = Product(name)
            ps = ProductStock(p, quantity)
            c.shopping_list.append(ps)
        return c 
        
# Function to print product information
def print_product(p):
    print(f'PRODUCT: {p.name} \nPRICE: {p.price}')

# Process customer order and information
def process_customer(c):
    calculate_costs(c, s)
    print(f"\n-------------------------------------------")
    print(f'\nCUSTOMER NAME: {c.name} \nCUSTOMER BUDGET: {c.budget}\n')
    totalcost = 0
    # Print information of items on shopping list
    for item in c.shopping_list:
        print_product(item.product)        
        print(f'Quantity Ordered: {item.quantity}')
        
        # Calculate the cost
        cost = item.quantity * item.product.price
        totalcost += cost
        print(f'Cost: {cost}')
        print(f"-------------------------------------------")
    print(f"\n{c.name}'s totalcost: €{totalcost}")
    print(f"-------------------------------------------")

    # Process the order (Update shop stock and shop balance)
    print(f"\n-------------------------------------------")
    print(f"---------------Process Order---------------")
    print(f"-------------------------------------------")
    check_stock(c,s)
    
    # If the total cost is more than the customer budget return error
    if totalcost > c.budget:
        print(f"\nUNABLE TO PROCESS ORDER\n")
        print(f"Your budget is too low")
        # Calculate short amount
        shortAmount = abs (c.budget - totalcost)
        print(f"You are short by €{shortAmount}\n\nPlease make another order\n")
    
    # Update shop balance with total cost
    elif totalcost <= c.budget:
        s.cash += totalcost
        find_product(c,s)
        print(f"\nSHOP BALANCE: {s.cash}\n")

# Function to print shop details         
def print_shop(s):
    print(f'\nSHOP BALANCE: {s.cash}\n')
    print(f"------------------------")
    for item in s.stock:
        print_product(item.product)
        print(f'QUANTITY: {item.quantity}')
        print(f"------------------------")

# Loop through the customer shopping list and product stock to find matching items
def find_product(c, s):
    for item in c.shopping_list:
        for prod in s.stock:
                
                # If matching products are in stock print the products details
                if item.product.name == prod.product.name and item.quantity <= prod.quantity:
                    update_quantity = prod.quantity - item.quantity
                    prod.quantity = update_quantity
                    print(f"\nPRODUCT: {prod.product.name}\nPRICE: {prod.product.price}\nQUANTITY: {update_quantity}")
                
                # If the product is not in stock return error
                elif item.product.name == prod.product.name and item.quantity > prod.quantity:
                    print(f"\nUNABLE TO PROCESS ORDER\n")
                    print(f"Sorry {item.product.name} quantity is currently not in stock\n.\nPlease make another order\n")
                    main()

# Check stock for item and return error if not in stock
def check_stock(c, s):
    for item in c.shopping_list:
        for prod in s.stock:
            if item.product.name == prod.product.name and item.quantity > prod.quantity:
                print(f"\nUNABLE TO PROCESS ORDER\n")
                print(f"Sorry {item.product.name} quantity is currently not in stock\n\nPlease make another order\n")
                main()

# Function to calculate costs
def calculate_costs(c, s):
    for shop_item in s.stock:
        for list_item in c.shopping_list:
            if (list_item.product.name == shop_item.product.name):
                list_item.product.price = shop_item.product.price
                totalcost = list_item.product.price * list_item.quantity
                #print(f"Totalcost:{totalcost}")
                
# Function for live order (Allows for customer details to be input)
def live_customer():
    name = input("Enter your name:  ")
    budget = float(input("Enter your budget:  "))
    c = Customer(name, budget)
    c.shopping_list = []
    add_item = "y"
    while (add_item == "y"):
        name = input("Enter Product:  ")
        quantity = int(input("Enter Quantity:  "))
        p = Product(name)
        ps = ProductStock(p, quantity)
        c.shopping_list.append(ps)
        add_item = input("Would you like to make another purchase? y/n:  ")

    return c

# Options menu for customers to select options
def options_menu():
    print(f"-------------------------------------------")
    print(f"----------WELCOME TO THE SHOP--------------")
    print(f"-------------------------------------------")
    print(f"\n")
    print(f"Please select an option")
    print(f"-------------------------------------------")
    print(f"\n")
    print(f"\n1 - View Shop Items\n")
    print(f"\n2 - Order From CSV File\n")
    print(f"\n3 - Order Live\n")
    print(f"\n4 - Exit\n")
    print(f"\n")

# Main function for the program
def main():
    while True:
        options_menu()
        choice = input("Enter number: ")

        if (choice == "1"):
            print_shop(s)

        elif (choice == "2"):
            csv_file = (input("Enter file name: "))
            c = read_customer(csv_file)
            process_customer(c)

        elif (choice == "3"):
            c = live_customer()
            process_customer(c)

        elif (choice == "4"):
            exit()

        else:
            print("Error")
            exit()

if __name__ == "__main__":
    s = create_and_stock_shop()
    main()

   

