# https://stackoverflow.com/questions/1112343/how-do-i-capture-sigint-in-python

from multiprocessing import Process
from socket import SHUT_RDWR
import argparse
import multiprocessing
import os
import signal
import socket
import sys
import threading
import time


def getArgs():
    parser = argparse.ArgumentParser(description='Print some poems!')

    parser.add_argument('-a', '--anonymous', dest='anonymous',
            action='store_true', default=False,
            help='Don\'t show IP Address (default: False)')

    # https://stackoverflow.com/questions/12818146/python-argparse-ignore-unrecognised-arguments
    args, unknown = parser.parse_known_args()
    return args



def welcome():
    banner = """
     ______     __     ______     ______     ______     ______    
    /\___  \   /\ \   /\  ___\   /\___  \   /\  __ \   /\  ___\   
    \/_/  /__  \ \ \  \ \ \__ \  \/_/  /__  \ \  __ \  \ \ \__ \  
      /\_____\  \ \_\  \ \_____\   /\_____\  \ \_\ \_\  \ \_____\ 
      \/_____/   \/_/   \/_____/   \/_____/   \/_/\/_/   \/_____/ 
      """
      
    print(banner)
                                                                  


def signal_handler(sig, frame):
    print()
    sys.exit(0)

def listen(serverSocket):
    serverSocket.listen(1)
    print('The server is ready to receive.')
    hostname = socket.gethostname()    
    args = getArgs()

    if (not args.anonymous):
        ip = socket.gethostbyname(hostname)    
        print("IP address:", ip)

    print("port number:", serverSocket.getsockname()[1])
    

def recieve(connectionSocket, nickname_client, close):
    while True:
        #sys.stdout.write("\033[K")
        sentence = connectionSocket.recv(1024).decode()
        if (len(sentence) < 1):
            connectionSocket.close()
            print("\n!!", nickname_client, "disconnected. Press enter to continue.")
            #print('\r', end='\rServer> ')
            #return 0
            close.put(1)
            #return close
            break
        sentence.rstrip('\n')
        print('\r', nickname_client, '> ', sentence, sep='')
        print('\r', end='\rServer> ')
        #return sentence
        #print("sentence:", sentence)

def send(connectionSocket, sentence):
    connectionSocket.send(sentence.encode())

def handshake(connectionSocket):
    handshake_client = connectionSocket.recv(1024).decode()
    print("handshake accepted")
    nickname_client = connectionSocket.recv(1024).decode()
    print("hello", nickname_client)
    print("----------")
    return nickname_client


def main():
    #serverPort = int(sys.argv[1])
    serverSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    serverSocket.bind(('', 0))
    signal.signal(signal.SIGINT, signal_handler)
    
    welcome()

    #nick = input('Enter yournickname: ')
    nick = "Server"

    while True:
        listen(serverSocket)
        connectionSocket, addr = serverSocket.accept()
        nickname_client = handshake(connectionSocket)
        
        close = multiprocessing.Queue()
        rec = Process(target=recieve,args=(connectionSocket,nickname_client,close,))
        rec.start()

        while True:
            sentence = input(nick + '> ')
            
            if (sentence == "\quit"):
                #send(connectionSocket, sentence)
                connectionSocket.shutdown(SHUT_RDWR)
                connectionSocket.close()
                rec.terminate()
                break
            
            try:
                if (close.get(False) == 1):
                    connectionSocket.close()
                    rec.terminate()
                    break
            except:
                pass
            
            try:
                send(connectionSocket, sentence + "\n")
            except:
                #print("sending did not work")
                break
            

main()

#t1 = threading.Thread(target=send, args=(connectionSocket,sentence,))
#t1.start()
