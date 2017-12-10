#ifndef CATEGORIES_H
#define CATEGORIES_H

#include <iostream>
#include <cstring>
#include <vector>
#include "users.h"

using namespace std;

class Location;
class Hierarchy;
class Category;
class SubCategory;
class Event;
class Market;
class Selection;
class Bet;

void LoadHierarchy(Hierarchy *);
void SaveHierarchy(Hierarchy *);
void LoadBets(vector<Bet*> * );
void SaveBets(vector<Bet*> *);
void LoadLogs(vector<Log*> * );
void SaveLogs(vector<Log*> *);

class Location{
	public:
		virtual void AddCategory(string){return;}
		virtual void AddSub(string){return ;	}
		virtual void AddEvent(string, string, string){return ;		}
		virtual void AddMarket(string){return ;		}
		virtual void AddSelection(string, string ){return ;		}
		virtual void Remove(int) = 0;
		virtual void create_copy(int) = 0;
		virtual void print() = 0;
		virtual void simple_print(DataP) = 0;
		virtual int real_position(int ,DataP) = 0;
		virtual bool isHier(){return false;	}
		virtual bool isCat(){return false;	}
		virtual bool isSub(){return false;	}
		virtual bool isEvent(){return false;	}
		virtual bool isMarket(){return false; }
		virtual bool isSelection(){return false;	}
		virtual string get_name() = 0;
		virtual int get_size() = 0;
    	virtual string get_date(){return string();		}
    	virtual string get_time(){return string();		}
		virtual string get_path() = 0;
		virtual string get_node_id() = 0;
		virtual bool get_visibility( ){return true;	}
		virtual string get_strprice(){return string();	}
		virtual string get_print(bool ){return string();		}
		virtual int get_result(){return 0;		}
		virtual double get_price(){return 0;	}
		virtual void set_name(string){return;	}
		virtual void set_result(int ){return ;	}
		virtual void set_won(int ){return ;	}
    	virtual void set_lost(int ){return ;	}
    	virtual void set_voided(int ){return ;	}
    	virtual void set_visibility( ){return ;	}
		virtual Location * at(int) = 0;
};

class Hierarchy: public Location{
	private:
		string Name;
		string node_id;
  		vector<Category> cats;
  		string path;
  		bool visible;
	public:
  		Hierarchy(string, string);
    	//void printLocation();
    	void AddCategory(string);
    	void Remove(int );
    	void create_copy(int );
    	void print();
		void simple_print(DataP);
		int real_position(int ,DataP);
    	string get_name();
    	int get_size();
    	string get_path();
    	string get_node_id();
    	void set_name(string);
    	bool isHier(){return true;	}
    	Location * at(int);	
};

class Category: public Location{
	private:
		string Name;
		string node_id;
  		vector<SubCategory> subs;
  		string path;
  		bool visible;
	public:
  		Category(string, string, string);
    	//~Category();
    	void printLocation();
    	void AddSub(string);
    	void Remove(int );
    	void create_copy(int );
		void print();
    	void simple_print(DataP);
		int real_position(int ,DataP);
    	string get_name();
    	int get_size();
    	string get_path();
    	string get_node_id();
    	bool get_visibility();
    	void set_visibility();
    	bool isCat(){return true;	}
    	void set_name(string);
		Location * at(int);
};

class SubCategory: public Location{
	private:
		string Name;
		string node_id;
		vector<Event> events;
		string path;
		bool visible;
  	public:
  		SubCategory(string, string, string);
    	//~SubCategory();
    	void AddEvent(string,string,string);
    	void Remove(int );
    	void create_copy(int );
		void print();
    	void simple_print(DataP);
    	int real_position(int ,DataP);
    	string get_name();
    	int get_size();
    	string get_path();
    	string get_node_id();
    	bool get_visibility();
    	void set_visibility();
    	bool isSub(){return true;	}
    	void set_name(string);
    	Location * at(int);
};
  
class Event: public Location{
	private:
		string Name,Date,Time;
		string node_id;
  		vector<Market> market;
  		string path;
  		bool visible;
 	public:
  		Event(string,string,string, string, string);
    	//~Event();
    	void AddMarket(string);
    	void Remove(int );
    	void create_copy(int );
		void print();
    	void simple_print(DataP);
    	int real_position(int ,DataP);
    	string get_name();
    	string get_date();
    	string get_time();
    	int get_size();
    	string get_path();
    	string get_node_id();
    	bool get_visibility();
    	void set_visibility();
    	Location * at(int);
    	void set_name(string);
    	bool isEvent(){return true;	}
};

class Market: public Location{
	private:
		string Name;
		string node_id;
  		vector<Selection> selections;
  		string path;
  		bool visible;
	public:
  		Market(string, string, string);
    	//~Market();
    	void AddSelection(string ,string );
    	void Remove(int );
    	void create_copy(int );
		void print();
    	void simple_print(DataP);
    	int real_position(int ,DataP);
    	string get_name();
    	int get_size();
    	string get_path();
    	string get_node_id();
    	bool get_visibility();
    	void set_visibility();
    	void set_won(int );
    	void set_lost(int );
    	void set_voided(int );
    	bool isMarket(){return true;	}
    	void set_name(string);
    	Location * at(int);
};

class Selection: public Location{
	private:
		string Name;
		string node_id;
  		double Price;
  		string strPrice;
  		string path;
  		int result;
  		bool visible;
  	public:
  		Selection(string ,string, string, string );
    	//~Selection();
    	void create_copy(int );
    	void print();
    	string get_name();
    	void simple_print(DataP);
    	int real_position(int ,DataP);
    	double get_price();
    	int get_size();
    	string get_path();
    	string get_node_id();
    	string get_strprice();
    	string get_print(bool );
    	int get_result();
    	bool get_visibility();
    	void set_result(int );
    	void set_name(string);
    	void set_visibility();
    	bool isSelection(){return true;	}
    	Location * at(int);
    	void Remove(int );
    	
};

class Bet{
	private:
		int bet_id;
		int user_id;
		string node_id;
		string result;
		double stake;
	public:
		static int betid;
		Bet(int, string, double, string);
		int get_bet_id();
		int get_user_id();
		string get_node_id();
		double get_stake();
		string get_result();
		void set_result(string, DataP);
		void print();
};

class Log{
	private:
		int user_id;
		string action;
		string info;
	public:
		Log(int, string, string);
		void print();
		string fprint();
};

#endif
