#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include "users.h"
#include "categories.h"

using namespace std;


void Registration(){
	cout<< "User Registration\n\nUsername: ";
	bool usernameNotInUse = false;
	string username, pw, fullname;
	char cstrfullname[50];
	vector<RegUser*> Users;
	LoadUsers(&Users);
	vector<Log*> Logs;
	LoadLogs(&Logs);

	
	cin >> username;
	while( !usernameNotInUse){
		for(int i = 0 ; i < Users.size() ; i++)
			if(username == Users[i]->get_username()){
				cout << "This username is already in use, please try again:\n";
				cout << "Username: ";
				cin >> username;
				cout << endl;
				break;
			}
		usernameNotInUse = true;
	}
	cin.clear();
 	cin.ignore(100, '\n');
	cout << endl << "Full name: ";
	cin >> noskipws >> fullname;
	cout << fullname << endl;
	cin.clear();
 	cin.ignore(100, '\n');
	cout << endl << "Password: ";
	cin >> pw;
	cout << endl;
	
	ofstream usersf;
	usersf.open("users.csv", ios_base::app);
	usersf << Users.size() + 1 << "\t" << username << "\t" << fullname << "\t" << pw << "\t"
			<< "1" << "\t" << "A" << "\t" << 0 << "\t \n";
	usersf.close();
	cout << "Registration Completed!\n\n";
	
	string msg = username;
	msg += " - ";
	msg += fullname;
	msg += " - ";
	Logs.push_back(new Log(Users.size() + 1, "New user registration", msg));
	
	SaveLogs(&Logs);

	for(int i = 0; i < Users.size(); i++)
		delete Users[i];
	for(int i = 0; i < Logs.size(); i++)
		delete Logs[i];
}


bool Login(DataP Data){
	string input_username, input_password, msg;
	bool locked = false, logged = false, wrongpass = false;
	
	while( !logged){
		cout << "Username: ";
		cin >> input_username;
		if(input_username == "guest"){
			Data->Curr_user = new Guest;
			Data->LogsP->push_back(new Log(0, "Guest login", string() ));
			return true;
		}
		cout << "Password: ";
		cin >> input_password;
		for(int i = 0 ; i < Data->UsersP->size(); i++){
			if( input_username == (*Data->UsersP)[i]->get_username()){
				if( (*Data->UsersP)[i]->auth(input_password) && !(locked = (*Data->UsersP)[i]->islocked(&msg))){
					Data->Curr_user = (*Data->UsersP)[i];
					Data->LogsP->push_back(new Log((*Data->UsersP)[i]->get_user_id(), "User login", string() ));
					logged = true;
					break;
				}
				else if (!locked ){
					wrongpass = true;
					break;
				}
			}
		}
		if(logged){
			cout << "Successful login\n";
			return true;
		}
		else if(locked){
			cout << "Locked Account\nreason: " << msg << endl;
			return false;
		}
		else{
			cout << "Incorrect username/password\n";
		}
	}
	return true;
}
//static init
int Bet::betid = 0;
int RegUser::ID = 0;

int main(int argc, char** argv) {
	ifstream logo;
	string str;
	logo.open("logo.txt");
	while(getline(logo, str))
		cout << str << endl;;
	cout << endl << endl;


	if(argc > 1 && !strcmp(argv[1], "-R")){
		Registration();
		return 0;
	}

	//init
	vector<RegUser*> Users;
	LoadUsers(&Users);
	Hierarchy h("BetOvlakes", "BetOvlakes");
	LoadHierarchy(&h);
	vector<Bet*> Bets;
	LoadBets(&Bets);
	vector<Log*> Logs;
	LoadLogs(&Logs);

	DataP Data = new DataNode;
	Data->UsersP = &Users;
	Data->HierarchyP = &h;
	Data->BetsP = &Bets;
	Data->LogsP = &Logs;
	Data->Curr_Location = &h;
	Data->Toggle = true;

	cout << "\n\nWelcome to betOvlakes!!\nLogin to continue or "
			<< "enter \"guest\" to continue as a guest\n\n";
	if(!Login(Data))
		return 0;


	string command;
	cout << "Welcome!\n";
	Data->Curr_user->printOperations(Data);
	cin >> command;
	while(command != "Exit" && command!="X"){
		Data->Curr_user->Operate(command, Data);
		Data->Curr_user->printOperations(Data);
		cin >> command;
	}
	if(Data->Curr_user->get_user_id())
		Data->LogsP->push_back(new Log(Data->Curr_user->get_user_id(), "User logout", string() ));
	else{
		Data->LogsP->push_back(new Log(0, "Guest logout", string() ));
		delete Data->Curr_user;
	}
	SaveUsers(&Users);
	SaveBets(&Bets);
	SaveHierarchy(&h);
	SaveLogs(&Logs);


	for(int i = 0; i < Users.size(); i++)
		delete Users[i];
	for(int i = 0; i < Bets.size(); i++)
		delete Bets[i];
	for(int i = 0; i < Logs.size(); i++)
		delete Logs[i];
	delete Data;
	return 0;
}
