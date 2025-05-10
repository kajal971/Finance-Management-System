Finance Management System

Overview

The Finance Management System is a simple and interactive command-line application designed to help individuals manage their finances. 
It allows users to create multiple accounts, record income and expenses, make investments, and view account details, all in a clean, user-friendly interface.

Features


Multiple Accounts Support: Manage multiple accounts and easily switch between them.

Income and Expenditure Recording: Record income and expenditure along with descriptions.

Investment Management: Add investments, calculate maturity amounts, and track progress.

File Persistence: Save all account data, transactions, and investments to a file for long-term storage.

Customizable Account Names: Assign custom names to accounts for better organization.




Technologies Used


C++: Core language used for implementing the application logic.

File I/O: For saving and loading data from files (using file handling to persist account data).

Standard Template Library (STL): Used for handling various data structures (like vectors, strings).




How to Run




Clone the Repository:



To clone this repository to your local machine, run the following command in your terminal:

git clone https://github.com/kajal971/Finance-Management-System.git




Navigate to the Project Directory:



Once cloned, navigate to the project directory:

cd Finance-Management-System



Compile and Run the Code:


Use the following commands to compile and run the project:



g++ main.cpp -o finance
./finance




How to Use the Application


Upon starting the app, you’ll be prompted to enter your account name.

Once an account is created, you can:

Record Income and Expenditure with descriptions.

Make investments with amounts and durations.

View the balance, transactions, and investments of each account.

You can manage multiple accounts and easily switch between them by entering the corresponding index.




Example Interaction:



Welcome to Finance Management System!!

--OPTIONS--
1. Add Account
2. Switch Account
3. Record INCOME
4. Record EXPENDITURE
5. Make Investment
6. View Account Details
0. Exit
Enter choice: 3
Enter amount: 10000
Enter description: Pocket Money





