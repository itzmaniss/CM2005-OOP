#include <iostream>
#include <map>
// #include <vector>
// #include "OrderBookEntry.h"

using namespace std;

void printMenu()
{
	// 1 print help
	cout << "1: Print Help" << endl;

	// 2 print exchange stats
	cout << "2: Print Exchange stats" << endl;

	// 3 make an ask (to sell something)
	cout << "3: Make an ask" << endl;

	// 4 make a bid (to buy something)
	cout << "4: Make a bid" << endl;

	// 5 print wallet
	cout << "5: Print Wallet" << endl;

	// 6 continue
	cout << "6: Next time frame" << endl;

	cout << "==========" << endl;
}

int getUserOption()
{
	int userOption;

	cout << "Type in 1-6" << endl;
	cin >> userOption;
	cout << "You chose: " << userOption << endl;

	return(userOption);
}

void printHelp()
{
	cout << "Help - Your aim is to make money.  Analyse the market and make bids and offers." << endl;
}

void printMarketStats()
{
	cout << "Market looks good!" << endl;
}

void enterAsk()
{
	cout << "Make an ask - Enter the amount." << endl;
}

void enterBid()
{
	cout << "Make a bid - Enter the amount." << endl;
}

void printWallet()
{
	cout << "Your wallet is empty." << endl;
}

void gotoNextTimeFrame()
{
	cout << "Going to next time frame." << endl;
}

void processUserOption(int userOption)
{
	if (userOption == 0)	// bad input
	{
		cout << "Invalid choice. Choose 1-6" << endl;
	}
	else
	{
		// map from ints to function pointers
		map<int, void(*)()> menu;
		menu[1] = printHelp;
		menu[2] = printMarketStats;
		menu[3] = enterAsk;
		menu[4] = enterBid;
		menu[5] = printWallet;
		menu[6] = gotoNextTimeFrame;

		menu[userOption]();
	}
}

int main()
{
	while (true)
	{
		printMenu();
		int userOption = getUserOption();
		processUserOption(userOption);
		cout << endl;
	}

	return 0;
}
