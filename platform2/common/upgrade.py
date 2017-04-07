#!/usr/bin/env python
# -*- coding: utf-8 -*-

__author__ = 'weishijian'


import getpass
import sys
import telnetlib
import json
import os




TIME_OUT = 5


def upgrade():
    userName = ""
    password = ""

    # step 1
    # read info.json


    ip = []
    commands = []

    if os.path.exists("./info.json"):
        f = open("./info.json", "r")
        j = f.read()
        a = json.loads(j)
        ip = a["ip"]
        commands = a["command"]
        print("you want to upgrade those ip(s):\n")
        for item in ip:
            print (item)

        confirmation = input("please make a confirmation (Y/N):")
        if confirmation != "Y":
            exit(1)

        print ("you want to use those command(s) to upgrade:\n")
        for item in commands:
            print (item)
        confirmation = input("please make a confirmation (Y/N):")
        if confirmation != "Y":
            exit(1)

    else:
        print ("please create %s first, using -g parameter." %("info.json"))
        exit(1)

    # step 2 get username and password
    username = input("please input username(used for telnet):")
    print ("please input password(used for telnet):")
    password = getpass.getpass()

    # step 3 telnet
    for item in ip:
        print ("connecting", item)
        try:
            tn = telnetlib.Telnet(item,23, TIME_OUT)
        except:
            info = sys.exc_info()
            print ("telnet failed, the reason is :", info[1])
            exit(0)
        tn.set_debuglevel(0)
        tn.read_until(b'login: ')
        tn.write((username + " \n").encode(encoding="utf-8"))
        tn.read_until(b'Password: ')
        tn.write((password + "\n").encode(encoding="utf-8"))

        try:
            ret = tn.read_until(b"login: ", TIME_OUT)
            print ("ret:", ret.decode())
            print (type(ret))
            if b'login: ' in ret:
                print ("the username and password you input may be wrong, please check and re-run this app again.")
                exit(1)
            else:
                for item_command in commands:
                    tn.write((item_command + " \n").encode(encoding="utf-8"))
                    ret = tn.read_until(b"#", TIME_OUT)
                    print (ret.decode())

        except EOFError:
            print ('no data can be readed')
            exit(1)


def usage():
    '''
    '''
    print ("\nUSAGE: python upgrade.py -f  show help file.")
    print ("                         -u  start upgrade.")
    print ("                         -g  generate json file.")

def generate():
    dic = {}
    dic["ip"] = ["", ""]
    dic["command"] = ["", ""]
#    print dic
    a = json.dumps(dic)
#    print a
    f = open("./info.json", 'w')
    f.write(a)
    f.close()





if __name__ == '__main__':
    if len(sys.argv) == 1:
        usage()
    elif len(sys.argv) == 2:
        if sys.argv[1] == "-f":
            usage()
        elif sys.argv[1] == "-u":
            upgrade()
        elif sys.argv[1] == "-g":
            generate()
        else:
            usage()
    else:
        usage()

