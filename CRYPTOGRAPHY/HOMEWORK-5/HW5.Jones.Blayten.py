from Cryptodome.Cipher import AES
from Cryptodome.Random import get_random_bytes
from Cryptodome.Util.Padding import pad, unpad
from base64 import b64encode, b64decode
import rsa
import hashlib
import hmac
import string
import os.path
import time

from rsa.key import PrivateKey, PublicKey

sentinel = 5

while sentinel != 0:
    sentinel = input("Please select a mode:\n1) Encryption Mode: AES\n2) Decryption Mode: AES\n3) Encryption Mode: RSA\n4) Decryption Mode: RSA\n5) Performance testing\n6) Collision Detection\n0) Quit\n")
    while not sentinel.isdigit():
        print("\nINVALID INPUT! PLEASE PUT IN A NUMBER!\n")
        sentinel = input("\nPlease select a mode:\n1) Encryption Mode: AES\n2) Decryption Mode: AES\n3) Encryption Mode: RSA\n4) Decryption Mode: RSA\n5) Performance testing\n6) Collision Detection\n0) Quit\n")

    if int(sentinel) == 1:
        data = input("\nPlease input the message you would like to encrypt:\n")
        key = get_random_bytes(16)

        keyFile = open("AESsharedkey.txt", "w")
        sign = hmac.HMAC(key, data.encode('utf-8'), digestmod=hashlib.sha256)
        keyFile.write(b64encode(key).decode('utf-8'))
        keyFile.close()

        newFile = open("mactext.txt", "w")
        newFile.write(data + "\n" + b64encode(sign.digest()).decode('utf-8'))
        newFile.close()

        print("\nMESSAGE AND SIGNATURE: " + data + "\n" + b64encode(sign.digest()).decode('utf-8') + "\n")
        print("\nFILE \"mactext.txt\" SUCCESSFULLY CREATED!\n\n")

    elif int(sentinel) == 2:
        fileName = "mactext.txt"

        keyFile = open("AESsharedkey.txt", "r")
        keyEntries = keyFile.read().split(' ')
        key = b64decode(keyEntries[0])
        keyFile.close()
        encryptedFile = open(fileName, "r")
        cipherText = encryptedFile.read().split('\n')
        msg = cipherText[0]
        encryptedFile.close()
        sign1 = b64decode(cipherText[1])
        sign2 = hmac.HMAC(key, msg.encode('utf-8'), digestmod=hashlib.sha256)
        if(sign1 == sign2.digest()):
            print("\nVERIFIED!\n")
        else:
            print("\nUNVERIFIED!\n")

    elif int(sentinel) == 3:
        fileName = "./" + input("\nPlease input the name of the file that includes your private key (including the extension):\n")
        while not os.path.isfile(fileName):
            print("\nINVALID FILENAME! PLEASE TRY AGAIN!\n")
            fileName = "./" + input("\nPlease input the name of the file that includes your private key (including the extension):\n")

        privFile = open(fileName, "r")
        keyCode = privFile.read().split(' ')
        privFile.close()

        if keyCode != None:
            priv = rsa.PrivateKey(int(keyCode[0]), int(keyCode[1]), int(keyCode[2]), int(keyCode[3]), int(keyCode[4]))
            pub = rsa.PublicKey(int(keyCode[0]), int(keyCode[1]))

            fileName = "./" + input("\nPlease input the name of the file that includes your public key (including the extension):\n")
            while not os.path.isfile(fileName):
                print("\nINVALID FILENAME! PLEASE TRY AGAIN!\n")
                fileName = "./" + input("\nPlease input the name of the file that includes your public key (including the extension):\n")

            pubFile = open(fileName, "r")
            keyCode2 = pubFile.read().split(' ')
            pubFile.close()

            if keyCode2 != None:
                data = input("\nPlease input the message you would like to encrypt:\n")

                #checks to see whose public key is currently being used and chooses the other one
                if keyCode2[0] == str(pub.n):
                    secondaryPub = rsa.PublicKey(int(keyCode2[2]), int(keyCode2[3]))
                else:
                    secondaryPub = rsa.PublicKey(int(keyCode2[0]), int(keyCode2[1]))

                textSign = b64encode(rsa.sign(data.encode('utf-8'), priv, 'SHA-256'))

                newFile = open("sigtext.txt", "w")
                newFile.write(textSign.decode('utf-8') + "\n" + data)
                newFile.close()

                print("\nSIGNATURE: " + textSign.decode('utf-8') + "\n")

                print("\nFILE \"sigtext.txt\" SUCCESSFULLY CREATED!\n\n")
            else:
                print("\n\nFILE DOES NOT CONTAIN VALUES!\n\n")

        else:
            print("\n\nFILE DOES NOT CONTAIN VALUES!\n\n")

    elif int(sentinel) == 4:
        fileName = "./" + input("\nPlease input the name of the file you would like to decrypt from (including the extension):\n")
        while not os.path.isfile(fileName):
            print("\nINVALID FILENAME! PLEASE TRY AGAIN!\n")
            fileName = "./" + input("\nPlease input the name of the file you would like to decrypt from (including the extension):\n")
        encryptedFile = open(fileName, "r")
        keyCode = encryptedFile.read().split('\n')
        encryptedFile.close()

        if keyCode != None:
            textSign = keyCode[0]
            msg = keyCode[1]

            fileName = "./" + input("\nPlease input the name of the file that includes your private key (including the extension):\n")
            while not os.path.isfile(fileName):
                print("\nINVALID FILENAME! PLEASE TRY AGAIN!\n")
                fileName = "./" + input("\nPlease input the name of the file that includes your private key (including the extension):\n")

            privFile = open(fileName, "r")
            keyCode2 = privFile.read().split(' ')
            privFile.close()

            if keyCode2 != None:
                priv = rsa.PrivateKey(int(keyCode2[0]), int(keyCode2[1]), int(keyCode2[2]), int(keyCode2[3]), int(keyCode2[4]))
                pub = rsa.PublicKey(int(keyCode2[0]), int(keyCode2[1]))

                fileName = "./" + input("\nPlease input the name of the file that includes your public key (including the extension):\n")
                while not os.path.isfile(fileName):
                    print("\nINVALID FILENAME! PLEASE TRY AGAIN!\n")
                    fileName = "./" + input("\nPlease input the name of the file that includes your public key (including the extension):\n")

                pubFile = open(fileName, "r")
                keyCode3 = pubFile.read().split(' ')
                pubFile.close()

                if keyCode3 != None:
                    #checks to see whose public key is currently being used and chooses the other one
                    if keyCode3[0] == str(pub.n):
                        secondaryPub = rsa.PublicKey(int(keyCode3[2]), int(keyCode3[3]))
                    else:
                        secondaryPub = rsa.PublicKey(int(keyCode3[0]), int(keyCode3[1]))

                    try:
                        rsa.verify(msg.encode('utf-8'), b64decode(textSign), secondaryPub)
                        print("\nVERIFIED!\n")
                    except rsa.pkcs1.VerificationError:
                        print("\nUNVERIFIED!\n")
                        
                

                else:
                    print("\n\nFILE DOES NOT CONTAIN VALUES!\n\n")

            else:
                print("\n\nFILE DOES NOT CONTAIN VALUES!\n\n")
           
        else:
            print("\n\nFILE DOES NOT CONTAIN VALUES!\n\n")

    elif int(sentinel) == 5:
        data = input("\nPlease input the message you would like to run time trials for:\n")

        #HERE IS WHERE THE CODE GETS A BIT MESSY
        print("~~~~~~~~HMAC Signature: Performance~~~~~~~~")
        total1 = 0
        total2 = 0
        for i in range(100):
            start = time.time()
            #ENCODE
            key = get_random_bytes(16)
            sign = hmac.HMAC(key, data.encode('utf-8'), digestmod=hashlib.sha256)
            #MEASURE
            end = time.time()
            total1 += (end - start)
        print("AVG TIMING: " + str(total1/100) + " seconds")
        print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n")

        pub, priv = rsa.newkeys(1024)
        print("~~~~~~~~RSA Signature Generation Performance~~~~~~~~")
        total1 = 0
        total2 = 0
        for i in range(100):
            start = time.time()
            #ENCODE
            textSign = rsa.sign(data.encode('utf-8'), priv, 'SHA-256')
            #MEASURE
            end = time.time()
            total1 += (end - start)

            start = time.time()
            #DECODE
            rsa.verify(data.encode('utf-8'), textSign, pub)
            #MEASURE
            end = time.time()
            total2 += (end - start)
        print("AVG TIMING: " + str(total1/100) + " seconds")
        print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n")
        print("~~~~~~~~RSA Signature Verification Performance~~~~~~~~")
        print("AVG TIMING: " + str(total2/100) + " seconds")
        print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n")
        #END OF TEST TRIALS
    elif int(sentinel) == 6:
        numBytes = input("\nHow many bytes would you like to take from?\n")
        while not sentinel.isdigit():
            print("\nINVALID INPUT! PLEASE PUT IN A NUMBER!\n")
            numBytes = input("\nHow many bytes would you like to take from?\n")
        combos = {}
        match = False
        count = 0
        while(not match):
            count += 1
            rand = get_random_bytes(int(numBytes))
            sha = hashlib.sha256()
            sha.update(rand)
            hashVal = str(sha.hexdigest()[0:2])
            if(hashVal in combos.keys()):
                otherVal = combos[hashVal]
                if(otherVal != rand):
                    print("\nMATCH FOUND IN " + str(count) + " TRIES!\n" + str(otherVal) + "; HASH: " + hashVal + "\n" + str(rand) + "; HASH: " + hashVal + "\n")
                    match = True
            else:
                combos[hashVal] = rand
        print("\nNOW TESTING AVERAGE TRIALS:")
        totalCount = 0
        for i in range(20):
            combos = {}
            match = False
            count = 0
            while(not match):
                count += 1
                rand = get_random_bytes(int(numBytes))
                sha = hashlib.sha256()
                sha.update(rand)
                hashVal = str(sha.hexdigest()[0:2])
                if(hashVal in combos.keys()):
                    otherVal = combos[hashVal]
                    if(otherVal != rand):
                        match = True
                else:
                    combos[hashVal] = rand
            totalCount += count
        print("\nAVERAGE NUMBER OF TRIALS IS: " + str(totalCount/20) + "\n")
            
        
    elif int(sentinel) == 0:
        print("\nThank you! Goodbye!\n")
        break
    else:
        print("\nINVALID INPUT! PLEASE PUT IN A VALID NUMBER!\n")
        sentinel = input("\nPlease select a mode:\n1) Encryption Mode: AES\n2) Decryption Mode: AES\n3) Encryption Mode: RSA\n4) Decryption Mode: RSA\n5) Performance testing\n0) Quit\n")