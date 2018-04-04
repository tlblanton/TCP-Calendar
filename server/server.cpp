/*
 * ** server.c -- a stream socket server demo
 * **/


/*
 * This code was originally taken from the examples provided by Professor Ra.
 * for use by Tyler Blanton for Intro to Computer Networks lab 2
 */




#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <string>
#include <iostream>
#include <regex>
#include "functions.h"
#include "User.h"


#define MYPORT 3780    // the port users will be connecting to

#define BACKLOG 10     // how many pending connections queue will hold

using namespace std;

void sigchld_handler(int s)
{
    while(wait(NULL) > 0);
}

int main(void)
{
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct sockaddr_in my_addr;    // my address information
    struct sockaddr_in their_addr; // connector's address information
    int sin_size;
    struct sigaction sa;
    int yes=1;
    char *recvbuf;
    char *caddr;
    int  numbytes;
    bool loginInProgress = false;
    std::string username, password, phone, email;
    string activeUser = "-1";	//username of active user

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

   printf("SOCK_FD=%d\n",sockfd);
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }
    
    my_addr.sin_family = AF_INET;         // host byte order
    my_addr.sin_port = htons(MYPORT);     // short, network byte order
    my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
    memset(&(my_addr.sin_zero), '\0', 8); // zero the rest of the struct

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    while(1) {  // main accept() loop
        sin_size = sizeof(struct sockaddr_in);
        if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, (socklen_t*)&sin_size)) == -1) {
            perror("accept");
            continue;
        }
        printf("server: got connection from %s\n",(char *) inet_ntoa(their_addr.sin_addr));
        if (!fork()) { // this is the child process
            close(sockfd); // child doesn't need the listener
	    recvbuf=(char *) calloc(128,sizeof(char));
            for(;;) {
		cout << "at beginning of for(;;)";
	    	numbytes=recv(new_fd,recvbuf,128,0);
		if (numbytes < 0) {
			perror("recv");
			close(new_fd);
	    		exit(1);
	    	}
                else if (numbytes==0 || strncmp(recvbuf,"bye",3)==0) {
			printf("Clinet(%s) has been disconnected\n",
                        (char *) inet_ntoa(their_addr.sin_addr));
			close(new_fd);
			exit(0);
	        }
		//------------------------Custom Handlers------------------------//
		//---------------------------------------------------------------//
		//---------------------------------------------------------------//

		//---------------------ADD USER--------------------//
		else if(strncmp(recvbuf, "add user", 8)==0)
		{
			string prompt = "Enter Username for new user: ";
			if(send(new_fd, prompt.c_str(), numbytes, 0)!= -1)
			{
				numbytes = recv(new_fd, recvbuf, 128, 0);
				username = recvbuf;
				prompt = "Enter Password for new user: ";
				if(send(new_fd, prompt.c_str(), numbytes, 0)!= -1)
				{
					numbytes = recv(new_fd, recvbuf, 128, 0);
					password = recvbuf;
					
					if(password.size()  < 8 || password.size() > 15)
					{
						strcpy(recvbuf, "Unable to add user. Invalid password. Must be between 8 and 16 characters.");
					}
					else
					{
					
						prompt = "Enter phone number for new user: ";
	
						send(new_fd, prompt.c_str(), numbytes, 0);
						numbytes = recv(new_fd, recvbuf, 128, 0);
						phone = recvbuf;
	
						prompt = "Enter email for new user: ";
						send(new_fd, prompt.c_str(), numbytes, 0);
						numbytes = recv(new_fd, recvbuf, 128, 0);
						email = recvbuf;
	
						User newUser(username, password, phone, email);
	
						int success = AddUser(newUser);
						
						if(success == 0)
						{
							strcpy(recvbuf, "User Added Successfully.");
						}	 
						else
						{
							strcpy(recvbuf, "Unable to add user. Username already in use by another account.");
						}	
					}
				}
			}
		}
	
		else if(strncmp(recvbuf, "delete user", 11)==0)
		{
			string prompt = "Enter username of user to be deleted: ";
			if(send(new_fd, prompt.c_str(), numbytes, 0) != -1)
			{
				numbytes = recv(new_fd, recvbuf, 128, 0);
				string username = recvbuf;
				DeleteUser(username);
			}
			strcpy(recvbuf, "Entry deleted.");
		}
		//-------------- LOGIN -------------------//
		else if(strncmp(recvbuf, "login", 8)==0)
		{
			string prompt = "Enter Username: ";
			if(send(new_fd, prompt.c_str(), numbytes, 0)!=-1)
			{
				numbytes = recv(new_fd, recvbuf, 128, 0);
				username = recvbuf;
				prompt = "Enter password: ";
				if(send(new_fd, prompt.c_str(), numbytes, 0) != -1)
				{
					numbytes = recv(new_fd, recvbuf, 128, 0);
					password = recvbuf;
					string error = Login(username, password);
					if(error == "")
					{
						strcpy(recvbuf, "User successfully logged in.");
						activeUser = username;
					}
					else
					{
						cout << "error" << endl;
						strcpy(recvbuf, "ERROR: Credentials error. Try typing [add user] to create new account.");
						cout << "revbuf: " << recvbuf << endl;
					}
				}
			}		
		}

		//---------------------- MODIFY USER -----------------------------//
		else if(strncmp(recvbuf, "modify user", 11)==0)
		{
			string prompt = "Enter username of user to edit: ";
			if(send(new_fd, prompt.c_str(), numbytes, 0) != -1)
			{
				numbytes = recv(new_fd, recvbuf, 128, 0);
				username = recvbuf;
				if(UserExists(username))
				{
					prompt = "what would you like to modify? select [name/password/phone number, email]";
					if(send(new_fd, prompt.c_str(), numbytes, 0) != -1)
					{

						numbytes = recv(new_fd, recvbuf, 128, 0);
						string response = recvbuf;
						if(response == "name")
						{
							prompt = "enter new name: ";
		
							if(send(new_fd, prompt.c_str(), numbytes, 0) != -1)
							{
								numbytes = recv(new_fd, recvbuf, 128, 0);
								string newName = recvbuf;
								UpdateName(username, newName);
								strcpy(recvbuf,"Name succesfully updated.");

							}

						}
						else if(response == "password")
						{
							
							prompt = "enter current password: ";	
							if(send(new_fd, prompt.c_str(), numbytes, 0) != -1)
							{
								numbytes = recv(new_fd, recvbuf, 128, 0);
								password = recvbuf;
								string error = Login(username, password);
								if(error == "")
								{
									prompt = "Enter new password: ";
									if(send(new_fd, prompt.c_str(), numbytes, 0) != -1)
									{
										

										numbytes = recv(new_fd, recvbuf, 128, 0);
										string newPassword = recvbuf;
										UpdatePassword(username, password, newPassword);
										strcpy(recvbuf, "Password successfully updated.");
									}
								}
								else
								{
									strcpy(recvbuf, "ERROR: Invalid password. New password not set");
								}
							}
						}
						else if(response == "phone number")
						{	
							prompt = "enter new phone number: ";	
							if(send(new_fd, prompt.c_str(), numbytes, 0) != -1)
							{
								numbytes = recv(new_fd, recvbuf, 128, 0);
								string newPhone = recvbuf;
								bool isSuccess = UpdatePhone(username, newPhone);
								if(isSuccess)
								{
									strcpy(recvbuf, "Phone number successfully updated.");
								}
								else
								{
									strcpy(recvbuf, "ERROR: Unable to update phone number. Please contact the engineer who coded this mess.");
								}
							}
						}
						else if(response == "email")
						{
							prompt = "enter new email: ";	
							if(send(new_fd, prompt.c_str(), numbytes, 0) != -1)
							{
								numbytes = recv(new_fd, recvbuf, 128, 0);
								string newEmail = recvbuf;
								bool isSuccess = UpdateEmail(username, newEmail);
								
								if(isSuccess)
								{
									strcpy(recvbuf, "Email number successfully updated.");
								}
								else
								{
									strcpy(recvbuf, "ERROR: Unable to update email. Please contact the engineer who coded this mess.");
								}
							}
						}
						else
						{
							strcpy(recvbuf, "invalid selection");
						}
					}
				}
				else
				{
					strcpy(recvbuf, "ERROR: No user exists with that username");
				}
			}
		}

		
		//------------------ ADD APPOINTMENT ----------------------//
		else if(strncmp(recvbuf, "add appt", 8) == 0)
		{
			string prompt = "enter username for appointment log: ";	
			if(send(new_fd, prompt.c_str(), numbytes, 0) != -1)
			{
				numbytes = recv(new_fd, recvbuf, 128, 0);
				username = recvbuf;
				if(username != activeUser)
				{
					prompt = "Enter password to login and add appt:";
					if(send(new_fd, prompt.c_str(), numbytes, 0) != -1)
					{
						numbytes = recv(new_fd, recvbuf, 128, 0);
						password = recvbuf;
						if(Login(username, password) != "")
						{
							//error case
							strcpy(recvbuf, "Unable to login user and so unable to add appt.");
						}
						else
						{
							activeUser = username;
						}
					}
				}
				if(activeUser == username)
				{
					bool valid = true;
					string startTime, endTime, place, content;
					//add appt here
					prompt = "Enter start date with format \"HH:MM MM/DD/YYYY\"";
					if(send(new_fd, prompt.c_str(), numbytes, 0) != -1)
					{
						numbytes = recv(new_fd, recvbuf, 128, 0);
						startTime = recvbuf;

						if(startTime.size() != 16)
						{
							strcpy(recvbuf, "Improper start time date format. Operation cancelled.");
							valid = false;
						}

					}
					
					if(valid)
					{
						prompt = "Enter end date with format \"HH:MM MM/DD/YYYY\"";
						if(send(new_fd, prompt.c_str(), numbytes, 0) != -1)
						{
							numbytes = recv(new_fd, recvbuf, 128, 0);
							endTime = recvbuf;
							if(endTime.size() != 16)
							{
								strcpy(recvbuf, "Improper end time date format. Operation Cancelled.");
								valid = false;
							}
						}
						
						if(valid)
						{
							if(ApptConflict(username, startTime, endTime))
							{
								strcpy(recvbuf, "ERROR: Time conflict. Unable to add appointment.");
							}
							else
							{
		
								prompt = "Enter place for appointment";
								if(send(new_fd, prompt.c_str(), numbytes, 0) != -1)
								{
									numbytes = recv(new_fd, recvbuf, 128, 0);
									place = recvbuf;
								}
		
								prompt = "Enter contents of appointment";
								if(send(new_fd, prompt.c_str(), numbytes, 0) != -1)
								{
									numbytes = recv(new_fd, recvbuf, 128, 0);
									content = recvbuf;
								}
		
								AddAppt(username, startTime, endTime, place, content);
		
		
								strcpy(recvbuf, "appt added successfully");
							}
						}
					}
				}
				
			}
		}

		else if(strncmp(recvbuf, "remove appt", 11) == 0)
		{
			if(activeUser != "-1")
			{
				string prompt = "Select which appt to remove for user " + username + ":\n";
				
				string appts = GetApptContentsByUser(username);

				if(appts == "")
				{
					strcpy(recvbuf, "Logged in user has no appointments to delete");
				}
				else
				{
					string toSend = prompt + appts;
					
					if(send(new_fd, toSend.c_str(), numbytes, 0) != -1)
					{
						numbytes = recv(new_fd, recvbuf, 128, 0);
						string apptToDeleteStr = recvbuf;
						int toDelete = stoi(apptToDeleteStr);
						
						DeleteAppt(username, toDelete);	
					}	
					strcpy(recvbuf, "Appointment removed successfully");	
				}
			}
			else
			{
				strcpy(recvbuf, "No user logged in. Please type \"login\" before trying to remove apppointment.");
			}
		}
		

		//------------------ VIEW APPTS ----------------------//
		else if(strncmp(recvbuf, "view appts", 10) == 0)
		{
			string prompt = "What woul you like to view apartments by? [year/month/day/time]";
			if(send(new_fd, prompt.c_str(), numbytes, 0) != -1)
			{
				numbytes = recv(new_fd, recvbuf, 128, 0);
				string response = recvbuf;
				if(response == "year")
				{
					strcpy(recvbuf, "yearly appts");
				}
				else if(response == "month")
				{
					strcpy(recvbuf, "monthly appts");
				}
				else if(response == "day")
				{
					strcpy(recvbuf, "daily appts");
				}
				else if(response == "time")
				{
					strcpy(recvbuf, "time appts");
				}
				else
				{
					strcpy(recvbuf, "Invalid selection. Returning to start.");
				}
			}
		}
		
		//------------------ HELP ----------------------//
		else if(strncmp(recvbuf, "help", 4) == 0)
		{

			strcpy(recvbuf,  "\nadd user\ndelete user\nmodify user\nadd appt\nremove appt\nmodify appt\nupdate appt\nview appts\n");
		}

		//--------------------------------------------------------------//
	
	printf("Received from %s: %s\n",inet_ntoa(their_addr.sin_addr),recvbuf); 
        if (send(new_fd, recvbuf, numbytes, 0) == -1) {
                perror("send");
		close(new_fd);
	    	exit(1);
            }
	}

	
            close(new_fd);
            exit(0);
        }
        close(new_fd);  // parent doesn't need this
    }

    return 0;
}
