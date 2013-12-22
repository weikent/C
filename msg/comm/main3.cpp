/* 
 *     Created:  22  7月 2013 21:05:24
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

    /* char aaa[] = "{\"sysID\":\"1\",\"seqOfIns\":\"1\",\"numOfDev\":\"1\",\"optCode\":\"0030\",\"timeout\":\"1234756609589\",\"devArray\":[{\"devID\":\"34bca6010203\",\"numOfCont\":\"1\",\"contArray\":[{\"contID\":\"0\",\"contType\":\"500\",\"numOfAct\":\"1\",\"actArray\":[{\"actType\":\"0\",\"actValue\":\"0\"}]}]},{\"devID\":\"34bca6010203\",\"numOfCont\":\"1\",\"contArray\":[{\"contID\":\"0\",\"contType\":\"500\",\"numOfAct\":\"1\",\"actArray\":[{\"actType\":\"0\",\"actValue\":\"0\"}]}]}]}"; */

    char aaa[] = "{\"sysID\":\"1\",\"seqOfIns\":\"1\",\"numOfDev\":\"1\",\"optCode\":\"0030\",\"timeout\":\"1234756609589\",\"devArray\":[{\"devID\":\"34bca6010203\",\"numOfCont\":\"1\",\"senArray\":[{\"senID\":\"0\",\"senType\":\"0005\",\"numOfParam\":\"1\",\"paramArray\":[{\"paramType\":\"0055\",\"paramValue\":\"220\"},{\"paramType\":\"0060\",\"paramValue\":\"10\"}]}],\"contArray\":[{\"contID\":\"0\",\"contType\":\"500\",\"numOfAct\":\"1\",\"actArray\":[{\"actType\":\"0\",\"actValue\":\"0\"}]}]}]}";
    /* char aaa[] = "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"; */
    cout << "length " << sizeof(aaa) << endl;
    mq.send_message(a, &msgdata, 1, aaa);
    return 0;
}




