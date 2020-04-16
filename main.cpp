#include <iostream>
#include <string>
using namespace std;
int main()
{
	string choice;
	cout <<"What would you like to do?"<<endl
		 <<"Type"<<endl
		 <<"\"1\"to create a table."<<endl
		 <<"\"2\" to transform an expression."<<endl;
	cin>>choice;
	while (choice !="1"&&choice !="2")
	{
		cout <<"Invalid choice. Please try again."<<endl
			 <<"Type"<<endl
			 <<"\"1\"to create a table."<<endl
			 <<"\"2\" to transform an expression."<<endl;
		cin >> choice;
	}
	if (choice=="1")
	{
		cout <<"TABLE";
	}
	else
	{
		cout <<"TRANSFORM";
	}
}
