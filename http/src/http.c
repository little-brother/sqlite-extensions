/*
	The extension for making HTTP requests.

	http_request(url, method, headers, body, response_type, as_blob)
	Performs a HTTP-request and returns a responsed value.
	select http_request('https://httpbin.org/post', 'POST', null,'a=10&b=100') x; --> form: a = 10, b = 100
	
	http_timeouts(resolve_timeout, connect_timeout, send_timeout, receive_timeout)
	Set request timeouts in milliseconds. Default timeouts are 1000 (1 sec).
	select http_timeouts(100, 100, 100, 100);
	
	http_encode(uri_component)
	Encode special characters in a URI component.
	select http_encode('абв'); --> %D0%B0%D0%B1%D0%B2
*/

#define UNICODE
#define _UNICODE

#include "sqlite3ext.h"
SQLITE_EXTENSION_INIT1
#include <windows.h>
#include <winhttp.h>
#include <stdio.h>
#include <tchar.h>

char* utf16to8(const TCHAR* in) {
	char* out;
	if (!in || _tcslen(in) == 0) {
		out = (char*)calloc (1, sizeof(char));
	} else {
		int len = WideCharToMultiByte(CP_UTF8, 0, in, -1, NULL, 0, 0, 0);
		out = (char*)calloc (len, sizeof(char));
		WideCharToMultiByte(CP_UTF8, 0, in, -1, out, len, 0, 0);
	}
	return out;
}

TCHAR* utf8to16(const char* in) {
	TCHAR *out;
	if (!in || strlen(in) == 0) {
		out = (TCHAR*)calloc (1, sizeof (TCHAR));
	} else {
		DWORD size = MultiByteToWideChar(CP_UTF8, 0, in, -1, NULL, 0);
		out = (TCHAR*)calloc (size, sizeof (TCHAR));
		MultiByteToWideChar(CP_UTF8, 0, in, -1, out, size);
	}
	return out;
}

// nResolveTimeout, nConnectTimeout, nSendTimeout, nReceiveTimeout
static int timeouts[4] = {1000, 1000, 1000, 1000};

static void http_request(sqlite3_context* ctx, int argc, sqlite3_value **argv) {
	if (sqlite3_value_type(argv[0]) != SQLITE_TEXT) 
		return sqlite3_result_null(ctx);

	HINTERNET hSession = 0, hConnect = 0, hRequest = 0;
	DWORD errCode = 0;
	DWORD errStage = 0;

	hSession = WinHttpOpen(TEXT("SQLite http module"), 
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
	errCode = !errCode && !hSession ? GetLastError() : errCode;
	errStage = errCode && !errStage ? 1 : errStage; 

	WinHttpSetTimeouts(hSession, timeouts[0], timeouts[1], timeouts[2], timeouts[3]);

	const char* url8 = sqlite3_value_text(argv[0]);
	BOOL isHttp = strncmp(url8, "http:", 5) == 0;

	if (!errCode && hSession) {		
		TCHAR* url16 = utf8to16(url8 + (strstr(url8, "://") ? 8 - isHttp : 0)); 
		TCHAR* path16 = _tcschr(url16, TEXT('/'));
		if (path16)
			path16[0] = 0;

		hConnect = WinHttpConnect(hSession, url16, isHttp ? INTERNET_DEFAULT_HTTP_PORT : INTERNET_DEFAULT_HTTPS_PORT, 0);
		errCode = !errCode && !hConnect ? GetLastError() : errCode;
		errStage = errCode && !errStage ? 2 : errStage; 

		free(url16);
	}

	TCHAR* method16 = utf8to16(argc > 1 && sqlite3_value_type(argv[1]) == SQLITE_TEXT ? (const char*)sqlite3_value_text(argv[1]) : "GET"); 
	_tcsupr(method16);
	BOOL isHEAD = _tcscmp(method16, TEXT("HEAD")) == 0;
	if (!errCode && hConnect) {
		TCHAR* path16 = utf8to16(strchr(url8 + (strstr(url8, "://") ? 8 - isHttp : 0), '/'));
		hRequest = WinHttpOpenRequest(hConnect, method16, path16, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_REFRESH | WINHTTP_FLAG_SECURE);
		errCode = !errCode && !hRequest ? GetLastError() : errCode;
		errStage = errCode && !errStage ? 3 : errStage; 

		free(path16);
	}
	free(method16);

	if (!errCode && hRequest) {
		const char* data8 = argc > 3 && sqlite3_value_type(argv[3]) == SQLITE_TEXT ? (const char*)sqlite3_value_text(argv[3]) : 0;
		DWORD dlen = data8 ? strlen(data8) : 0;

		TCHAR* headers16 = argc > 2 && sqlite3_value_type(argv[2]) == SQLITE_TEXT ? utf8to16((const char*)sqlite3_value_text(argv[2])) : 
			dlen > 0 ? utf8to16("Content-Type: application/x-www-form-urlencoded") : 0;

		BOOL isAlive = WinHttpSendRequest(hRequest, 
			headers16 ? (void*)headers16 : WINHTTP_NO_ADDITIONAL_HEADERS, headers16 ? _tcslen(headers16) : 0, 
			data8 ? (void*)data8 : WINHTTP_NO_REQUEST_DATA, dlen,	
			dlen, 0);
		errCode = !errCode && !isAlive ? GetLastError() : errCode;
		errStage = errCode && !errStage ? 4 : errStage; 

		if (headers16)
			free(headers16);

		BOOL hasResults = isAlive && !errCode ? WinHttpReceiveResponse(hRequest, NULL) : FALSE;
		errCode = !errCode ? GetLastError() : errCode;
		errStage = errCode && !errStage ? 5 : errStage; 

		if (!errCode) {
			if (isHEAD) {
				sqlite3_result_int(ctx, isAlive);
			} else {
				TCHAR* responseType16 = utf8to16(argc > 4 && sqlite3_value_type(argv[4]) == SQLITE_TEXT ? (const char*)sqlite3_value_text(argv[4]) : "BODY"); 
				_tcsupr(responseType16);
	
				if (_tcscmp(responseType16, TEXT("BODY")) == 0) {
					if (hasResults) {
						char* res8 = 0;
						int rlen = 0;
						do {
							DWORD len = 0;
							if (!WinHttpQueryDataAvailable(hRequest, &len))
								break;
	
							if (len == 0)
								break;
	
							DWORD dlen = 0;
							DWORD offset = rlen;
							rlen += len;
							res8 = res8 ? realloc(res8, rlen * sizeof(char)) : calloc(rlen, sizeof(char));
							if (!WinHttpReadData(hRequest, (LPVOID)res8 + offset, len, &dlen))
								break;
						} while (TRUE);
	
						if (res8) {
							BOOL asBLOB = argc > 5 && sqlite3_value_type(argv[5]) == SQLITE_INTEGER && sqlite3_value_int(argv[5]) == 1;
							if (asBLOB) {
								sqlite3_result_blob(ctx, res8, rlen, SQLITE_TRANSIENT);
							} else {	
								sqlite3_result_text(ctx, res8, rlen + 1, SQLITE_TRANSIENT);
							}
							free(res8);
						} else {
							sqlite3_result_text(ctx, "Error: Can't get data", -1, SQLITE_TRANSIENT);
						}
					} else {
						sqlite3_result_null(ctx);
					}
				} else if (_tcscmp(responseType16, TEXT("HEADERS")) == 0 || _tcscmp(responseType16, TEXT("COOKIES")) == 0) {
					DWORD QUERY_FLAG = _tcscmp(responseType16, TEXT("COOKIES")) == 0 ? WINHTTP_QUERY_SET_COOKIE : WINHTTP_QUERY_RAW_HEADERS_CRLF;
	
					TCHAR* result16 = 0;
					DWORD idx = 0;
	
					do {
						DWORD len = 0;
						WinHttpQueryHeaders(hRequest, QUERY_FLAG, 0, WINHTTP_NO_OUTPUT_BUFFER, &len, &idx);
						if (len == 0)
							break;
	
						int rlen = result16 ? _tcslen(result16) : 0;
						result16 = result16 ? realloc(result16, (rlen + len + 3) * sizeof(TCHAR)) : calloc(len + 3, sizeof(TCHAR));
	
						if (WinHttpQueryHeaders(hRequest, QUERY_FLAG, 0, result16 + rlen, &len, &idx)) {
							_tcscat(result16, TEXT("\r\n"));
						} else {
							break;
						}
					} while (TRUE);
					
					if (result16) {
						result16[_tcslen(result16) - 1] = 0; // remove last \r\n
	
						char* result8 = utf16to8(result16);
						sqlite3_result_text(ctx, result8, -1, SQLITE_TRANSIENT);
						free(result8);
						free(result16);
					} else {
						sqlite3_result_null(ctx);
					}
				} else if (_tcscmp(responseType16, TEXT("STATUS")) == 0) {
					DWORD statusCode = 0;
					DWORD size = sizeof(statusCode);
					
					BOOL rc = WinHttpQueryHeaders(hRequest, 
						WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, 
						WINHTTP_HEADER_NAME_BY_INDEX, 
						&statusCode, &size, WINHTTP_NO_HEADER_INDEX);
					errCode = !errCode && !rc ? GetLastError() : errCode;
					errStage = errCode && !errStage ? 6 : errStage; 

					if (!errCode) 
						sqlite3_result_int(ctx, statusCode);	
				} else {
					sqlite3_result_text(ctx, "Error: Unknown response type", -1, SQLITE_TRANSIENT);
				}
			}
		}
	} 

	if (errCode) {
		char err8[255];
		sprintf(err8, "Error: Connection error %i/%i", errCode, errStage);
		sqlite3_result_text(ctx, err8, -1, SQLITE_TRANSIENT);
	}

	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);
}

static void http_timeouts(sqlite3_context* ctx, int argc, sqlite3_value **argv) {
	for (int i = 0; i < 4; i++) {
		if (sqlite3_value_type(argv[i]) != SQLITE_INTEGER)
			return sqlite3_result_error(ctx, "All timeouts should be numbers", -1);
	}

	for (int i = 0; i < 4; i++)
		timeouts[i] = sqlite3_value_int(argv[i]);

	sqlite3_result_text(ctx, "Success", -1, SQLITE_TRANSIENT);
}

// https://stackoverflow.com/a/21491633/6121703
static char encTable[256] = {0};
char* encode(const unsigned char *s, char *enc){
	if (encTable[65] == 0) {
		for (int i = 0; i < 256; i++)
			encTable[i] = isalnum(i) || i == '*' || i == '-' || i == '.' || i == '_' ? i : 0;
	}	

	for (; *s; s++){
		if (encTable[*s])
			*enc = encTable[*s];
		else
			sprintf(enc, "%%%02X", *s);
		while (*++enc);
	}

	return (enc);
}

static void http_encode(sqlite3_context* ctx, int argc, sqlite3_value **argv) {
	if (sqlite3_value_type(argv[0]) != SQLITE_TEXT)
		return sqlite3_result_null(ctx);

	const char* in = (const char*)sqlite3_value_text(argv[0]);
	char* out = (char*)calloc (strlen(in) * 4, sizeof(char));

	encode(in, out);

	sqlite3_result_text(ctx, out, -1, SQLITE_TRANSIENT);
	free(out);
}

__declspec(dllexport) int sqlite3_http_init(sqlite3* db, char **pzErrMsg, const sqlite3_api_routines* pApi) {
	int rc = SQLITE_OK;
	SQLITE_EXTENSION_INIT2(pApi);
	return SQLITE_OK == sqlite3_create_function(db, "http_request", -1, SQLITE_UTF8, 0, http_request, 0, 0) &&
		SQLITE_OK == sqlite3_create_function(db, "http_timeouts", 4, SQLITE_UTF8, 0, http_timeouts, 0, 0) &&
		SQLITE_OK == sqlite3_create_function(db, "http_encode", 1, SQLITE_UTF8, 0, http_encode, 0, 0) ? 
		SQLITE_OK : SQLITE_ERROR;
}
