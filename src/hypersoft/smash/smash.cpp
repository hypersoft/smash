/*
 * File:   smash.cpp
 * Author: Triston J. Taylor
 *
 * Created on May 11, 2012, 12:10 AM
 */

#include <stdarg.h>
#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#include <string.h>
#include <getopt.h>
#include <jsapi.h>
#include <jsatom.h>
#include <unistd.h>
#include <sys/stat.h>
#include <StringBuffer.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "parts.d/global_scope_macros.cpp"
#include "parts.d/global_scope_variables.cpp"

int ExecScriptFile(JSContext * context, JSObject * global, const char * file, jsval * jsResult) {

	register FILE * filep;

	filep = fopen(file, "rb");

	if (!filep) {

		JS_ReportError(context, "%s: %s", file, "No such file or directory");

		return 0;

	}

	JSObject * jsTemp = JS_CompileFileHandle(context, global, file, filep);

	if (jsTemp) {

		return JS_ExecuteScript(context, global, jsTemp, jsResult);

	} else {

		JS_ReportPendingException(context);

		return 0;

	}

}

JSBool ShellInclude(JSContext *cx, uintN argc, jsval *vp) {

	jsval *argv = JS_ARGV(cx, vp);
	JSBool status;
	JSString *str = JS_ValueToString(cx, argv[0]);
	if (!str) return false;

	JSAutoByteString cmd(cx, str);
	if (!cmd) return JS_FALSE;

	status = ExecScriptFile(cx, JS_GetGlobalForScopeChain(cx), cmd.ptr(), vp);

	return status;

}

JSBool ShellReadLine(JSContext *cx, uintN argc, jsval *vp) {

	jsval *argv = JS_ARGV(cx, vp);

	JSString *str = JS_ValueToString(cx, argv[0]);

	JSAutoByteString cmd(cx, str);
	if (!cmd) {
		JS_ReportError(cx, "can't convert prompt to c string");
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return JS_FALSE;
	}

	char * line = readline(cmd.ptr());
	if (!line) {
		*vp = JSVAL_NULL;
		return JS_TRUE;
	}
	JSString * x = JS_NewStringCopyZ(cx, line);
	free(line);
	*vp = STRING_TO_JSVAL(x);
	return JS_TRUE;
	
}

JSBool ShellClear(JSContext *cx, uintN argc, jsval *vp) {

	jsval *argv = JS_ARGV(cx, vp);

	JSString *str = JS_ValueToString(cx, argv[0]);

	JSAutoByteString cmd(cx, str);
	if (!cmd) {
		JS_ReportError(cx, "can't convert environment variable key to c string");
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return JS_FALSE;
	}

	unsetenv(cmd.ptr());

	*vp = JSVAL_TRUE;
	return JS_TRUE;

}

JSBool ShellKeys(JSContext *cx, uintN argc, jsval *vp) {

    jsval *argv = JS_ARGV(cx, vp);

	JSObject* argsObj = JS_NewArrayObject(cx, 0, NULL);
	int i = 0;
	for (int x = 0; environ[x]; ++x) {
		int split = 0;
		char * variable = environ[x];
		char t;
		while ((t = variable[split]) != 0 && t != '=') split++;
		JSString * str = JS_NewStringCopyN(cx, variable, split);
		JS_DefineElement(cx, argsObj, i++, STRING_TO_JSVAL(str), NULL, NULL, JSPROP_ENUMERATE);
	}
	*vp = OBJECT_TO_JSVAL(argsObj);
	return JS_TRUE;

}

JSBool ShellGet(JSContext *cx, uintN argc, jsval *vp) {

	jsval *argv = JS_ARGV(cx, vp);

	JSString *str = JS_ValueToString(cx, argv[0]);

	JSAutoByteString cmd(cx, str);
	if (!cmd) {
		JS_ReportError(cx, "can't convert environment variable key to c string");
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return JS_FALSE;
	}

	char * variable = getenv(cmd.ptr());
	if (variable == NULL) {
		*vp = JSVAL_NULL;
		return JS_TRUE;
	}

	JSString * out = JS_NewStringCopyZ(cx, variable);
	*vp = STRING_TO_JSVAL(out);
	return JS_TRUE;

}

JSBool ShellSet(JSContext *cx, uintN argc, jsval *vp) {

	jsval *argv = JS_ARGV(cx, vp);

	JSString *fstr = JS_ValueToString(cx, argv[0]);
	JSString *fcontents = JS_ValueToString(cx, argv[1]);
	int overwrite = (argc > 2) ? JSVAL_TO_INT(argv[2]) : 1;

	JSAutoByteString filename(cx, fstr);
	if (!filename) {
		JS_ReportError(cx, "can't convert environment variable key to c string");
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return JS_FALSE;
	}

	JSAutoByteString contents(cx, fcontents);
	if (!contents) {
		JS_ReportError(cx, "can't convert environment variable data to c string");
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return JS_FALSE;
	}

	int r = setenv(filename.ptr(), contents.ptr(), overwrite);
	*vp = BOOLEAN_TO_JSVAL(r == 0);

	return JS_TRUE;

}

JSBool ShellSetFileContent(JSContext *cx, uintN argc, jsval *vp) {

	jsval *argv = JS_ARGV(cx, vp);

	JSString *fstr = JS_ValueToString(cx, argv[0]);
	if (!fstr) {
		JS_ReportError(cx, "can't get file name from Javascript context");
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return JS_FALSE;
	}

	JSString *fcontents = JS_ValueToString(cx, argv[1]);
	if (!fstr) {
		JS_ReportError(cx, "can't get file contents from Javascript context");
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return JS_FALSE;
	}

	JSAutoByteString filename(cx, fstr);
	// todo: abort if file is a directory

	JSAutoByteString contents(cx, fcontents);

	FILE * file = fopen(filename.ptr(), "w");
	if (!file) {
		JS_ReportError(cx, "can't open file: %s; for writing", filename.ptr());
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return JS_FALSE;
	}

	long contentLength = strlen(contents.ptr());

	fwrite(contents.ptr(), 1, contentLength, file);
	fclose(file);

	*vp = DOUBLE_TO_JSVAL(contentLength);

	return JS_TRUE;

}

JSBool ShellSetFileContentAppend(JSContext *cx, uintN argc, jsval *vp) {

	jsval *argv = JS_ARGV(cx, vp);

	JSString *fstr = JS_ValueToString(cx, argv[0]);
	if (!fstr) {
		JS_ReportError(cx, "can't get file name from Javascript context");
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return JS_FALSE;
	}

	JSString *fcontents = JS_ValueToString(cx, argv[1]);
	if (!fstr) {
		JS_ReportError(cx, "can't get file contents from Javascript context");
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return JS_FALSE;
	}

	JSAutoByteString filename(cx, fstr);
	// todo: abort if file is a directory

	JSAutoByteString contents(cx, fcontents);

	FILE * file = fopen(filename.ptr(), "a");
	if (!file) {
		JS_ReportError(cx, "can't open file: %s; for writing", filename.ptr());
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return JS_FALSE;
	}

	long contentLength = strlen(contents.ptr());

	fwrite(contents.ptr(), 1, contentLength, file);
	fclose(file);

	*vp = DOUBLE_TO_JSVAL(contentLength);

	return JS_TRUE;

}

JSBool ShellGetFileContent(JSContext *cx, uintN argc, jsval *vp) {

	jsval *argv = JS_ARGV(cx, vp);

	JSString *str = JS_ValueToString(cx, argv[0]);
	if (!str) {
		JS_ReportError(cx, "can't get file name from Javascript context");
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return JS_FALSE;
	}

	JSAutoByteString cmd(cx, str);
	if (!cmd) {
		JS_ReportError(cx, "can't convert file name to c string");
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return JS_FALSE;
	}

	FILE * fp = fopen(cmd.ptr(), "rb");
	if (fp == NULL) {
		JS_ReportError(cx, "unable to open file %s", cmd.ptr());
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return JS_FALSE;
	}

	struct stat st;
	int ok = fstat(fileno(fp), &st);
	if (ok != 0) {
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		JS_ReportError(cx, "unable to stat file %s", cmd.ptr());
		return JS_FALSE;
	}

	char *buf = NULL;
	size_t len = st.st_size + 1;
	size_t i = 0;

	buf = (char *) js_malloc(len * sizeof (char));
	if (!buf) {
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		JS_ReportError(cx, "unable to allocate input buffer for file %s", cmd.ptr());
		return JS_FALSE;
	}

	int c;
	while ((c = getc(fp)) != EOF)
		buf[i++] = (unsigned char) c;
	fclose(fp);
	buf[i] = 0;
	JSString * contents = JS_NewStringCopyN(cx, (const char *) buf, len - 1);
	js_free(buf);

	if (!contents) {
		JS_ReportError(cx, "unable to allocate output buffer for file %s", cmd.ptr());
		return JS_FALSE;
	}

	*vp = STRING_TO_JSVAL(contents);

	return JS_TRUE;

}

int exitcode = 0;

static JSBool ShellExit(JSContext *cx, uintN argc, jsval *vp) {
	jsval *argv = JS_ARGV(cx, vp);
	exitcode = JSVAL_TO_INT(argv[0]);
	TerminateSpiderMonkey(cx);
	exit(exitcode);
	return JS_TRUE;
}

JSBool ShellSystemWrite(JSContext *cx, uintN argc, jsval *rval) {

	jsval *argv = JS_ARGV(cx, rval);

	JSString *str = JS_ValueToString(cx, argv[0]);
	JSString *cnt = JS_ValueToString(cx, argv[1]);

	JSAutoByteString cmd(cx, str);
	if (!cmd)
		return JS_FALSE;

	JSAutoByteString content(cx, cnt);
	if (!content)
		return JS_FALSE;

	FILE *fp;

	/* Open the command for reading. */
	fp = popen(cmd.ptr(), "w");
	if (fp == NULL) {
		JS_ReportError(cx, "failed to run: %s", cmd.ptr());
		return JS_FALSE;
	}

	long contentLength = strlen(content.ptr());

	fwrite(content.ptr(), 1, contentLength, fp);

	int status = pclose(fp);
	*rval = INT_TO_JSVAL(status);
	return JS_TRUE;

}

JSBool ShellSystemRead(JSContext *cx, uintN argc, jsval *rval) {

	jsval *argv = JS_ARGV(cx, rval);

	JSString *str = JS_ValueToString(cx, argv[0]);
	if (!str)
		return JS_FALSE;

	JSAutoByteString cmd(cx, str);
	if (!cmd)
		return JS_FALSE;

	FILE *fp;
	char path[1035];

	/* Open the command for reading. */
	fp = popen(cmd.ptr(), "r");
	if (fp == NULL) {
		printf("Failed to run command\n");
		return JS_FALSE;
	}

	StringBuffer io = NewStringBuffer();
	/* Read the output a line at a time - output it. */
	while (fgets(path, sizeof (path) - 1, fp) != NULL) {
		StringBufferWrite(&io, path);
	}

	int status = pclose(fp);

	JSString * out = JS_NewStringCopyN(cx, io.address, io.index);
	StringBufferRelease(&io);

	JSObject * result = JS_NewObject(cx, NULL, NULL, NULL);

	//	jsval ref = STRING_TO_JSVAL(out);
	JS_DefineProperty(cx, result, "status", INT_TO_JSVAL(status), NULL, NULL, JSPROP_ENUMERATE);
	JS_DefineProperty(cx, result, "output", STRING_TO_JSVAL(out), NULL, NULL, JSPROP_ENUMERATE);

	//	*rval = ref;
	*rval = OBJECT_TO_JSVAL(result);
	return JS_TRUE;

}

JSBool ShellSystem(JSContext *cx, uintN argc, jsval *rval) {

	jsval *argv = JS_ARGV(cx, rval);

	JSString *str = JS_ValueToString(cx, argv[0]);
	if (!str)
		return JS_FALSE;

	char * cmd = JS_EncodeString(cx, str);
	if (!cmd)
		return JS_FALSE;

	int rc = system(cmd);
	JS_free(cx, cmd);
	JS_SET_RVAL(cx, rval, INT_TO_JSVAL(rc));

	return JS_TRUE;

}

bool buffer_ends_with_newline(char * buffer, int length) {
	if (length == 0) length = strlen(buffer);
	return buffer[length - 2] == 10 || buffer[length - 1] == 10;
}

static JSBool ShellEchoError(JSContext *cx, uintN argc, jsval *vp) {

	jsval *argv;
	uintN i, argFinal = argc - 1;
	JSString *str;
	char *bytes;
	bool have_newline = false;
	argv = JS_ARGV(cx, vp);

	for (i = 0; i < argc; i++) {
		str = JS_ValueToString(cx, argv[i]);
		if (!str)
			return JS_FALSE;
		bytes = JS_EncodeString(cx, str);
		if (!bytes)
			return JS_FALSE;
		fprintf(stderr, "%s%s", i ? " " : "", bytes);
		if (i == argFinal) have_newline = buffer_ends_with_newline(bytes, 0);
		JS_free(cx, bytes);
	}

	if (!have_newline) putc('\n', stderr);
	fflush(stderr);

	JS_SET_RVAL(cx, vp, JSVAL_VOID);
	return JS_TRUE;
}

static JSBool ShellEcho(JSContext *cx, uintN argc, jsval *vp) {
	jsval *argv;
	uintN i, argFinal = argc - 1;
	JSString *str;
	char *bytes;
	bool have_newline = false;
	argv = JS_ARGV(cx, vp);

	for (i = 0; i < argc; i++) {
		str = JS_ValueToString(cx, argv[i]);
		if (!str)
			return JS_FALSE;
		bytes = JS_EncodeString(cx, str);
		if (!bytes)
			return JS_FALSE;
		printf("%s%s", i ? " " : "", bytes);
		if (i == argFinal) have_newline = buffer_ends_with_newline(bytes, 0);
		JS_free(cx, bytes);
	}

	if (!have_newline) putc('\n', stdout);
	fflush(stdout);

	JS_SET_RVAL(cx, vp, JSVAL_VOID);
	return JS_TRUE;
}

static JSFunctionSpec shell_functions[] = {
	JS_FN("include", ShellInclude, 1, JSPROP_ENUMERATE),
	JS_FN("echo", ShellEcho, 0, JSPROP_ENUMERATE),
	JS_FN("system", ShellSystem, 1, JSPROP_ENUMERATE),
	JS_FN("getFileContent", ShellGetFileContent, 1, JSPROP_ENUMERATE),
	JS_FN("setFileContent", ShellSetFileContent, 2, JSPROP_ENUMERATE),
	JS_FN("get", ShellGet, 0, JSPROP_ENUMERATE),
	JS_FN("clear", ShellClear, 1, JSPROP_ENUMERATE),
	JS_FN("readLine", ShellReadLine, 1, JSPROP_ENUMERATE),
	JS_FN("set", ShellSet, 0, JSPROP_ENUMERATE),
	JS_FN("keys", ShellKeys, 0, JSPROP_ENUMERATE),
	JS_FN("exit", ShellExit, 1, JSPROP_ENUMERATE),
	JS_FS_END
};

static JSPropertySpec shell_properties[] = {
	//  { "prototype", 0, CTYPESPROP_FLAGS, CType::PrototypeGetter, NULL },
	{ 0, 0, 0, NULL, NULL}
};

int ExecShellScript(const char * file, int argc, char** argv, JSContext * context, JSObject * env, jsval * vp) {

	register FILE * filep;
	register int c;

	filep = fopen(file, "rb");

	if (!filep) {

		JS_ReportError(context, "%s: %s", file, "No such file or directory");

		return 0;

	}

	if (getc(filep) == '#' && getc(filep) == '!') {

		c = 1;

		while (c != EOF && c != '\n') c = getc(filep);

		ungetc(c, filep);

	} else {
		fclose(filep);
		JS_ReportError(context, "%s: %s", file, "is not a shell script");
		return 0;
	}

	JSObject* argsObj = JS_NewArrayObject(context, 0, NULL);
	JS_DefineProperty(context, env, "parameter", OBJECT_TO_JSVAL(argsObj), NULL, NULL, 0);

	JSString* str = JS_NewStringCopyZ(context, file);
	JS_DefineElement(context, argsObj, 0, STRING_TO_JSVAL(str), NULL, NULL, JSPROP_ENUMERATE);
	int i;
	for (i = 0; i < argc; i++) {
		str = JS_NewStringCopyZ(context, argv[i]);
		JS_DefineElement(context, argsObj, i + 1, STRING_TO_JSVAL(str), NULL, NULL, JSPROP_ENUMERATE);
	}

	setenv(SMASH_RESOURCE_PATH_ENV_ID, GET_STRING_DEF(SMASH_RESOURCE_PATH), 0);

	JS_InitCTypesClass(context, env);
	JS_DefineFunctions(context, env, shell_functions);
	JS_DefineProperties(context, env, shell_properties);
	jsval fun;
	JS_GetProperty(context, env, "system", &fun);
	JS_DefineFunction(context, JSVAL_TO_OBJECT(fun), "read", ShellSystemRead, 1, JSPROP_ENUMERATE);
	JS_DefineFunction(context, JSVAL_TO_OBJECT(fun), "write", ShellSystemWrite, 2, JSPROP_ENUMERATE);
	jsval fun2;
	JS_GetProperty(context, env, "echo", &fun2);
	JS_DefineFunction(context, JSVAL_TO_OBJECT(fun2), "error", ShellEchoError, 1, JSPROP_ENUMERATE);
	jsval fun3;
	JS_GetProperty(context, env, "setFileContent", &fun3);
	JS_DefineFunction(context, JSVAL_TO_OBJECT(fun3), "append", ShellSetFileContentAppend, 2, JSPROP_ENUMERATE);

#ifdef SMASH_MAIN_LOADER
	//ExecScriptFile(context, env, GET_STRING_DEF(SMASH_MAIN_LOADER), NULL);
#endif

#ifdef SMASH_SHELL_LOADER
	ExecScriptFile(context, env, GET_STRING_DEF(SMASH_SHELL_LOADER), NULL);
#endif

	JSObject * jsTemp = JS_CompileFileHandle(context, env, file, filep);

	fclose(filep);

	if (jsTemp) {

		return JS_ExecuteScript(context, env, jsTemp, vp);

	} else {

		JS_ReportPendingException(context);

		return 1;

	}

}

/* The error reporter callback. */
void reportError(JSContext *cx, const char *message, JSErrorReport *report) {

	if (!report->filename) {

		fprintf(stderr, "%s: %s: %s\n",
						"main_argv[0]", "exec", message
						);

	} else {

		fprintf(stderr, "%s: %u: %s\n",
						report->filename, report->lineno, message
						);

	}

	JS_ClearPendingException(cx);

}

JSContext * InitializeSpiderMonkey() {

	runtime = JS_NewRuntime(jsRuntimeSize);
	JSContext * context = JS_NewContext(runtime, jsStackSize);

	JS_SetOptions(context,
					JSOPTION_VAROBJFIX |
					JSOPTION_JIT |
					JSOPTION_METHODJIT |
					JSOPTION_COMPILE_N_GO
					);

	JS_SetVersion(context, JSVERSION_LATEST);

	JS_SetErrorReporter(context, reportError);

	JSObject * global = JS_NewCompartmentAndGlobalObject(context, &global_class, NULL);

	JS_InitStandardClasses(context, global);

	return context;

}

bool terminated = false;

void TerminateSpiderMonkey(JSContext * context) {

	if (terminated) return;
	JS_DestroyContext(context);
	JS_DestroyRuntime(runtime);
	JS_ShutDown();
	terminated = true;

}

int FileExists(const char* filename) {
	struct stat buffer;
	int fail = stat(filename, &buffer);
	if (!fail) return 1;
	return 0;
}

static int verbose_flag = 0;
static int print_result_flag = 0;

int SpiderMonkeyMain(int argc, char ** argv, char ** envp) {

	JSContext * context = InitializeSpiderMonkey();

	int c, r = 0;
	JSObject * env;
	jsval result;

	while (1) {
		static struct option long_options[] ={
			/* These options set a flag. */
			{"verbose", no_argument, &verbose_flag, 1},
			{"brief", no_argument, &verbose_flag, 0},
			/* These options don’t set a flag.
				 We distinguish them by their indices. */
			{"print-result", no_argument, 0, 'p'},
			{"run-script", no_argument, 0, 'r'},
			{"shell-script", required_argument, 0, 's'},
			{0, 0, 0, 0}
		};
		/* getopt_long stores the option index here. */
		int option_index = 0;

		c = getopt_long(argc, argv, "prs:",
						long_options, &option_index);

		/* Detect the end of the options. */
		if (c == -1) {
			//printf ("got end of options\n");
			break;
		}

		switch (c) {
			case 0:
				/* If this option set a flag, do nothing else now. */
				if (long_options[option_index].flag != 0) break;
				printf("option %s", long_options[option_index].name);
				if (optarg) printf(" with arg %s", optarg);
				printf("\n");
				break;

			case 'p':
				print_result_flag = 1;
				break;

			case 'r':
				printf("option -r with value `%s'\n", optarg);
				break;

			case 's':
				env = JS_NewObject(context, &global_class, JS_GetGlobalObject(context), NULL);
				r = ExecShellScript(optarg, argc - optind, argv + optind, context, env, &result);
				if (r == JS_FALSE && exitcode == 0) exitcode = 1;
				optind = argc;
				break;

			case '?':
				/* getopt_long already printed an error message. */
				break;

			default:
				break;
		}
	}

	if (print_result_flag && result) {
		JSString * result_string = JS_ValueToString(context, result);
		char * bytes = JS_EncodeString(context, result_string);
		if (!bytes) {
			return 1;
		} else {
			printf("%s\n", bytes);
			JS_free(context, bytes);
		}
	}

	TerminateSpiderMonkey(context);

	/* Print any remaining command line arguments (not options). */
	if (optind < argc) {
		printf("non-option ARGV-elements: ");
		while (optind < argc)
			printf("%s ", argv[optind++]);
		putchar('\n');
	}

	return (exitcode);

}

int main(int argc, char** argv, char** envp) {

	rl_readline_name = "smash";

	return SpiderMonkeyMain(argc, argv, envp);

}

