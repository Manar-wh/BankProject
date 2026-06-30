#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>
#include <cctype>

void ShowTransactionsScreen();

using namespace std;

enum enMainMenuOptions {
    ShowClientListOption = 1, AddNewClientOption = 2, DeleteClientOption = 3,
    UpdateClientInfoOption = 4, FindClientOption = 5, TransactionsOption = 6, ManageUsersOption = 7, LogoutOption = 8
};

enum enTransactions {
    DepositOption = 1, WithdrawOption = 2, TotalBalancesOption = 3, MainMenuOption = 4
};

enum enManageUserOptions {
    ShowUserListOption = 1, AddNewUserOption = 2, DeleteUserOption = 3,
    UpdateUserInfoOption = 4, FindUserOption = 5, eMainMenuOption = 6
};

enum enPermissionsOptions
{
    perClientList = 1, perAddNewClient = 2, perDeletClient = 4, perUpdateClient = 8,
    perFindClient = 16, perTransactions = 32, perManageUsers = 64
};

const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";

struct stClientData {
    string AccountNumber;
    string PinCode;
    string Name;
    string phone;
    double BalanceAccount = 0;
    bool IsDeletedClient = false;
};

struct stUser {
    string Username;
    string Password;
    int Permissions = 0;
    bool IsDeletedUser = false;
};

stUser CurrentUser;

void ShowMainMenu();
void LoginScreen();
void ShowManageUserScreen();
void AccessDeniedMessage();
bool CheckAccessPermission(enPermissionsOptions Permission);

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

    if (!CheckAccessPermission(enPermissionsOptions::perClientList))
    {
        AccessDeniedMessage();
        return;
    }
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
    getline(cin >> ws, Client.PinCode);
    cout << "Enter Name ? ";
    getline(cin >> ws, Client.Name);
    cout << "Enter Phone ? ";
    getline(cin >> ws, Client.phone);
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
        for (stClientData& c : vAllClients)
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

    if (!CheckAccessPermission(enPermissionsOptions::perDeletClient))
    {
        AccessDeniedMessage();
        return;
    }
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

    if (!CheckAccessPermission(enPermissionsOptions::perAddNewClient))
    {
        AccessDeniedMessage();
        return;
    }
    cout << "---------------------------------------\n";
    cout << "\tAdd New Client Screen.\n";
    cout << "---------------------------------------\n";
    AddNewClients();
}

void ShowUpdateClientScreen() {
    if (!CheckAccessPermission(enPermissionsOptions::perUpdateClient))
    {
        AccessDeniedMessage();
        return;
    }
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

    if (!CheckAccessPermission(enPermissionsOptions::perFindClient))
    {
        AccessDeniedMessage();
        return;
    }
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

    TransactionAmountPerform(vAllClients, WithdrawAmount * -1, AccountNumber);

}

void ShowTotalBalancesScreen() {

    vector <stClientData> vAllClients;

    vAllClients = LoadDataFromFiletovector(ClientsFileName);

    PrintTotalBalance(vAllClients);
}

stUser ConvertUserLineToRecord(string Line, string Delim = "#//#") {
    stUser User;
    vector <string> vWords;
    vWords = SplitString(Line, Delim);

    if (vWords.size() >= 3) {
        User.Username = vWords[0];
        User.Password = vWords[1];
        User.Permissions = stoi(vWords[2]);
    }
    return User;
}

vector <stUser> LoadUsersDataFromFiletovector(string FileName) {
    vector <stUser> vAllUsers;
    string Line;
    stUser User;

    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        while (getline(MyFile, Line))
        {
            User = ConvertUserLineToRecord(Line);
            vAllUsers.push_back(User);
        }
        MyFile.close();
    }
    return vAllUsers;
}

bool FindUserByUsernameAndPassword(const string& Username, const string& Password, stUser& User) {

    vector <stUser> vAllUsers = LoadUsersDataFromFiletovector(UsersFileName);

    for (stUser& u : vAllUsers)
    {
        if (u.Username == Username && u.Password == Password)
        {
           User = u;
           return true;
        }
    }
    return false;
}

bool FindUserByUsername(vector <stUser>& vAllUsers, const string& Username, stUser& User) {

    for (stUser& u : vAllUsers)
    {
        if (u.Username == Username)
        {
            User = u;
            return true;
        }
    }
    return false;
}

bool LoadUserInfo(const string& Username, const string& Password) {

    if (!FindUserByUsernameAndPassword(Username, Password, CurrentUser))
    {
        return false;
    }
    return true;
}

string ConvertUserRecordToLine(stUser User, string Delim = "#//#") {
    string Line = "";
    Line += User.Username + Delim;
    Line += User.Password + Delim;
    Line += to_string(User.Permissions);
    return Line;
}

void AddUserDataLineToFile(string FileName, string stDataLine) {
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);
    if (MyFile.is_open())
    {
        MyFile << stDataLine << endl;
        MyFile.close();
    }
}

void PrintOneUserList(stUser User) {
    cout << "|" << left << setw(12) << User.Username;
    cout << "|" << left << setw(15) << User.Password;
    cout << "|" << left << setw(12) << User.Permissions;
}

void PrintUsersList() {

    vector <stUser> vAllUsers;
    vAllUsers = LoadUsersDataFromFiletovector(UsersFileName);

    cout << "Client List (" << vAllUsers.size() << ") Client(s).\n";
    cout << "\n-------------------------------------------------------------------------------------------------------------\n";
    cout << "|" << left << setw(12) << " Username ";
    cout << "|" << left << setw(15) << " Password ";
    cout << "|" << left << setw(12) << " Permission ";
    cout << "\n-------------------------------------------------------------------------------------------------------------\n";

    for (stUser& u : vAllUsers)
    {
        PrintOneUserList(u);
        cout << "\n";
    }
    cout << "-------------------------------------------------------------------------------------------------------------\n";
}

int ReadPermissionSet() {
    int Permissions = 0;
    char Answer = 'Y';

    cout << "\nDo you want to give full access? Y/N: ";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {
        return - 1;
    }

    cout << "\nDo you want to give access to: ";

    cout << "\nShow Client List? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {
        Permissions += enPermissionsOptions::perClientList;
    }

    cout << "\nAdd New Client? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {
        Permissions += enPermissionsOptions::perAddNewClient;
    }

    cout << "\nDelete Client? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {
        Permissions += enPermissionsOptions::perDeletClient;
    }

    cout << "\nUpdate Client info? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {
        Permissions += enPermissionsOptions::perUpdateClient;
    }

    cout << "\nFind Client? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {
        Permissions += enPermissionsOptions::perFindClient;
    }

    cout << "\nTransactions? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {
        Permissions += enPermissionsOptions::perTransactions;
    }

    cout << "\nManage Users? ";
    cin >> Answer;
    if (toupper(Answer) == 'Y')
    {
        Permissions += enPermissionsOptions::perManageUsers;
    }
    return Permissions;
}

bool CheckAccessPermission(enPermissionsOptions Permission) {

    if (CurrentUser.Permissions == -1)
    {
        return true;
    }

    if ((CurrentUser.Permissions & Permission) == Permission)
    {
        return true;
    }
    return false;
}

void AccessDeniedMessage() {

    cout << "\n------------------------------------------\n";
    cout << "Access Denied!\n";
    cout << "You don't have permission to do this.\n";
    cout << "Please contact your admin.";
    cout << "\n------------------------------------------\n";

}

bool IsUserExist(string FileName , const string& Username) {

    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            stUser User = ConvertUserLineToRecord(Line);
            if (User.Username == Username)
            {
                MyFile.close();
                return true;
            }
        }
        MyFile.close();
    }
    return false;
}

stUser ReadNewUser() {
    stUser User;

    string Username;
    cout << "Please enter Username ? ";
    cin >> User.Username;

    while (IsUserExist(UsersFileName, User.Username))
    {
        cout << "\nUser with [" << User.Username << "] already exists, Enter another Username? ";
        cin >> User.Username;
    }

    cout << "Enter Password ? ";
    cin >> User.Password;

    User.Permissions = ReadPermissionSet();

    return User;
}

void AddNewUser() {
    stUser User;
    User = ReadNewUser();
    AddUserDataLineToFile(UsersFileName, ConvertUserRecordToLine(User));
}

void AddNewUsers() {
    char AddMore = 'Y';
    do
    {
        AddNewUser();
        cout << "\nUsers Added Successfully, Do you want to add another User? [Y/N]  ";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}

void ShowAddNewUserScreen() {

    cout << "---------------------------------------\n";
    cout << "\tAdd New User Screen.\n";
    cout << "---------------------------------------\n";
    AddNewUsers();
}

void PrintUserCard(const stUser& User) {

    cout << "\nThe Following are the user details: \n";
    cout << "\n------------------------------------------------------\n";
    cout << "Username     : " << User.Username << "\n";
    cout << "Password     : " << User.Password << "\n";
    cout << "Permissions  : " << User.Permissions << "\n";
    cout << "\n------------------------------------------------------\n";
}

bool MarkUserToDelete(vector <stUser>& vAllUsers, string Username) {
    for (stUser& u : vAllUsers)
    {
        if (u.Username == Username)
        {
            u.IsDeletedUser = true;
            return true;
        }
    }
    return false;
}

void FillUserFileAfterEdit(vector <stUser>& vAllUsers, string FileName) {
    fstream MyFile;
    MyFile.open(FileName, ios::out);

    if (MyFile.is_open())
    {
        for (stUser& u : vAllUsers)
        {
            if (u.IsDeletedUser == false)
            {
                MyFile << ConvertUserRecordToLine(u) << endl;
            }
        }
        MyFile.close();
    }
    
}

void DeleteUserByUsername(string Username) {
    stUser User;
    vector <stUser> vAllUsers = LoadUsersDataFromFiletovector(UsersFileName);
    if (FindUserByUsername(vAllUsers,Username, User))
    {
        PrintUserCard(User);

        char confrmation = 'Y';
        cout << "\nAre you sure to delete this User? [Y/N]: ";
        cin >> confrmation;

        if (toupper(confrmation) == 'Y')
        {
            MarkUserToDelete(vAllUsers ,Username);
            FillUserFileAfterEdit(vAllUsers, UsersFileName);

            vAllUsers = LoadUsersDataFromFiletovector(UsersFileName);
            cout << "\nUser Deleted Successfully.\n";
        }
    }
    else
    {
        cout << "\nUser with Username (" << Username << ") is Not Found!\n";
    }
}

void ShowDeleteUserScreen() {

    cout << "---------------------------------------\n";
    cout << "\tDelete User Screen.\n";
    cout << "---------------------------------------\n";

    string Username;
    cout << "\nEnter Username: ";
    cin >> Username;

    DeleteUserByUsername(Username);
}

stUser ReadUserForUpdate(string Username) {
    stUser User;
    User.Username = Username;

    cout << "\nEnter Password ? ";
    cin >> User.Password;
    User.Permissions = ReadPermissionSet();
    return User;
}

void UpdateUserByUsername(vector <stUser>& vAllUsers, string Username) {
    stUser User;
    if (FindUserByUsername(vAllUsers, Username, User))
    {
        PrintUserCard(User);

        char confirmation = 'N';
        cout << "\nAre you sure you want to update this user info? [Y/N]: ";
        cin >> confirmation;

        if (toupper(confirmation) == 'Y')
        {
            for (stUser& u : vAllUsers)
            {
                if (u.Username == Username)
                {
                    u = ReadUserForUpdate(Username);
                    break;
                }
            }
            FillUserFileAfterEdit(vAllUsers, UsersFileName);
            cout << "\nUser Updated Successfully.";
        }
    }
    else
    {
        cout << "\nUser with Username (" << Username << ") is Not Found!";
    }
}

void ShowUpdateUserScreen() {

    cout << "---------------------------------------\n";
    cout << "\tUpdate User Info Screen.\n";
    cout << "---------------------------------------\n";

    vector <stUser> vAllUsers = LoadUsersDataFromFiletovector(UsersFileName);
    string Username;
    cout << "\nEnter Username: ";
    cin >> Username;

    UpdateUserByUsername(vAllUsers, Username);
}

void ShowFindUserScreen() {

    cout << "---------------------------------------\n";
    cout << "\tFind User Screen.\n";
    cout << "---------------------------------------\n";

    vector <stUser> vAllUsers = LoadUsersDataFromFiletovector(UsersFileName);
    stUser User;
    string Username;
    cout << "\nEnter Username: ";
    cin >> Username;

    if (FindUserByUsername(vAllUsers, Username, User)) {
        PrintUserCard(User);
    }
    else {
        cout << "\nUser with Username (" << Username << ") is Not Found!";
    }
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

    if (!CheckAccessPermission(enPermissionsOptions::perTransactions))
    {
        AccessDeniedMessage();
        return;
    }

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

void GoBackToManageUserMenu() {
    cout << "\n\nPress any key to go back to Manage User Menu...";
    system("pause>0");
    ShowManageUserScreen();
}

void PerformManageUserMenuOption(enManageUserOptions choice) {

    switch (choice)
    {

    case enManageUserOptions::ShowUserListOption:
        system("cls");
        PrintUsersList();
        GoBackToManageUserMenu();
        break;

    case enManageUserOptions::AddNewUserOption:
        system("cls");
        ShowAddNewUserScreen();
        GoBackToManageUserMenu();
        break;

    case enManageUserOptions::DeleteUserOption:
        system("cls");
        ShowDeleteUserScreen();
        GoBackToManageUserMenu();
        break;

    case enManageUserOptions::UpdateUserInfoOption:
        system("cls");
        ShowUpdateUserScreen();
        GoBackToManageUserMenu();
        break;

    case enManageUserOptions::FindUserOption:
        system("cls");
        ShowFindUserScreen();
        GoBackToManageUserMenu();
        break;

    case enManageUserOptions::eMainMenuOption:
        ShowMainMenu();
        return;
    }
}

void ShowManageUserScreen() {

    if (!CheckAccessPermission(enPermissionsOptions::perManageUsers))
    {
        AccessDeniedMessage();
        return;
    }

    system("cls");
    cout << "=======================================\n";
    cout << "\tManage User Menu Screen\n";
    cout << "=======================================\n";
    cout << "\t[1] List User.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User Info.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menu.\n";
    cout << "=======================================\n";

    short Choice = 0;
    cout << "Choose what do you want to do? [1 to 6]: ";
    cin >> Choice;

    while (Choice < 1 || Choice > 6) {
        cout << "Invalid Choice, please enter a number from 1 to 6: ";
        cin >> Choice;
    }

    PerformManageUserMenuOption((enManageUserOptions)Choice);
}

void PerformMainMenuOption(enMainMenuOptions Option) {
    system("cls");
    switch (Option)
    {
    case enMainMenuOptions::ShowClientListOption:
        PrintClientList();
        GoBackToMainMenu();
        break;

    case enMainMenuOptions::AddNewClientOption:
        ShowAddNewClientScreen();
        GoBackToMainMenu();
        break;

    case enMainMenuOptions::DeleteClientOption:
        ShowDeleteClientScreen();
        GoBackToMainMenu();
        break;

    case enMainMenuOptions::UpdateClientInfoOption:
        ShowUpdateClientScreen();
        GoBackToMainMenu();
        break;

    case enMainMenuOptions::FindClientOption:
        ShowFindClientScreen();
        GoBackToMainMenu();
        break;

    case enMainMenuOptions::TransactionsOption:
        ShowTransactionsScreen();
        GoBackToMainMenu();
        break;

    case enMainMenuOptions::ManageUsersOption:
        ShowManageUserScreen();
        GoBackToMainMenu();
        break;

    case enMainMenuOptions::LogoutOption:
        LoginScreen();
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
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Logout.\n";
    cout << "=======================================\n";

    short Choice = 0;
    cout << "Choose what do you want to do? [1 to 8]: ";
    cin >> Choice;

    while (Choice < 1 || Choice > 8) {
        cout << "Invalid Choice, please enter a number from 1 to 8: ";
        cin >> Choice;
    }

    PerformMainMenuOption((enMainMenuOptions)Choice);
}

void LoginScreen() {

    string Username, Password;
    bool LoginFailed = false;
    do
    {
        system("cls");
        cout << "=======================================\n";
        cout << "\tLogin Screen\n";
        cout << "=======================================\n";

        if (LoginFailed)
        {
            cout << "\nInvalid Username/Password. ";
        }

        cout << "\nPlease enter your username: ";
        cin >> Username;

        cout << "\nPlease enter your password: ";
        cin >> Password;

        if (LoadUserInfo(Username, Password))
        {
            LoginFailed = false;
        } 
        else {
            LoginFailed = true;
        }

    } while (LoginFailed);
   
    ShowMainMenu();
}

int main() {

    LoginScreen();

    return 0;
}