#pragma once

#define DECLARE_CLASS_DATA(theClass) \
    private: \
        struct theClass##Data; \
        theClass##Data* m_pData;

//#define MEMBER_LIST(V) \
//    V(type0, name0)
//    V(type1, name1)

#define DECLEARE_CLASS_DATA_MEMBER(type, name) \
    type name;

#define DECLARE_CLASS_DATA_STRUCT(theClass, MEMBER_LIST) \
struct theClass::theClass##Data { \
    MEMBER_LIST(DECLEARE_CLASS_DATA_MEMBER) \
};

#define USE_CLASS_DATA_MEMBER(type, name) \
    type& m_##name = (m_pData->name); (m_##name);

#define USE_CLASS_DATA(MEMBER_LIST) \
    MEMBER_LIST(USE_CLASS_DATA_MEMBER)

#define NEW_CLASS_DATA(theClass) \
    { \
        m_pData = new theClass##Data; \
    } 

#define DELETE_CLASS_DATA() \
    { \
        ASSERT(m_pData != NULL); \
        delete (m_pData); \
        m_pData = NULL; \
    }
