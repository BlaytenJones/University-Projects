from Cryptodome.Cipher import AES
from Cryptodome.Random import get_random_bytes
from Cryptodome.Util.Padding import pad, unpad
from base64 import b64encode, b64decode
import rsa
import string
import os.path
import time

from rsa.key import PrivateKey, PublicKey

sentinel = 5

while sentinel != 0:
    sentinel = input("Please select a mode:\n1) Encryption Mode: AES\n2) Decryption Mode: AES\n3) Encryption Mode: RSA\n4) Decryption Mode: RSA\n5) Performance testing\n0) Quit\n")
    while not sentinel.isdigit():
        print("\nINVALID INPUT! PLEASE PUT IN A NUMBER!\n")
        sentinel = input("\nPlease select a mode:\n1) Encryption Mode: AES\n2) Decryption Mode: AES\n3) Encryption Mode: RSA\n4) Decryption Mode: RSA\n5) Performance testing\n0) Quit\n")

    if int(sentinel) == 1:
        keysize = input("\nPlease choose a key size:\n0) 16 bytes (128 bits)\n1) 24 bytes (192 bits)\n2) 32 bytes (256 bits)\n")
        while not keysize.isdigit() or (int(keysize) > 2):
            print("\nINVALID INPUT! PLEASE PUT IN A VALID OPTION!\n")
            keysize = input("\nPlease choose a key size:\n0) 16 bytes (128 bits)\n1) 24 bytes (192 bits)\n2) 32 bytes (256 bits)\n")

        data = input("\nPlease input the message you would like to encrypt:\n")
        key = get_random_bytes([16, 24, 32][int(keysize)])

        IV = get_random_bytes(AES.block_size)
        keyFile = open("AESsharedkey.txt", "w")
        keyFile.write(b64encode(key).decode('utf-8') + " " + b64encode(IV).decode('utf-8'))
        keyFile.close()

        cipher = AES.new(key, AES.MODE_CBC, IV)
        cipherText = b64encode(IV + cipher.encrypt(pad(data.encode('utf-8'), AES.block_size))).decode('utf-8')

        newFile = open("ctext.txt", "w")
        newFile.write(cipherText)
        newFile.close()

        print("\nENCIPHERED MESSAGE: " + cipherText + "\n")
        print("\nFILE \"ctext.txt\" SUCCESSFULLY CREATED!\n\n")

    elif int(sentinel) == 2:
        keysize = input("\nPlease choose a key size:\n0) 16 bytes (128 bits)\n1) 24 bytes (192 bits)\n2) 32 bytes (256 bits)\n")
        while not keysize.isdigit() or (int(keysize) > 2):
            print("\nINVALID INPUT! PLEASE PUT IN A VALID OPTION!\n")
            keysize = input("\nPlease choose a key size:\n0) 16 bytes (128 bits)\n1) 24 bytes (192 bits)\n2) 32 bytes (256 bits)\n")

        fileName = "ctext.txt"
        #fileName = "./" + input("\nPlease input the name of the file you would like to decrypt from (including the extension):\n")
        #while not os.path.isfile(fileName):
            #print("\nINVALID FILENAME! PLEASE TRY AGAIN!\n")
            #fileName = "./" + input("\nPlease input the name of the file you would like to decrypt from (including the extension):\n")

        keyFile = open("AESsharedkey.txt", "r")
        keyEntries = keyFile.read().split(' ')
        key = b64decode(keyEntries[0])
        keyFile.close()
        encryptedFile = open(fileName, "r")
        cipherText = encryptedFile.read()
        encryptedFile.close()
        decoded = b64decode(cipherText)
        IV = decoded[:AES.block_size]
        cipher = AES.new(key, AES.MODE_CBC, IV)
        text = decoded[AES.block_size:]
        message = (unpad(cipher.decrypt(text), AES.block_size)).decode('utf-8')
        print("\nENCIPHERED MESSAGE: " + cipherText + "\n")
        print("\nDECIPHERED MESSAGE: " + message + "\n")

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

                textSign = b64encode(rsa.sign(data.encode('utf-8'), priv, 'SHA-512'))
                cipherText = b64encode(rsa.encrypt(data.encode('utf-8'), secondaryPub))

                newFile = open("ctext.txt", "w")
                newFile.write(textSign.decode('utf-8') + "\n" + cipherText.decode('utf-8'))
                newFile.close()

                print("\nENCIPHERED MESSAGE: " + cipherText.decode('utf-8') + "\n")

                print("\nFILE \"ctext.txt\" SUCCESSFULLY CREATED!\n\n")
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
            cipherText = keyCode[1]

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
                        message = rsa.decrypt(b64decode(cipherText), priv)
                        try:
                            rsa.verify(message, b64decode(textSign), secondaryPub)
                            print("\nENCIPHERED MESSAGE: " + cipherText + "\n")
                            print("\nDECRYPTED MESSAGE: " + message.decode('utf-8') + "\n\n")
                        except rsa.pkcs1.VerificationError:
                            print("\nUNABLE TO VERIFY OR DECODE THIS MESSAGE! TRY AGAIN!\n")
                    except rsa.pkcs1.DecryptionError:
                        print("\nUNABLE TO DECRYPT THIS MESSAGE! TRY AGAIN!\n")
                

                else:
                    print("\n\nFILE DOES NOT CONTAIN VALUES!\n\n")

            else:
                print("\n\nFILE DOES NOT CONTAIN VALUES!\n\n")
           
        else:
            print("\n\nFILE DOES NOT CONTAIN VALUES!\n\n")

    elif int(sentinel) == 5:
        data = input("\nPlease input the message you would like to run time trials for:\n")

        #HERE IS WHERE THE CODE GETS A BIT MESSY
        print("~~~~~~~~AES Encryption: 128-bit Performance~~~~~~~~")
        total1 = 0
        total2 = 0
        for i in range(100):
            start = time.time()
            #ENCODE
            key = get_random_bytes(16)
            IV = get_random_bytes(AES.block_size)
            tmp = b64encode(key).decode('utf-8') + " " + b64encode(IV).decode('utf-8')
            cipher = AES.new(key, AES.MODE_CBC, IV)
            cipherText = b64encode(IV + cipher.encrypt(pad(data.encode('utf-8'), AES.block_size))).decode('utf-8')
            #MEASURE
            end = time.time()
            total1 += (end - start)

            start = time.time()
            #DECODE
            decoded = b64decode(cipherText)
            IV = decoded[:AES.block_size]
            text = decoded[AES.block_size:]
            cipher = AES.new(key, AES.MODE_CBC, IV)
            message = (unpad(cipher.decrypt(text), AES.block_size)).decode('utf-8')
            #MEASURE
            end = time.time()
            total2 += (end - start)
        print("AVG TIMING: " + str(total1/100) + " seconds")
        print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n")
        print("~~~~~~~~AES Decryption: 128-bit Performance~~~~~~~~")
        print("AVG TIMING: " + str(total2/100) + " seconds")
        print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n")

        print("~~~~~~~~AES Encryption: 192-bit Performance~~~~~~~~")
        total1 = 0
        total2 = 0
        for i in range(100):
            start = time.time()
            #ENCODE
            key = get_random_bytes(24)
            IV = get_random_bytes(AES.block_size)
            tmp = b64encode(key).decode('utf-8') + " " + b64encode(IV).decode('utf-8')
            cipher = AES.new(key, AES.MODE_CBC, IV)
            cipherText = b64encode(IV + cipher.encrypt(pad(data.encode('utf-8'), AES.block_size))).decode('utf-8')
            #MEASURE
            end = time.time()
            total1 += (end - start)

            start = time.time()
            #DECODE
            decoded = b64decode(cipherText)
            IV = decoded[:AES.block_size]
            text = decoded[AES.block_size:]
            cipher = AES.new(key, AES.MODE_CBC, IV)
            message = (unpad(cipher.decrypt(text), AES.block_size)).decode('utf-8')
            #MEASURE
            end = time.time()
            total2 += (end - start)
        print("AVG TIMING: " + str(total1/100) + " seconds")
        print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n")
        print("~~~~~~~~AES Decryption: 192-bit Performance~~~~~~~~")
        print("AVG TIMING: " + str(total2/100) + " seconds")
        print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n")

        print("~~~~~~~~AES Encryption: 256-bit Performance~~~~~~~~")
        total1 = 0
        total2 = 0
        for i in range(100):
            start = time.time()
            #ENCODE
            key = get_random_bytes(32)
            IV = get_random_bytes(AES.block_size)
            tmp = b64encode(key).decode('utf-8') + " " + b64encode(IV).decode('utf-8')
            cipher = AES.new(key, AES.MODE_CBC, IV)
            cipherText = b64encode(IV + cipher.encrypt(pad(data.encode('utf-8'), AES.block_size))).decode('utf-8')
            #MEASURE
            end = time.time()
            total1 += (end - start)

            start = time.time()
            #DECODE
            decoded = b64decode(cipherText)
            IV = decoded[:AES.block_size]
            text = decoded[AES.block_size:]
            cipher = AES.new(key, AES.MODE_CBC, IV)
            message = (unpad(cipher.decrypt(text), AES.block_size)).decode('utf-8')
            #MEASURE
            end = time.time()
            total2 += (end - start)
        print("AVG TIMING: " + str(total1/100) + " seconds")
        print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n")
        print("~~~~~~~~AES Decryption: 256-bit Performance~~~~~~~~")
        print("AVG TIMING: " + str(total2/100) + " seconds")
        print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n")


        pub, priv = rsa.newkeys(1024)
        print("~~~~~~~~RSA Encryption: 1024-bit Performance~~~~~~~~")
        total1 = 0
        total2 = 0
        for i in range(100):
            start = time.time()
            #ENCODE
            textSign = b64encode(rsa.sign(data.encode('utf-8'), priv, 'SHA-512'))
            cipherText = b64encode(rsa.encrypt(data.encode('utf-8'), pub))
            #MEASURE
            end = time.time()
            total1 += (end - start)

            start = time.time()
            #DECODE
            message = rsa.decrypt(b64decode(cipherText), priv)
            rsa.verify(message, b64decode(textSign), pub)
            #MEASURE
            end = time.time()
            total2 += (end - start)
        print("AVG TIMING: " + str(total1/100) + " seconds")
        print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n")
        print("~~~~~~~~RSA Decryption: 1024-bit Performance~~~~~~~~")
        print("AVG TIMING: " + str(total2/100) + " seconds")
        print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n")

        pub, priv = rsa.newkeys(2048)
        print("~~~~~~~~RSA Encryption: 2048-bit Performance~~~~~~~~")
        total1 = 0
        total2 = 0
        for i in range(100):
            start = time.time()
            #ENCODE
            textSign = b64encode(rsa.sign(data.encode('utf-8'), priv, 'SHA-512'))
            cipherText = b64encode(rsa.encrypt(data.encode('utf-8'), pub))
            #MEASURE
            end = time.time()
            total1 += (end - start)

            start = time.time()
            #DECODE
            message = rsa.decrypt(b64decode(cipherText), priv)
            rsa.verify(message, b64decode(textSign), pub)
            #MEASURE
            end = time.time()
            total2 += (end - start)
        print("AVG TIMING: " + str(total1/100) + " seconds")
        print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n")
        print("~~~~~~~~RSA Decryption: 2048-bit Performance~~~~~~~~")
        print("AVG TIMING: " + str(total2/100) + " seconds")
        print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n")

        pub, priv = rsa.newkeys(4096)
        print("~~~~~~~~RSA Encryption: 4096-bit Performance~~~~~~~~")
        total1 = 0
        total2 = 0
        for i in range(100):
            start = time.time()
            #ENCODE
            textSign = b64encode(rsa.sign(data.encode('utf-8'), priv, 'SHA-512'))
            cipherText = b64encode(rsa.encrypt(data.encode('utf-8'), pub))
            #MEASURE
            end = time.time()
            total1 += (end - start)

            start = time.time()
            #DECODE
            message = rsa.decrypt(b64decode(cipherText), priv)
            rsa.verify(message, b64decode(textSign), pub)
            #MEASURE
            end = time.time()
            total2 += (end - start)
        print("AVG TIMING: " + str(total1/100) + " seconds")
        print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n")
        print("~~~~~~~~RSA Decryption: 4096-bit Performance~~~~~~~~")
        print("AVG TIMING: " + str(total2/100) + " seconds")
        print("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n\n")
        #END OF TEST TRIALS

    elif int(sentinel) == 0:
        print("\nThank you! Goodbye!\n")
        break;

    else:
        print("\nINVALID INPUT! PLEASE PUT IN A VALID NUMBER!\n")
        sentinel = input("\nPlease select a mode:\n1) Encryption Mode: AES\n2) Decryption Mode: AES\n3) Encryption Mode: RSA\n4) Decryption Mode: RSA\n5) Performance testing\n0) Quit\n")