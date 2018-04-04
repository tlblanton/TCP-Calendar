

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <string>
#include "User.h"

using namespace std;

void CheckLogin(string username, string password);

string XorEncryptionToggle(string toToggle);

int AddUser(User newUser);

bool UserExists(string username);

bool CheckForExistingUserByUsername(User user);

void DeleteUser(string username);

string Login(string username, string password);

bool UpdateName(string oldName, string name);
bool UpdatePassword(string username, string currentPass, string newPass);
bool UpdatePhone(string username, string phone);
bool UpdateEmail(string username, string newEmail);

bool AddAppt(string username, string startTime, string endTime, string place, string content);
bool ApptConflict(string username, string startTime, string endTime);
string GetApptContentsByUser(string username);

bool DeleteAppt(string username, int toDelete);

#endif
