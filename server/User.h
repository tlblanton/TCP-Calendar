#ifndef USER_H
#define USER_H


#include <iostream>
#include <string>

using namespace std;


class User
{
public:
	string username, password, phone, email;

	User(string u, string p, string ph, string e)
	{
		username = u;
		password = p;
		phone = ph;
		email = e;
	}
};



#endif
