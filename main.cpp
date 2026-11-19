
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
using namespace std;

struct Expense
{
    int id;
    string date, category, description;
    double amount;
};

vector<Expense> expenses;

// Save expenses in file
void saveExpenses()
{
    ofstream file("expenses.txt");

    for(auto e : expenses)
        file << e.id << "|" << e.date << "|" << e.category
             << "|" << e.description << "|" << e.amount << endl;

    file.close();
}

// Load expenses from file
void loadExpenses()
{
    ifstream file("expenses.txt");
    string line;

    while(getline(file, line))
    {
        stringstream ss(line);
        string temp;
        Expense e;

        getline(ss, temp, '|');
        e.id = stoi(temp);
        getline(ss, e.date, '|');
        getline(ss, e.category, '|');
        getline(ss, e.description, '|');
        getline(ss, temp);
        e.amount = stod(temp);

        expenses.push_back(e);
    }

    file.close();
}

// User Registration
void registration()
{
    string username, password;

    cout << "Create Username: ";
    getline(cin, username);

    cout << "Create Password: ";
    getline(cin, password);

    ofstream file("users.txt", ios::app);
    file << username << "|" << password << endl;
    file.close();

    cout << "Registration Successful!\n";
}

// Login System
bool login()
{
    string username, password, line;

    cout << "Username: ";
    getline(cin, username);

    cout << "Password: ";
    getline(cin, password);

    ifstream file("users.txt");

    while(getline(file, line))
    {
        stringstream ss(line);
        string u, p;

        getline(ss, u, '|');
        getline(ss, p);

        if(u == username && p == password)
        {
            cout << "Login Successful!\n";
            return true;
        }
    }

    cout << "Invalid Login!\n";
    return false;
}

// Calculate monthly expense
double monthlyTotal(string month)
{
    double total = 0;

    for(auto e : expenses)
        if(e.date.substr(0, 7) == month)
            total += e.amount;

    return total;
}

// Budget Setting
void setBudget()
{
    double budget;

    cout << "Enter Monthly Budget: ";
    cin >> budget;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    ofstream file("budget.txt");
    file << budget;
    file.close();

    cout << "Budget Saved!\n";
}

// Read Budget
double getBudget()
{
    ifstream file("budget.txt");
    double budget = 0;
    file >> budget;
    return budget;
}

// Add Daily Expense
void addExpense()
{
    Expense e;
    int maxID = 0;

    for(auto x : expenses)
        if(x.id > maxID)
            maxID = x.id;

    e.id = maxID + 1;

    cout << "Enter Date (YYYY-MM-DD): ";
    getline(cin, e.date);

    cout << "Enter Category: ";
    getline(cin, e.category);

    cout << "Enter Description: ";
    getline(cin, e.description);

    cout << "Enter Amount: ";
    cin >> e.amount;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if(e.amount <= 0 || e.date.length() != 10 ||
       e.date[4] != '-' || e.date[7] != '-')
    {
        cout << "Invalid expense details!\n";
        return;
    }

    expenses.push_back(e);
    saveExpenses();

    cout << "Expense Added Successfully!\n";

    string month = e.date.substr(0, 7);

    if(getBudget() > 0 && monthlyTotal(month) > getBudget())
        cout << "WARNING: Monthly Budget Exceeded!\n";
}

// View Expense History
void viewExpenses()
{
    if(expenses.empty())
    {
        cout << "No Expenses Found!\n";
        return;
    }

    for(auto e : expenses)
    {
        cout << "\nID: " << e.id;
        cout << "\nDate: " << e.date;
        cout << "\nCategory: " << e.category;
        cout << "\nDescription: " << e.description;
        cout << "\nAmount: " << e.amount << "\n";
    }
}

// Category-wise Expense
void categoryExpense()
{
    string category;
    double total = 0;

    cout << "Enter Category: ";
    getline(cin, category);

    for(auto e : expenses)
    {
        if(e.category == category)
            total += e.amount;
    }

    cout << "Category Total: " << total << endl;
}

// Monthly Expense Report
void monthlyReport()
{
    string month;

    cout << "Enter Month (YYYY-MM): ";
    getline(cin, month);

    cout << "Monthly Expense: " << monthlyTotal(month) << endl;
}

// Budget Warning
void budgetWarning()
{
    string month;

    cout << "Enter Month (YYYY-MM): ";
    getline(cin, month);

    double total = monthlyTotal(month);
    double budget = getBudget();

    cout << "Total Spent: " << total << endl;
    cout << "Budget: " << budget << endl;

    if(budget <= 0)
        cout << "Please set a budget first!\n";
    else if(total > budget)
        cout << "Budget Exceeded!\n";
    else
        cout << "You are within your budget!\n";
}

// Expense Prediction
void expensePrediction()
{
    string month;
    int daysPassed, totalDays;

    cout << "Enter Month (YYYY-MM): ";
    getline(cin, month);

    cout << "Days Passed: ";
    cin >> daysPassed;

    cout << "Total Days in Month: ";
    cin >> totalDays;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if(daysPassed <= 0 || totalDays < daysPassed)
    {
        cout << "Invalid days!\n";
        return;
    }

    double spent = monthlyTotal(month);
    double predicted = (spent / daysPassed) * totalDays;

    cout << "Predicted Monthly Expense: " << predicted << endl;

    if(getBudget() > 0 && predicted > getBudget())
        cout << "Warning: Budget May Be Exceeded!\n";
}

// Search Expense
void searchExpense()
{
    string keyword;
    bool found = false;

    cout << "Enter Category or Description: ";
    getline(cin, keyword);

    for(auto e : expenses)
    {
        if(e.category == keyword ||
           e.description.find(keyword) != string::npos)
        {
            cout << "\nID: " << e.id;
            cout << "\nCategory: " << e.category;
            cout << "\nDescription: " << e.description;
            cout << "\nAmount: " << e.amount << endl;
            found = true;
        }
    }

    if(!found)
        cout << "Expense Not Found!\n";
}

// Delete Expense
void deleteExpense()
{
    int id;
    cout << "Enter Expense ID: ";
    cin >> id;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for(int i = 0; i < expenses.size(); i++)
    {
        if(expenses[i].id == id)
        {
            expenses.erase(expenses.begin() + i);
            saveExpenses();

            cout << "Expense Deleted!\n";
            return;
        }
    }

    cout << "Expense Not Found!\n";
}

// Main Menu
int main()
{
    loadExpenses();

    int choice;

    cout << "===== SMART EXPENSE TRACKER =====\n";
    cout << "1. Register\n";
    cout << "2. Login\n";
    cout << "Enter Choice: ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if(choice == 1)
        registration();

    if(choice != 1 && choice != 2)
    {
        cout << "Invalid Choice!\n";
        return 0;
    }

    if(!login())
        return 0;

    do
    {
        cout << "\n===== EXPENSE TRACKER MENU =====\n";
        cout << "1. Add Daily Expense\n";
        cout << "2. View Expense History\n";
        cout << "3. Category-wise Expense\n";
        cout << "4. Monthly Expense Report\n";
        cout << "5. Set Monthly Budget\n";
        cout << "6. Budget Warning\n";
        cout << "7. Expense Prediction\n";
        cout << "8. Search Expense\n";
        cout << "9. Delete Expense\n";
        cout << "10. Exit\n";

        cout << "Enter Choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch(choice)
        {
            case 1: addExpense(); break;
            case 2: viewExpenses(); break;
            case 3: categoryExpense(); break;
            case 4: monthlyReport(); break;
            case 5: setBudget(); break;
            case 6: budgetWarning(); break;
            case 7: expensePrediction(); break;
            case 8: searchExpense(); break;
            case 9: deleteExpense(); break;
            case 10: cout << "Thank You!\n"; break;
            default: cout << "Invalid Choice!\n";
        }

    } while(choice != 10);

    return 0;
}
