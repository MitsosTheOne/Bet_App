#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include "users.h"
#include "categories.h"
using namespace std;


bool User::islocked(string *msg){
	return 0;
}


RegUser::RegUser(string username_, string pass_, string fullname_) 
			: username(username_), pass(pass_), fullname(fullname_){
	this->user_id = ++ID;
}
string RegUser::get_username(){
	return this->username;
}
string RegUser::get_password(){
	return this->pass;
}
string RegUser::get_fullname(){
	return this->fullname;
}
string RegUser::get_status(){
	return string();
}
string RegUser::get_lockedreason(){
	return string();
}
double RegUser::get_balance(){
	return 0;
}
string RegUser::get_freebets(){
	return string();
}
int RegUser::get_user_id(){
	return this->user_id;
}
void RegUser::set_freebets(string freebets_){
	return ;
}


bool RegUser::auth(string password_){
	return (this->pass == password_);
}
bool RegUser::islocked(string *msg){
	return 0;	
}


void Guest::Operate(string op, DataP Data){
	if(isdigit(op[0]) ){
		int pos = atoi(op.c_str());
		if(pos > 0 && pos <= Data->Curr_Location->at(0)->get_size() 
			&& !Data->Curr_Location->isMarket()){
			Data->Curr_Location = Data->Curr_Location->at(pos);
		}
		else{
			cout << "invalid ID\n";
		}
	}
	else if(!strcasecmp(op.c_str(), "Home") || !strcasecmp(op.c_str(), "H"))
		Home(Data);
	else if(!strcasecmp(op.c_str(), "Toggle") || !strcasecmp(op.c_str(), "T"))
		Toggle(Data);
	else
		cout << "Unknown command: " << op << endl;
}
void Guest::Home(DataP Data){
	Data->Curr_Location = Data->HierarchyP;
}
void Guest::Toggle(DataP Data){
	if(Data->Toggle)
		Data->Toggle = false;
	else
		Data->Toggle = true;
}
void Guest::printOperations(DataP Data){
	cout << "Choose:\n"
		<<	"the ID of the node you want to visit\n"
		<<	"H (Home), to return to Home screen\n"
		<<	"T (Toggle), to change rates display\n"
		<<	"X (Exit), to exit system.\n\n";
		
	cout << "Location:" << Data->Curr_Location->get_path() << endl;
	Data->Curr_Location->simple_print(Data);	
}


Punter::Punter(string username_, string pass_, string fullname_, string status_, double balance_, string freebets_)
 : RegUser(username_, pass_, fullname_), status(status_), balance(balance_){
	 if(status[0] == 'L' ){
	 	this->Locked_reason = this->status.substr(2);
	 	this->status = this->status.substr(0,1);
	 }
	this->set_freebets(freebets_);
}

string Punter::get_status(){
	return status;
}
double Punter::get_balance(){
	return this->balance;
}
string Punter::get_freebets(){
	return this->freebets;
}
string Punter::get_lockedreason(){
	return this->Locked_reason;
}
void Punter::set_freebets(string freebets_){
	this->freebets = freebets_;
	if(!freebets.empty()){
		//adding freebets to vector
		string tmp = freebets;
		int pos;
		double value;
		while( sscanf(tmp.c_str(), "%lf", &value)){
	 		freebetsV.push_back(value);
	 		pos = tmp.find(",") + 1 ;
	 		if(!pos)
				break; 
			tmp = tmp.substr(pos);
	 	}
	 }
}
void Punter::add_freebets(double value){
	if(freebetsV.size())
		this->freebets += ",";
	ostringstream tmp;
	tmp << value;
	this->freebets += tmp.str();
	this->freebetsV.push_back(value);
}
void Punter::set_status(string status_){
	this->status = status_;
}
void Punter::set_lockedreason(string lockedreason_){
	this->Locked_reason = lockedreason_;
}
void Punter::set_balance(double balance_){
	this->balance = balance_;
}
bool Punter::islocked(string *message){
	if(this->status == "L"){
		*message = this->Locked_reason; 
		return 1;
	}
	return 0;
}
void Punter::Operate(string op, DataP Data){
	if(isdigit(op[0]) ){
		int pos = atoi(op.c_str());
		if(pos > 0 && pos <= Data->Curr_Location->at(0)->get_size() 
			&& !Data->Curr_Location->isMarket()){
			Data->Curr_Location = Data->Curr_Location->at(pos);
			Data->LogsP->push_back(new Log(user_id, "moved to", Data->Curr_Location->get_node_id()));
			return; 
		}
		else{
			cout << "invalid ID\n";
			return;
		}
	}
	else if(!strcasecmp(op.c_str(), "Home") || !strcasecmp(op.c_str(), "H"))
		Home(Data);
	else if(!strcasecmp(op.c_str(), "Toggle") || !strcasecmp(op.c_str(), "T"))
		Toggle(Data);
	else if(!strcasecmp(op.c_str(), "Account") || !strcasecmp(op.c_str(), "A"))
		Account(Data);
	else if(!strcasecmp(op.c_str(), "Place") || !strcasecmp(op.c_str(), "P"))
		Place(Data);
	else
		cout << "Unknown command: " << op << endl;
}
void Punter::Home(DataP Data){
	Data->Curr_Location = Data->HierarchyP;
	Data->LogsP->push_back(new Log(user_id, "moved to Home","" ));
}
void Punter::Toggle(DataP Data){
	Data->Toggle = !Data->Toggle;
	Data->LogsP->push_back(new Log(user_id, "changed display toggle to",
		 (Data->Toggle ? "decimal" : "fractional")));
}
void Punter::Account(DataP Data){
	Data->LogsP->push_back(new Log(user_id, "entered Account menu", ""));
	cout << "Punter: " << username << endl
		<< "Balance: " << balance << endl
		<< "Freebets: " << freebets << endl << endl
		<< "Choose:\n"
		<< "R (Rename), to change username\n"
		<< "P (Password), to change password\n"
		<< "B (Bets), to view your bet history\n"
		<< "D (Deposit), to refill your balance\n\n";
	string command;
	cin >> command;
	if(command == "R" || command =="Rename"){
		cout << "New Username: ";
		string new_name;
		bool usernameNotInUse;
		cin >> new_name;
		
		while( !usernameNotInUse){
			for(int i = 0 ; i < Data->UsersP->size() ; i++)
				if(new_name == (*Data->UsersP)[i]->get_username()){
					cout << "This username is already in use, please try again:\n";
					cout << "New Username: ";
					cin >> new_name;
					cout << endl;
					break;
				}
			usernameNotInUse = true;
		}
		
		username = new_name;
		cout << "Username changed\"" << username << "\"\n";
		Data->LogsP->push_back(new Log(user_id, "Renamed to ", username));
	}
	else if(command == "P" || command =="Password"){
		string pw, pw2;
		cout << "Enter new password: ";
		cin >> pw;
		cout << "Re enter new password: ";
		cin >> pw2;
		if(pw == pw2){
			pass = pw;
			cout << "Password changed\n";
			Data->LogsP->push_back(new Log(user_id, "changed password ", ""));
		}
		else{
			cout << "Passwords dont match - password didnt change\n";
		}
	}
	else if(command == "B" || command =="Bets"){
		string result;
		cout << "bet_id\tnode_id\tstake\tresult\n";
		for(int i = 0; i < Data->BetsP->size(); i++){
			if((*Data->BetsP)[i]->get_user_id() == this->user_id){
				if((*Data->BetsP)[i]->get_result() == "W")
					result = "Won";
				else if((*Data->BetsP)[i]->get_result() == "L")
					result = "Lost";
				else if((*Data->BetsP)[i]->get_result() == "V")
					result = "Voided";
				else if((*Data->BetsP)[i]->get_result() == "-")
					result = "Pending";
				
				cout << (*Data->BetsP)[i]->get_bet_id() << "\t" 
					<< (*Data->BetsP)[i]->get_node_id() << "\t"
					<< (*Data->BetsP)[i]->get_stake() << "\t" 
					<< result << endl;
			}
		}
		Data->LogsP->push_back(new Log(user_id, "Requested to see their Bets ", ""));
	}
	else if(command == "D" || command =="Deposit"){
		double value;
		cout << "Enter deposit value: "; 
		cin >> value;
		balance += value;
		ostringstream msg ;
		msg << "value: " << value;
		Data->LogsP->push_back(new Log(user_id, "Deposited a value to their account ", msg.str()));
	}
}
void Punter::Place(DataP Data){
	if(!Data->Curr_Location->isMarket()){
		cout << "Place not available in current node\n";
		return;
	}
	if( !balance){
		cout << "0 balance, refill your balance from Account menu and try again\n";
		return;
	}
	cout << "Balance: " << balance << endl;
	int selection = process_input(Data);
	if(!selection)
		return;
	if( Data->Curr_Location->at(selection)->get_result()){
		cout << "This selection is unavailable - Match settled or voided\n";
		return;
	}
	
	cout << "Selected: " << Data->Curr_Location->at(selection)->get_name() 
		<< "#" << Data->Curr_Location->at(selection)->get_print(Data->Toggle) << endl;

	
	if(freebetsV.size()){
		char c = 'a';
		cout << "Available freebets: \n";
		for(int i = 0; i < freebetsV.size(); i++)
			cout << c++ << ": " << freebetsV[i] << endl; 

	}
	string input;
	cout << "\nBalance: " 
		<< balance << "\nChoose a freebet or enter value of bet: ";
	cin >> input;
	
	bool usedfb = false;
	int sel;
	double value;
	if( isdigit(input[0])){
		value = atof(input.c_str());
		if(value > balance){
			cout << "Not enough balance, refill your balance from Account menu "
						"or try again with smaller bet\n";
			return;
		}
	}
	else{
		sel = input[0] -'a';
		if(sel < 0 || sel >= freebetsV.size() ){
			cout << "Invalid selection\n";
			return;
		}
		usedfb = true;
		value = freebetsV[sel];
	}
	

	cout << "\nPlacing bet ( " << value << " ) on " 
		<< Data->Curr_Location->at(selection)->get_name() 
		<< "#" << Data->Curr_Location->at(selection)->get_strprice() << "\nConfirm? (y/n): "; 
	cin >> input;
	if(input != "y"){
		return;
	}
	if( usedfb ){ // remove freebet
		ostringstream tmpd;
		tmpd << freebetsV[sel];

		string tmp = tmpd.str() + ",";
		size_t f = freebets.find(tmp);
		if(f != string::npos)
			freebets.replace(f, tmp.length(), "");
		else{
			tmp = tmpd.str();
			f = freebets.find(tmp);
			freebets.replace(f, tmp.length(), "");
		}
		f = freebets.find_last_of(",");
		if(f != string::npos)
			freebets.replace(f, 1, "");

		freebetsV.erase(freebetsV.begin() + sel);
	}
	string node_id = Data->Curr_Location->at(selection)->get_node_id();
	ostringstream msg2;
	msg2 << "value: " << value;
	Data->LogsP->push_back(new Log(user_id, "Placed a bet: " + node_id, msg2.str()));
	Data->BetsP->push_back(new Bet(user_id, node_id, value, "-"));
	balance-=value;
	return;
}
void Punter::printOperations(DataP Data){
	if(Data->Curr_Location->isMarket())
		cout << "Choose:\n"
			<<	"the ID of the node you want to visit\n"
			<<	"H (Home), to return to Home screen\n"
			<<	"T (Toggle), to change rates display\n"
			<< 	"A (Account), to change account info\n"
			<<	"P (Place), to place a bet\n"
			<<	"X (Exit), to exit system.\n\n";
	else
		cout << "Choose:\n"
			<<	"the ID of the node you want to visit\n"
			<<	"H (Home), to return to Home screen\n"
			<<	"T (Toggle), to change rates display\n"
			<< 	"A (Account), to change account info\n"
			<<	"X (Exit), to exit system.\n\n";
		
	cout << "Location:" << Data->Curr_Location->get_path() << endl;
	Data->Curr_Location->simple_print(Data);
}


Trader::Trader(string username_, string pass_, string fullname_) : RegUser(username_, pass_, fullname_){
	//cout << "new trader" << username << endl;
}
void Trader::Operate(string op, DataP Data){
	if(isdigit(op[0]) ){
		int pos = atoi(op.c_str());
		if(pos > 0 && pos <= Data->Curr_Location->at(0)->get_size() 
			&& !Data->Curr_Location->isMarket()){
			Data->Curr_Location = Data->Curr_Location->at(pos);
		}
		else{
			cout << "invalid ID\n";
		}
	}
	else if(!strcasecmp(op.c_str(), "Home") || !strcasecmp(op.c_str(), "H"))
		Home(Data);
	else if(!strcasecmp(op.c_str(), "Toggle") || !strcasecmp(op.c_str(), "T"))
		Toggle(Data);
	else if(!strcasecmp(op.c_str(), "Bets") || !strcasecmp(op.c_str(), "B"))
		Bets(Data);
	else if(!strcasecmp(op.c_str(), "Freebets") || !strcasecmp(op.c_str(), "F"))
		Freebets(Data);
	else if(!strcasecmp(op.c_str(), "Settle") || !strcasecmp(op.c_str(), "S"))
		Settle(Data);
	else if(!strcasecmp(op.c_str(), "Void") || !strcasecmp(op.c_str(), "V"))
		Void(Data);
		cout << "Unknown command: " << op << endl;
}

void Trader::Home(DataP Data){
	Data->Curr_Location = Data->HierarchyP;
}
void Trader::Toggle(DataP Data){
	Data->Toggle = !Data->Toggle;
	Data->LogsP->push_back(new Log(user_id, "changed display toggle to",
		 (Data->Toggle ? "decimal" : "fractional")));
}

void Trader::Bets(DataP Data){
	cout << "bet_id\tuser_id\tnode_id\tstake\tresult\n";
	for(int i = 0; i < Data->BetsP->size(); i++)
		(*Data->BetsP)[i]->print();
	Data->LogsP->push_back(new Log(user_id, "Requested to see Bets ", ""));
}
void Trader::Settle(DataP Data){
	if(!Data->Curr_Location->isMarket()){
		cout << "Settle not available in current node\n";
		return;
	}
	int selection = process_input(Data);
	if(!selection)
		return;
	string res;
	cout<< "Enter selection's result: ";
	cin>> res;
	if(res=="W"||res=="Win")
		Data->Curr_Location->at(selection)->set_result(1);
	else if(res=="L"||res=="Lose")
		Data->Curr_Location->at(selection)->set_result(2);
	string node_id=Data->Curr_Location->at(selection)->get_node_id();
	for(int i=0;i<Data->BetsP->size();i++){
		if((*Data->BetsP)[i]->get_node_id()==node_id)
			(*Data->BetsP)[i]->set_result(res,Data);
	}
	Data->LogsP->push_back(new Log(user_id, "Settled a Selection ", ""));
}
void Trader::Void(DataP Data){
	if(!Data->Curr_Location->isMarket()){
		cout << "Void not available in current node\n";
		return;
	}
	int selection=process_input(Data);
	if(!selection)
		return;
	
	string temp_node_id = Data->Curr_Location->at(selection)->get_node_id();
	Data->LogsP->push_back(new Log(user_id, "Voided a selection ", temp_node_id));
	for(int i = 0; i < Data->BetsP->size(); i++)
		if((*Data->BetsP)[i]->get_node_id() == temp_node_id)
			(*Data->BetsP)[i]->set_result("V",Data);
	Data->Curr_Location->at(0)->set_voided(selection);
}

void Trader::Freebets(DataP Data){
	string new_freebet;
	int choice;
	for(int i = 0; i < Data->UsersP->size(); i++)
		cout << i + 1 << ". " << (*Data->UsersP)[i]->get_fullname() << endl;
	cout<<"Enter the number of the User to whom you want to give a freebet: ";
	cin >> choice;
	cout<<"Enter ammount of freebet: ";
	cin >> new_freebet;
	(*Data->UsersP)[choice]->add_freebets(new_freebet);
	ostringstream msg1;
	msg1 << "Gave freebet to user " << choice ;
	string msg2 = "value: ";
	msg2.append(new_freebet.substr(1));
	Data->LogsP->push_back(new Log(user_id, msg1.str(), msg2));
	(*Data->UsersP)[choice]->set_freebets(new_freebet);
}

void Trader::printOperations(DataP Data){
	cout << "Choose:\n"
		<<	"the ID of the node you want to visit\n"
		<<	"H (Home), to return to Home screen\n"
		<<	"T (Toggle), to change rates display\n"
		<<	"B (Bets), to display the last 20 bets\n"
		<<	"F (Freebets), to give a freebet to a user\n"
		<<	"S (Settle), to settle a bet\n"
		<<	"V (Void),	to void a bet\n"
		<<	"X (Exit), to exit system.\n\n";
		
	cout << "Location:" << Data->Curr_Location->get_path() << endl;
	Data->Curr_Location->simple_print(Data);
}


Director::Director(string username_, string pass_, string fullname_) : RegUser(username_, pass_, fullname_){
	//cout << "new director " << username << endl;
}
void Director::Operate(string op, DataP Data){
	if(isdigit(op[0]) ){
		int pos = atoi(op.c_str());
		if(pos > 0 && pos <= Data->Curr_Location->at(0)->get_size() 
			&& !Data->Curr_Location->isMarket()){
			Data->Curr_Location = Data->Curr_Location->at(pos);
		}
		else{
			cout << "invalid ID\n";
		}
	}
	else if(!strcasecmp(op.c_str(), "Home") || !strcasecmp(op.c_str(), "H"))
		Home(Data);
	else if(!strcasecmp(op.c_str(), "Toggle") || !strcasecmp(op.c_str(), "T"))
		Toggle(Data);
	else if(!strcasecmp(op.c_str(), "Bets") || !strcasecmp(op.c_str(), "B"))
		Bets(Data);
	else if(!strcasecmp(op.c_str(), "Freebets") || !strcasecmp(op.c_str(), "F"))
		Freebets(Data);
	else if(!strcasecmp(op.c_str(), "Settle") || !strcasecmp(op.c_str(), "S"))
		Settle(Data);
	else if(!strcasecmp(op.c_str(), "Void") )
		Void(Data);
	else if(!strcasecmp(op.c_str(), "Save") )
		Save(Data);
	else if(!strcasecmp(op.c_str(), "Logs") || !strcasecmp(op.c_str(), "L"))
		Logs(Data);
	else if(!strcasecmp(op.c_str(), "Users") || !strcasecmp(op.c_str(), "U"))
		Users(Data);
	else if(!strcasecmp(op.c_str(), "Visibility") || !strcasecmp(op.c_str(), "V"))
		Visibility(Data);
	else if(!strcasecmp(op.c_str(), "Rename") || !strcasecmp(op.c_str(), "R"))
		Rename(Data);
	else if(!strcasecmp(op.c_str(), "New") || !strcasecmp(op.c_str(), "N"))
		New(Data);
	else if(!strcasecmp(op.c_str(), "Copy") || !strcasecmp(op.c_str(), "C"))
		Copy(Data);
	else if(!strcasecmp(op.c_str(), "Delete") || !strcasecmp(op.c_str(), "D"))
		Delete(Data);
	else
		cout << "Unknown command: " << op << endl;
}

void Director::Home(DataP Data){
	Data->Curr_Location = Data->HierarchyP;
}
void Director::Toggle(DataP Data){
	Data->Toggle = !Data->Toggle;
	Data->LogsP->push_back(new Log(user_id, "changed display toggle to",
		 (Data->Toggle ? "decimal" : "fractional")));
}

void Director::Bets(DataP Data){
	cout << "bet_id\tuser_id\tnode_id\tstake\tresult\n";
	for(int i = 0; i < Data->BetsP->size(); i++)
		(*Data->BetsP)[i]->print();
	Data->LogsP->push_back(new Log(user_id, "Requested to see Bets ", ""));
}

void Director::Freebets(DataP Data){
	string new_freebet;
	int choice;
	for(int i = 0; i < Data->UsersP->size(); i++)
		cout << i + 1 << ". " << (*Data->UsersP)[i]->get_fullname() << endl;
	cout<<"Enter the number of the User to whom you want to give a freebet: ";
	cin >> choice;
	cout<<"Enter ammount of freebet: ";
	cin >> new_freebet;
	(*Data->UsersP)[choice]->add_freebets(new_freebet);
	ostringstream msg1;
	msg1 << "Gave freebet to user " << choice ;
	string msg2 = "value: ";
	msg2.append(new_freebet.substr(1));
	Data->LogsP->push_back(new Log(user_id, msg1.str(), msg2));
	(*Data->UsersP)[choice]->set_freebets(new_freebet);
}

void Director::Settle(DataP Data){
	if(!Data->Curr_Location->isMarket()){
		cout << "Settle not available in current node\n";
		return;
	}
	int selection = process_input(Data);
	if(!selection)
		return;
	string res;
	cout<< "Enter selection's result: ";
	cin>> res;
	if(res=="W"||res=="Win")
		Data->Curr_Location->at(selection)->set_result(1);
	else if(res=="L"||res=="Lose")
		Data->Curr_Location->at(selection)->set_result(2);
	string node_id=Data->Curr_Location->at(selection)->get_node_id();
	for(int i=0;i<Data->BetsP->size();i++){
		if((*Data->BetsP)[i]->get_node_id()==node_id)
			(*Data->BetsP)[i]->set_result(res,Data);
	}
	Data->LogsP->push_back(new Log(user_id, "Settled a Selection ", ""));
}
void Director::Void(DataP Data){
	if(!Data->Curr_Location->isMarket()){
		cout << "Void not available in current node\n";
		return;
	}
	int selection=process_input(Data);
	if(!selection)
		return;
	
	string temp_node_id = Data->Curr_Location->at(selection)->get_node_id();
	Data->LogsP->push_back(new Log(user_id, "Voided a selection ", temp_node_id));
	for(int i = 0; i < Data->BetsP->size(); i++)
		if((*Data->BetsP)[i]->get_node_id() == temp_node_id)
			(*Data->BetsP)[i]->set_result("V",Data);
	Data->Curr_Location->at(0)->set_voided(selection);
}

void Director::Save(DataP Data){
	SaveUsers(Data->UsersP);
	cout<<"Users saved!"<<endl;
	SaveHierarchy(Data->HierarchyP);
	cout<<"Hierarchy saved!"<<endl;
	Data->LogsP->push_back(new Log(user_id, "Files \"users.csv\",\"hierarchy.dat\" saved", ""));
}
void Director::Logs(DataP Data){
	Data->LogsP->push_back(new Log(user_id, "Requested to view Logs ", ""));
	cout << "Last 25 Logs: \n";
	for(int i = Data->LogsP->size() - 1; i + 26 > Data->LogsP->size()  && i >= 0; i--)
		(*Data->LogsP)[i]->print();
}

void Director::Users(DataP Data){
	Data->LogsP->push_back(new Log(user_id, "Entered user management ", ""));
	cout << "Epiloges:"<<endl<<"V (View), to view the user panel."<<endl<<"S (Search), to search for a user."<<endl<<"L (Lock), to toggle status."<<endl;
	string command;
	cin >> command;
	if(!strcasecmp(command.c_str(), "V") || !strcasecmp(command.c_str(), "View") ){
		Data->LogsP->push_back(new Log(user_id, "Requested to view Users ", ""));
		for(int i=0;i<Data->UsersP->size();i++)
			cout << i + 1 << "\t" << (*Data->UsersP)[i]->get_username() << "\t" 
    		<< (*Data->UsersP)[i]->get_fullname() << "\t" << (*Data->UsersP)[i]->get_password() << "\t" 
    		<< (*Data->UsersP)[i]->get_type() << "\t" << (*Data->UsersP)[i]->get_status() << "\t"
    		<< (*Data->UsersP)[i]->get_balance() << "\t" << (*Data->UsersP)[i]->get_freebets() << endl;
    	}
	else if(!strcasecmp(command.c_str(), "S") || !strcasecmp(command.c_str(), "Search")){
		string search_name;
		cout<<"Enter name to start searching: ";
		cin>>search_name;
		Data->LogsP->push_back(new Log(user_id, "Searched user ", search_name));
		size_t found;
		for(int i=0;i<Data->UsersP->size();i++){
			found=(*Data->UsersP)[i]->get_fullname().find(search_name);
			if(found != string::npos)
				cout << i + 1 << "\t" << (*Data->UsersP)[i]->get_username() << "\t" 
    			<< (*Data->UsersP)[i]->get_fullname() << "\t" << (*Data->UsersP)[i]->get_password() << "\t" 
    			<< (*Data->UsersP)[i]->get_type() << "\t" << (*Data->UsersP)[i]->get_status() << "\t"
    			<< (*Data->UsersP)[i]->get_balance() << "\t" << (*Data->UsersP)[i]->get_freebets() << endl;
		}
	}
	else if(!strcasecmp(command.c_str(), "L") || !strcasecmp(command.c_str(), "Locked")){
		string search_name;
		cout<<"Enter account name (username) to lock / unlock: ";
		cin>>search_name;
		string msg;
		for(int i=0;i<Data->UsersP->size();i++)
			if(search_name == (*Data->UsersP)[i]->get_username()){
				if((*Data->UsersP)[i]->get_type() == 1){
					if(!(*Data->UsersP)[i]->islocked(&msg)){
						cout<<"Give a reason for locking: ";
						string reason;
						cin >> reason;
						Data->LogsP->push_back(new Log(user_id, "Locked user " 
							+ (*Data->UsersP)[i]->get_user_id(), reason));
						(*Data->UsersP)[i]->set_lockedreason(reason);
						(*Data->UsersP)[i]->set_status("L");
						cout<<"Account locked successfully!"<<endl;
					}
					else{
						cout<<"Give a reason for unlocking: ";
						string reason;
						cin >> reason;
						Data->LogsP->push_back(new Log(user_id, "Unlocked user " 
							+ (*Data->UsersP)[i]->get_user_id(), reason));
						(*Data->UsersP)[i]->set_lockedreason(reason);
						(*Data->UsersP)[i]->set_status("A");
						cout<<"Account unlocked successfully!"<<endl; 
					}
					break;
				}
				else{
					cout<<"This is not a Punter's account!"<<endl;
					break;
				}
			}
			else if(i==Data->UsersP->size())
				cout<<"No match was found!"<<endl;
	}
}

void Director::Visibility(DataP Data){
	int selection=process_input(Data);
	if(!selection)
		return;
	Data->LogsP->push_back(new Log(user_id, "Changed visibility of node: " 
		+ Data->Curr_Location->at(selection)->get_node_id() , !Data->Curr_Location->at(selection)->get_visibility()?"true":"false" ));
	Data->Curr_Location->at(selection)->set_visibility();
}

void Director::Rename(DataP Data){
	int selection=process_input(Data);
	if(!selection)
		return;
	string new_name;
	cout<< "Enter node's new name: ";
	cin>> new_name;
	Data->LogsP->push_back(new Log(user_id, "Renamed a node to" + new_name, Data->Curr_Location->at(selection)->get_node_id() ));
	Data->Curr_Location->at(selection)->set_name(new_name);
}
void Director::New(DataP Data){
	string node_name;
	cout<<"Enter Name of new node or enter 'Cancel' / 'Abort' to cancel command: ";
	cin >> node_name;
	if(!strcasecmp(node_name.c_str(), "Cancel") || !strcasecmp(node_name.c_str(), "Abort") 
		|| !strcasecmp(node_name.c_str(), "c") || !strcasecmp(node_name.c_str(), "a") )
		return;
	
	if(Data->Curr_Location->isHier())
		Data->Curr_Location->AddCategory(node_name);
	else if(Data->Curr_Location->isCat())
		Data->Curr_Location->AddSub(node_name);
	else if(Data->Curr_Location->isSub()){
		string date,time;
		cout<<"Enter date of Event: ";
		cin>>date;
		cout<<"Enter time of Event: ";
		cin>>time;
		Data->Curr_Location->AddEvent(node_name,date,time);
	}
	else if(Data->Curr_Location->isEvent())
		Data->Curr_Location->AddMarket(node_name);
	else{
		string price;
		cout<<"Enter price of selection: ";
		cin>>price;
		Data->Curr_Location->AddSelection(node_name,price);
	}
	Data->LogsP->push_back(new Log(user_id, "Added a new node " + node_name, 
		Data->Curr_Location->at(Data->Curr_Location->get_size() )->get_node_id() ));
}

void Director::Copy(DataP Data){
	int selection=process_input(Data);
	if(!selection)
		return;
	
	if(!Data->Curr_Location->isSelection())
		Data->Curr_Location->create_copy(selection-1);
	else
		cout<<"Nothing to copy"<<endl;
}

void Director::Delete(DataP Data){
	int selection = process_input(Data);
	if(!selection)
		return;
	cout << "About to delete node " << Data->Curr_Location->get_node_id()
		<< " - " << Data->Curr_Location->get_name() << "\nConfirm(Y/N): ";
	string input;
	cin >> input;
	if(!strcasecmp(input.c_str(), "N")){
		cout << "Aborting...\n";
		return;
	}
	cout << "Node deleted!\n";
	Data->LogsP->push_back(new Log(user_id, "Removed node ", 
		Data->Curr_Location->at(Data->Curr_Location->get_size() )->get_node_id() ));
	Data->Curr_Location->Remove(selection);
}

bool Director::isDirector(){return true;}

void Director::printOperations(DataP Data){
	cout << "Choose:\n"
		<<	"the ID of the node you want to visit\n"
		<<	"H (Home), to return to Home screen\n"
		<<	"T (Toggle), to change rates display\n"
		<<	"B (Bets), to display the last 20 bets\n"
		<<	"F (Freebets), to give a freebet to a user\n"
		<<	"S (Save), to save current situation and sychronize system files\n"
		<<	"Management:\n"
		<<	"L (Logs), to display the last  25 logs in system\n"
		<<	"U (Users), to edit users\n"
		<<	"V (Visibility), to change visibility of nodes\n"
		<<	"R (Rename), to rename a node\n"
		<<	"N (New), to add a new node\n"
		<<	"C (Copy), to copy a node (as invisible)\n"
		<<	"D (Delete), to delete a node\n"
		<<	"X (Exit), to exit system.\n\n";
		
	cout << "Location:/" << Data->Curr_Location->get_path() << endl;
	Data->Curr_Location->simple_print(Data);
}

int process_input(DataP Data){
	cout<< "Choose a selection or enter \"cancel\"\n";
	Data->Curr_Location->simple_print(Data);
	string input;
	int selection;
	cin >> input;
	if(!strcmp(input.c_str(), "Cancel") || !strcmp(input.c_str(), "Abort") 
		|| !strcmp(input.c_str(), "c") || !strcmp(input.c_str(), "a") ){
		return 0;
	}
	else if( isdigit(input[0])){
		if(!(selection= Data->Curr_Location->real_position(atoi(input.c_str()),Data))){
			cout<<"Out of boarder"<<endl;
			return 0;
		}
		//selection = atoi(input.c_str() );
		//selection=Data->Curr_Location->real_position(selection);
	}
	else{
		cout << "Unknown command\n";
		return 0;
	}
	if(selection < 0 || selection > Data->Curr_Location->at(0)->get_size() ){
		cout << "Invalid selection\n";
		return 0;
	}
	return selection;
}


void LoadUsers(vector<RegUser*> * Users){
	RegUser * p;
	ifstream users;
	users.open("users.csv");
	string line, username, password, fullname, type, status, balance, freebets;
	char cstr[500];
	char s1[100];
	char s2[100];
	char s3[100];
	int result;
	getline(users, line);
	
    while ( getline (users, line) ){
    	strcpy(cstr, line.c_str());
    	strtok(cstr, "\t" );
    	username = strtok(NULL, "\t" );
    	fullname = strtok(NULL, "\t" );
		password = strtok(NULL, "\t" );
		type = strtok(NULL, "\t" );
		strcpy(cstr, line.c_str());
		sscanf(cstr, "%*[^\t\n]\t%*[^\t\n]\t%*[^\t\n]\t%*[^\t\n]\t%*[^\t\n]\t %[^\n]", cstr); // skip 5 columns
    	switch(atoi(type.c_str() )){
			case 1:	result = sscanf(cstr, "%[^\t\n]\t%[^\t\n]\t%[^\t\n]", s1, s2 ,s3);	//Punter
					status = s1;
					balance  = s2; 
					if(result == 3) freebets = s3;
					else	freebets = string();
					Users->push_back(new Punter(username, password, fullname, status, atof(balance.c_str() ), freebets));
					break;
			case 2:	Users->push_back(new Trader(username, password, fullname));	//Trader
					break;
			case 3: Users->push_back(new Director(username, password, fullname));	//Director
					break;
		}
	}
	users.close();
}

void SaveUsers(vector<RegUser*>  * Users){
	ofstream usersf;
	usersf.open("users.csv", ofstream::out | ofstream::trunc); //clear file
	string tmp;
	usersf << "user_id	username	fullname	password	type	status	balance	freebets\n";
	for(int i = 0; i < Users->size(); i++){
		usersf << i + 1 << "\t" << (*Users)[i]->get_username() << "\t" 
			<< (*Users)[i]->get_fullname() << "\t" << (*Users)[i]->get_password() << "\t" 
			<< (*Users)[i]->get_type() << "\t"	<< (*Users)[i]->get_status() 
			<< ((*Users)[i]->get_status() == "L" ? ",":"") << (*Users)[i]->get_lockedreason() << "\t"
			<< (*Users)[i]->get_balance() << "\t" << (*Users)[i]->get_freebets() << endl;
	}
	usersf.close();
}
