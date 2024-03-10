''' client.py
usage: python client.py HOSTNAMEorIP PORT
Allows user to select one of three functions from the menu that
allows them to withdrawal, deposit, and check the money in their account
(NONE OF THE CALCULATION IS DONE HERE!)
ReModified by Blayten K. Jones
10/12/17 modified for Python 3
03/13/23 modified to be basic UI for "banking" system
'''

import sys

# Import socket library
from socket import *

# Set hostname or IP address from command line or default to localhost
# Set port number by converting argument string to integer or use default
# Use defaults
if sys.argv.__len__() != 3:
    serverName = 'localhost'
    serverPort = 2474
# Get from command line
else:
    serverName = sys.argv[1]
    serverPort = int(sys.argv[2])

# Choose SOCK_STREAM, which is TCP
clientSocket = socket(AF_INET, SOCK_STREAM)

# Connect to server using hostname/IP and port
clientSocket.connect((serverName, serverPort))

terminal = input('WELCOME TO YOUR PERSONAL AUTOMATED TELLER MACHINE (ATM). PLEASE CHOOSE AN OPTION FROM THE MENU:\n0) EXIT\n1) VIEW CURRENT BALANCE\n2) WITHDRAW MONEY\n3) DEPOSIT MONEY\n\nCHOICE: ')
# User menu
while(terminal != '0'):
    # Turing does not support python 3.10 and therefore I have to use an if-else ladder instead of match-case...
    if(terminal == '1'):
        # CURRENT BALANCE
        # Sends control in-band to let the Server know which function the user is wanting to do.
        encodedFunctionSignal = ('!' + terminal + '!').encode('utf-8')
        # Send it into socket to server
        clientSocket.send(encodedFunctionSignal)
        # Receive response from server via socket
        serverMessage = clientSocket.recv(1024)
        serverMessage = serverMessage.decode('utf-8')
        if(serverMessage[0] != '!'):
            print('\nCURRENT BALANCE: $' + serverMessage + '\n')
        else:
            # prints error message
            print('\n' + serverMessage[1:] + '\n')
    elif(terminal == '2'):
        # WITHDRAW
        amount = input('\nPLEASE INSERT THE AMOUNT YOU WOULD LIKE TO WITHDRAW\nAMOUNT: $')
        while((not amount.isdigit()) or (int(amount) < 0)):
            amount = input('\nINVALID INPUT. INPUT MUST BE A POSITIVE WHOLE NUMBER! PLEASE TRY AGAIN.\nAMOUNT: $')
        print('\nATTEMPTING TO WITHDRAW $' + amount + '...\n')
        # Sends control in-band to let the Server know which function the user is wanting to do.
        encodedFunctionSignal = ('!' + terminal + '!' + amount).encode('utf-8')
        # Send it into socket to server
        clientSocket.send(encodedFunctionSignal)
        # Receive response from server via socket
        serverMessage = clientSocket.recv(1024)
        serverMessage = serverMessage.decode('utf-8')
        if(serverMessage[0] != '!'):
            print('\nSUCCESSFULLY WITHDREW $' + amount + '.\nCURRENT BALANCE: $' + serverMessage + '\n')
        else:
            # prints error message
            print('\n' + serverMessage[1:] + '\n')
    elif(terminal == '3'):
        # DEPOSIT
        amount = input('\nPLEASE INSERT THE AMOUNT YOU WOULD LIKE TO DEPOSIT\nAMOUNT: $')
        while((not amount.isdigit()) or (int(amount) < 0)):
            amount = input('\nINVALID INPUT. INPUT MUST BE A POSITIVE WHOLE NUMBER! PLEASE TRY AGAIN.\nAMOUNT: $')
        print('\nATTEMPTING TO DEPOSIT $' + amount + '...\n')
        # Sends control in-band to let the Server know which function the user is wanting to do.
        encodedFunctionSignal = ('!' + terminal + '!' + amount).encode('utf-8')
        # Send it into socket to server
        clientSocket.send(encodedFunctionSignal)
        # Receive response from server via socket
        serverMessage = clientSocket.recv(1024)
        serverMessage = serverMessage.decode('utf-8')
        if(serverMessage[0] != '!'):
            print('\nSUCCESSFULLY DEPOSITED $' + amount + '.\nCURRENT BALANCE: $' + serverMessage + '\n')
        else:
            # prints error message
            print('\n' + serverMessage[1:] + '\n')
    else:
        print('\nINVALID INPUT! PLEASE ENTER A MENU OPTION BETWEEN 0 AND 3 (INCLUSIVE).\n')

    terminal = input('PLEASE CHOOSE AN OPTION FROM THE MENU:\n0) EXIT\n1) VIEW CURRENT BALANCE\n2) WITHDRAW MONEY\n3) DEPOSIT MONEY\n\nCHOICE: ')

# Sends control in-band to let the Server know that the client is currently quitting.
encodedFunctionSignal = ('!0!').encode('utf-8')
# Send it into socket to server
clientSocket.send(encodedFunctionSignal)

print('\nTHANK YOU FOR USING OUR ATM SERVICE. HAVE A GREAT DAY!')

clientSocket.close()