#define STRINGIZE(x) #x
#define GET_STRING_DEF(x) STRINGIZE(x)

#define SMASH_C_STRING_TO_JSVAL(CONTEXT, CSTR) STRING_TO_JSVAL(JS_NewStringCopyZ(CONTEXT, CSTR))
