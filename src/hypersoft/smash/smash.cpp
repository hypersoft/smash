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

#include "parts.d/shell_functions.cpp"

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

	JSObject * global = JS_NewCompartmentAndGlobalObject(context, &global_class, NULL);

	JS_InitStandardClasses(context, global);

	int c, r = 0;
	JSObject * env;
	jsval result;

	while (1) {
		static struct option long_options[] = {
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
				r = ExecShellScript(optarg, argc - optind, argv + optind, context, global, &result);
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

