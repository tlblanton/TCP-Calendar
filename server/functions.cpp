#include "functions.h"
#include <iostream>
#include <fstream>
#include "User.h"
#include <vector>
#include <sstream>
#include <string>
#include <iterator>


using namespace std;



bool UserExists(User user)
{

	ifstream inFile;
	inFile.open("logins.txt");
	string line;
	while(inFile >> line)
	{
		std::vector<string> vect;

		std::stringstream ss(line);	
		string token;

		getline(ss,token, ',');
		string i;

		if(token == user.username)
		{
			return true;
		}
	}	
	return false;
	
}


bool UserExists(string username)
{

	ifstream inFile;
	inFile.open("logins.txt");
	string line;
	while(inFile >> line)
	{
		std::vector<string> vect;

		std::stringstream ss(line);	
		string token;

		getline(ss,token, ',');
		string i;

		if(token == username)
		{
			return true;
		}
	}	
	return false;
	
}


/*
 * Returns error if there is an error. Returns empty string if all goes well
 */ 
string Login(string username, string password)
{
	if(UserExists(username))
	{
		vector<string> vect;
		ifstream infile("logins.txt");
		string line;
		
			
		while(infile >> line)
		{
			vector<string> result;
			stringstream ss(line);
		
			int count = 0;
			bool ourUsername = false;
			while( ss.good() )
			{
    				string substr;
    				getline( ss, substr, ',' );
    				result.push_back( substr );
				if(count == 0)
				{
					if(substr != username)
					{	
						count++;
						continue;
					}
					else
					{
						ourUsername = true;
					}
				}

				if(count == 1 && ourUsername)
				{
					string decryptedPass = XorEncryptionToggle(substr);
					cout << "decryptedPass: " << decryptedPass << " is being compared to password: " << password << endl; 
					if(decryptedPass == password)
					{
						return "";
					}
					return "invalid password";
				}
				count++;
			}
			//cout << "The results: " << endl;
			//for (int i=0; i< vect.size(); i++)
        		//	std::cout << vect.at(i)<<std::endl;			

		}


	

		return "--";


	}
	else
	{
		return "No existing user with username '" + username + "'. Type 'add user' to add new user";
	}
}


int AddUser(User newUser)
{
	if(UserExists(newUser))
	{
		return 1;	//Error
	}

	string encryptedPassword = XorEncryptionToggle(newUser.password);
	ofstream outfile;
	outfile.open("logins.txt", std::ios_base::app);

	outfile << newUser.username << "," << encryptedPassword << "," << newUser.phone << "," << newUser.email << endl;
	
	cout << "User added to file" << endl;	

	return 0;
}


void DeleteUser(string username)
{
	std::vector<string> lines;
	ifstream infile;
	infile.open("logins.txt");
	string line;

	while(infile >> line)
	{
		lines.push_back(line);
	}
	infile.close();

	for(int i = 0; i < lines.size(); ++i)
	{	
		std::stringstream ss(lines[i]);	
		string token;

		getline(ss,token, ',');
		string str;

		if(token == username)
		{
			lines.erase(lines.begin() + i, lines.begin() + (i+1));
		}
	}


	ofstream outfile("logins.txt");
	for(int i = 0; i < lines.size(); ++i)
	{
		outfile << lines[i] << endl;
	}
	outfile.close();
		
}


//
// This function encrypts or decrypts the string passed to it.
//
string XorEncryptionToggle(string toToggle)
{
	char key = '+';
	string returnStr = toToggle;
	for(int i = 0; i < toToggle.size(); ++i)
	{
		returnStr[i] = toToggle[i] ^ key;
	}

	return returnStr;

}


bool UpdateName(string oldName, string name)
{

	std::vector<string> lines;
	ifstream infile;
	infile.open("logins.txt");
	string line;

	while(infile >> line)
	{
		lines.push_back(line);
	}
	infile.close();

	string newLine = "";

	for(int i = 0; i < lines.size(); ++i)
	{	
		std::stringstream ss(lines[i]);	
		string token;

		getline(ss,token, ',');
		string str;

		if(token == oldName)
		{
			newLine = name + lines[i].substr(lines[i].find(oldName) + oldName.size());

			lines.erase(lines.begin() + i, lines.begin() + (i+1));
		}
	}

	lines.push_back(newLine);

	ofstream outfile("logins.txt");
	for(int i = 0; i < lines.size(); ++i)
	{
		outfile << lines[i] << endl;
	}
	outfile.close();

}

bool UpdatePassword(string username, string currentPass, string newPass)
{
	
	std::vector<string> lines;
	ifstream infile;
	infile.open("logins.txt");
	string line;

	while(infile >> line)
	{
		lines.push_back(line);
	}
	infile.close();

	string newLine = "";


	
	for(int i = 0; i < lines.size(); ++i)
	{	
		std::stringstream ss(lines[i]);	
		string token, name;

		getline(ss, name, ',');
		getline(ss, token, ',');
		string str;



		if(name == username)
		{
			
			newLine = name + "," + XorEncryptionToggle(newPass) + lines[i].substr(lines[i].find(XorEncryptionToggle(currentPass)) + currentPass.size());



			// This might be what is throwing the "out of range" error when trying to update twice in a row
			//newLine = name + lines.at(i+1);

			lines.erase(lines.begin() + i, lines.begin() + (i+1));

		}
	}

	lines.push_back(newLine);

	ofstream outfile("logins.txt");
	for(int i = 0; i < lines.size(); ++i)
	{
		outfile << lines[i] << endl;
	}
	outfile.close();

}

//---------------------------------------------------------//
//-------------------------- UPDATE PHONE------------------//
//---------------------------------------------------------//
bool UpdatePhone(string username, string newPhone)
{

	std::vector<string> lines;
	ifstream infile;
	infile.open("logins.txt");
	string line;

	while(infile >> line)
	{
		lines.push_back(line);
	}
	infile.close();

	string newLine = "";


	
	for(int i = 0; i < lines.size(); ++i)
	{	
		std::stringstream ss(lines[i]);	
		string token, name, password;

		getline(ss, name, ',');
		getline(ss, password,',');
		getline(ss, token, ',');
		string str;



		if(name == username)
		{
			newLine = name + "," + password + "," + newPhone + lines[i].substr(lines[i].find(token) + token.size());



			// This might be what is throwing the "out of range" error when trying to update twice in a row
			//newLine = name + lines.at(i+1);

			lines.erase(lines.begin() + i, lines.begin() + (i+1));

		}
	}

	lines.push_back(newLine);

	ofstream outfile("logins.txt");
	for(int i = 0; i < lines.size(); ++i)
	{
		outfile << lines[i] << endl;
	}
}

//----------------------------------------------//
////--------------- UPDATE EMAIL ---------------//
//----------------------------------------------//
bool UpdateEmail(string username, string newEmail)
{

	std::vector<string> lines;
	ifstream infile;
	infile.open("logins.txt");
	string line;

	while(infile >> line)
	{
		lines.push_back(line);
	}
	infile.close();

	string newLine = "";


	
	for(int i = 0; i < lines.size(); ++i)
	{	
		std::stringstream ss(lines[i]);	
		string token, name, password, phone;

		getline(ss, name, ',');
		getline(ss, password,',');
		getline(ss, phone, ',');
		getline(ss, token, ',');
		string str;

		if(name == username)
		{
			newLine = name + "," + password + "," + phone + + "," + newEmail;

			// This might be what is throwing the "out of range" error when trying to update twice in a row
			//newLine = name + lines.at(i+1);

			lines.erase(lines.begin() + i, lines.begin() + (i+1));
		}
	}

	lines.push_back(newLine);

	ofstream outfile("logins.txt");
	for(int i = 0; i < lines.size(); ++i)
	{
		outfile << lines[i] << endl;
	}
}


//-------------------------------------//
//----------- ADD APPT ----------------//
//-------------------------------------//
bool AddAppt(string username, string startTime, string endTime, string place, string content)
{
	
	std::vector<string> lines;
	ifstream infile;
	infile.open("appts.txt");
	string line;

	//TODO: User ifstream.getline() here to get full line. 
	while(getline(infile, line))
	{
		cout << "pushing back: " << line << endl;
		lines.push_back(line);
	}
	string newLine = username + "," + startTime + "," + endTime + "," + place + "," + content;

	cout << "new appt is: " << newLine << endl;
	lines.push_back(newLine);

	infile.close();

	ofstream outfile("appts.txt");
	for(int i = 0; i < lines.size(); ++i)
	{
		cout << "WRITING: " << lines[i] << endl;
		outfile << lines[i] << endl;
	}
	outfile.close();
}

//----------APPT CONFLICT: Returns bool--------------//
//pulls of appointments for the specifided user and checks if there is a conflict based on the start and end times
//
bool ApptConflict(string username, string startTime, string endTime)
{
	std::vector<string> lines;
	ifstream infile;
	infile.open("appts.txt");
	string line;

	while(getline(infile, line))
	{
		lines.push_back(line);
	}
	infile.close();

	string newLine = "";

	vector<string>appointments;
		
	for(int i = 0; i < lines.size(); ++i)
	{	
		std::stringstream ss(lines[i]);	
		string name, start, end, place, content;

		getline(ss, name, ',');
		getline(ss, start,',');
		getline(ss, end, ',');
		getline(ss, place, ',');
		getline(ss, content);

		if(name == username)
		{
			stringstream startss(start);
			stringstream endss(end);
			
			

			
			
		}

	}


	return false;
}




string GetApptContentsByUser(string username)
{

	std::vector<string> lines;
	ifstream infile;
	infile.open("appts.txt");
	string line;

	while(getline(infile, line))
	{
		lines.push_back(line);
	}
	infile.close();

	string newLine = "";


	string appointments;
	int appointmentNum = 1;	
	for(int i = 0; i < lines.size(); ++i)
	{	
		std::stringstream ss(lines[i]);	
		string name, start, end, place, content;

		getline(ss, name, ',');
		getline(ss, start,',');
		getline(ss, end, ',');
		getline(ss, place, ',');
		getline(ss, content);

		if(username == name)
		{
			stringstream newss;
			newss << appointmentNum;
			string num;
			newss >> num;

			appointments += num + ") " + content + "\n";
			appointmentNum++;
		}
		
	}


	return appointments;
}


bool DeleteAppt(string username, int toDelete)
{
	std::vector<string> lines;
	ifstream infile;
	infile.open("appts.txt");
	string line;

	while(getline(infile, line))
	{
		lines.push_back(line);
	}
	infile.close();

	
	int deleteNum = 1;

	for(int i = 0; i < lines.size(); ++i)
	{	
		std::stringstream ss(lines[i]);	
		string name;

		getline(ss, name, ',');
		string str;

		if(name == username)
		{
			if(deleteNum == toDelete)
			{
				lines.erase(lines.begin() + i, lines.begin() + (i+1));
				break;
			}
			else
			{
				deleteNum++;
			}
		}
	}


	ofstream outfile("appts.txt");
	for(int i = 0; i < lines.size(); ++i)
	{
		outfile << lines[i] << endl;
	}
	outfile.close();
		
}








