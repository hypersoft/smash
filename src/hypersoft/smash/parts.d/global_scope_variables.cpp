/* JSAPI variables. */
JSRuntime*  runtime;
const long jsRuntimeSize = (8 * 1024 * 1024);
const long jsStackSize = 8192;

/* The class of the global object. */
static JSClass global_class = {
    "global", JSCLASS_GLOBAL_FLAGS,
    JS_PropertyStub, JS_PropertyStub, JS_PropertyStub, JS_StrictPropertyStub,
    JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, JS_FinalizeStub,
    JSCLASS_NO_OPTIONAL_MEMBERS
};

void TerminateSpiderMonkey(JSContext * cx);

const char * SMASH_RESOURCE_PATH_ENV_ID = "SMASH_RESOURCE_PATH";