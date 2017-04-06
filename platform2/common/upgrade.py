#!/usr/bin/env python
# -*- coding: utf-8 -*-

__author__ = 'weishijian'


import getpass
import sys
import telnetlib
import json
import os

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
        print "you want to upgrade those ip(s):\n"
        for item in ip:
            print item
        confirmation = raw_input("please make a confirmation (Y/N):")
        if confirmation != "Y":
            exit(1)

        print "you want to use those command(s) to upgrade:\n"
        for item in commands:
            print item
        confirmation = raw_input("please make a confirmation (Y/N):")
        if confirmation != "Y":
            exit(1)

    else:
        print "please create %s first, using -g parameter." %("info.json")
        exit(1)

    # step 2 get username and password
    username = raw_input("please input username(used for telnet):")
    print "please input password(used for telnet):"
    password = getpass.getpass()


    # step 3 telnet
    for item in ip:
        tn = telnetlib.Telnet(HOST)
        tn.set_debuglevel(0)
        tn.read_until("login: ")
        tn.write(user + "\n")
        if password:
            tn.read_until("Password: ")
            tn.write(password + "\n")


def usage():
    '''
    '''
    print "\nUSAGE: python upgrade.py -f  show help file."
    print "                         -u  start upgrade."
    print "                         -g  generate json file."

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

