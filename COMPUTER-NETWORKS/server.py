''' server.py
usage: python server.py PORT
Reads in text, changes all letters to uppercase, and returns
the text to the client
Modified by Dale R. Thompson
ReModified by Blayten K. Jones
10/12/17 converted to Python 3
03/13/23 modified to be "banking database"
'''

import sys

# Import socket library
from socket import *

# Set port number by converting argument string to integer
# If no arguments set a default port number
# Defaults
if sys.argv.__len__() != 2:
    serverPort = 2474
# Get port number from command line
else:
    serverPort = int(sys.argv[1])

# Choose SOCK_STREAM, which is TCP
# This is a welcome socket
serverSocket = socket(AF_INET, SOCK_STREAM)

# The SO_REUSEADDR flag tells the kernel to reuse a local socket
# in TIME_WAIT state, without waiting for its natural timeout to expire.
serverSocket.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)

# Start listening on specified port
serverSocket.bind(('', serverPort))

# Listener begins listening
serverSocket.listen(1)

currentBalance = 100

failFlag = False

print('The server is ready to receive')

while 1:
    # Wait for connection and create a new socket
    # It blocks here waiting for connection
    connectionSocket, addr = serverSocket.accept()
    print('\nCLIENT CONNECTED... AWAITING INPUT...\n')
    failFlag = False

    functionSignalDecoded = '!4'
    # Checks to see if the signal is valid, if the host has gracefully exited, or if the connectionSocket has unexpectedly closed
    # failFlag is only set to true if an exception is arose.
    while(functionSignalDecoded[0] == '!' and functionSignalDecoded[1] != '0' and failFlag == False):
        try:
            # Read bytes from socket
            functionSignal = connectionSocket.recv(1024)
            functionSignalDecoded = ''
            functionSignalDecoded = functionSignal.decode('utf-8')
            currentBalanceBytes = ''

            if(functionSignalDecoded[1] == '1'):
                # CURRENT BALANCE
                currentBalanceBytes = str(currentBalance).encode('utf-8')
                # Send it into established connection
                connectionSocket.send(currentBalanceBytes)
            elif(functionSignalDecoded[1] == '2'):
                # WITHDRAW
                withdrawAmt = int(functionSignalDecoded[3:])
                if(withdrawAmt > currentBalance):
                    errorMessageBytes = '!WITHDRAW AMOUNT IS LARGER THAN CURRENT AVAILABLE BALANCE. TRY A DIFFERENT AMOUNT...'.encode('utf-8')
                    connectionSocket.send(errorMessageBytes)
                else:
                    currentBalance -= withdrawAmt
                    currentBalanceBytes = str(currentBalance).encode('utf-8')
                    # Send it into established connection
                    connectionSocket.send(currentBalanceBytes)
            elif(functionSignalDecoded[1] == '3'):
                # DEPOSIT
                depositAmt = int(functionSignalDecoded[3:])
                currentBalance += depositAmt
                currentBalanceBytes = str(currentBalance).encode('utf-8')
                # Send it into established connection
                connectionSocket.send(currentBalanceBytes)
            else:
                # ERROR
                errorMessageBytes = '!UNKNOWN SIGNAL RECIEVED; FATAL ERROR'.encode('utf-8')
                connectionSocket.send(errorMessageBytes)
        # if the client forcefully disconnects, it will print an exception to the server, close the socket, and set the fail flag
        except Exception as ex:
            print("Exception", ex)
            connectionSocket.close()
            failFlag = True

    if(functionSignalDecoded[0] != '!'):
        errorMessageBytes = '!SIGNAL NOT RECIEVED; FATAL ERROR'.encode('utf-8')
        connectionSocket.send(errorMessageBytes)

    # Close connection to client but do not close welcome socket
    connectionSocket.close()
    print('\nCLIENT DISCONNECTED...\n')
