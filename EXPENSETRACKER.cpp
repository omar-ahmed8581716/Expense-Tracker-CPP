#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>

using namespace std;

const string FileName = "Transactions.txt";

enum enTransaction {
    enAddIncome = 1,
    enAddExpense = 2,
    enshowAllTransaction = 3,
    enFindTransaction = 4,
    enDeleteTransaction = 5,
    enShowBalance = 6,
    enViewStatistics = 7,
    enExit = 8
};

enum enTransactionType { Income = 1 , Expense = 2};

enum enStatistics { SpecificMonth = 1 , SpecificYear = 2 , AllTime =3 , Back = 4 };

struct stTransaction
{
    short ID = 0;
    string Date = "";
    double Amount = 0.0;
    string Category = "";
    enTransactionType TransactionType = enTransactionType::Income;
    bool MarkForDelete = false;
};

void ShowErrorFeedback()
{
    cout << "\a";
    system("color 4F");
}

void ResetConsoleColor()
{
    system("color 07");
}

short ReadNumberBetweenRange(short From, short To , string Message , string Value)
{
    short Number;
    cout << Message;
    cin >> Number;

    while (cin.fail() || Number < From || Number > To)
    {
        ShowErrorFeedback();
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << " Invalid " << Value << " , Enter anthor "<<Value<<" : ";
        cin >> Number;
    }

    ResetConsoleColor();
    return Number;
}

// **************** Date Function *****************

bool IsLeapYear(short Year)
{
    return (Year % 400 == 0) || (Year % 4 == 0 && Year % 100 != 0);
}

short NumberOfDaysInMonth(short Year, short Month)
{
    short Days[13] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };

    return (Month == 2 && IsLeapYear(Year)) ? 29 : Days[Month];
}

string ReadDate()
{
    cout << "\n Okay , Enter Date of Transaction\n";

    short Year = ReadNumberBetweenRange(1900,2100," Enter Year  : ","Year");

    short Month = ReadNumberBetweenRange(1, 12," Enter Month : ","Month");

    short NumberOfDays = NumberOfDaysInMonth(Year, Month);

    short Day = ReadNumberBetweenRange(1 , NumberOfDays," Enter Day : ","Day");

    string Date = to_string(Day) + "/" + to_string(Month) + "/" + to_string(Year);

    cout << "\n Okay Date is : " << Date << endl;
    return Date;
}

string MonthName(short Month)
{
    string MonthName[13] = { "" ,"January" , "February" , "March" , "April" ,
        "May" , "June" , "July" , "August" , "September" , 
        "October" , "November" , "December"};

    return MonthName[Month];
}

// **************** Genral Function *****************

short GenerateID(vector<stTransaction> &vTransaction)
{
    short Id = 0;

    for ( stTransaction &Tran : vTransaction)
    {
        if (Tran.ID > Id)
           Id = Tran.ID;
    }

    return Id + 1;
}

stTransaction ReadTransaction(short ID , enTransactionType TranTypy)
{
    stTransaction Transaction;

    Transaction.Date = ReadDate();

    cout << " Okay , Enter Amount : ";
    cin >> Transaction.Amount;

    while (cin.fail() || Transaction.Amount < 1)
    {
        ShowErrorFeedback();
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << " Invaled Value , Enter Correct Value : ";
        cin >> Transaction.Amount;
    }

    ResetConsoleColor();
    cout << " Enter Category : ";
    getline(cin >> ws, Transaction.Category);

    Transaction.ID = ID;
    Transaction.TransactionType = TranTypy;

    return Transaction;
}

string TransactionTypeToString(enTransactionType Type)
{
    switch (Type)
    {
    case enTransactionType::Income:
        return "Income";

    case enTransactionType::Expense:
        return "Expense";
    default:
        return "Unknown";
    }
}

enTransactionType StringToTransactionType(string TypeText)
{
    if (TypeText == "Income")
        return enTransactionType::Income;
    else
        return enTransactionType::Expense;
}

void ShowMainMenu()
{
    cout << "\n";
    cout << "+--------------------------------------------+\n";
    cout << "|             EXPENSE TRACKER                |\n";
    cout << "|          Personal Finance System           |\n";
    cout << "+--------------------------------------------+\n";
    cout << "|   [1]  Add Income                          |\n";
    cout << "|   [2]  Add Expense                         |\n";
    cout << "|   [3]  View Transactions                   |\n";
    cout << "|   [4]  Find Transaction                    |\n";
    cout << "|   [5]  Delete Transaction                  |\n";
    cout << "|   [6]  View Balance                        |\n";
    cout << "|   [7]  Statistics                          |\n";
    cout << "|   [8]  Exit                                |\n";
    cout << "+--------------------------------------------+\n";
    cout << "\n";
}

short ReadOptionFromMenu(short From , short To)
{
    short Option = ReadNumberBetweenRange(From, To, " Enter Your Option from this Menu : ", "Option");
    return Option;
}

vector<string> SplitString(string Text, string Seperator = "#//#")
{
    short Pos = 0;
    string Word = "";
    vector<string> vString;

    while ((Pos = Text.find(Seperator)) != std::string::npos)
    {
        Word = Text.substr(0, Pos);

        if (Word != "")
        {
            vString.push_back(Word);
        }

        Text.erase(0, Pos + Seperator.length());
    }

    if (Text != "")
    {
        vString.push_back(Text);
    }

    return vString;
}

string ConvertTransactionToLine(stTransaction Tran, string Separator = "#//#")
{
    string Line = "";

    Line += to_string(Tran.ID) + Separator;
    Line += Tran.Date + Separator;
    Line += to_string(Tran.Amount) + Separator;
    Line += Tran.Category + Separator;

    string TranName = TransactionTypeToString(Tran.TransactionType);
    Line += TranName;

    return Line;
}

stTransaction ConvertLineToTransaction(string Line, string Separator = "#//#")
{
    vector<string> vTransactionData = SplitString(Line, Separator);
    stTransaction Transaction;

    if (vTransactionData.size() >= 5)
    {
        Transaction.ID = stoi(vTransactionData[0]);
        Transaction.Date = vTransactionData[1];
        Transaction.Amount = stod(vTransactionData[2]);
        Transaction.Category = vTransactionData[3];
        Transaction.TransactionType = StringToTransactionType(vTransactionData[4]);
    }

    return Transaction;
}

vector<stTransaction> LoadDateFromFile(string FileName)
{
    vector<stTransaction> vTransaction ;
    fstream MyFile;

    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;

        while (getline(MyFile, Line))
        {
            if (Line != "")
            {
                vTransaction.push_back(ConvertLineToTransaction(Line));
            }
        }
        MyFile.close();
    }
    return vTransaction;
}

void SaveTransactionsToFile(string FileName,string Line)
{
    fstream MyFile;

    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << Line << endl;   
        MyFile.close();
    }
}

double TotalIncome(vector<stTransaction>& vTransaction)
{
    double TotalMoney = 0;

    for (stTransaction& Tran : vTransaction)
    {
        if (Tran.TransactionType == enTransactionType::Income)
            TotalMoney += Tran.Amount;
    }

    return TotalMoney;
}

short NumberOfIncomeTransaction(vector<stTransaction>& vTransaction)
{
    short TotalTransaction = 0;

    for (stTransaction& Tran : vTransaction)
    {
        if (Tran.TransactionType == enTransactionType::Income)
            TotalTransaction++;
    }

    return TotalTransaction;
}

short NumberOfExpenseTransaction(vector<stTransaction>& vTransaction)
{
    short TotalTransaction = 0;

    for (stTransaction& Tran : vTransaction)
    {
        if (Tran.TransactionType == enTransactionType::Expense)
            TotalTransaction++;
    }

    return TotalTransaction;
}

double TotalExpense(vector<stTransaction>& vTransaction)
{
    double TotalMoney = 0;

    for (stTransaction& Tran : vTransaction)
    {
        if (Tran.TransactionType == enTransactionType::Expense)
            TotalMoney += Tran.Amount;
    }

    return TotalMoney;
}

double TotalBalance(vector<stTransaction>& vTransaction)
{
    double Balance = 0;

    Balance = TotalIncome(vTransaction) - TotalExpense(vTransaction);

    return Balance;
}

short ReadID(string Message)
{
    short Id = 0;
    cout << Message;
    cin >> Id;

    while (cin.fail() || Id < 1 )
    {
        ShowErrorFeedback();
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << " Invaled Id , Enter Correct Id : ";
        cin >> Id;
    }

    ResetConsoleColor();
    return Id;
}

bool IsTransactionFindID(short ID, stTransaction& Transaction , const  vector<stTransaction>& vTransaction)
{
  
    for (const stTransaction& Tran : vTransaction)
    {
        if (ID == Tran.ID)
        {
            Transaction = Tran;
            return true;
        }
    }
    return false;
}

void PrintTransactionDetails(stTransaction Transaction)
{
    cout << "\n";
    cout << "+--------------------------------------------------+\n";
    cout << "|              TRANSACTION DETAILS                 |\n";
    cout << "+--------------------------------------------------+\n";
    cout << "|  ID        : " << left << setw(35) << Transaction.ID << " |\n";
    cout << "|  Date      : " << left << setw(35) << Transaction.Date << " |\n";
    cout << "|  Category  : " << left << setw(35) << Transaction.Category << " |\n";
    cout << "|  Amount    : " << left << setw(35) << Transaction.Amount << " |\n";
    cout << "|  Type      : " << left << setw(35)
        << TransactionTypeToString(Transaction.TransactionType) << " |\n";

    cout << "+--------------------------------------------------+\n";
}

double HighestExpense(vector<stTransaction>& vTransaction)
{                                                      
    double Highest = 0;
                                                       
    for (stTransaction& Transaction : vTransaction)
    {
        if (Transaction.TransactionType == enTransactionType::Expense)
        {
            if (Transaction.Amount > Highest)
            {
                Highest = Transaction.Amount;
            }
        }
    }
    return Highest;
}

double LowestExpense(vector<stTransaction>& vTransaction)
{
    double Lowest = 0;

    for (stTransaction& Transaction : vTransaction)
    {
        if (Transaction.TransactionType == enTransactionType::Expense) // 50 
        {
            Lowest = Transaction.Amount;
            break;
        }
    }
   
    for (stTransaction& Transaction : vTransaction)
    {
        if (Transaction.TransactionType == enTransactionType::Expense)
        {
            if (Transaction.Amount < Lowest)
            {
                Lowest = Transaction.Amount;
            }
        }
    }
    return Lowest;
}

string HighestExpenseCategory(vector<stTransaction>& vTransaction)
{
    string Category = "";
    double Highest = 0;

    if (NumberOfExpenseTransaction(vTransaction) == 0)
        return "No Category ";

    for (stTransaction& Transaction : vTransaction)
    {
        if (Transaction.TransactionType == enTransactionType::Expense)
        {
            if (Transaction.Amount > Highest)
            {
                Highest = Transaction.Amount;
                Category = Transaction.Category;
            }
        }
    }
    return Category;
}

string HighestIncomeCategory(vector<stTransaction>& vTransaction)
{
    string Category = "";
    double Highest = 0;

    if (NumberOfIncomeTransaction(vTransaction) == 0)
        return "No Category ";

    for (stTransaction& Transaction : vTransaction)
    {
        if (Transaction.TransactionType == enTransactionType::Income)
        {
            if (Transaction.Amount > Highest)
            {
                Highest = Transaction.Amount;
                Category = Transaction.Category;
            }
        }
    }
    return Category;
}

void GoBackToMainMenu()
{
    cout << "\n Press any key to return to the Main Menu...";

    system("pause>0");
    ResetConsoleColor();
}

short ReadMonth()
{
    short Month = 0;
    cout << "\n Okay , Enter Number of Month do you want : ";
    cin >> Month;

    while (cin.fail() || Month > 12 || Month < 1)
    {
        ShowErrorFeedback();
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << " Invaled Month , Enter Correct Month : ";
        cin >> Month;
    }

    ResetConsoleColor();
    return Month;
}

short ReadYear()
{
    short Year = 0;
    cout << "\n Okay , Enter Year do you want : ";
    cin >> Year;    // 1900 , 2100

    while (cin.fail() || Year > 2100 || Year < 1900)
    {
        ShowErrorFeedback();
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << " Invaled Year , Enter Correct Year : ";
        cin >> Year;
    }

    ResetConsoleColor();
    return Year;
}

vector<stTransaction>  GetTransactionOfSpecificMonth(short Month , short Year , const vector<stTransaction>& vTransaction )
{
    vector<stTransaction> vSpecificMonth;

    for (const stTransaction& Tran : vTransaction)
    {
       vector<string> Date = SplitString(Tran.Date, "/");
       if (Date.size() >= 3)
       {
           short M = stoi(Date[1]);
           short Y = stoi(Date[2]);

           if (Month == M && Year == Y)
           {
               vSpecificMonth.push_back(Tran);
           }
       }
    }
    return vSpecificMonth;
}

void PrintTransactionOfSpecificMonth(vector<stTransaction>& vTransaction ,short Month , short Year)
{
    string NameOfMonth = MonthName(Month);

    if (vTransaction.empty())
    {
        cout << "\n No Transactions found for the Month ["<<NameOfMonth<<" , "<<Year<< "] .\n";
        return;
    }

    else
    {
        short TotalTransaction = NumberOfIncomeTransaction(vTransaction) + NumberOfExpenseTransaction(vTransaction);

        double TotalIncomeAmount = TotalIncome(vTransaction);
        double TotalExpenseAmount = TotalExpense(vTransaction);
        double MonthlyNet = TotalBalance(vTransaction);

        double HighestExpenseAmount = HighestExpense(vTransaction);
        double LowestExpenseAmount = LowestExpense(vTransaction);

        string HighestIncomeCat = HighestIncomeCategory(vTransaction);
        string HighestExpenseCat = HighestExpenseCategory(vTransaction);


        cout << "\n+ -------------------------------------------------+\n";
        cout << "|          Show Statistics Of " << NameOfMonth << " " << Year << "            |";
        cout << "\n+ -------------------------------------------------+\n";

        cout << "|--------------------------------------------------|\n";
        cout << "|  Total Transactions       : "
            << left << setw(20) << TotalTransaction << " |\n";

        cout << "|  Income Transactions      : "
            << left << setw(20)
            << NumberOfIncomeTransaction(vTransaction) << " |\n";

        cout << "|  Expense Transactions     : "
            << left << setw(20)
            << NumberOfExpenseTransaction(vTransaction) << " |\n";
        cout << "|--------------------------------------------------|\n";

        cout << "|  Total Income             : "
            << left << setw(20) << TotalIncomeAmount << " |\n";

        cout << "|  Total Expense            : "
            << left << setw(20) << TotalExpenseAmount << " |\n";

        cout << "|  Monthly Net              : "
            << left << setw(20) << MonthlyNet << " |\n";
        cout << "|--------------------------------------------------|\n";

        cout << "|  Highest Expense          : "
            << left << setw(20) << HighestExpenseAmount << " |\n";

        cout << "|  Lowest Expense           : "
            << left << setw(20) << LowestExpenseAmount << " |\n";
        cout << "|--------------------------------------------------|\n";

        cout << "|  Highest Expense Category : "
            << left << setw(20) << HighestExpenseCat << " |\n";

        cout << "|  Highest Income Category  : "
            << left << setw(20) << HighestIncomeCat << " |\n";

        cout << "+--------------------------------------------------+\n";
    }

}

vector<stTransaction>  GetTransactionOfSpecificYear( short Year, const vector<stTransaction>& vTransaction)
{
    vector<stTransaction> vSpecificYear;

    for (const stTransaction& Tran : vTransaction)
    {
        vector<string> Date = SplitString(Tran.Date, "/");
        short Y = stoi(Date[2]);

        if (Year == Y)
        {
            vSpecificYear.push_back(Tran);
        }
    }
    return vSpecificYear;
}

void PrintTransactionOfSpecificYear(vector<stTransaction>& vTransaction, short Year)
{

    if (vTransaction.empty())
    {
        cout << "\n No Transactions found for the year [" << Year << "] \n";
    }

    else
    {
        short TotalTransaction = NumberOfIncomeTransaction(vTransaction) + NumberOfExpenseTransaction(vTransaction);

        double TotalIncomeAmount = TotalIncome(vTransaction);
        double TotalExpenseAmount = TotalExpense(vTransaction);
        double YearNet = TotalBalance(vTransaction);

        double HighestExpenseAmount = HighestExpense(vTransaction);
        double LowestExpenseAmount = LowestExpense(vTransaction);

        string HighestIncomeCat = HighestIncomeCategory(vTransaction);
        string HighestExpenseCat = HighestExpenseCategory(vTransaction);

        cout << "\n+ -------------------------------------------------+\n";
        cout << "|          Show Statistics Of " << Year << "                 |";
        cout << "\n+ -------------------------------------------------+\n";

        cout << "|--------------------------------------------------|\n";
        cout << "|  Total Transactions       : "
            << left << setw(20) << TotalTransaction << " |\n";

        cout << "|  Income Transactions      : "
            << left << setw(20)
            << NumberOfIncomeTransaction(vTransaction) << " |\n";

        cout << "|  Expense Transactions     : "
            << left << setw(20)
            << NumberOfExpenseTransaction(vTransaction) << " |\n";
        cout << "|--------------------------------------------------|\n";

        cout << "|  Total Income             : "
            << left << setw(20) << TotalIncomeAmount << " |\n";

        cout << "|  Total Expense            : "
            << left << setw(20) << TotalExpenseAmount << " |\n";

        cout << "|  Year Net                 : "
            << left << setw(20) << YearNet << " |\n";
        cout << "|--------------------------------------------------|\n";

        cout << "|  Highest Expense          : "
            << left << setw(20) << HighestExpenseAmount << " |\n";

        cout << "|  Lowest Expense           : "
            << left << setw(20) << LowestExpenseAmount << " |\n";
        cout << "|--------------------------------------------------|\n";

        cout << "|  Highest Expense Category : "
            << left << setw(20) << HighestExpenseCat << " |\n";

        cout << "|  Highest Income Category  : "
            << left << setw(20) << HighestIncomeCat << " |\n";

        cout << "+--------------------------------------------------+\n";
    }

}

void ShowStatistcMenu()
{
    cout << "\n";
    cout << "+--------------------------------------------+\n";
    cout << "|             Statistc Menu                  |\n";
    cout << "+--------------------------------------------+\n";
    cout << "|   [1]  Specific Month                      |\n";
    cout << "|   [2]  Specific Year                       |\n";
    cout << "|   [3]  All Time                            |\n";
    cout << "|   [4]  Back                                |\n";
    cout << "+--------------------------------------------+\n";
    cout << "\n";
}

void GoBackToStatistcMenu()
{
    cout << "\n Press any key to return to the Statistc Menu...";

    system("pause>0");
    ResetConsoleColor();
}

// **************** Add Income Function *****************

void AddIncome(vector<stTransaction> &vTransaction)
{
    cout << "\n+ -------------------------------------------------+\n";
    cout << "|                    Add Income                    |";
    cout << "\n+ -------------------------------------------------+\n\n";

    short ID = GenerateID(vTransaction);
    enTransactionType TranType = enTransactionType::Income;

    stTransaction Tran = ReadTransaction(ID, TranType);

    char Answer = 'y';
    cout << "\n Do you want Added this Transaction (Y/N) ? : ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        vTransaction.push_back(Tran);
        string Line = ConvertTransactionToLine(Tran);
        SaveTransactionsToFile(FileName, Line);

        system("color 2F");
        cout << "\n Transaction Added Successfuly . ";
        cout << "\n The ID of this Transaction is : " << ID << endl;
    }
   
    else
    {
        cout << "\n Transaction Cancelled, Nothing was saved.\n";
    }
}

// **************** Add Expense Function *****************

void AddExpense(vector<stTransaction>& vTransaction)
{
    cout << "\n+ -------------------------------------------------+\n";
    cout << "|                    Add Expense                   |";
    cout << "\n+ -------------------------------------------------+\n\n";

    double CurrentBalance = TotalBalance(vTransaction);

    if (CurrentBalance < 1)
    {
        cout << " You can not add expense Because Your current balance is : " << CurrentBalance << endl;
        return;
    }

    else
    {
        short ID = GenerateID(vTransaction);
        stTransaction Tran = ReadTransaction(ID, enTransactionType::Expense);

        while (cin.fail() || Tran.Amount > CurrentBalance || Tran.Amount < 1)
        {
            ShowErrorFeedback();
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << " Invalid Amount Cannot exceed current balance (" << CurrentBalance << ") . Enter Again : ";
            cin >> Tran.Amount;
        }


        ResetConsoleColor();

        char Answer = 'y';
        cout << "\n Do you want to Add this Transaction? (y/n): ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y')
        {
            vTransaction.push_back(Tran);
            string Line = ConvertTransactionToLine(Tran);
            SaveTransactionsToFile(FileName, Line);

            system("color 2F");
            cout << "\n Transaction Added Successfully.";
            cout << "\n The ID of this Transaction is : " << ID << endl;
        }
        else
        {
            cout << "\n Transaction Cancelled, Nothing was saved.\n";
        }

    }
}

// **************** Showeing Function *****************

void PrintOneTransaction(stTransaction Tran)
{ 
    cout << "| " << left << setw(5)  << Tran.ID ;
    cout << "| " << left << setw(12) << Tran.Date;
    cout << "| " << left << setw(20) << Tran.Category;
    cout << "| " << left << setw(10) << Tran.Amount;
    cout << "| " << left << setw(8)  << TransactionTypeToString(Tran.TransactionType) <<"|";
}

void PrintAllTransaction(vector<stTransaction> &vTransaction)
{
    cout << "\n\t-+-+- Transaction List [" << vTransaction.size() << "] Transaction  +-+-+\n";
    cout << "\n+------+-------------+---------------------+-----------+---------+\n";
    cout << "|  ID  |    Date     |     Category        |   Amount  |   Type  |\n";
    cout << "+------+-------------+---------------------+-----------+---------+\n";

    for (stTransaction& Tran : vTransaction)
    {
        PrintOneTransaction(Tran);
        cout << endl;
    }

    cout << "+------+-------------+---------------------+-----------+---------+\n";

}

// **************** Finding Function *****************

void FindTransactionByID(vector<stTransaction>& vTransaction)
{
    cout << "\n+ -------------------------------------------------+\n";
    cout << "|                Find TRANSACTION                  |";
    cout << "\n+ -------------------------------------------------+\n\n";

    short ID = ReadID(" Okay , Enter ID of this Transaction do you want : ");
    stTransaction Transaction;

    while ( !IsTransactionFindID(ID, Transaction,vTransaction))
    {
        ShowErrorFeedback();
        ID = ReadID(" Invalid Id , Plaese Enter Id again : ");
    }

    system("color 2F");
    PrintTransactionDetails(Transaction);
}

// **************** Deleteing Function *****************

bool MarkForDeleteByID(short ID, vector<stTransaction>& vTransaction)
{
    for (stTransaction& Tran : vTransaction)
    {
        if (ID == Tran.ID)
        {
            Tran.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

vector<stTransaction> RewriteTransactionsFile(vector<stTransaction>& vTransaction)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    string Line = "";

    if (MyFile.is_open())
    {
        for (stTransaction& Tran : vTransaction)
        {
            if (Tran.MarkForDelete == false)
            {
                Line = ConvertTransactionToLine(Tran);
                MyFile << Line << endl;
            }
        }
        MyFile.close();
    }
    return vTransaction;
}

bool DeleteTransaction(vector<stTransaction>& vTransaction)
{
    cout << "\n+ -------------------------------------------------+\n";
    cout << "|                DELETE TRANSACTION                |";
    cout << "\n+ -------------------------------------------------+\n\n";

    short ID = ReadID(" Okay , Enter ID of this Transaction do you want : ");
    stTransaction Transaction;

    while ( !IsTransactionFindID(ID, Transaction,vTransaction))
    {
        ShowErrorFeedback();
        ID = ReadID(" Invalid Id , Plaese Enter Id again : ");
    }

    ResetConsoleColor();
    PrintTransactionDetails(Transaction);

    char Answer = 'y';
    cout << "\n Do You want Delete this Transaction (y/n) ? : ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        MarkForDeleteByID(ID, vTransaction);
        RewriteTransactionsFile(vTransaction);
        system("color 2F");
        vTransaction = LoadDateFromFile(FileName); // To Refresh Clients 
        cout << "\n Transaction Deleted Sccussfuly . ";
        return true;
    }
    else
    {
        cout << "\n Okay , Transaction stayed unchanged . ";
        return false;
    }
}

// **************** Show Balance Function *****************

void ShowTotalBalance(vector<stTransaction>& vTransaction)
{

    double Income  = TotalIncome(vTransaction);
    double Expense = TotalExpense(vTransaction);
    double Balance = TotalBalance(vTransaction);

    cout << "\n+ ---------------------------------------+\n";
    cout << "|                Balance                 |";
    cout << "\n+ ---------------------------------------+\n";

    cout << "|      Total Income   : " << Income << "\t\t |\n";
    cout << "|      Total Expense  : " << Expense << "\t\t |\n";

    cout << "+----------------------------------------+\n";

    cout << "|      Current Balance: " << Balance << "\t\t |\n";

    cout << "+----------------------------------------+\n";
}

// **************** Show Statistics Function *****************

void ShowStatistics(vector<stTransaction> & vTransaction)
{
    short TotalTransaction = NumberOfIncomeTransaction(vTransaction) + NumberOfExpenseTransaction(vTransaction);

    double TotalIncomeAmount  = TotalIncome(vTransaction);
    double TotalExpenseAmount = TotalExpense(vTransaction);
    double Balance = TotalBalance(vTransaction);

    double HighestExpenseAmount = HighestExpense(vTransaction);
    double LowestExpenseAmount = LowestExpense(vTransaction);

    string HighestIncomeCat = HighestIncomeCategory(vTransaction);
    string HighestExpenseCat = HighestExpenseCategory(vTransaction);

    cout << "\n+ -------------------------------------------------+\n";
    cout << "|                SHOW STATISITCS                   |";
    cout << "\n+ -------------------------------------------------+\n";

    cout << "\n\n+--------------------------------------------------+\n";
    cout << "|                 Transaction                      |\n";
    cout << "|                                                  |\n";
    cout << "|  Total Transactions       : "
        << left << setw(20) << TotalTransaction << " |\n";

    cout << "|  Income Transactions      : "
        << left << setw(20)
        << NumberOfIncomeTransaction(vTransaction) << " |\n";

    cout << "|  Expense Transactions     : "
        << left << setw(20)
        << NumberOfExpenseTransaction(vTransaction) << " |\n";

    cout << "+--------------------------------------------------+\n"; 
    cout << "|                Financial Summary                 |\n";
    cout << "|                                                  |\n";
    cout << "|  Total Income             : "
        << left << setw(20) << TotalIncomeAmount << " |\n";

    cout << "|  Total Expense            : "
        << left << setw(20) << TotalExpenseAmount << " |\n";

    cout << "|  Current Balance          : "
        << left << setw(20) << Balance << " |\n";

    cout << "+--------------------------------------------------+\n";
    cout << "|                 Expense Analysis                 |\n";
    cout << "|                                                  |\n";
    cout << "|  Highest Expense          : "
        << left << setw(20) << HighestExpenseAmount << " |\n";

    cout << "|  Lowest Expense           : "
        << left << setw(20) << LowestExpenseAmount << " |\n";

    cout << "|  Highest Expense Category : "
        << left << setw(20) << HighestExpenseCat << " |\n";

    cout << "+--------------------------------------------------+\n";
    cout << "|                 Income Analysis                  |\n";
    cout << "|                                                  |\n";
    cout << "|  Highest Income Category  : "
        << left << setw(20) << HighestIncomeCat << " |\n";

    cout << "+--------------------------------------------------+\n";


}

// **************** Excuation Function *****************

void StatisticsExcuation()
{
    enStatistics Option;

    do
    {
        system("cls");
        ShowStatistcMenu();
        Option = (enStatistics)ReadOptionFromMenu(1,4);
        vector<stTransaction> vTransaction = LoadDateFromFile(FileName);

        switch (Option)
        {
        case enStatistics::SpecificMonth:
        {
            system("cls");

          short Month = ReadMonth();
          short Year  = ReadYear();
          vTransaction = GetTransactionOfSpecificMonth(Month, Year, vTransaction);
          PrintTransactionOfSpecificMonth(vTransaction, Month, Year);
          GoBackToStatistcMenu();
            break;
        }
        case enStatistics::SpecificYear:
        {
            system("cls");
            short Year = ReadYear();
           vTransaction = GetTransactionOfSpecificYear(Year, vTransaction);
            PrintTransactionOfSpecificYear(vTransaction,Year);
            GoBackToStatistcMenu();
            break;
        }
        case enStatistics::AllTime:
        {
            system("cls");
            ShowStatistics(vTransaction);
            GoBackToStatistcMenu();
            break;
        }
        case enStatistics::Back:
        {
            system("cls");
            break;
        }
        
        }
    } while (Option != enStatistics::Back);
}

void programExcuation()
{
    enTransaction Option;

    do
    {
        system("cls");
        ShowMainMenu();
        Option = (enTransaction) ReadOptionFromMenu(1,8);
        vector<stTransaction> vTransaction = LoadDateFromFile(FileName);

        switch (Option)
        {
        case enTransaction::enAddIncome:
        {
            system("cls");
            AddIncome(vTransaction);
            GoBackToMainMenu();
            break;
        }
        case enTransaction::enAddExpense:
        {
            system("cls");
            AddExpense(vTransaction);
            GoBackToMainMenu();
            break;
        }
        case enTransaction::enshowAllTransaction:
        {
            system("cls");
            PrintAllTransaction(vTransaction);
            GoBackToMainMenu();
            break;
        }
        case enTransaction::enFindTransaction:
        {
            system("cls");
            FindTransactionByID(vTransaction);
            GoBackToMainMenu();
            break;
        }
        case enTransaction::enDeleteTransaction:
        {
            system("cls");
            DeleteTransaction(vTransaction);
            GoBackToMainMenu();
            break;
        }
        case enTransaction::enShowBalance:
        {
            system("cls");
            ShowTotalBalance(vTransaction);
            GoBackToMainMenu();
            break;
        }
        case enTransaction::enViewStatistics:
        {
            system("cls");
            StatisticsExcuation();
            break;
        }
        case enTransaction::enExit:
        {
            system("cls");
            cout << "+--------------------------------------------------+\n";
            cout << "|             THANK YOU FOR USING                  |\n";
            cout << "|               EXPENSE TRACKER                    |\n";
            cout << "|                                                  |\n";
            cout << "|               See You Soon!                      |\n";
            cout << "+--------------------------------------------------+\n";
            break;
        }

        }
    } while (Option != enTransaction::enExit);

}

void StartProgram()
{
    programExcuation();
}



int main()
{

    StartProgram();

  

    return 0;
}


