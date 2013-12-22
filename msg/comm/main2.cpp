/* 
 *     Created:  22  7月 2013 22:46:14
 *     Author:   weishijian@weishijian-MacUbuntu
       Description:  
 */

//#include <config.h>

#include "msgqueue.h"
#include "global/global.h"
#include <iostream>

using namespace std;


int main(int argc, char *argv[])
{
    MsgData msgdata;
    msgdata.msg_type = 0;

    msgqueue mq;
    int a = mq.create_queue();

    cout << "id = " << a<<endl;
//    mq.send_message(a, msgdata, 0, "aaaaaaaaaaaaaaaa");

    char aaa[] = "{\"sysID\":\"1\",\"seqOfIns\":\"1\",\"numOfDev\":\"1\",\"optCode\":\"0070\",\"timeout\":\"1234756609589\"}";
    /* char aaa[] = "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"; */
    cout << "length " << sizeof(aaa) << endl;
    mq.send_message(a, &msgdata, 1, aaa);
//    mq.read_message(a, &msgdata, 1);
    return 0;
}





