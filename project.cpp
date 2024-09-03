#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cctype>
#include <vector>
#include <iomanip>
#include <thread>
#include <chrono>
using namespace std;
const string ClientsFileName = "clients.txt";
void Main();
void TransactionMenu();
enum EnChoicee
{
  showlist = 1,
  addnew = 2,
  deleteclient = 3,
  updateclient = 4,
  findclient = 5,
  Transaction = 6,
  Exit = 7
};
enum transaction_menu
{
  depoist = 1,
  withdraw = 2,
  total_balance = 3,
  menu = 4
};
struct sClient
{
  string AccountNumber;
  string PinCode;
  string Name;
  string Phone;
  double AccountBalance;
};

void WriteClientData_To_file(const string &filename, const string &line)
{
  fstream myfile(filename, ios::app);
  if (myfile.is_open())
  {
    if (!line.empty())
    {
      myfile << line << endl;
    }
    myfile.close();
  }
  else
  {
    cout << "Cannot open the file: " << filename << endl;
  }
}

vector<string> SplitString(const string &line, const string &delimiter)
{
  vector<string> vstring;
  string word;
  short pos = 0;
  string str = line;
  while ((pos = str.find(delimiter)) != string::npos)
  {
    word = str.substr(0, pos);
    if (!word.empty())
    {
      vstring.push_back(word);
    }
    str.erase(0, pos + delimiter.length());
  }
  if (!str.empty())
  {
    vstring.push_back(str);
  }
  return vstring;
}

sClient ConvertLineToRecord(const string &line, const string &separator = "#//#")
{
  sClient client;
  vector<string> split = SplitString(line, separator);
  if (split.size() == 5) // Ensure the correct number of fields
  {
    client.AccountNumber = split[0];
    client.PinCode = split[1];
    client.Name = split[2];
    client.Phone = split[3];
    client.AccountBalance = stod(split[4]);
  }
  return client;
}
string ConvertRecordToLine(const sClient &clientdata, const string &delimiter = "#//#")
{
  return clientdata.AccountNumber + delimiter +
         clientdata.PinCode + delimiter +
         clientdata.Name + delimiter +
         clientdata.Phone + delimiter +
         to_string(clientdata.AccountBalance);
}
// لحد دلوقتي دي حاجات طبيعيه والاتنين الي فوق دول عكس بعض بحول السطر لسجلات والسجلات لسطور
vector<sClient> LoadDataFromFile(const string &filename)
{
  vector<sClient> vsclient;
  fstream myfile(filename, ios::in);
  // OR   اي حاجه فيهم عادي المهم انا بقرا البيانات من الفايل بس ممكن استخدم الي تحت علي طول وخلاص بدل ما احدد
  //  ifstream myfile(filename);
  if (myfile.is_open())
  {
    string line;
    while (getline(myfile, line)) // طالما عمال اقرأ
    {
      sClient client = ConvertLineToRecord(line);
      // الفانكشن دي بتاخد السطر الي في الفايل الي بعد كل جزء فيه شفرات وتحوله الي ريكورد او سجلات مرتبه في ستراكتشر
      //  كمان الفانكشن جواها كمان فانكشن علشان تفصل الشفرات وقبل كل شفره او سبيراتور تضيفها لفكتور سترنج ثم ترجع تعمل وظيفتها
      // ووظيفتها هيا انها تعبي الفيكتور الاسترنج دا في ستراكتشر منظم يبقا الفانكشن دي بترجعلي ايه ؟
      // بالظبط بترجعلي ستراكتشر فيه كل حاجه عن كل عميل الي هوا كان في الفايل في سطر واحد
      // باجي انا هنا بقا احط كل ستركتشر منظم في فيكتور من نوع ستركتشر بيبقا فيه كل العملاء او الكلاينت بقا وبرجعه تاني من الفانكشن دي
      vsclient.push_back(client);
    }
    myfile.close();
  }
  else
  {
    cout << "Cannot open the file: " << filename << endl;
  }
  return vsclient;
}

void PrintClientData(const sClient &client)
{
  cout << "| " << setw(15) << left << client.AccountNumber
       << "| " << setw(10) << left << client.PinCode
       << "| " << setw(40) << left << client.Name
       << "| " << setw(12) << left << client.Phone
       << "| " << setw(12) << left << client.AccountBalance;
}

// دي تبع transaction فقط
void PrintClientDataFromTransaction(const sClient &client)
{
  cout << "|" << setw(30) << left << client.AccountNumber
       << "|" << setw(40) << left << client.Name
       << "|" << setw(30) << left << client.AccountBalance;
}

void PrintClientDataForOneClient(const sClient &client)
{
  cout << "Account Number: " << client.AccountNumber << endl;
  cout << "Pin Code: " << client.PinCode << endl;
  cout << "Account Name: " << client.Name << endl;
  cout << "Phone: " << client.Phone << endl;
  cout << "Account Balance: " << client.AccountBalance << endl;
  cout << string(120, '_') << endl;
}

void PrintAllClientsData(const vector<sClient> &vClients)
{
  cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).\n";
  cout << "____________________________________________________________________________________________________\n";
  cout << "| " << left << setw(15) << "Account Number"
       << "| " << left << setw(10) << "Pin Code"
       << "| " << left << setw(40) << "Client Name"
       << "| " << left << setw(12) << "Phone"
       << "| " << left << setw(12) << "Balance" << "|\n";
  cout << "_______________________________________________________\n";

  for (const sClient &client : vClients)
  {
    PrintClientData(client);
    cout << endl;
  }

  cout << "_______________________________________________________\n";
}

bool SearchForClient(string accountNumber, vector<sClient> &vClients, sClient &client)
{
  for (const sClient &c : vClients)
  {
    if (accountNumber == c.AccountNumber) // لو موجود
    {
      client = c; // دا الاستكرتشر بتاع الكلاينت
      return true;
    }
  }
  return false;
}
void FindClients(vector<sClient> &vClients)
{
  string accnumber;
  sClient client;
  cout << "Please Enter Account Number?" << endl;
  getline(cin >> ws, accnumber);
  if (SearchForClient(accnumber, vClients, client))
  {
    PrintClientDataForOneClient(client);
  }
  else
  {
    cout << "client With Account Number " << accnumber << "Not Exist" << endl;
  }
}
void Save_New_Client_Data_To_File_After_Refresh_(const string &filename, const vector<sClient> &vClients, const string &accountNumber = "")
{
  ofstream myfile(filename, ios::out);
  if (myfile.is_open())
  {
    for (const sClient &c : vClients)
    {
      if (c.AccountNumber != accountNumber)
      {
        myfile << ConvertRecordToLine(c) << endl;
      }
    }
    myfile.close();
  }
  else
  {
    cout << "Cannot open the file: " << filename << endl;
  }
}

string ReadAccountNumber(const vector<sClient> &allData)
{
  string accNumber;
  bool valid = false;
  while (!valid)
  // while false to continue the loop
  {
    cout << "Enter Account Number: ";
    cin >> accNumber;
    cin.ignore(); // Clear input buffer
    valid = true;
    for (const sClient &client : allData)
    {
      if (client.AccountNumber == accNumber)
      {
        cout << "This Client Already Exists. Please Enter a New Account Number." << endl;
        valid = false;
        break;
      }
    }
    // لو مدخلش الحاله الشرطيه الشرط هيفضل ترو ومش هيعيد الادخال
  }
  return accNumber;
}

sClient ReadInfoForNewClient(const vector<sClient> &allData)
{
  sClient client;
  client.AccountNumber = ReadAccountNumber(allData);
  cout << "Enter Pin Code: ";
  getline(cin, client.PinCode);
  cout << "Enter Account Name: ";
  getline(cin, client.Name);
  cout << "Enter Phone: ";
  getline(cin, client.Phone);
  cout << "Enter Account Balance: ";
  cin >> client.AccountBalance;
  return client;
}

void Add_New_Client(vector<sClient> &allData)
{
  char choice;
  do
  {
    sClient clientData = ReadInfoForNewClient(allData);
    // كدا معايا استركتشر فيه كل البيانات بتاعت الكلاينت الجديد اروح بقا احطها في الفايل
    WriteClientData_To_file(ClientsFileName, ConvertRecordToLine(clientData)); // الفانكشن دي يعتبر بتاخد استرنج مشفر بين كل حاجه
    // مميزه وتحطه في الفايل بتاعي
    // كدا حطيت الداتا في الفايل ومتنساش تقفله علشان تسمع علي طول
    cout << "Data Added Successfully" << endl;
    cout << "Do You Want To Add More Clients? [y/n]: ";
    cin >> choice;
    cin.ignore(); // Clear input buffer
  } while (toupper(choice) == 'Y');
  // هيخرج من اللوب لو المستخدم مش هيضيف كلاينت تاني
}

void DeleteClientData(vector<sClient> &vClients)
{
  string accNumber;
  sClient clientData;
  char choice;
  cout << "Please Enter Account Number: ";
  cin >> accNumber;
  cin.ignore();                                         // Clear input buffer
  if (SearchForClient(accNumber, vClients, clientData)) // لو الاكونت نمبر موجود اصلا ..؟
  {
    PrintClientDataForOneClient(clientData);               // بطبعله الداتا بتاعت الكلاينت دا الي رجعتلي من الفانكشن الي فوق
    cout << "Are You Sure To Delete This Client? [y/n]: "; // بخليه يختار تاني يمكن يتراجع D:)
    cin >> choice;
    cin.ignore(); // Clear input buffer
    if (toupper(choice) == 'Y')
    {
      Save_New_Client_Data_To_File_After_Refresh_(ClientsFileName, vClients, accNumber); // الفانكشن دا عباره عن انه
      // بيحفظ كل البيانات تاني للفايل عدا الداتا الي الاكونت نمبر بتاعها دا
      cout << "Client Deleted Successfully" << endl;
    }
    else
    {
      cout << "Data Remains The Same" << endl;
    }
  }
  else
  {
    cout << "Account with Account Number " << accNumber << " Does Not Exist" << endl;
  }
}

sClient UpdateData()
{
  // طبعا الاكونت نمبر دا برايمري كاي مينفعش يتغير
  sClient data;
  cout << "Enter Pin Code: ";
  getline(cin >> ws, data.PinCode);
  cout << "Enter Name: ";
  getline(cin, data.Name);
  cout << "Enter Phone: ";
  getline(cin, data.Phone);
  cout << "Enter Account Balance: ";
  cin >> data.AccountBalance;
  return data;
}

void updateDataForClient(vector<sClient> &vClients)
{
  string accNumber;
  sClient clientData;
  sClient updateData;
  char choice;
  cout << "Please Enter Account Number: ";
  cin >> accNumber;
  cin.ignore();                                         // Clear input buffer
  if (SearchForClient(accNumber, vClients, clientData)) // لو موجود اصلا
  {
    cout << "The Following Are The Client's Details:" << endl;
    PrintClientDataForOneClient(clientData);
    cout << "Are You Sure To Update Your Data? [y/n]: ";
    cin >> choice;
    cin.ignore(); // Clear input buffer
    if (toupper(choice) == 'Y')
    {
      updateData = UpdateData();
      // بخليه يضيف البيانات عد الاكونت نمبر طبعا لانه مميز لكل عميل
      for (sClient &client : vClients)
      {
        if (client.AccountNumber == accNumber)
        {
          client.PinCode = updateData.PinCode;
          client.Name = updateData.Name;
          client.Phone = updateData.Phone;
          client.AccountBalance = updateData.AccountBalance;
          break; // مش لازم اكمل بعدها خلاص لقيته
        }
      }
      Save_New_Client_Data_To_File_After_Refresh_(ClientsFileName, vClients);
      cout << "Data Updated Successfully" << endl;
    }
    else
    {
      cout << "Data Remains The Same" << endl;
    }
  }
  else
  {
    cout << "Client with Account Number " << accNumber << " Not Found" << endl;
  }
}
double ReadDepoist()
{
  double depoist;
  do
  {
    cout << "Please Enter Depoist Amount?" << endl;
    cin >> depoist;
    cin.ignore();
  } while (depoist <= 0);

  return depoist;
}
double ReadWithDramAmount() // هترجعلي الامونت من المستحدم
{
  double Withdraw;
  do
  {
    cout << "Please Enter Withdraw Amount?" << endl;
    cin >> Withdraw;
    cin.ignore();
  } while (Withdraw <= 0);
  return Withdraw;
}
void AddedDepoistAmount(string accnumber, vector<sClient> &vClients, double &depoistAmount, double &NewBalance)
{
  for (sClient &client : vClients)
  {
    if (client.AccountNumber == accnumber) // هبحث الاول عن رقم العميل
    {
      NewBalance = client.AccountBalance += depoistAmount; // علشان اعرف ارجع البلانس الجديد وفي نفس الوقت اكون ضفته للفيكتور
      break;                                               // مش لازم اكمل اللوب
    }
  }
}
void WithDrawFromClient(string accnumber, vector<sClient> &vClients, double &withdraw, double &NewBalance)
{
  for (sClient &client : vClients)
  {
    if (client.AccountNumber == accnumber) // هبحث الاول عن رقم العميل
    {
      NewBalance = client.AccountBalance -= withdraw; // علشان اعرف ارجع البلانس الجديد وفي نفس الوقت اكون ضفته للفيكتور
      break;                                          // مش لازم اكمل اللوب
    }
  }
}

void DepoistProcess(vector<sClient> &vClients)
{
  string accnumber;
  char choice;
  double NewBalance = 0.0;
  double depoist;
  sClient clientData;
  cout << "Please Enter your Account Number" << endl;
  getline(cin, accnumber);                                    // الي هتجيلي متعبيه من الفانكشن
  while (!(SearchForClient(accnumber, vClients, clientData))) // لو موجود
  {
    cout << "client with  Account Number " << accnumber << " Not Exist." << endl;
    cout << "Please Enter your Account Number" << endl;
    getline(cin, accnumber);
  }
  {
    PrintClientDataForOneClient(clientData);
    depoist = ReadDepoist();
    cout << "Do You Want To Depoist Amount " << depoist << " [y/n]?" << endl;
    cin >> choice;
    if (toupper(choice) == 'Y')
    {
      AddedDepoistAmount(accnumber, vClients, depoist, NewBalance);
      Save_New_Client_Data_To_File_After_Refresh_(ClientsFileName, vClients); // حفظ الفايل بقا علشان نعرف نطبع الجديد
      cout << "Done Susessfult Client Balance = " << NewBalance << endl;
      cout << "Press Any Key To Go To Menu" << endl;
      system("pause>0");
      system("cls");
      return TransactionMenu();
    }
    else
    {
      cout << "Amount Not Addeed:-)" << endl;
      cout << "Press Any Key To Go To Menu..." << endl;
      system("pause>0");
      cout << endl;
      system("cls");
      return TransactionMenu();
    }
  }
}
void WithdrawinAccoount(string accnumber, vector<sClient> &alldata, double &accbalance)
{
  for (sClient &client : alldata)
  {
    if (client.AccountNumber == accnumber)
    {
      accbalance = client.AccountBalance;
    }
    break;
  }
}
void withdrawProcess(vector<sClient> &vClients)
{
  string accnumber;
  char choice;
  double withdraw;
  double account_balance = 0.0;
  double New_Balance = 0.0;
  sClient clientData;
  cout << "Please Enter your Account Number" << endl;
  getline(cin, accnumber);                                    // الي هتجيلي متعبيه من الفانكشن
  while (!(SearchForClient(accnumber, vClients, clientData))) // لو مش  موجود
  {
    cout << "client with Account Number " << accnumber << " Not Exist." << endl;
    cout << "Please Enter your Account Number" << endl;
    getline(cin, accnumber);
  }
  PrintClientDataForOneClient(clientData);
  withdraw = ReadWithDramAmount();
  WithdrawinAccoount(accnumber, vClients, account_balance); // هيجبلي القيمه الي في حسابي
  bool flage = true;
  while (flage)
  {
    if (account_balance - withdraw >= 0) // لو  هينفع يسحب
    {
      flage = false;
      break;
    }
    else
    {
      cout << "Amount Excced The Balance ,you Can with Draw Up To " << account_balance << endl;
      withdraw = ReadWithDramAmount(); // هعيد عمليه الطلب تاني
    }
  }
  // مش عايزه يخرج من اللوب الا لو دخل قيمه ينفع تتسحب
  cout << "Do You Want To Withdraw Amount " << withdraw << " [y/n]?" << endl;
  cin >> choice;
  if (toupper(choice) == 'Y')
  {
    WithDrawFromClient(accnumber, vClients, withdraw, New_Balance);         // تمت عمليه السحب من الفكتور وسمعت فيه
    Save_New_Client_Data_To_File_After_Refresh_(ClientsFileName, vClients); // حفظ الفايل بقا علشان نعرف نطبع الجديد
    cout << "Done Susessfult Client Balance = " << New_Balance << endl;
    cout << "Press Any Key To Go To Menu..." << endl;
    system("pause>0");
    system("cls");
    return TransactionMenu();
  }
  else
  {
    cout << "Amount Not Withdraw:-)" << endl;
    cout << "Press Any Key To Go To Menu" << endl;
    system("pause>0");
    cout << endl;
    system("cls");
    return TransactionMenu();
  }
}
EnChoicee ReadMenu()
{
  int choice;
  cout << "============================================================" << endl;
  cout << setw(15) << " " << "Main Menu Screen" << setw(15) << " " << endl;
  cout << "============================================================" << endl;
  cout << setw(12) << " " << "[1] Show Clients List." << endl;
  cout << setw(12) << " " << "[2] Add New Clients." << endl;
  cout << setw(12) << " " << "[3] Delete Clients." << endl;
  cout << setw(12) << " " << "[4] Update Clients." << endl;
  cout << setw(12) << " " << "[5] Find Clients." << endl;
  cout << setw(12) << " " << "[6] Transaction." << endl;
  cout << setw(12) << " " << "[7] Exit." << endl;
  cout << "============================================================" << endl;
  cout << "Choose What Do You Want To Do? [1 To 7]: ";
  cin >> choice;
  cout << endl;
  cin.ignore(); // Clear input buffer
  return (EnChoicee)choice;
}

void ShowEndScreen()
{
  cout << "--------------------End Screen----------------------\n"
       << endl;
}

void GO_To_Main_Menu()
{
  cout << "Press Any Key To Go Back To Main Menu..." << system("pause");
  // بوقف السيتسم لحد ما يضغط علي اي زرار بعد كدا بعمل (reset)
  // للاسكرين برجعها اسود تاني ثم بوديه للمين فانكشن يختار حاجه تانيه
  cout << endl
       << endl;
  system("cls");
  return Main();
}
void DeleteScreen()
{
  cout << "---------------------------------------------------" << endl;
  cout << setw(10) << " " << "Delete Client Screen " << endl;
  cout << "---------------------------------------------------" << endl;
}
void UpdateClientScreen()
{
  cout << "---------------------------------------------------" << endl;
  cout << setw(10) << " " << "Update Client Screen " << endl;
  cout << "---------------------------------------------------" << endl;
}
void FindClientsScreen()
{
  cout << "---------------------------------------------------" << endl;
  cout << setw(10) << " " << "Find Client Screen " << endl;
  cout << "---------------------------------------------------" << endl;
}
void ExitScreen()
{
  cout << "---------------------------------------------------" << endl;
  cout << setw(10) << " " << " Progeams Ends :-)" << endl;
  cout << "---------------------------------------------------" << endl;
}
void DepoistScreen()
{
  cout << "---------------------------------------------------" << endl;
  cout << setw(10) << " " << " Depoist Screen Screen " << endl;
  cout << "---------------------------------------------------" << endl;
}
void WithdrawScreen()
{
  cout << "---------------------------------------------------" << endl;
  cout << setw(10) << " " << " Withdraw Screen Screen" << endl;
  cout << "---------------------------------------------------" << endl;
}
void TransactionMenuScreen()
{
  cout << "=========================================================" << endl;
  cout << setw(20) << " " << "Transaciion Menu Screen" << endl;
  cout << "=========================================================" << endl;
  cout << setw(12) << " " << "[1] Deposit." << endl;
  cout << setw(12) << " " << "[2] Withdraw." << endl;
  cout << setw(12) << " " << "[3] Total Balance." << endl;
  cout << setw(12) << " " << "[4] Main Menu." << endl;
  cout << "=========================================================" << endl;
  cout << "Choose What Do You Want To Do?[1 To 4]" << endl;
}
transaction_menu Read_Choose_Of_transaction_screen()
{
  TransactionMenuScreen();
  int choice;
  cin >> choice;
  cin.ignore(); // Clear input buffer
  cout << endl;
  return (transaction_menu)choice;
}
double TotalBalance(vector<sClient> &alldata)
{
  double total_Balance = 0.0;
  for (sClient &client : alldata)
  {
    total_Balance += client.AccountBalance;
  }
  return total_Balance;
}
void total_Balance_Process(vector<sClient> &alldata)
{
  cout << setw(30) << " " << "Balance List (" << alldata.size() << ")" << "Client(s)." << endl;
  cout << "____________________________________________________";
  cout << "________________";
  cout << "____________________________________________________" << endl;

  cout << "|" << left << setw(30) << "Account Number";
  cout << "|" << left << setw(40) << "Client Name";
  cout << "|" << left << setw(30) << " Balance" << endl;

  cout << "____________________________________________________";
  cout << "________________";
  cout << "____________________________________________________" << endl;

  for (sClient &client : alldata)
  {
    PrintClientDataFromTransaction(client);
    cout << endl;
  }

  cout << endl
       << endl;

  cout << "____________________________________________________";
  cout << "________________";
  cout << "____________________________________________________\n"
       << endl;
  double Total_Balance = TotalBalance(alldata);
  cout << setw(60) << " " << "Total Balance = " << Total_Balance << endl;
  cout << "Press Any Key To Go To Menu..." << endl;
  system("pause>0");
  cout << endl;
  system("cls");
  return TransactionMenu();
}
void TransactionMenu()
{
  vector<sClient> allClientsData = LoadDataFromFile(ClientsFileName); // الفايل دا هيبقا فيه كل الداتا وهبعته
  transaction_menu choice = Read_Choose_Of_transaction_screen();
  system("cls");
  switch (choice)
  {
  case depoist:
    DepoistScreen();
    DepoistProcess(allClientsData);
    allClientsData = LoadDataFromFile(ClientsFileName); // reload
    break;
  case withdraw:
    WithdrawScreen();
    withdrawProcess(allClientsData);
    allClientsData = LoadDataFromFile(ClientsFileName); // reload
    break;
  case total_balance:
    total_Balance_Process(allClientsData);
    break;
  case menu:
    return Main();
    break;
  }
}
void Main()
{
  vector<sClient> allClientsData = LoadDataFromFile(ClientsFileName);
  EnChoicee enchoice = ReadMenu();
  system("cls");
  switch (enchoice)
  {
  case showlist:
    PrintAllClientsData(allClientsData);
    GO_To_Main_Menu();
    break;
  case addnew:
    Add_New_Client(allClientsData);
    // كدا انا اهاحطيت الداتا في الفايل وخلاص بس الداتا مسمعتش في الفيكتور الي بطبع منه وبشتغل عليه لازم اخليه يقرأ تاني من الفايل
    allClientsData = LoadDataFromFile(ClientsFileName); // Reload data
    GO_To_Main_Menu();
    break;
  case deleteclient:
    DeleteScreen();
    DeleteClientData(allClientsData);
    // كدا انا اه حزفت الداتا الداتا في الفايل وخلاص بس الداتا مسمعتش في الفيكتور الي بطبع منه وبشتغل عليه لازم اخليه يقرأ تاني من الفايل
    allClientsData = LoadDataFromFile(ClientsFileName); // Reload data
    GO_To_Main_Menu();
    break;
  case updateclient:
    UpdateClientScreen();
    updateDataForClient(allClientsData);
    allClientsData = LoadDataFromFile(ClientsFileName); // Reload data
    // نفس الكلام لازم اعمل لود علشان عدلت علي الفايل نفسه
    GO_To_Main_Menu();
    break;
  case findclient:
    FindClientsScreen();
    FindClients(allClientsData);
    // اسهل فانكشن لاني مستخدمها ف كلوا قبل  كدا
    GO_To_Main_Menu();
    break;
  case Transaction:
    return TransactionMenu();
    break;
  case Exit:
    ExitScreen();
    this_thread::sleep_for(chrono::seconds(7));
    return; // بتطلعلي برا الفانكشن كله
  default:
    cout << "Invalid choice, please try again." << endl;
    GO_To_Main_Menu();
    break;
  }
}

int main()
{
  Main();
  return 0;
}
