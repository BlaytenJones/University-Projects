Creator: Blayten K. Jones
Student ID: 010979697
Last Updated: 3/15/2023
__________________________________________________________________________________________________________________________________________________

GENERAL SET-UP PROCEDURE (STEPS MUST BE DONE IN ORDER):

1) First run the server.py file in the terminal of your choice using the command "python3 server.py".
	a) A successful run of the program should print to the terminal that "the server is ready to receive".
	b) A specific port can be specified by putting it after the command as an argument. (ex: "python3 server.py 2345")

2) Then run the client.py file in the terminal of the choice using the command "python3 client.py".
	a) A successful run of the program should connect to the socket port of the server.py file and then welcome you to the ATM.
	b) If a specified port was given to the server.py, the same port can be used here. If both of the programs are running on the same
         device, then the user can simply type "localhost." Otherwise, an IP will have to specified. (ex: "python3 client.py localhost 2345")

3) Once the server.py and client.py are set up, you will be presented with a menu that looks like this:
	
	WELCOME TO YOUR PERSONAL AUTOMATED TELLER MACHINE (ATM). PLEASE CHOOSE AN OPTION FROM THE MENU:
	0) EXIT
	1) VIEW CURRENT BALANCE
	2) WITHDRAW MONEY
	3) DEPOSIT MONEY

	CHOICE:

   From here, you will enter the numeric value that matches to the operation you want to do.
	0) will force the client to close while the server stays open listening for new client connections.
	1) will have the server return the current balance and the client will print it.
	2) allows the user to withdraw a specified amount of money (note: this must be a positive integer and also less than the current balance).
	3) allows the user to deposit a specified amount of money (note: this must also be a positive integer).
   Any other invalid choices will be declined (such as non-positives or non-integers).
	a) When giving an invalid choice, the program will repeat the previous question. This means that if you withdraw/deposit an invalid amount,
	   you will be asked to redo the option. If you did not want to withdraw/deposit and did it on accident, you can return back by entering 0

4) Once you are done, you can choose 0 from the menu adn the client can gracefully disconnect while the server program will print that it notices
   the client disconnect. The server will continue listening for a new client and the data inside the server will be persistent as long as it is
   running.
	a) If the client instead does a forceful disconnect, the program will have two different responses depending on the current environment:
		1) Linux/Turing: The server will detect an error and crash due to an index-out of bounds error (this is only if the user disconnects
		   by completely closing the tab; killing the process with ^z will cause the server not to detect it and run in an infinite loop and
		   not accept any new clients).
		2) Windows: The server will continue running regardless of a forceful disconnect and will event print an error saying that the
		   client committed to a forceful disconnect.
__________________________________________________________________________________________________________________________________________________
