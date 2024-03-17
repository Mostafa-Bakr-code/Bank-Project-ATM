#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
#include<vector>
#include<iomanip>

using namespace std;

// string fileName = "please navigate to your clientsdata.txt file in your bank-project folder.."
string fileName = "../Bank-Project/clientsData.txt";

enum enMainMenu { QuickWithdraw = 1, NormalWithDraw = 2, Deposit = 3, Balance = 4, logOut = 5 };
enum enQuickWithdrawAmounts {fifty = 1, hundred = 2, twoHundred = 3, fiveHundred = 4, oneThousand = 5, Exit = 6 };

struct stClientRecord {
	string accountNumber, pinCode, clientName, phone;
	double balance;
};

//_______________________________________________________________________________________________
// Declarations

void showMainMenu(stClientRecord &activeUserRecord, vector<stClientRecord>& vClientsRecord, short activeUserIndex);
void logIn();
void returnToMainMenu(stClientRecord &activeUserRecord, vector<stClientRecord>& vClientsRecord, short activeUserIndex);

//Files
//________________________________________________________________________________________________

string fromStclientRecordToLine(stClientRecord& clientRecord, string seperator = "#//#") {


	string clientRecordLine = "";

	clientRecordLine += clientRecord.accountNumber + seperator;
	clientRecordLine += clientRecord.clientName + seperator;
	clientRecordLine += clientRecord.pinCode + seperator;
	clientRecordLine += clientRecord.phone + seperator;
	clientRecordLine += to_string(clientRecord.balance);

	return clientRecordLine;
}

stClientRecord fromLinetoStClientRecord(string clientRecordLine, string seperator = "#//#") {

	stClientRecord clientRecord;


	// from string line to vector of lines (strings) [split] ....

	vector<string> vrecords;
	short pos = 0;

	while ((pos = clientRecordLine.find(seperator)) != string::npos) {

		vrecords.push_back(clientRecordLine.substr(0, pos));
		clientRecordLine.erase(0, pos + seperator.length());
	}

	if ((pos = clientRecordLine.find(seperator)) == string::npos) {
		vrecords.push_back(clientRecordLine.substr(0, clientRecordLine.length()));
	}
	// from vector of lines (strings) to record ....

	clientRecord.accountNumber = vrecords[0];
	clientRecord.clientName = vrecords[1];
	clientRecord.pinCode = vrecords[2];
	clientRecord.phone = vrecords[3];
	clientRecord.balance = stod(vrecords[4]);
	return clientRecord;
}

void loadDataFromVectorTofile(string fileName, vector<stClientRecord>& vClientsRecord) {

	fstream myFile;
	myFile.open(fileName, ios::out);

	if (myFile.is_open()) {

		for (stClientRecord record : vClientsRecord) {

			myFile << fromStclientRecordToLine(record) << endl;
		}
		myFile.close();
	}
}

vector<stClientRecord> loadDataFromFileToStVector(string fileName) {

	vector<stClientRecord> vClientsRecord;
	fstream myFile;
	myFile.open(fileName, ios::in);

	if (myFile.is_open()) {
		string line;
		while (getline(myFile, line)) {
			vClientsRecord.push_back(fromLinetoStClientRecord(line));
		}
	}
	return vClientsRecord;
}

//________________________________________________________________________________________________

int readNumberFromTo(int from, int to) {

	int number;

	do {
		cin >> number;

		if (number < from || number > to || cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Please entre a number between " << from << "-" << to << " : ";
		}

	} while (number < from || number > to);

	return number;
}

string readString() {

	string myString;
	getline(cin >> ws, myString);
	return myString;
}

float readPositiveNumber() {

	float number;
	bool inputFail = false;

	do {
		cin >> number;

		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid Input... Please entre a number: ";
			inputFail = true;
		}

		if (number < 0) {
			cout << "Please Entre a Positive Number! .... ";
			inputFail = false;
		}

	} while (number < 0 || inputFail);

	return number;
}

char readChar() {

	char myChar;
	bool isValidInput = false;

	do {
		cin >> myChar;
		myChar = tolower(myChar);

		if (myChar != 'y' && myChar != 'n') {
			cout << "Please Entre y or n ...\n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else {
			isValidInput = true;
		}

	} while (!isValidInput);

	return myChar;
}

//_______________________________________________________________________________________________

void deposit(stClientRecord &activeUserRecord, vector<stClientRecord>& vClientsRecord, short activeUserIndex) {

	cout << "===========================================================\n";
	cout << "                      Deposit-Screen\n";
	cout << "===========================================================\n";

	cout << "Entre Amount you want to deposit: ";
	int depositAmount = 0;
	depositAmount = readPositiveNumber();
	cout << "\nAre you sure you want to deposit " << depositAmount << " to your account? y/n?";
	if (readChar() == 'y') {
		vClientsRecord[activeUserIndex].balance += depositAmount;
		cout << "\nTransaction completed....\n\n";
		cout << "Your current balance is " << vClientsRecord[activeUserIndex].balance << "\n\n";
	}
	else {
		returnToMainMenu(activeUserRecord,vClientsRecord,activeUserIndex);
	}
}

void quickWithdrawMenu() {
	cout << "===========================================================\n";
	cout << "                     Quick-Withdraw-Screen\n";
	cout << "===========================================================\n";
	cout << " [1] 50\n [2] 100\n [3] 200\n [4] 500\n [5] 1000\n [6] Exit\n\n";
	cout << "                      Select Option [1-6]\n\n";
	
}

short quickWithDrawAmount() {

	enQuickWithdrawAmounts withdrawAmount = enQuickWithdrawAmounts(readNumberFromTo(1, 6));

	switch (withdrawAmount) {
	case enQuickWithdrawAmounts::fifty: return 50; break;
	case enQuickWithdrawAmounts::hundred: return 100; break;
	case enQuickWithdrawAmounts::twoHundred: return 200; break;
	case enQuickWithdrawAmounts::fiveHundred: return 500; break;
	case enQuickWithdrawAmounts::oneThousand: return 1000; break;
	}
}

void quickWithdraw(stClientRecord& activeUserRecord, vector<stClientRecord>& vClientsRecord, short activeUserIndex) {

	quickWithdrawMenu();

	short withdrawAmount = quickWithDrawAmount();
	
	if (withdrawAmount > vClientsRecord[activeUserIndex].balance) {
		cout << "Not enough balance...\n";
	}
	else {
		cout << "\nAre you sure you want to withdraw " << withdrawAmount << " to your account? y/n?";
		if (readChar() == 'y') {
			vClientsRecord[activeUserIndex].balance -= withdrawAmount;
			cout << "\nTransaction completed....\n\n";
			cout << "Your current balance is " << vClientsRecord[activeUserIndex].balance << "\n\n";
		}
		else {
			returnToMainMenu(activeUserRecord, vClientsRecord, activeUserIndex);
		}
	}
	returnToMainMenu(activeUserRecord, vClientsRecord, activeUserIndex);
}

void normalWithdraw(stClientRecord& activeUserRecord, vector<stClientRecord>& vClientsRecord, short activeUserIndex) {

	cout << "===========================================================\n";
	cout << "                      Withdraw-Screen\n";
	cout << "===========================================================\n";

	cout << "Entre Amount you want to withdraw: ";
	int withdrawAmount = 0;
	withdrawAmount = readPositiveNumber();
	if (withdrawAmount > vClientsRecord[activeUserIndex].balance) {
		cout << "Not enough balance...\n";
	}
	else {
		cout << "\nAre you sure you want to withdraw " << withdrawAmount << " to your account? y/n?";
		if (readChar() == 'y') {
			vClientsRecord[activeUserIndex].balance -= withdrawAmount;
			cout << "\nTransaction completed....\n\n";
			cout << "Your current balance is " << vClientsRecord[activeUserIndex].balance << "\n\n";
		}
		else {
			returnToMainMenu(activeUserRecord,vClientsRecord,activeUserIndex);
		}
	}
	returnToMainMenu(activeUserRecord,vClientsRecord,activeUserIndex);
}

void showBalance(vector<stClientRecord>& vClientsRecord, short activeUserIndex) {

	cout << "===========================================================\n";
	cout << "                       Balance\n";
	cout << "===========================================================\n";
	cout << "           Your Current Balane is: " << vClientsRecord[activeUserIndex].balance << "\n\n";
}

void returnToMainMenu(stClientRecord &activeUserRecord, vector<stClientRecord>& vClientsRecord, short activeUserIndex) {
	cout << "\n\nPress Enter to go back to Main Menu..." << endl;
	system("pause>0");
	showMainMenu(activeUserRecord,vClientsRecord,activeUserIndex);
}

void exitProgram(stClientRecord &activeUserRecord, vector<stClientRecord>& vClientsRecord, short activeUserIndex) {

	cout << "\nAre you sure you want to Exit the program? y/n\n";
	if (readChar() == 'y') {
		loadDataFromVectorTofile(fileName, vClientsRecord);
		logIn();
	}
	else {
		returnToMainMenu(activeUserRecord,vClientsRecord,activeUserIndex);
	}
}

void performMainMenuOptions(enMainMenu selectOption, stClientRecord &activeUserRecord, vector<stClientRecord>& vClientsRecord, short activeUserIndex) {

	switch (selectOption) {
	case enMainMenu::QuickWithdraw:system("cls"); quickWithdraw(activeUserRecord, vClientsRecord, activeUserIndex); returnToMainMenu(activeUserRecord, vClientsRecord, activeUserIndex); break;
	case enMainMenu::NormalWithDraw: system("cls"); normalWithdraw(activeUserRecord,vClientsRecord,activeUserIndex); returnToMainMenu(activeUserRecord,vClientsRecord,activeUserIndex); break;
	case enMainMenu::Deposit: system("cls"); deposit(activeUserRecord,vClientsRecord,activeUserIndex); returnToMainMenu(activeUserRecord,vClientsRecord,activeUserIndex); break;
	case enMainMenu::Balance: system("cls"); showBalance(vClientsRecord,activeUserIndex); returnToMainMenu(activeUserRecord,vClientsRecord,activeUserIndex); break;
	case enMainMenu::logOut: system("cls"); exitProgram(activeUserRecord, vClientsRecord,activeUserIndex); break;
	}

}

void showMainMenu(stClientRecord &activeUserRecord, vector<stClientRecord>& vClientsRecord, short activeUserIndex) {

	system("cls");

	cout << "Welcome Back " << activeUserRecord.clientName << "...\n";
	for (int i = 0; i < 50; i++) {
		cout << "=";
	}
	cout << "\n";
	cout << "                " << "ATM Main Menu\n";

	for (int i = 0; i < 50; i++) {
		cout << "=";
	}

	cout << "\n";

	cout << "           " << "[1] Quick Withdraw.\n";
	cout << "           " << "[2] Normal Withdraw.\n";
	cout << "           " << "[3] Deposit.\n";
	cout << "           " << "[4] Balance.\n";
	cout << "           " << "[5] Log Out & Save.\n";



	for (int i = 0; i < 50; i++) {
		cout << "=";
	}

	cout << "\n           Select your option [1] - [5]\n";

	performMainMenuOptions(enMainMenu(readNumberFromTo(1, 5)),activeUserRecord,vClientsRecord,activeUserIndex);

}

void lodInScreen() {

	cout << "===========================================================\n";
	cout << "                       LOG IN SCREEN\n";
	cout << "===========================================================\n";
}

void logIn() {

	system("cls");
	lodInScreen();

	vector<stClientRecord> vClientsRecord = loadDataFromFileToStVector(fileName);

	short userCounter = 0;

	do {
		cout << "Account Number: ";
		string userName = readString();
		short passCounter = 0;

		for (int i = 0; i < vClientsRecord.size(); i++) {

			if (userName == vClientsRecord[i].accountNumber) {

				do {
					cout << "PIN: ";

					string password = readString();

					if (password == vClientsRecord[i].pinCode) {
						stClientRecord activeUserRecord = vClientsRecord[i];
						short activeUserIndex = i;
						showMainMenu(activeUserRecord,vClientsRecord,activeUserIndex);
						return;
					}
					passCounter++;
					cout << "Wrong PIN..." << "\nyou have used " << passCounter << " of 3 trials\n";
				}

				while (passCounter < 3);
				cout << "\n\nAccount locked... please contact your admin..\n";
				exit(0);
			}
		}
		userCounter++;
		cout << "Account number " << userName << " doesn't exist ... you have used " << userCounter << " of 3 trials\n";
	} while (userCounter < 3);

	cout << "\n\nAccount locked... please contact your admin..\n";
}

//__________________________________________________________________________________

int main() {

	logIn();
}