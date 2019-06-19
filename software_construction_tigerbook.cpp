#include <string>
#include <iostream>
#include <fstream>
#include <sstream>        //Brian Brewster
using namespace std;  //bab0096
					  //bab0096_2.cpp
class Group
{
public:
	Group();
	Group(string groupname);
	string getName();
private:
	string groupName;
};
class User
{
public:
	User();
	User(string username);
	string getName();
	string getGroups();
	string getFile();
	bool getCheck();
	void setCheck(bool check);
private:
	string userName;
	bool filecheck;
};
class MessageBuffer
{
public:
	void messageFormat(string message, string groupname, string username);
	void storeMessage(string message, string filename);
	string getMessagesFromHomeGroup(string groupname);
	string getMessagesFromWallGroup(string groupname);
	int getNumOfMessagesFromGroup(string groupname);
	int getNumOfMessagesFromWallGroup(string groupname);
	int formatHomePageFile();
	int formatWallPageFile();
	void printHomePage(int i);
	void printWallPage(int i);
	string findHomeMessage(int i);
	string findWallMessage(int i);
	string findHomeNum(int i);
	string findWallNum(int i);
	void deleteMessage(string message, string tNum);
	string homeMessageFormat(string message);
	string wallMessageFormat(string message);
private:
	string print[100];
};
class System
{
public:
	void unicast(string recipient);
	void broadcast(string groupname);
	void multicast(string groupname);
	void createUser(string username);
	void createGroup(string groupname);
	void joinGroup(string groupname);
	bool isGroup(string groupname);
	bool isUser(string username);
	int getCtr();
	void displayMenu();
	User getUser();
private:
	MessageBuffer messageBuffer;
	ifstream read;
	ofstream write;
	User activeUser;
	int ctr;
} tigerbook;

int main()
{
	tigerbook.displayMenu();
	return 0;
}

//group constructor
Group::Group()
{

}
//group constructor
Group::Group(string groupname)
{
	groupName = groupname;
}
//return groupname
string Group::getName()
{
	return groupName;
}

//user constructor
User::User()
{

}
//User constructor
User::User(string username)
{
	userName = username;
}
//return username of active user
string User::getName()
{
	return userName;
}
//get a list of groups joined by user
string User::getGroups()
{
	string temp;
	string result;
	string filename = getName() + "_groups.txt";
	ifstream read;
	read.open(filename.c_str());
	if (read.is_open())
	{
		while (getline(read, temp))
		{
			result = temp + "\n" + result;
		}
	}

	else
	{
		cout << "Unable to open file.";
	}
	read.close();
	return result;
}
//returns messages sent to user through unicast as string
string User::getFile()
{
	string temp, result, filename;
	filename = "_" + getName() + ".txt";
	ifstream read;
	read.open(filename.c_str());
	if (read.is_open())
	{
		while (getline(read, temp))
		{
			result = temp + result;
		}
	}
	read.close();
	return result;
}
//returns true if user has been sent atleast one message
bool User::getCheck()
{
	return filecheck;
}
//sets to true when sent a user is sent a message
void User::setCheck(bool check)
{
	filecheck = check;
}

//format message and send to be stored
void MessageBuffer::messageFormat(string message, string groupname, string username)
{
	int ctr2 = tigerbook.getCtr();
	string result;
	ifstream ifdummy;
	ofstream ofdummy;
	ostringstream convert;
	ifdummy.open("time.txt");
	if (ifdummy.fail())
	{
		//need to write in 0 value to time file to extract for format.
		ofdummy.open("time.txt");
		ofdummy << ctr2;

		//format
		convert << ctr2;
		result = "{!" + convert.str() + "!}" + "<!" + username + "!>" + message;

		ifdummy.close();
		ofdummy.close();
		storeMessage(result, groupname);
	}

	else if (ifdummy.good())
	{
		//need to read in value from time file for message format
		ifdummy >> ctr2;


		//need to format
		convert << ctr2;
		result = "{!" + convert.str() + "!}" + "<!" + username + "!>" + message;
		
		ifdummy.close();
		ofdummy.close();
		storeMessage(result, groupname);
	}

	else
	{
		cout << "Unable to open file." << endl;
	}
}
//stores message in correct file
void MessageBuffer::storeMessage(string message, string filename)
{
	//store message in proper text file.
	ofstream myFile;
	string result = "_" + filename + ".txt";
	myFile.open(result.c_str(), ios::app);
	myFile << message << endl;
	myFile.close();
}
//returns string for group's messages
string MessageBuffer::getMessagesFromHomeGroup(string groupname)
{
	string result, temp;
	string filename = "_" + groupname + ".txt";
	ifstream read;
	read.open(filename.c_str());
	if (read.is_open())
	{
		while (getline(read, temp))
		{
			result = "(" + groupname + ")" + temp + "\n" + result;
		}
	}

	else
	{
		//cout << "Unable to open file.";
	}
	read.close();
	return result;
}
//returns string for group's messages
string MessageBuffer::getMessagesFromWallGroup(string groupname)
{
	string result, temp, name;
	string filename = "_" + groupname + ".txt";
	ifstream read;
	int i;
	read.open(filename.c_str());
	if (read.is_open())
	{
		while (getline(read, temp))
		{
			i = temp.find("<!");
			name = temp.substr(i + 2, tigerbook.getUser().getName().length());
			if (name == tigerbook.getUser().getName())
			{
				result = "(" + groupname + ")" + temp + "\n" + result;
			}
		}
	}

	else
	{
		//cout << "Unable to open file.";
	}
	read.close();
	return result;
}
//returns num of messages for home info
int MessageBuffer::getNumOfMessagesFromGroup(string groupname)
{
	string result, temp;
	int i = 0;
	string filename = "_" + groupname + ".txt";
	ifstream read;
	read.open(filename.c_str());
	if (read.is_open())
	{
		while (getline(read, temp))
		{
			i++;
		}
	}

	else
	{
		//cout << "Unable to open file.";
	}
	read.close();
	return i;
}
//return num of messages for wall info
int MessageBuffer::getNumOfMessagesFromWallGroup(string groupname)
{
	string result, temp, name;
	int i = 0;
	string filename = "_" + groupname + ".txt";
	ifstream read;
	read.open(filename.c_str());
	if (read.is_open())
	{
		while (getline(read, temp))
		{
			i = temp.find("<!");
			name = temp.substr(i + 2, tigerbook.getUser().getName().length());
			if (name == tigerbook.getUser().getName())
			{
				i++;
			}
		}
	}

	else
	{
		//cout << "Unable to open file.";
	}
	read.close();
	return i;
}
//formats all home page information to one file to be sorted through
int MessageBuffer::formatHomePageFile()
{
	string result, temp, ifilename, ofilename;
	int i = 0;
	int j;
	ifilename = tigerbook.getUser().getName() + "_groups.txt";
	ofilename = tigerbook.getUser().getName() + "_homepage.txt";
	ifstream read;
	ofstream write;
	read.open(ifilename.c_str());

	if (read.is_open())
	{
		while (getline(read, temp))
		{
			result = getMessagesFromHomeGroup(temp) +  result;
			j = getNumOfMessagesFromGroup(temp);
			i = i + j;
		}
	}
	read.close();

	ifilename = "_" + tigerbook.getUser().getName() + ".txt";
	read.open(ifilename.c_str());
	if (read.is_open())
	{
		while (getline(read, temp))
		{
			result = temp + "\n" + result;
			i++;
		}
	}
	read.close();

	//need to write result into a new file here which contains all the information for a user's homepage
	write.open(ofilename.c_str());
	write << result;
	return i;
}
//formats all wall page information to one file to be sorted through
int MessageBuffer::formatWallPageFile()
{
	string result, temp, ifilename, ofilename;
	int i = 0;
	int j;
	ifilename = tigerbook.getUser().getName() + "_groups.txt";
	ofilename = tigerbook.getUser().getName() + "_wallpage.txt";
	ifstream read;
	ofstream write;
	read.open(ifilename.c_str());

	if (read.is_open())
	{
		while (getline(read, temp))
		{
			result = getMessagesFromWallGroup(temp) + result;
			j = getNumOfMessagesFromWallGroup(temp);
			i = i + j;
		}
	}
	read.close();

	ifilename = "users.txt";
	read.open(ifilename.c_str());
	if (read.is_open())
	{
		while (getline(read, temp))
		{
			result = getMessagesFromWallGroup(temp) + result;
			j = getNumOfMessagesFromWallGroup(temp);
			i = i + j;
		}
	}
	read.close();

	//need to write result into a new file here which contains all the information for a user's homepage
	write.open(ofilename.c_str());
	write << result;
	return i;
}
//prints home page
void MessageBuffer::printHomePage(int i)
{
	string num, result, temp;
	if (i == 1)
	{
		temp = findHomeMessage(i);
		num = findHomeNum(i);
		result = homeMessageFormat(temp);
		cout << endl << result << endl;
		deleteMessage(temp, num);
		print[0] = result;
	}

	else
	{
		int j = 2;
		while (j != 0)
		{
			temp = findHomeMessage(i);
			num = findHomeNum(i);
			result = homeMessageFormat(temp);
			cout << endl << result << endl << endl;
			deleteMessage(temp, num);
			i--;
			print[i] = result;
			j--;
		}

		if (i != 0)
		{
			string answer;
			cout << "Would you like to see more messages? (yes/no): ";
			cin >> answer;

			if (answer == "yes")
			{
				while (i != 0)
				{
					temp = findHomeMessage(i);
					num = findHomeNum(i);
					result = homeMessageFormat(temp);
					cout << endl << result << endl;
					deleteMessage(temp, num);
					i--;
					print[i] = result;
				}
			}
		}
	}
}
//prints wall page
void MessageBuffer::printWallPage(int i)
{
	string num, result, temp;

	if (i == 1)
	{
		temp = findWallMessage(i);
		num = findWallNum(i);
		result = wallMessageFormat(temp);
		cout << endl << result << endl;
		deleteMessage(temp, num);
	}

	else
	{
		int j = 2;
		while (j != 0)
		{
			temp = findWallMessage(i);
			num = findWallNum(i);
			result = wallMessageFormat(temp);
			cout << endl << result << endl;
			deleteMessage(temp, num);
			i--;
			j--;
		}

		if (i != 0)
		{
			string answer;
			cout << "Would you like to see more messages? (yes/no): ";
			cin >> answer;

			if (answer == "yes")
			{
				while (i != 0)
				{
					temp = findWallMessage(i);
					num = findWallNum(i);
					result = wallMessageFormat(temp);
					cout << endl << result << endl;
					deleteMessage(temp, num);
					i--;
				}
			}
		}
	}
}
//returns the most recent message in the given file
string MessageBuffer::findHomeMessage(int i)
{
	string num, num2, filename, temp, result;
	int index, index2;
	ifstream read;
	filename = tigerbook.getUser().getName() + "_homepage.txt";
	read.open(filename.c_str());

	if (i = 1)
	{
		getline(read, temp);
		read.close();
		return temp;
	}

	getline(read, temp);
	index = temp.find("!");
	index2 = temp.find("!", index + 1);
	if (index2 - index == 2)
	{
		num = temp.at(index + 1);
	}
	else
	{
		num = temp.substr(index + 1, 2);
	}

	while (getline(read, temp))
	{
		index = temp.find("!");
		index2 = temp.find("!", index + 1);
		if (index2 - index <= 2)
		{
			num2 = temp.at(index + 1);
			if (num2 > num) num = num2;
		}
		else
		{
			num2 = temp.substr(index + 1, 2);
			if (num2 > num) num = num2;
		}

	}
	read.close();

	read.open(filename.c_str());
	while (getline(read, temp))
	{
		index = temp.find("!");
		index2 = temp.find("!", index + 1);
		if (index2 - index <= 2)
		{
			num2 = temp.at(index + 1);
			if (num2 == num)
			{
				return temp;
			}
		}
		else
		{
			num2 = temp.substr(index + 1, 2);
			if (num2 == num)
			{
				return temp;
			}
		}
	}
	return "";
}
//returns the most recent message in the given file
string MessageBuffer::findWallMessage(int i)
{
	string num, num2, filename, temp, result;
	int index, index2;
	ifstream read;
	filename = tigerbook.getUser().getName() + "_wallpage.txt";
	read.open(filename.c_str());

	if (i = 1)
	{
		getline(read, temp);
		read.close();
		return temp;
	}

	getline(read, temp);
	index = temp.find("!");
	index2 = temp.find("!", index + 1);
	if (index2 - index == 2)
	{
		num = temp.at(index + 1);
	}
	else
	{
		num = temp.substr(index + 1, 2);
	}

	while (getline(read, temp))
	{
		index = temp.find("!");
		index2 = temp.find("!", index + 1);
		if (index2 - index <= 2)
		{
			num2 = temp.at(index + 1);
			if (num2 > num) num = num2;
		}
		else
		{
			num2 = temp.substr(index + 1, 2);
			if (num2 > num) num = num2;
		}

	}
	read.close();

	read.open(filename.c_str());
	while (getline(read, temp))
	{
		index = temp.find("!");
		index2 = temp.find("!", index + 1);
		if (index2 - index <= 2)
		{
			num2 = temp.at(index + 1);
			if (num2 == num)
			{
				return temp;
			}
		}
		else
		{
			num2 = temp.substr(index + 1, 2);
			if (num2 == num)
			{
				return temp;
			}
		}
	}
	return "";
}
//find time stamp of latest message
string MessageBuffer::findHomeNum(int i)
{
	string num, num2, filename, temp, result;
	int index, index2;
	ifstream read;
	filename = tigerbook.getUser().getName() + "_homepage.txt";
	read.open(filename.c_str());

	if (i = 1)
	{
		getline(read, temp);
		index = temp.find("!");
		index2 = temp.find("!", index + 1);
		num = temp.substr(index + 1);
		read.close();
		return num;
	}

	getline(read, temp);
	index = temp.find("!");
	index2 = temp.find("!", index + 1);
	if (index2 - index <= 2)
	{
		num = temp.at(index + 1);
	}
	else
	{
		num = temp.substr(index + 1, 2);
	}
	
	while (getline(read, temp))
	{
		index = temp.find("!");
		index2 = temp.find("!", index + 1);
		if (index2 - index <= 2)
		{
			num2 = temp.at(index + 1);
			if (num2 > num) num = num2;
		}
		else
		{
			num2 = temp.substr(index + 1, 2);
			if (num2 > num) num = num2;
		}
	}
	read.close();
	return num;
}
//find time stamp of latest message
string MessageBuffer::findWallNum(int i)
{
	string num, num2, filename, temp, result;
	int index, index2;
	ifstream read;
	filename = tigerbook.getUser().getName() + "_homepage.txt";
	read.open(filename.c_str());

	if (i = 1)
	{
		getline(read, temp);
		index = temp.find("!");
		index2 = temp.find("!", index + 1);
		num = temp.substr(index + 1);
		read.close();
		return num;
	}

	getline(read, temp);
	index = temp.find("!");
	index2 = temp.find("!", index + 1);
	if (index2 - index == 2)
	{
		num = temp.at(index + 1);
	}
	else
	{
		num = temp.substr(index + 1, 2);
	}
	if (!read.eof())
	{
		while (getline(read, temp))
		{
			index = temp.find("!");
			index2 = temp.find("!", index + 1);
			if (index2 - index <= 2)
			{
				num2 = temp.at(index + 1);
				if (num2 > num) num = num2;
			}
			else
			{
				num2 = temp.substr(index + 1, 2);
				if (num2 > num) num = num2;
			}

		}
	}
	read.close();
	return num;
}
//deletes the message from the file and re-writes the file without the message
void MessageBuffer::deleteMessage(string message, string tNum)
{
	string num, num2, filename, temp, result, line;
	int index, index2, index3;// index4;
	ifstream read;
	ofstream write;
	filename = tigerbook.getUser().getName() + "_homepage.txt";
	read.open(filename.c_str());
	while (getline(read, temp))
	{
		result = temp + "\n" + result;
	}
	read.close();
	index = result.find(tNum);

	if (index > 15)
	{
		/*index4 = result.find(message);
		line = result.substr(index4, message.length());
		cout << line << endl << endl;*/
		index2 = result.find('\n', index);
		index3 = result.rfind('\n', index);
		result.erase(index3, index2 - index3);
		//cout << result;
	}

	else
	{
		/*line = result.substr(0, message.length());
		cout << line << endl << endl;*/
		result.erase(0, message.length());
		//cout << result;
	}

	write.open(filename.c_str(), ios::trunc);
	write << result;
	write.close();
}
//formats message to be printed
string MessageBuffer::homeMessageFormat(string message)
{
	string result, group, sender, message2, message3, message4;
	int i, i2;
	if (message.at(0) == '(')
	{
		i = message.find("){", 0);
		group = message.substr(0, i + 1);
		i = message.find("<!", 0);
		i2 = message.find("!>", 0);
		sender = message.substr(i + 2, i2 - i - 2);
		i = message.find(">", 0);
		message2 = message.substr(i + 1, message.length() - i);
		i = message2.find("\\n");
		message3 = message2.substr(0, i);
		message4 = message2.substr(i + 2,message.length() - i + 1);

		result = sender + " " + group + " >\n" + message3 + "\n" + message4;
		return result;
	}

	else
	{
		i = message.find("<!", 0);
		i2 = message.find("!>", 0);
		sender = message.substr(i + 2, i2 - i - 2);
		cout << sender << endl;
		i = message.find(">", 0);
		message2 = message.substr(i + 1, message.length() - i);
		cout << message2 << endl;
		i = message2.find("\\n");
		message3 = message2.substr(0, i);
		message4 = message2.substr(i + 2, message.length() - i + 1);

		result = sender + " >\n" + message3 + "\n" + message4;
		return result;

	}
	return "";
}
//formats message to be printed
string MessageBuffer::wallMessageFormat(string message)
{
	string result, group, sender, message2, message3, message4;
	int i, i2;
	if (message.at(0) == '(')
	{
		i = message.find("){", 0);
		group = message.substr(0, i + 1);
		i = message.find("{!", 0);
		i2 = message.find("!>", 0);
		sender = message.substr(i + 2, i2 - i - 2);
		i = message.find(">", 0);
		message2 = message.substr(i + 1, message.length() - i);
		i = message2.find("\\n");
		message3 = message2.substr(0, i);
		message4 = message2.substr(i + 2, message.length() - i + 1);

		result = group + " >\n" + message3 + "\n" + message4;
		return result;
	}

	else
	{
		i = message.find("<!", 0);
		i2 = message.find("!>", 0);
		sender = message.substr(i + 2, i2 - i - 2);
		cout << sender << endl;
		i = message.find(">", 0);
		message2 = message.substr(i + 1, message.length() - i);
		cout << message2 << endl;
		i = message2.find("\\n");
		message3 = message2.substr(0, i);
		message4 = message2.substr(i + 2, message.length() - i + 1);

		result = sender + " >\n" + message3 + "\n" + message4;
		return result;

	}
	return "";
}

//sends unicast message to message buffer to be formatted
void System::unicast(string recipient)
{
	ifstream i;
	ofstream o;
	string uniMessage;
	cout << "Enter the message: ";
	cin.ignore();

	while (uniMessage.find("^!") == -1)
	{
		string tempMessage;
		getline(cin, tempMessage);
		uniMessage = uniMessage + "\\n" + tempMessage;
	}
	int length = uniMessage.find("^!") - 4;
	uniMessage = uniMessage.substr(2, length);

	messageBuffer.messageFormat(uniMessage, recipient, activeUser.getName());

	i.open("time.txt");
	i >> ctr;
	ctr++;
	o.open("time.txt");
	o << ctr;
	i.close();
	o.close();

	string one = "============================================================================================";
	string two = "|			" + activeUser.getName() + " has unicasted a message to " + recipient + " |";
	string three = "============================================================================================";
	string title = one + "\n" + two + "\n" + three;
	cout << title << endl;
}
//sends broadcast message to message buffer to be formatted
void System::broadcast(string groupname)
{
	ifstream i;
	ofstream o;
	string broadMessage;
	cout << "Enter the message: ";
	cin.ignore();

	while (broadMessage.find("^!") == -1)
	{
		string tempMessage;
		getline(cin, tempMessage);
		broadMessage = broadMessage + "\\n" + tempMessage;
	}
	int length = broadMessage.find("^!") - 4;
	broadMessage = broadMessage.substr(2, length);

	//need to send message to messagebuffer to be formatted and stored in _ALL text file.
	messageBuffer.messageFormat(broadMessage, groupname, activeUser.getName());
	
	i.open("time.txt");
	i >> ctr;
	ctr++;
	o.open("time.txt");
	o << ctr;
	i.close();
	o.close();

	string one = "============================================================================================";
	string two = "|			" + activeUser.getName() + " has broadcasted a new message.                  |";
	string three = "============================================================================================";
	string title = one + "\n" + two + "\n" + three;
	cout << title << endl;
}
//sends multicast message to message buffer to be formatted
void System::multicast(string groupname)
{
	ifstream i;
	ofstream o;
	string multiMessage;
	cout << "Enter the message: ";
	cin.ignore();

	while (multiMessage.find("^!") == -1)
	{
		string tempMessage;
		getline(cin, tempMessage);
		multiMessage = multiMessage + "\\n" + tempMessage;
	}
	int length = multiMessage.find("^!") - 4;
	multiMessage = multiMessage.substr(2, length);

	//need to send message to messagebuffer to be formatted and stored in _groupname text file.
	messageBuffer.messageFormat(multiMessage, groupname, activeUser.getName());
	
	i.open("time.txt");
	i >> ctr;
	ctr++;
	o.open("time.txt");
	o << ctr;
	i.close();
	o.close();

	string one = "============================================================================================";
	string two = "|			" + activeUser.getName() + " has multicasted a message to group " + groupname+ " |";
	string three = "============================================================================================";
	string title = one + "\n" + two + "\n" + three;
	cout << title << endl;
}
//create a user object and add username to file of users
void System::createUser(string username)
{
	activeUser = User(username);
	write.open("users.txt", ios::app);
	write << username << endl;
	write.close();
}
//create a group object and add groupname to file of users
void System::createGroup(string groupname)
{
	Group g = Group(groupname);
	//need to send groupname to text file of groups created.
	write.open("groups.txt", ios::app);
	write << groupname << endl;
	write.close();

	string one = "============================================================================================";
	string two = "|			          New group " + groupname + " created                                    |";
	string three = "============================================================================================";
	string title = one + "\n" + two + "\n" + three;
	cout << title << endl;
}
//adds the current user's username to the file of groupsjoined
void System::joinGroup(string groupname)
{
	//need to send active username and group name to message buffer to be stored in list of groupsjoined for that user.
	string result = activeUser.getName() + "_groups.txt";
	write.open(result.c_str(), ios::app);
	write << groupname << endl;
	write.close();

	//print banner
	string one = "============================================================================================";
	string two = "|			" + activeUser.getName() + " has joined group " + groupname + "             |";
	string three = "============================================================================================";
	string title = one + "\n" + two + "\n" + three;
	cout << title << endl;
}
//checks whether or not the group exists
bool System::isGroup(string groupname)
{
	string nameCheck;
	read.open("groups.txt");
	if (read.is_open())
	{
		while (getline(read, nameCheck))
		{
			if (groupname == nameCheck)
			{
				read.close();
				return true;
			}
		}
	}
	read.close();
	return false;
}
//checks whether or not the user exists
bool System::isUser(string username)
{
	string nameCheck;
	read.open("users.txt");
	if (read.is_open())
	{
		while (getline(read, nameCheck))
		{
			if (username == nameCheck)
			{
				read.close();
				return true;
			}
		}
	}
	read.close();
	return false;
}
//get count int for time file
int System::getCtr()
{
	return ctr;
}
//display options for user to choose from
void System::displayMenu()
{
	string one = "============================================================================================";
	string two = "|                              Distributed TigerBook System!                               |";
	string three = "============================================================================================";
	string title = one + "\n" + two + "\n" + three;
	cout << title << endl << endl;

	string groupname;
	string username;
	cout << "Please enter user name: ";
	cin >> username;

	//need to check if already created, if so, display welcome back banner, else display normal banner.
	if (isUser(username))
	{
		activeUser = User(username);
		two = "|              Welcome back to the Distributed TigerBook System, " + username + "!                        |";
	}
	else
	{
		createUser(username);
		ofstream write;
		string i = activeUser.getName() + "_groups.txt";
		write.open(i.c_str(), ios::app);
		write << "All" << endl;
		write.close();
		two = "|              Welcome to the Distributed TigerBook System, " + username + "!                        |";
	}
	title = one + "\n" + two + "\n" + three;
	cout << endl << title << endl;

	char choice;
	do
	{	
		string temp, result;
		int i;
		bool check = false;
		string menu1 = "Broadcast (b), Multicast (m), Unicast (u), Wall page (w), Home page (h),";
		string menu2 = "Create new group (g), Join a group (j), Quit (q)";
		string menu = menu1 + "\n" + menu2;
		cout << endl << menu << endl;

		cout << "Enter option: ";
		cin >> choice;

		switch (choice)
		{
		case 'b':
			broadcast("All");
			break;
		case 'm':
			do
			{
				cout << "Please enter the group name: ";
				cin >> groupname;

				//need to check if group has been created, if not, display error and prompt again. if created ->
				if (isGroup(groupname))
				{
					multicast(groupname);
					check = true;
				}
			} while (check == false);
			check = false;
			break;
		case 'u':
			do
			{
				cout << "Please enter recipient username: ";
				cin >> username;

				//need to check if recipient isUser(), if no, then prompt again
				if (isUser(username))
				{
					unicast(username);
					check = true;
				}
			} while (check == false);
			check = false;
			break;
		case 'w':
			i = messageBuffer.formatWallPageFile();
			messageBuffer.printWallPage(i);
			break;
		case 'h':
			i = messageBuffer.formatHomePageFile();
			messageBuffer.printHomePage(i);
			break;
		case 'g':
			do
			{
				cout << "Please enter the group name: ";
				cin >> groupname;

				//need to check if group has been created, if so, display error and prompt again, if not, ->
				if (!isGroup(groupname))
				{
					check = true;
					createGroup(groupname);
				}
			} while (check == false);
			check = false;
			break;
		case 'j':
			do
			{
				cout << "Please enter group name: ";
				cin >> groupname;

				//need to check if group has been created, if not, prompts again, if so, adds activeUsers username to that group.
				if (isGroup(groupname))
				{
					check = true;
					joinGroup(groupname);
				}
			} while (check == false);
			check = false;
			break;
		case 'q':
			string one = "============================================================================================";
			string two = "|              Thank you for using Distributed Tigerbook Social Network                    |";
			string three = "============================================================================================";
			string title = one + "\n" + two + "\n" + three;
			cout << title;
			break;
		}
	} while (choice != 'q');
}
//get active user for message buffer functions
User System::getUser()
{
	return activeUser;
}