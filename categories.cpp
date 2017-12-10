#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <fstream>
#include <sstream>
#include "categories.h"

using namespace std;

//**********Category***********//
Hierarchy::Hierarchy(string N, string path_) : Name(N), path(path_), node_id(""){
	
}

void Hierarchy::simple_print(DataP Data){
	int k = 0;
	for(int i = 0; i < cats.size(); i++)
		if(cats[i].get_visibility() || Data->Curr_user->isDirector()){
			cout << ++k << ". " << cats[i].get_name() << flush;
			if(!cats[i].get_visibility())
				cout << " (Invisible)" << flush;
			cout<<endl;
		}
}

void Hierarchy::AddCategory(string N){
	string new_path = this->path;
	new_path.append("/");
	new_path.append(N);
	ostringstream new_node_id;
	new_node_id << this->node_id << cats.size() + 1;
	
	cats.push_back(Category(N, new_path, new_node_id.str() ));
}

void Hierarchy::Remove(int pos){
	cats.erase(cats.begin() + pos - 1);
}

void Hierarchy::create_copy(int i){
	cats.push_back(cats[i]);
	cats[cats.size()-1].set_name(cats[i].get_name() + "-copied");
	cats[cats.size()-1].set_visibility();
}
void Hierarchy::print(){
	cout << Name << endl;
	for(int i=0;i<cats.size();i++){
		cout << i + 1 << "." << flush;
		cats[i].print();
	}
}
int Hierarchy::real_position(int pos,DataP Data){
	int count = 0;
	for(int i = 0; i < cats.size();i++){
		if(cats[i].get_visibility() || Data->Curr_user->isDirector()){
			count++;
			if(count == pos)
				return i + 1;
		}
	}
	return 0;
}

string Hierarchy::get_name(){
	return this->Name;
}
int Hierarchy::get_size(){
	return this->cats.size();
}
string Hierarchy::get_node_id(){
	return this->node_id;
}
string Hierarchy::get_path(){
	return this->path;
}
Location * Hierarchy::at(int i){
	if(i)
		return &cats[i - 1];
	return this;
}
void Hierarchy::set_name(string n){
	size_t found = path.find_last_of("/");
	path.replace(found+1,Name.length(),n);
	this->Name = n;
}


//**********Category***********//
Category::Category(string N, string path_, string node_id_) 
	: Name(N), path(path_), node_id(node_id_), visible(true){

}

//Destructor;;;;;

void Category::AddSub(string N){
	string new_path = this->path;
	new_path.append("/");
	new_path.append(N);
	ostringstream new_node_id;
	new_node_id << this->node_id << "." << subs.size() + 1;
	subs.push_back(SubCategory(N, new_path, new_node_id.str() ));
}

void Category::Remove(int pos){
	subs.erase( subs.begin() + pos  - 1);
}
void Category::create_copy(int i){
	subs.push_back(subs[i]);
	subs[subs.size()-1].set_name(subs[i].get_name() + "-copied");
	subs[subs.size()-1].set_visibility();
}

void Category::print(){
	cout<<Name<<endl;
	for(int i=0;i<subs.size();i++){
		cout <<"\t"<< i + 1 << "." << flush;
		subs[i].print();
	}

}
void Category::simple_print(DataP Data){
	int k = 0;
	for(int i = 0; i < subs.size(); i++)
		if(subs[i].get_visibility() || Data->Curr_user->isDirector()){
			cout << ++k << ". " << subs[i].get_name() << flush;
			if(!subs[i].get_visibility())
				cout<<" (Invisible)" << flush;
			cout<<endl;
		}
}

int Category::real_position(int pos,DataP Data){
	int count = 0;
	for(int i = 0; i < subs.size();i++){
		if(subs[i].get_visibility() || Data->Curr_user->isDirector()){
			count++;
			if(count == pos)
				return i + 1;
		}
	}	
}

string Category::get_name(){
	return this->Name;
}
int Category::get_size(){
	return this->subs.size();
}
string Category::get_node_id(){
	return this->node_id;
}
string Category::get_path(){
	return this->path;
}
bool Category::get_visibility(){
	return this->visible;
}
void Category::set_name(string n){
	this->Name=n;
}
void Category::set_visibility(){
	visible = !visible;
}
Location * Category::at(int i){
	if(i)
		return &subs[i - 1];
	return this;
}

//**********SubCategory***********//
SubCategory::SubCategory(string N, string path_, string node_id_) 
	: Name(N), path(path_), node_id(node_id_), visible(true){

}

//Destructor;;;;;


void SubCategory::AddEvent(string N,string D,string T){
	string new_path = this->path;
	new_path.append("/");
	new_path.append(N);
	ostringstream new_node_id;
	new_node_id << this->node_id << "." << events.size() + 1;
	
	events.push_back(Event(N, D, T, new_path, new_node_id.str() ));
}

void SubCategory::Remove(int pos){
	events.erase(events.begin() + pos - 1);
}
void SubCategory::create_copy(int i){
	events.push_back(events[i]);
	events[events.size()-1].set_name(events[i].get_name() + "-copied");
	events[events.size()-1].set_visibility();
}
void SubCategory::print(){
	cout<<Name<<endl;
	for(int i=0;i<events.size();i++){
		cout <<"\t\t"<< i + 1 << "." << flush;
		events[i].print();
	}
}

void SubCategory::simple_print(DataP Data){
	int k = 0;
	for(int i = 0; i < events.size(); i++)
		if(events[i].get_visibility() || Data->Curr_user->isDirector()){
			cout << ++k << ". " << events[i].get_name() << flush;
			if(!events[i].get_visibility())
				cout<<" (Invisible)" << flush;
			cout<<endl;
		}
}

int SubCategory::real_position(int pos,DataP Data){
	int count = 0;
	for(int i = 0; i < events.size();i++){
		if(events[i].get_visibility() || Data->Curr_user->isDirector()){
			count++;
			if(count == pos)
				return i + 1;
		}
	}	
}
string SubCategory::get_name(){
	return this->Name;
}
int SubCategory::get_size(){
	return this->events.size();
}
string SubCategory::get_node_id(){
	return this->node_id;
}
string SubCategory::get_path(){
	return this->path;
}
bool SubCategory::get_visibility(){
	return this->visible;
}
void SubCategory::set_name(string n){
	size_t found = path.find_last_of("/");
	path.replace(found+1,Name.length(),n);
	this->Name = n;
}
void SubCategory::set_visibility(){
	visible = !visible;
}
Location * SubCategory::at(int i){
	if(i)
		return &events[i - 1];
	return this;
}


//**********Event***********//
Event::Event(string N, string D, string T, string path_, string node_id_) 
	: Name(N), Date(D), Time(T), path(path_), node_id(node_id_), visible(true){

}

//Destructor;;;;;

void Event::AddMarket(string N){
	string new_path = this->path;
	new_path.append("/");
	new_path.append(N);
	ostringstream new_node_id;
	new_node_id << this->node_id << "." << market.size() + 1;
	
	market.push_back(Market(N, new_path, new_node_id.str() ));
}

void Event::Remove(int pos){
	market.erase(market.begin() + pos - 1);
}
void Event::create_copy(int i){
	market.push_back(market[i]);
	market[market.size()-1].set_name(market[i].get_name() + "-copied");
	market[market.size()-1].set_visibility();
}
void Event::print(){
	cout<<Name<<endl;
	for(int i=0;i<market.size();i++){
		cout <<"\t\t\t"<< i + 1 << "." << flush;
		market[i].print();
	}
}

void Event::simple_print(DataP Data){
	int k = 0;
	for(int i = 0; i < market.size(); i++)
		if(market[i].get_visibility() || Data->Curr_user->isDirector()){
			cout << ++k << ". " << market[i].get_name() << flush;
			if(!market[i].get_visibility())
				cout<<" (Invisible)" << flush;
			cout<<endl;
		}
}

int Event::real_position(int pos,DataP Data){
	int count = 0;
	for(int i = 0; i < market.size();i++){
		if(market[i].get_visibility() || Data->Curr_user->isDirector()){
			count++;
			if(count == pos)
				return i + 1;
		}
	}	
	return 0;
}

string Event::get_name(){
	return this->Name;
}
string Event::get_date(){
	return this->Date;
}
string Event::get_time(){
	return this->Time;
}
int Event::get_size(){
	return this->market.size();
}
string Event::get_node_id(){
	return this->node_id;
}
string Event::get_path(){
	return this->path;
}
bool Event::get_visibility(){
	return this->visible;
}
void Event::set_name(string n){
	size_t found = path.find_last_of("/");
	path.replace(found+1,Name.length(),n);
	this->Name = n;
}
void Event::set_visibility(){
	visible = !visible;
}
Location * Event::at(int i){
	if(i)
		return &market[i - 1];
	return this;
}

//**********Market***********//
Market::Market(string N, string path_, string node_id_) 
	: Name(N), path(path_), node_id(node_id_), visible(true){

}

//Destructor;;;;;


void Market::AddSelection(string N, string P){
	string new_path = this->path;
	new_path.append("/");
	new_path.append(N);
	ostringstream new_node_id;
	new_node_id << this->node_id << "." << selections.size() + 1;
	
	selections.push_back(Selection(N, P, new_path, new_node_id.str() ));
}

void Market::Remove(int pos){
	selections.erase(selections.begin() + pos - 1);
}
void Market::create_copy(int i){
	selections.push_back(selections[i]);
	selections[selections.size()-1].set_name(selections[i].get_name() + "-copied");
	selections[selections.size()-1].set_visibility();
}

void Market::print(){
	cout<<Name<<endl;
	for(int i=0;i<selections.size();i++){
		cout <<"\t\t\t\t"<< i + 1 << "." << flush;
		selections[i].print();
	}
}

int Market::real_position(int pos,DataP Data){
	int count = 0;
	for(int i = 0; i < selections.size();i++){
		if(selections[i].get_visibility() || Data->Curr_user->isDirector()){
			count++;
			if(count == pos)
				return i + 1;
		}
	}	
	return 0;
}


void Market::simple_print(DataP Data){
	int k = 0;
	for(int i = 0; i < selections.size(); i++)
		if(selections[i].get_visibility() || Data->Curr_user->isDirector()){
			cout << ++k << ". " << selections[i].get_name() << " # " 
				<< selections[i].get_print(Data->Toggle) << flush;
			if(!selections[i].get_visibility())
				cout<<" (Invisible)" << flush;
			cout<<endl;
		}
}


string Market::get_name(){
	return this->Name;
}
int Market::get_size(){
	return this->selections.size();
}
string Market::get_node_id(){
	return this->node_id;
}
string Market::get_path(){
	return this->path;
}
bool Market::get_visibility(){
	return this->visible;
}
void Market::set_won(int pos){
	selections[pos-1].set_result(1);
	for(int i = 0; i < selections.size() && i != pos - 1; i++)
		selections[i].set_result(2);
}
void Market::set_lost(int pos){
	selections[pos-1].set_result(2);
	if(selections.size() == 2)
		for(int i = 0; i < selections.size() && i != pos - 1; i++)
			selections[i].set_result(1);
}
void Market::set_voided(int pos){
	selections[pos-1].set_result(3);
}
void Market::set_visibility(){
	visible = !visible;
}

void Market::set_name(string n){
	size_t found = path.find_last_of("/");
	path.replace(found+1,Name.length(),n);
	this->Name = n;
}

Location * Market::at(int i){
	if(i)
		return &selections[i - 1];
	return this;
}

//**********Selection***********//
Selection::Selection(string N, string P, string path_, string node_id_) 
	: Name(N), strPrice(P), path(path_), node_id(node_id_), visible(true){
		
	if(P == "Won")
		this->result = 1;
	else if(P == "Lost")
		this->result = 2;
	else if(P == "Voided")
		this->result = 3;
	else{
		this->result = 0;
		char * s1, *s2, s3[20];
		strcpy(s3, strPrice.c_str());
		s1 = strtok(s3,"/");
		s2 = strtok(NULL, "\n");
		this->Price = 1 + atoi(s1) / (double) atoi(s2);
	}
}
void Selection::create_copy(int i){
	return;
}
//Destructor;;;;;

void Selection::print(){
	cout<<Name << " # " << Price << endl;
}
void Selection::simple_print(DataP Data){
	return;
}

int Selection::real_position(int pos,DataP Data){
	return pos;
}


string Selection::get_name(){
	return this->Name;
}
double Selection::get_price(){
	return this->Price;
}
string Selection::get_strprice(){
	return this->strPrice;
}
int Selection::get_size(){
	return 0;
}
string Selection::get_path(){
	return this->path;
}
string Selection::get_node_id(){
	return this->node_id;
}
string Selection::get_print(bool Toggle){
	if(result == 1)
		return string("Won");
	else if(result == 2)
		return string("Lost");
	else if(result == 3)
		return string("Voided");
	if(Toggle){
		ostringstream strs;
		strs << Price;
		return strs.str();
	}
	else
		return strPrice;
}
int Selection::get_result(){
	return this->result;
}
bool Selection::get_visibility(){
	return this->visible;
}
void Selection::set_result(int result_){
	if(result)
		return;
	this->result = result_;
}

void Selection::set_name(string n){
	size_t found = path.find_last_of("/");
	path.replace(found+1,Name.length(),n);
	this->Name = n;
}

void Selection::set_visibility(){
	visible = !visible;
}
Location * Selection::at(int i){
	return this;
}
void Selection::Remove(int pos){
	return;
}


Bet::Bet( int ui, string ni,double s, string r):user_id(ui),node_id(ni),stake(s),result(r){
	bet_id=++betid;
}

int Bet::get_bet_id(){
	return bet_id;
}
int Bet::get_user_id(){
	return user_id;
}
string Bet::get_node_id(){
	return node_id;
}
double Bet::get_stake(){
	return stake;
}
string Bet::get_result(){
	return result;
}
void Bet::set_result(string res, DataP Data){
	this->result = res;
	if(result == "V")
		for(int i = 0; i < Data->UsersP->size(); i++){
			if(user_id == (*Data->UsersP)[i]->get_user_id())
				(*Data->UsersP)[i]->set_balance((*Data->UsersP)[i]->get_balance() + stake);
		}
	else if(result == "W")
		for(int i = 0; i < Data->UsersP->size(); i++){
			if(user_id == (*Data->UsersP)[i]->get_user_id()){
				double temp_price = Data->Curr_Location->at(atoi(&node_id[8]))->get_price();
				double temp_balance = stake * temp_price;
				(*Data->UsersP)[i]->set_balance((*Data->UsersP)[i]->get_balance() + temp_balance*0.95);
				Data->sys_budget+=temp_balance*0.05;
			}
		}
	else{
		Data->sys_budget += stake;
	}
}

void Bet::print(){
	cout << bet_id << "\t" << user_id << "\t" << node_id << "\t" << stake << "\t" << result << endl;
}

Log::Log(int user_id_, string action_, string info_ ) : user_id(user_id_), action(action_), info(info_){
	//cout << "New Log: User_id:" << user_id << " - " << action <<  " - " << info << endl;
}
void Log::print(){
	if(info.empty())
		cout << "* User_id:" << user_id << " - " << action << endl;
	else
		cout << "* User_id:" << user_id << " - " << action <<  " - " << info << endl;
}
string Log::fprint(){
	ostringstream f;
	if(info.empty())
		f << "User_id: " << user_id << " - " << action << endl;
	else
		f << "User_id: " << user_id << " - " << action <<  " - " << info << endl;
	return f.str();
}


void LoadHierarchy(Hierarchy * h){
	string str_line;
	char line[500];
	int cat, sub, ev, mark, sel;
	int res, res2;
	double rate;
	char s1[50];
	char s2[50];
	char s3[50];
	string str1, str2, name, date, time;
	bool visible;

	ifstream hierarchy;
	hierarchy.open("hierarchy.dat");

	while ( getline (hierarchy, str_line) ){
		visible = true;
    	if( !strncmp(str_line.c_str(), "V", 1)){	//1st char of every line indicates visibility
    		visible = false;
    		str_line = str_line.substr(1);
    	}
		strncpy(line, str_line.c_str(), 499);
    	sscanf(line, "%50s %50[^\n]", s1, s2 );
    	res = sscanf(s1, "%d.%d.%d.%d.%d", &cat, &sub, &ev, &mark, &sel);
    	if(res == 1){ 					// Category
    		h->AddCategory(s2);
      		if(!visible){
	    		h->at(cat)->set_visibility();
	    	}
		}
    	else if(res == 2){ 			// SubCategory
    		h->at(cat)->AddSub(s2); 
     		if(!visible){
	    		h->at(cat)->at(sub)->set_visibility();
	    	}
    	}
      	else if(res == 3){			//Event
    		res2 = sscanf(s2, "%50[^-]-%50s %50s", s1, s2, s3);
    		name = s1;
    		if(res2 > 1)
				date = s2;
    		else
    			date = string();
    		if(res2 > 2)
				time = s3;
    		else
    			time = string();
    		h->at(cat)->at(sub)->AddEvent(name, date, time);
    		if(!visible){
	    		h->at(cat)->at(sub)->at(ev)->set_visibility();
	    	}

    	}
      	else if(res == 4){			//Market
    		h->at(cat)->at(sub)->at(ev)->AddMarket(s2);
    		if(!visible){
	    		h->at(cat)->at(sub)->at(ev)->at(mark)->set_visibility();
	    	}
    	}
      	else if(res == 5){				//Selection
      	    sscanf(s2, "%49[^#]#%49s", s1, s2);
      	    name = s1;
      	    str2 = s2;
      	    h->at(cat)->at(sub)->at(ev)->at(mark)->AddSelection(name, str2);
    		if(!visible){
	    		h->at(cat)->at(sub)->at(ev)->at(mark)->at(sel)->set_visibility();
	    	}
      	}
    }
    hierarchy.close();
}


void SaveHierarchy(Hierarchy * h){
	ofstream hierarchyf;
	hierarchyf.open("hierarchy.dat", ofstream::out | ofstream::trunc); //clear file

	Location * P;

	for(int i = 1; i <= h->get_size(); i++){ //Categories
		Location * P = h->at(i);
		if(!P->get_visibility())
			hierarchyf << "V";
		hierarchyf << P->get_node_id() << " " << P->get_name() << endl;
		for(int j = 1; j <= P->get_size(); j++){ //SubCategories
			Location * P = h->at(i)->at(j);
			if(!P->get_visibility())
				hierarchyf << "V";
			hierarchyf << P->get_node_id() << " " << P->get_name() << endl;
			for(int k = 1; k <= P->get_size(); k++){ //Events
				Location * P = h->at(i)->at(j)->at(k);
				if(!P->get_visibility())
					hierarchyf << "V";
				hierarchyf << P->get_node_id() << " "
						<< P->get_name() << "-" << P->get_date() << " " << P->get_time() << endl;
				for(int l = 1; l <= P->get_size(); l++){ //Markets
					Location * P = h->at(i)->at(j)->at(k)->at(l);
					if(!P->get_visibility())
						hierarchyf << "V";
					hierarchyf << P->get_node_id() << " " << P->get_name() << endl;
					for(int m = 1; m <= P->get_size(); m++){	//Selections
						Location * P = h->at(i)->at(j)->at(k)->at(l)->at(m);
						if(!P->get_visibility())
							hierarchyf << "V";
						hierarchyf << P->get_node_id() << " " << P->get_name() 
								<< "#" << P->get_print(false) << endl;
					}
				}
			}
		}
	}
	hierarchyf.close();
}

void LoadBets(vector<Bet*> * Bets){
	ifstream betsf;
	betsf.open("bets.csv");
	string line, bet_id, node_id, result;
	char cstr[500], * user_id, * stake;
	getline(betsf, line);

    while ( getline (betsf, line) ){
    	strncpy(cstr, line.c_str(), 499);
    	bet_id = strtok(cstr, "\t" );
    	user_id = strtok(NULL, "\t" );
		node_id = strtok(NULL, "\t" );
		stake = strtok(NULL, "\t" );
		result = strtok(NULL, "\n" ); 
		Bets->push_back(new Bet(atoi(user_id ), node_id, atof(stake), result));
	}
	betsf.close();
}

void SaveBets(vector<Bet*> * Bets){
	ofstream betsf;
	betsf.open("bets.csv", ofstream::out | ofstream::trunc); //clear file

	betsf << "bet_id\tuser_id\tnode_id\tstake\tresult\n";
	for(int i = 0; i < Bets->size(); i++){
		betsf << i + 1 << "\t" << (*Bets)[i]->get_user_id() << "\t" << (*Bets)[i]->get_node_id() << "\t"
			<< (*Bets)[i]->get_stake() << "\t" << (*Bets)[i]->get_result() << endl;
	}
	betsf.close();
}

void LoadLogs(vector<Log*> * Logs){
	ifstream logsf;
	logsf.open("audit.log");
	int id, res;
	char cstr[500];
	char s1[100];
	char s2[100];
	string line;

	while ( getline (logsf, line) ){
  		strncpy(cstr, line.c_str(), 499);
    	res = sscanf(cstr, "%*s %d - %[^-\n]-%[^\n]", &id, s1, s2);
    	if(!res)
    		break;
    	if(res < 2)
    		strcpy(s1, "");
    	if(res < 3)
    		strcpy(s2, "");

		Logs->push_back(new Log(id, s1, s2));
	}
	logsf.close();
}

void SaveLogs(vector<Log*> * Logs){
	ofstream logsf;
	logsf.open("audit.log", ofstream::out | ofstream::trunc); //clear file

	for(int i = 0; i < Logs->size(); i++)
		logsf << (*Logs)[i]->fprint();
	logsf.close();
}
