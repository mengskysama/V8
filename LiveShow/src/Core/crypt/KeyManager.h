#ifndef KEY_MANAGER_H_
#define KEY_MANAGER_H_

class KeyManager
{
public:
    int GetClientKey(char key[16]);
    int GetKeyBase(char *str, short version, short& len);
};

#endif
