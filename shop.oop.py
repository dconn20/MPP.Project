import csv

# Classes and Objects are basic concepts of Object Oriented Programming which revolve around the real life entities.
# A class is a user defined blueprint or prototype from which objects are created.
# An object is simply a collection of data (variables) and methods (functions) that act on those data. Similarly, a class is a blueprint for that object.
# It represents the set of properties or methods that are common to all objects of one type.
# An object consists of State, Behaviour and Identity.


# Product class
class Product:

    def __init__(self, name, price=0):
        self.name = name
        self.price = price
    
    def __repr__(self):
        return f'NAME: {self.name}\nPRICE: {self.price}\n'

# Productstock class
class ProductStock:
    
    def __init__(self, product, quantity):
        self.product = product
        self.quantity = quantity
    
    def name(self):
        return self.product.name
    
    def unit_price(self):
        return self.product.price
        
    def cost(self):
        return self.unit_price() * self.quantity
        
    def __repr__(self):
        return f"{self.product}QUANTITY: {self.quantity}\n"

# Customer class
class Customer:
    # Create customer from CSV file
    def __init__(self, path):
        self.shopping_list = []
        with open(path) as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=',')
            first_row = next(csv_reader)
            self.name = first_row[0]
            self.budget = float(first_row[1])
            for row in csv_reader:
                name = row[0]
                quantity = float(row[1])
                p = Product(name)
                ps = ProductStock(p, quantity)
                self.shopping_list.append(ps) 
    
    # Function to calculate product price           
    def calculate_costs(self, price_list):
        #totalcost = 0
        for shop_item in price_list:
            for list_item in self.shopping_list:
                if (list_item.name() == shop_item.name()):
                    list_item.product.price = shop_item.unit_price()
                    #charge = list_item.quantity * list_item.product.price
                    #totalcost =+ charge
                    #print(totalcost)
    
    # Used to calculate order costs
    def order_cost(self):
        cost = 0
        for list_item in self.shopping_list:
            cost += list_item.cost()
        return cost

    def __repr__(self):
        
        str = f"\nName: {self.name} \nBudget: {self.budget}\n"
        for item in self.shopping_list:
            cost = item.cost()
            str += f"\n{item}"
            if (cost == 0):
                str += f" {self.name} doesn't know how much that costs :("
            else:
                str += f"COST: {cost}\n--------------------------------\n"
                
        str += f"\n{self.name}'s Totalcost: {self.order_cost()}\n--------------------------------\n"
        return str 

# Shop class       
class Shop:
    # Create shop from CSV file
    def __init__(self, path):
        self.stock = []
        with open(path) as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=',')
            first_row = next(csv_reader)
            self.cash = float(first_row[0])
            for row in csv_reader:
                p = Product(row[0], float(row[1]))
                ps = ProductStock(p, int(row[2]))
                self.stock.append(ps)

    # Process customer order
    def process_order(self, c):
        print(f"\n-------------------------------------------")
        print(f"---------------Process Order---------------")
        print(f"-------------------------------------------")
        
        # Loop through shopping list and shop stock to find items
        for list_item in c.shopping_list:
            for item in self.stock:
                # If ordered quantity is lower than quantity in stock return error
                if (list_item.name() == item.name()) and (list_item.quantity > item.quantity):
                    print(f"UNABLE TO PROCESS ORDER\n")
                    print(f"Sorry {list_item.name()} quantity is currently not in stock\n\nPlease make another order")
                # If not update item quantity and print product details
                elif (list_item.name() == item.name()):
                    item.quantity = (item.quantity - list_item.quantity)
                    print (f"{item}")
                # Calculate the customer charge        
                list_item.product.price = item.unit_price()
                charge = (list_item.quantity * list_item.product.price)
        
        # If the charge is greater than the budget return error   
        if (c.budget < charge):
            print(f"-------------------------------------------")
            print(f"UNABLE TO PROCESS ORDER\n")
            print(f"Your budget is too low")
            
            # Calculate short amount
            shortAmount = abs (c.budget - charge)
            print(f"You are short by €{shortAmount}\n\nPlease make another order")
            
        # If the charge is less than the budget add the charge to the shop balance
        elif (c.budget > charge):
            self.cash = self.cash + charge
            print(f"Shop Balance: {self.cash}")

    # Return shop balance and products in shop stock
    def __repr__(self):
        str = ""
        str += f'\nSHOP BALANCE: {self.cash}\n'
        for item in self.stock:
            str += f"\n{item}\n-----------------------------\n"
        return str
     
    # Create and process live customer          
    def live_customer(self, c):
        # Create customer (name & budget)
        self.name = input("Enter your name:  ")
        self.budget = float(input("Enter your budget:  "))
        c = (self.name, self.budget)
        shopping_list = []
        add_item = "y"
        # Create shopping list (product & quantity)
        while (add_item == "y"):
            product_name = input("Enter Product:  ")
            quantity = int(input("Enter Quantity:  "))
            p = Product(product_name)
            ps = ProductStock(p, quantity)
            shopping_list.append(ps)
            add_item = input("Would you like to make another purchase? y/n:  ")
        # Print customer details and order details
        print(f"\nName: {self.name}\nBudget:{self.budget}\n")
        for list_item in shopping_list:
            for item in s.stock:
                list_item.product.price = item.unit_price()
                cost = (list_item.quantity * list_item.product.price)
                if list_item.name() == item.name():
                    print(f"{list_item}Cost: {cost}")

        # Process customer order
        print(f"\n-------------------------------------------")
        print(f"---------------Process Order---------------")
        print(f"-------------------------------------------")
        
        # Loop through shopping list and shop stock to find items
        for list_item in shopping_list:
            for item in self.stock: 
                # If quantity ordered not in stock return error
                if (list_item.name() == item.name()) and (list_item.quantity > item.quantity):
                    print(f"\nUNABLE TO PROCESS ORDER\n")
                    print(f"Sorry {item.product.name} quantity is currently not in stock\n\nPlease make another order")
                    s.main()
                # If there is enough in stock update item quantity and print product details
                elif (list_item.name() == item.name()):
                    item.quantity = (item.quantity - list_item.quantity)
                    print (f"{item}")
                    # Calculate customer charge
                    list_item.product.price = item.unit_price()
                    charge = (list_item.quantity * list_item.product.price)
        # If the charge exceeds the budget return error    
        if (self.budget < charge):
            print(f"UNABLE TO PROCESS ORDER\n")
            print(f"Your budget is too low")
            # Calculate short amount
            shortAmount = abs (self.budget - charge)
            print(f"You are short by €{shortAmount}\n\nPlease make another order")
        # If the charge is less then the budget update the shop balance with the charge
        elif (self.budget > charge):
            self.cash += charge
            print(f"Shop Balance: {self.cash}")

        
        return c
        
    # Options menu for customer to select options
    def options_menu(self):
        print(f"\n-------------------------------------------")
        print(f"----------WELCOME TO THE SHOP-------------")
        print(f"-------------------------------------------\n")
        print(f"Please select an option")
        print(f"-----------------------------------------")
        print(f"\n")
        print(f"\n1 - View Shop Items\n")
        print(f"\n2 - Order From CSV File\n")
        print(f"\n3 - Order Live\n")
        print(f"\n4 - Exit\n")
        print(f"\n")

    # Main function for the program
    def main(self):
        while True:
            s.options_menu()
            choice = input("Enter number: ")

            if (choice == "1"):
                print(s)
            
            elif (choice == "2"):
                csv_file = (input("Enter file name: "))
                c = Customer(csv_file)
                c.calculate_costs(s.stock)
                print(c)
                s.process_order(c)

            elif (choice == "3"):
                c = Customer
                s.live_customer(c)

            elif (choice == "4"):
                exit()

            else:
                print("Error")
                exit()

if __name__ == "__main__":

    s = Shop("./stock.csv")
    c = Customer
    s.main()


# References
# https://realpython.com/python3-object-oriented-programming/
# https://www.geeksforgeeks.org/classes-objects-java/
# https://study.com/academy/lesson/oop-object-oriented-programming-objects-classes-interfaces.html
# https://www.programiz.com/python-programming/class
# https://www.tutorialspoint.com/python/python_classes_objects.htm
