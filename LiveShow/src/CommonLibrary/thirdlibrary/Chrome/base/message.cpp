// message.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"




#include "ipc_message_utils.h"

struct HelloStruct
{
    int i;
};

#define IPC_MESSAGE_MACROS_ENUMS
#include "test_msg.h"

  
#undef IPC_MESSAGE_MACROS_ENUMS
#define IPC_MESSAGE_MACROS_CLASSES
#include "test_msg.h"


void hello()
{
  test_msg2* p= new test_msg2("hello");
}


int main(int argc, char* argv[])
{
	hello();
	return 0;
}



