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


void hello2()
{
	test_msg2* p= new test_msg2("ffsfds");
}

