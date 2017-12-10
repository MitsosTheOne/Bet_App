#ifndef USERS_H
#define USERS_H

#include <iostream>
#include <cstring>
#include <vector>

using namespace std;

class User;
class RegUser;
class Hierarchy;
class Bet;
class Log;
class Location;

typedef struct {
		vector<RegUser*> * UsersP;
		User * Curr_user;
		Hierarchy * HierarchyP;
		vector<Bet*> * BetsP;
		Location * Curr_Location;
		vector<Log*> * LogsP;
		bool Toggle;
		double sys_budget;
}DataNode;
typedef DataNode * DataP;

void delete_data(DataP);
int process_input(DataP);

void LoadUsers(vector<RegUser*> *);
void SaveUsers(vector<RegUser*> *);

class User{
	public:
    	virtual void Operate(string, DataP) = 0;
    	virtual bool islocked(string*);
    	virtual void printOperations(DataP) = 0;
    	virtual int get_user_id() = 0;
    	virtual bool isDirector(){return false;	}
};

class RegUser : public User{
	protected:

		string username;
		string pass;
		string fullname;
		int user_id;
	public:
		static int ID;
		RegUser(string, string, string );
		string get_username();
		string get_password();
		string get_fullname();
		virtual string get_status();
		virtual string get_lockedreason();
		virtual double get_balance();
		virtual void set_balance(double ){return;	}
		virtual string get_freebets();
		virtual void set_freebets(string);
		virtual void add_freebets(string ){return;		}
		virtual void set_status(string ){return;	}
		virtual void set_lockedreason(string ){return;	}
		virtual int get_type() = 0;
		int get_user_id();
		virtual bool islocked(string*);
		virtual void printOperations(DataP) = 0;
		bool auth(string);
    	virtual void Operate(string, DataP){return;	}
    
};


class Guest : public User{	
	public:
		void printOperations(DataP);
		int get_user_id(){return 0;		}
		void Operate(string, DataP);
		void Home(DataP);
		void Toggle(DataP);
};

class Punter : public RegUser{
	
	double balance;
	string Locked_reason;
	string status;
	string freebets;
	vector<double> freebetsV;
	
	public:
		Punter(string , string, string, string, double, string );
		Punter();
		bool islocked(string*);
		void Operate(string, DataP);
		void printOperations(DataP);
		string get_status();
		void set_status(string );
		string get_lockedreason();
		void set_lockedreason(string );
		double get_balance();
		void set_balance(double );
		string get_freebets();
		void set_freebets(string );
		void add_freebets(double );
		int get_type(){return 1;}
		void Home(DataP);
		void Toggle(DataP);
		void Account(DataP);
		void Place(DataP);
};

class Trader : public RegUser{

	public:
		Trader(string, string, string);
		int get_type(){return 2;}
		void printOperations(DataP);
		void Operate(string, DataP);
		void Home(DataP);
		void Toggle(DataP);
		void Void(DataP);
		void Settle(DataP);
		void Bets(DataP);
		void Freebets(DataP);
};

class Director : public RegUser{

	public:
		Director(string, string, string);
		int get_type(){return 3;}
		void printOperations(DataP);
		void Operate(string, DataP);
		void Home(DataP);
		void Toggle(DataP);
		void Bets(DataP);
		void Freebets(DataP);
		void Settle(DataP);
		void Void(DataP);
		void Save(DataP);
		void Logs(DataP);
		void Users(DataP);
		void Visibility(DataP);
		void Rename(DataP);
		void New(DataP);
		void Copy(DataP);
		void Delete(DataP);
		bool isDirector();
};




#endif	
