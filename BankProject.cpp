#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>

void ShowTransactionsScreen();

using namespace std;

enum enMainMenuOptions {
    ShowClientListOption = 1, AddNewClientOption = 2, DeleteClientOption = 3,
    UpdateClientInfoOption = 4, FindClientOption = 5, TransactionsOption = 6 ,ExitOption =7
};

enum enTransactions {
    DepositOption = 1, WithdrawOption = 2, TotalBalancesOption = 3, MainMenuOption = 4
};

const string ClientsFileName = "Clients.txt";

struct stClientData {
    string AccountNumber;
    string PinCode;
    string Name;
    string phone;
    double BalanceAccount = 0;
    bool IsDeletedClient = false;
};

void ShowMainMenu();
void GoBackToMainMenu();

vector <string> SplitString(string s, string Delim) {
    int pos = 0;
    string word = "";
    vector <string> vWords;

    while ((pos = s.find(Delim)) != std::string::npos)
    {
        word = s.substr(0, pos);

        if (word != "")
        {
            vWords.push_back(word);
        }
        s.erase(0, pos + Delim.length());
    }

    if (s != "")
    {
        vWords.push_back(s);
    }

    return vWords;
}

stClientData ConvertLineToRecord(string Line, string Delim = "#//#") {
    stClientData Client;
    vector <string> vWords;
    vWords = SplitString(Line, Delim);

    if (vWords.size() >= 5) {
        Client.AccountNumber = vWords[0];
        Client.PinCode = vWords[1];
        Client.Name = vWords[2];
        Client.phone = vWords[3];
        Client.BalanceAccount = stod(vWords[4]);
    }
    return Client;
}

vector <stClientData> LoadDataFromFiletovector(string FileName) {
    vector <stClientData> vAllClients;
    string Line;
    stClientData Client;

    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        while (getline(MyFile, Line))
        {
            Client = ConvertLineToRecord(Line);
            vAllClients.push_back(Client);
        }
        MyFile.close();
    }

    return vAllClients;
}

string ConvertRecordToLine(stClientData Client, string Delim = "#//#") {
    string Line = "";
    Line += Client.AccountNumber + Delim;
    Line += Client.PinCode + Delim;
    Line += Client.Name + Delim;
    Line += Client.phone + Delim;
    Line += to_string(Client.BalanceAccount);
    return Line;
}

void PrintOneClientList(stClientData Client) {
    cout << "|" << left << setw(16) << Client.AccountNumber;
    cout << "|" << left << setw(10) << Client.PinCode;
    cout << "|" << left << setw(40) << Client.Name;
    cout << "|" << left << setw(15) << Client.phone;
    cout << "|" << left << setw(15) << Client.BalanceAccount;
}

void PrintClientList() {
    vector <stClientData> vAllClients;
    vAllClients = LoadDataFromFiletovector(ClientsFileName);

    cout << "Client List (" << vAllClients.size() << ") Client(s).\n";
    cout << "\n-------------------------------------------------------------------------------------------------------------\n";
    cout << "|" << left << setw(16) << " Account Number ";
    cout << "|" << left << setw(10) << " Pin Code ";
    cout << "|" << left << setw(40) << " Client Name ";
    cout << "|" << left << setw(15) << " Phone ";
    cout << "|" << left << setw(15) << " Balance ";
    cout << "\n-------------------------------------------------------------------------------------------------------------\n";

    for (stClientData& c : vAllClients)
    {
        PrintOneClientList(c);
        cout << "\n";
    }
    cout << "-------------------------------------------------------------------------------------------------------------\n";
}

void AddDataLineToFile(string FileName, string stDataLine) {
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);
    if (MyFile.is_open())
    {
        MyFile << stDataLine << endl;
        MyFile.close();
    }
}

bool IsClientExist(string FileName, const string& AccountNumber) {
    
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            stClientData Client = ConvertLineToRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }
        }
        MyFile.close();
    }
    return false;
}

stClientData ReadNewClient() {
    stClientData Client;
    cout << "Please enter account number ? ";
    getline(cin >> ws, Client.AccountNumber);

    while (IsClientExist(ClientsFileName, Client.AccountNumber))
    {
        cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }

    cout << "Enter Pin Code ? ";
    getline(cin, Client.PinCode);
    cout << "Enter Name ? ";
    getline(cin, Client.Name);
    cout << "Enter Phone ? ";
    getline(cin, Client.phone);
    cout << "Enter Balance ? ";
    cin >> Client.BalanceAccount;
    return Client;
}

void AddNewClient() {
    stClientData Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

void AddNewClients() {
    char AddMore = 'Y';
    do
    {
        AddNewClient();
        cout << "\nClient Added Successfully, Do you want to add another client? [Y/N]  ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}

void PrintClientCard(const stClientData& Client) {

    cout << "\nThe Following are the client details: \n";
    cout << "\n------------------------------------------------------\n";
    cout << "Account Number : " << Client.AccountNumber << "\n";
    cout << "Pin Code       : " << Client.PinCode << "\n";
    cout << "Name           : " << Client.Name << "\n";
    cout << "phone          : " << Client.phone << "\n";
    cout << "Balance        : " << Client.BalanceAccount << "\n";
    cout << "\n------------------------------------------------------\n";
}

bool FindClientByAccountNumber(vector <stClientData>& vAllClients, stClientData& Client, string AccountNumber) {
    for (stClientData& c : vAllClients)
    {
        if (c.AccountNumber == AccountNumber)
        {
            Client = c;
            return true;
        }
    }
    return false;
}

bool MarkClientToDelete(vector <stClientData>& vAllClients, string AccountNumber) {
    for (stClientData& c : vAllClients)
    {
        if (c.AccountNumber == AccountNumber)
        {
            c.IsDeletedClient = true;
            return true;
        }
    }
    return false;
}

void FillFileAfterEdit(vector <stClientData>& vAllClients, string FileName) {
    fstream MyFile;
    MyFile.open(FileName, ios::out);

    if (MyFile.is_open())
    {
        for (stClientData& c : vAllClients)
        {
            if (c.IsDeletedClient == false)
            {
                MyFile << ConvertRecordToLine(c) << endl;
            }
        }
        MyFile.close();
    }
    else
    {
        cout << "\n ERROR: Could not open file for writing! Check file path.";
    }
}

void DeleteClientByAccountNumber(vector <stClientData>& vAllClients, string AccountNumber) {
    stClientData Client;

    if (FindClientByAccountNumber(vAllClients, Client, AccountNumber) == true)
    {
        PrintClientCard(Client);

        char confrmation = 'Y';
        cout << "\nAre you sure to delete this account? [Y/N]: ";
        cin >> confrmation;

        if (toupper(confrmation) == 'Y')
        {
            MarkClientToDelete(vAllClients, AccountNumber);
            FillFileAfterEdit(vAllClients, ClientsFileName);

            vAllClients = LoadDataFromFiletovector(ClientsFileName);
            cout << "\nClient Deleted Successfully.\n";
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!\n";
    }
}

stClientData ReadClientForUpdate(string AccountNumber) {
    stClientData Client;
    Client.AccountNumber = AccountNumber;

    cout << "\nEnter Pin Code ? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name ? ";
    getline(cin, Client.Name);

    cout << "Enter Phone ? ";
    getline(cin, Client.phone);

    cout << "Enter Balance ? ";
    cin >> Client.BalanceAccount;

    return Client;
}

void UpdateClientByAccountNumber(vector <stClientData>& vAllClients, string AccountNumber) {
    stClientData Client;

    if (FindClientByAccountNumber(vAllClients, Client, AccountNumber))
    {
        PrintClientCard(Client);

        char confirmation = 'N';
        cout << "\nAre you sure you want to update this client info? [Y/N]: ";
        cin >> confirmation;

        if (toupper(confirmation) == 'Y')
        {
            for (stClientData& c : vAllClients)
            {
                if (c.AccountNumber == AccountNumber)
                {
                    c = ReadClientForUpdate(AccountNumber);
                    break;
                }
            }
            FillFileAfterEdit(vAllClients, ClientsFileName);
            cout << "\nClient Updated Successfully.";
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
    }
}


double ReadAmount(string Message) {

    double Amount;
    cout << Message;
    cin >> Amount;
    return Amount;
}

void TransactionAmountPerform(vector <stClientData>& vAllClients, double Amount, string AccountNumber) {

    char Confirmation;
    cout << "\nAre you sure you want to preform this transaction [Y/N]?  ";
    cin >> Confirmation;

    if (toupper(Confirmation) == 'Y')
    {
        for (stClientData &c: vAllClients)
        {
            if (c.AccountNumber == AccountNumber)
            {
                c.BalanceAccount += Amount;
                FillFileAfterEdit(vAllClients, ClientsFileName);
                cout << "\nTransaction performed successfully.\n";
                return;

            }
        }
    }
}

string ReadAccountNumber() {

    string AccountNumber;
    cout << "\nPlease enter account number? ";
    cin >> AccountNumber;
    return AccountNumber;
}

double SumTotalBalance(const vector <stClientData>& vAllClients) {

    double SumBalance = 0;

    for (const stClientData& c : vAllClients)
    {
        SumBalance += c.BalanceAccount;
    }

    return SumBalance;
}

void PrintTotalBalance(vector <stClientData>& vAllClients) {


    cout << "Client List (" << vAllClients.size() << ") Client(s).\n";
    cout << "\n-------------------------------------------------------------------------------------------------------------\n";
    cout << "|" << left << setw(16) << " Account Number ";
    cout << "|" << left << setw(40) << " Client Name ";
    cout << "|" << left << setw(15) << " Balance ";
    cout << "\n-------------------------------------------------------------------------------------------------------------\n";

    for (stClientData& c : vAllClients) {
        cout << "|" << left << setw(16) << c.AccountNumber;
        cout << "|" << left << setw(40) << c.Name;
        cout << "|" << left << setw(15) << c.BalanceAccount;
        cout << endl;
    }
    cout << "\n-------------------------------------------------------------------------------------------------------------\n";

    double TotalBalance = SumTotalBalance(vAllClients);
    cout << "\t\tTotal Balances are = " << TotalBalance << "\n";
}

void ShowDeleteClientScreen() {
    cout << "---------------------------------------\n";
    cout << "\tDelete Client Screen.\n";
    cout << "---------------------------------------\n";

    vector <stClientData> vAllClients = LoadDataFromFiletovector(ClientsFileName);
    string AccountNumber;
    cout << "\nEnter Account Number: ";
    cin >> AccountNumber;

    DeleteClientByAccountNumber(vAllClients, AccountNumber);
}

void ShowAddNewClientScreen() {
    cout << "---------------------------------------\n";
    cout << "\tAdd New Client Screen.\n";
    cout << "---------------------------------------\n";
    AddNewClients();
}

void ShowUpdateClientScreen() {
    cout << "---------------------------------------\n";
    cout << "\tUpdate Client Info Screen.\n";
    cout << "---------------------------------------\n";

    vector <stClientData> vAllClients = LoadDataFromFiletovector(ClientsFileName);
    string AccountNumber;
    cout << "\nEnter Account Number: ";
    cin >> AccountNumber;

    UpdateClientByAccountNumber(vAllClients, AccountNumber);
}

void ShowFindClientScreen() {

    cout << "---------------------------------------\n";
    cout << "\tFind Client Screen.\n";
    cout << "---------------------------------------\n";

    vector <stClientData> vAllClients = LoadDataFromFiletovector(ClientsFileName);
    stClientData Client;
    string AccountNumber;
    cout << "\nEnter Account Number: ";
    cin >> AccountNumber;

    if (FindClientByAccountNumber(vAllClients, Client, AccountNumber)) {
        PrintClientCard(Client);
    }
    else {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
    }
}

void ShowExitScreen() {
    cout << "---------------------------------------\n";
    cout << "\tProgram Ended. Goodbye! :-)\n";
    cout << "---------------------------------------\n";
}

void GoBackToMainMenu() {
    cout << "\n\nPress any key to go back to Main Menu...";
    system("pause>0");
    ShowMainMenu();
}

void ShowDepositScreen() {

    stClientData Client;
    vector <stClientData> vAllClients;

    cout << "---------------------------------------\n";
    cout << "\tDeposit Screen.\n";
    cout << "---------------------------------------\n";

    vAllClients = LoadDataFromFiletovector(ClientsFileName);

    string AccountNumber = ReadAccountNumber();

    while (!FindClientByAccountNumber(vAllClients, Client, AccountNumber))
    {
        cout << "\nClient with [" << AccountNumber << "] Doesn't exsist.\n";
        AccountNumber = ReadAccountNumber();
    }

    system("cls");
    PrintClientCard(Client);

    double DepositAmount = ReadAmount("\nPlease enter Deposit amount? ");

    TransactionAmountPerform(vAllClients, DepositAmount, AccountNumber);


}

void ShowWithdrawScreen() {

    stClientData Client;
    vector <stClientData> vAllClients;

    cout << "---------------------------------------\n";
    cout << "\tWithdraw Screen.\n";
    cout << "---------------------------------------\n";

    vAllClients = LoadDataFromFiletovector(ClientsFileName);

    string AccountNumber = ReadAccountNumber();
    
    while (!FindClientByAccountNumber(vAllClients, Client, AccountNumber))
    {
        cout << "\nClient with [" << AccountNumber << "] Doesn't exsist.\n";
        AccountNumber = ReadAccountNumber();
    }

    system("cls");
    PrintClientCard(Client);

    double WithdrawAmount = ReadAmount("\nPlease enter Withdraw amount? ");

    while (WithdrawAmount > Client.BalanceAccount)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw up to: " << Client.BalanceAccount << endl;
        WithdrawAmount = ReadAmount("\nPlease enter Withdraw amount? ");
    }

    TransactionAmountPerform(vAllClients, WithdrawAmount* - 1, AccountNumber);

}

void ShowTotalBalancesScreen() {

    vector <stClientData> vAllClients;

    vAllClients = LoadDataFromFiletovector(ClientsFileName);

    PrintTotalBalance(vAllClients);
}

void GoBackToTransactionsMenu() {
    cout << "\n\nPress any key to go back to Transaction Menu...";
    system("pause>0");
    ShowTransactionsScreen();
}

void PerformTransactionsMenuOption(enTransactions choice) {

  switch (choice)
  {

  case DepositOption:
      system("cls");
      ShowDepositScreen();
      GoBackToTransactionsMenu();
      break;

  case WithdrawOption:
      system("cls");
      ShowWithdrawScreen();
      GoBackToTransactionsMenu();
      break;

  case TotalBalancesOption:
      system("cls");
      ShowTotalBalancesScreen();
      GoBackToTransactionsMenu();
      break;

  case MainMenuOption:
      ShowMainMenu();
      return;
  }
  
}

void ShowTransactionsScreen() {

    system("cls");
    cout << "=======================================\n";
    cout << "\tTransactions Menu Screen\n";
    cout << "=======================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] withdraw.\n";
    cout << "\t[3] Total Balnces.\n";
    cout << "\t[4] Main Menu.\n";
    cout << "=======================================\n";

    short Choice = 0;
    cout << "Choose what do you want to do? [1 to 4]: ";
    cin >> Choice;

    while (Choice < 1 || Choice > 4) {
        cout << "Invalid Choice, please enter a number from 1 to 4: ";
        cin >> Choice;
    }

    PerformTransactionsMenuOption((enTransactions)Choice);
}

void PerformMainMenuOption(enMainMenuOptions Option) {
    system("cls");
    switch (Option)
    {
    case ShowClientListOption:
        PrintClientList();
        GoBackToMainMenu();
        break;

    case AddNewClientOption:
        ShowAddNewClientScreen();
        GoBackToMainMenu();
        break;

    case DeleteClientOption:
        ShowDeleteClientScreen();
        GoBackToMainMenu();
        break;

    case UpdateClientInfoOption:
        ShowUpdateClientScreen();
        GoBackToMainMenu();
        break;

    case FindClientOption:
        ShowFindClientScreen();
        GoBackToMainMenu();
        break;

    case TransactionsOption:
        ShowTransactionsScreen();
        break;

    case ExitOption:
        ShowExitScreen();
        break;
    }
}

void ShowMainMenu() {
    system("cls");
    cout << "=======================================\n";
    cout << "\tMain Menu Screen\n";
    cout << "=======================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Exit.\n";
    cout << "=======================================\n";

    short Choice = 0;
    cout << "Choose what do you want to do? [1 to 7]: ";
    cin >> Choice;

    while (Choice < 1 || Choice > 7) {
        cout << "Invalid Choice, please enter a number from 1 to 7: ";
        cin >> Choice;
    }

    PerformMainMenuOption((enMainMenuOptions)Choice);
}

int main() {

    ShowMainMenu();

    return 0;
}