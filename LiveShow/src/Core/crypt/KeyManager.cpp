#include <stdlib.h> 
#include <string.h>
#include "KeyManager.h"
#include <assert.h>

int KeyManager::GetClientKey(char key[16])
{
	memcpy(key, "Authentic@51.COM", 16);
    return 1;
}

int KeyManager::GetKeyBase(char *str, short version, short& len)
{
    memcpy(str, "51Auth", 6);
    len = 6;
    return 1;
}