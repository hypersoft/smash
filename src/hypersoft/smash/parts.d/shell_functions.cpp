/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <nspr/prio.h>

#include "jsapi.h"


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

	JSAutoByteString prompt(cx, str);
	if (!prompt) {
		JS_ReportError(cx, "can't convert prompt to c string");
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return JS_FALSE;
	}

	char * line = readline(prompt.ptr());
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

	JSAutoByteString key(cx, str);
	if (!key) {
		JS_ReportError(cx, "can't convert environment variable key to c string");
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return JS_FALSE;
	}

	unsetenv(key.ptr());

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

	JSAutoByteString key(cx, str);
	if (!key) {
		JS_ReportError(cx, "can't convert environment variable key to c string");
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return JS_FALSE;
	}

	char * variable = getenv(key.ptr());
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

JSBool ShellFileExists(JSContext *cx, uintN argc, jsval *vp) {

	jsval *argv = JS_ARGV(cx, vp);

	JSString *str = JS_ValueToString(cx, argv[0]);

	JSAutoByteString filename(cx, str);
	if (!filename) {
		JS_ReportError(cx, "can't convert file name to c string");
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return JS_FALSE;
	}

	*vp = BOOLEAN_TO_JSVAL(PR_Access(filename.ptr(), PR_ACCESS_EXISTS) == PR_SUCCESS);
	return JS_TRUE;
	
}

JSBool ShellReadableFile(JSContext *cx, uintN argc, jsval *vp) {

	jsval *argv = JS_ARGV(cx, vp);

	JSString *str = JS_ValueToString(cx, argv[0]);

	JSAutoByteString filename(cx, str);
	if (!filename) {
		JS_ReportError(cx, "can't convert file name to c string");
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return JS_FALSE;
	}

	*vp = BOOLEAN_TO_JSVAL(PR_Access(filename.ptr(), PR_ACCESS_READ_OK) == PR_SUCCESS);
	return JS_TRUE;
	
}

JSBool ShellWritableFile(JSContext *cx, uintN argc, jsval *vp) {

	jsval *argv = JS_ARGV(cx, vp);

	JSString *str = JS_ValueToString(cx, argv[0]);

	JSAutoByteString filename(cx, str);
	if (!filename) {
		JS_ReportError(cx, "can't convert file name to c string");
		JS_SET_RVAL(cx, vp, JSVAL_VOID);
		return JS_FALSE;
	}

	*vp = BOOLEAN_TO_JSVAL(PR_Access(filename.ptr(), PR_ACCESS_WRITE_OK) == PR_SUCCESS);
	return JS_TRUE;
	
}

JSBool ShellFileStat(JSContext *cx, uintN argc, jsval *vp) {

	jsval *argv = JS_ARGV(cx, vp);

	JSString *str = JS_ValueToString(cx, argv[0]);

	JSAutoByteString filename(cx, str);
	if (!filename) {
		JS_ReportError(cx, "can't convert file name to c string");
		return JS_FALSE;
	}

	PRFileInfo64 fInfo;
	if (PR_GetFileInfo64(filename.ptr(), &fInfo) != PR_SUCCESS) {
		JS_ReportError(cx, "unable to get file information for file: %s", filename.ptr());
		return JS_FALSE;		
	};
	
	JSObject * out = JS_NewObject(cx, NULL, NULL, NULL);
	JS_DefineProperty(cx, out, "type", INT_TO_JSVAL(fInfo.type), NULL, NULL, JSPROP_ENUMERATE);
	JS_DefineProperty(cx, out, "creationTime", INT_TO_JSVAL(fInfo.creationTime), NULL, NULL, JSPROP_ENUMERATE);
	JS_DefineProperty(cx, out, "modificationTime", INT_TO_JSVAL(fInfo.modifyTime), NULL, NULL, JSPROP_ENUMERATE);
	JS_DefineProperty(cx, out, "size", INT_TO_JSVAL(fInfo.size), NULL, NULL, JSPROP_ENUMERATE);

	*vp = OBJECT_TO_JSVAL(out);
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
	JS_FN("fileExists", ShellFileExists, 1, JSPROP_ENUMERATE),
	JS_FN("readableFile", ShellReadableFile, 1, JSPROP_ENUMERATE),
	JS_FN("writableFile", ShellWritableFile, 1, JSPROP_ENUMERATE),
	JS_FN("fileStatus", ShellFileStat, 1, JSPROP_ENUMERATE),
//	JS_FN("fileList", ShellFileList, 1, JSPROP_ENUMERATE),
//	JS_FN("copyFile", ShellFileList, 1, JSPROP_ENUMERATE),
//	JS_FN("moveFile", ShellFileList, 1, JSPROP_ENUMERATE),
//	JS_FN("deleteFile", ShellFileList, 1, JSPROP_ENUMERATE),
	JS_FN("exit", ShellExit, 1, JSPROP_ENUMERATE),
	JS_FS_END
};
