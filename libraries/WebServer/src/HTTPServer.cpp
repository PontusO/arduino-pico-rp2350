/*
    HTTPServer.cpp - Dead simple web-server.
    Supports only one simultaneous client, knows how to handle GET and POST.

    Copyright (c) 2014 Ivan Grokhotkov. All rights reserved.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
    Modified 8 May 2015 by Hristo Gochkov (proper post and file upload handling)
*/

#include <Arduino.h>
#include <libb64/cencode.h>
#include "WiFiServer.h"
#include "WiFiClient.h"
#include "HTTPServer.h"
#include "FS.h"
#include "detail/RequestHandlersImpl.h"
#include <MD5Builder.h>

#ifndef log_e
#define log_e(...)
#define log_w(...)
#define log_v(...)
#endif

static const char AUTHORIZATION_HEADER[] = "Authorization";
static const char qop_auth[] PROGMEM = "qop=auth";
static const char qop_auth_quoted[] PROGMEM = "qop=\"auth\"";
static const char WWW_Authenticate[] = "WWW-Authenticate";
static const char Content_Length[] = "Content-Length";

HTTPServer::HTTPServer()
    : _corsEnabled(false)
    , _currentClient(nullptr)
    , _currentMethod(HTTP_ANY)
    , _currentVersion(0)
    , _currentStatus(HC_NONE)
    , _statusChange(0)
    , _nullDelay(true)
    , _currentHandler(nullptr)
    , _firstHandler(nullptr)
    , _lastHandler(nullptr)
    , _currentArgCount(0)
    , _currentArgs(nullptr)
    , _postArgsLen(0)
    , _postArgs(nullptr)
    , _headerKeysCount(0)
    , _currentHeaders(nullptr)
    , _contentLength(0)
    , _clientContentLength(0)
    , _chunked(false) {
    log_v("HTTPServer::HTTPServer()");
}

HTTPServer::~HTTPServer() {
    if (_currentHeaders) {
        delete[]_currentHeaders;
    }
    RequestHandler* handler = _firstHandler;
    while (handler) {
        RequestHandler* next = handler->next();
        delete handler;
        handler = next;
    }
}

String HTTPServer::_extractParam(String& authReq, const String& param, const char delimit) {
    int _begin = authReq.indexOf(param);
    if (_begin == -1) {
        return "";
    }
    return authReq.substring(_begin + param.length(), authReq.indexOf(delimit, _begin + param.length()));
}

static String md5str(String &in) {
    char out[33] = {0};
    MD5Builder _ctx;
    uint8_t i;
    uint8_t _buf[16];
    memset(_buf, 0x00, 16);
    _ctx.begin();
    _ctx.add((const uint8_t *)in.c_str(), in.length());
    _ctx.calculate();
    _ctx.getBytes((uint8_t *)_buf);
    for (i = 0; i < 16; i++) {
        sprintf(out + (i * 2), "%02x", _buf[i]);
    }
    out[32] = 0;
    return String(out);
}

bool HTTPServer::authenticate(const char * username, const char * password) {
    if (hasHeader(FPSTR(AUTHORIZATION_HEADER))) {
        String authReq = header(FPSTR(AUTHORIZATION_HEADER));
        if (authReq.startsWith(F("Basic"))) {
            authReq = authReq.substring(6);
            authReq.trim();
            char toencodeLen = strlen(username) + strlen(password) + 1;
            char *toencode = new char[toencodeLen + 1];
            if (toencode == nullptr) {
                return false;
            }
            char *encoded = new char[base64_encode_expected_len(toencodeLen) + 1];
            if (encoded == nullptr) {
                delete[] toencode;
                return false;
            }
            sprintf(toencode, "%s:%s", username, password);
            if (base64_encode_chars(toencode, toencodeLen, encoded) > 0 && authReq == encoded) {
                delete[] toencode;
                delete[] encoded;
                return true;
            }
            delete[] toencode;
            delete[] encoded;
        } else if (authReq.startsWith(F("Digest"))) {
            authReq = authReq.substring(7);
            log_v("%s", authReq.c_str());
            String _username = _extractParam(authReq, F("username=\""), '\"');
            if (!_username.length() || _username != String(username)) {
                return false;
            }
            // extracting required parameters for RFC 2069 simpler Digest
            String _realm    = _extractParam(authReq, F("realm=\""), '\"');
            String _nonce    = _extractParam(authReq, F("nonce=\""), '\"');
            String _uri      = _extractParam(authReq, F("uri=\""), '\"');
            String _response = _extractParam(authReq, F("response=\""), '\"');
            String _opaque   = _extractParam(authReq, F("opaque=\""), '\"');

            if ((!_realm.length()) || (!_nonce.length()) || (!_uri.length()) || (!_response.length()) || (!_opaque.length())) {
                return false;
            }
            if ((_opaque != _sopaque) || (_nonce != _snonce) || (_realm != _srealm)) {
                return false;
            }
            // parameters for the RFC 2617 newer Digest
            String _nc, _cnonce;
            if (authReq.indexOf(FPSTR(qop_auth)) != -1 || authReq.indexOf(FPSTR(qop_auth_quoted)) != -1) {
                _nc = _extractParam(authReq, F("nc="), ',');
                _cnonce = _extractParam(authReq, F("cnonce=\""), '\"');
            }
            String _H1 = md5str(String(username) + ':' + _realm + ':' + String(password));
            log_v("Hash of user:realm:pass=%s", _H1.c_str());
            String _H2 = "";
            if (_currentMethod == HTTP_GET) {
                _H2 = md5str(String(F("GET:")) + _uri);
            } else if (_currentMethod == HTTP_POST) {
                _H2 = md5str(String(F("POST:")) + _uri);
            } else if (_currentMethod == HTTP_PUT) {
                _H2 = md5str(String(F("PUT:")) + _uri);
            } else if (_currentMethod == HTTP_DELETE) {
                _H2 = md5str(String(F("DELETE:")) + _uri);
            } else {
                _H2 = md5str(String(F("GET:")) + _uri);
            }
            log_v("Hash of GET:uri=%s", _H2.c_str());
            String _responsecheck = "";
            if (authReq.indexOf(FPSTR(qop_auth)) != -1 || authReq.indexOf(FPSTR(qop_auth_quoted)) != -1) {
                _responsecheck = md5str(_H1 + ':' + _nonce + ':' + _nc + ':' + _cnonce + F(":auth:") + _H2);
            } else {
                _responsecheck = md5str(_H1 + ':' + _nonce + ':' + _H2);
            }
            log_v("The Proper response=%s", _responsecheck.c_str());
            if (_response == _responsecheck) {
                return true;
            }
        }
    }
    return false;
}

String HTTPServer::_getRandomHexString() {
    char buffer[33];  // buffer to hold 32 Hex Digit + /0
    int i;
    for (i = 0; i < 4; i++) {
        sprintf(buffer + (i * 8), "%08lx", rp2040.hwrand32());
    }
    return String(buffer);
}

void HTTPServer::requestAuthentication(HTTPAuthMethod mode, const char* realm, const String& authFailMsg) {
    if (realm == nullptr) {
        _srealm = String(F("Login Required"));
    } else {
        _srealm = String(realm);
    }
    if (mode == BASIC_AUTH) {
        sendHeader(String(FPSTR(WWW_Authenticate)), String(F("Basic realm=\"")) + _srealm + String(F("\"")));
    } else {
        _snonce = _getRandomHexString();
        _sopaque = _getRandomHexString();
        sendHeader(String(FPSTR(WWW_Authenticate)), String(F("Digest realm=\"")) + _srealm + String(F("\", qop=\"auth\", nonce=\"")) + _snonce + String(F("\", opaque=\"")) + _sopaque + String(F("\"")));
    }
    using namespace mime;
    send(401, String(FPSTR(mimeTable[html].mimeType)), authFailMsg);
}

RequestHandler& HTTPServer::on(const Uri &uri, HTTPServer::THandlerFunction handler) {
    return on(uri, HTTP_ANY, handler);
}

RequestHandler& HTTPServer::on(const Uri &uri, HTTPMethod method, HTTPServer::THandlerFunction fn) {
    return on(uri, method, fn, _fileUploadHandler);
}

RequestHandler& HTTPServer::on(const Uri &uri, HTTPMethod method, HTTPServer::THandlerFunction fn, HTTPServer::THandlerFunction ufn) {
    FunctionRequestHandler *handler = new FunctionRequestHandler(fn, ufn, uri, method);
    _addRequestHandler(handler);
    return *handler;
}

bool HTTPServer::removeRoute(const char *uri) {
    return removeRoute(String(uri), HTTP_ANY);
}

bool HTTPServer::removeRoute(const char *uri, HTTPMethod method) {
    return removeRoute(String(uri), method);
}

bool HTTPServer::removeRoute(const String &uri) {
    return removeRoute(uri, HTTP_ANY);
}

bool HTTPServer::removeRoute(const String &uri, HTTPMethod method) {
    bool anyHandlerRemoved = false;
    RequestHandler *handler = _firstHandler;
    RequestHandler *previousHandler = nullptr;

    while (handler) {
        if (handler->canHandle(method, uri)) {
            if (_removeRequestHandler(handler)) {
                anyHandlerRemoved = true;
                // Move to the next handler
                if (previousHandler) {
                    handler = previousHandler->next();
                } else {
                    handler = _firstHandler;
                }
                continue;
            }
        }
        previousHandler = handler;
        handler = handler->next();
    }

    return anyHandlerRemoved;
}

void HTTPServer::addHandler(RequestHandler* handler) {
    _addRequestHandler(handler);
}

bool HTTPServer::removeHandler(RequestHandler *handler) {
    return _removeRequestHandler(handler);
}

void HTTPServer::_addRequestHandler(RequestHandler* handler) {
    if (!_lastHandler) {
        _firstHandler = handler;
        _lastHandler = handler;
    } else {
        _lastHandler->next(handler);
        _lastHandler = handler;
    }
}

bool HTTPServer::_removeRequestHandler(RequestHandler *handler) {
    RequestHandler *current = _firstHandler;
    RequestHandler *previous = nullptr;

    while (current != nullptr) {
        if (current == handler) {
            if (previous == nullptr) {
                _firstHandler = current->next();
            } else {
                previous->next(current->next());
            }

            if (current == _lastHandler) {
                _lastHandler = previous;
            }

            // Delete 'matching' handler
            delete current;
            return true;
        }
        previous = current;
        current = current->next();
    }
    return false;
}

void HTTPServer::serveStatic(const char* uri, FS& fs, const char* path, const char* cache_header) {
    _addRequestHandler(new StaticRequestHandler(fs, path, uri, cache_header));
}


void HTTPServer::httpClose() {
    _currentStatus = HC_NONE;
    if (!_headerKeysCount) {
        collectHeaders(0, 0);
    }
}

void HTTPServer::sendHeader(const String& name, const String& value, bool first) {
    String headerLine = name;
    headerLine += F(": ");
    headerLine += value;
    headerLine += "\r\n";

    if (first) {
        _responseHeaders = headerLine + _responseHeaders;
    } else {
        _responseHeaders += headerLine;
    }
}

void HTTPServer::setContentLength(const size_t contentLength) {
    _contentLength = contentLength;
}

void HTTPServer::enableDelay(bool value) {
    _nullDelay = value;
}

void HTTPServer::enableCORS(bool value) {
    _corsEnabled = value;
}

void HTTPServer::enableCrossOrigin(bool value) {
    enableCORS(value);
}

void HTTPServer::_prepareHeader(String& response, int code, const char* content_type, size_t contentLength) {
    response = String(F("HTTP/1.")) + String(_currentVersion) + ' ';
    response += String(code);
    response += ' ';
    response += _responseCodeToString(code);
    response += "\r\n";

    using namespace mime;
    if (!content_type) {
        content_type = mimeTable[html].mimeType;
    }

    sendHeader(String(F("Content-Type")), String(FPSTR(content_type)), true);
    if (_contentLength == CONTENT_LENGTH_NOT_SET) {
        sendHeader(String(FPSTR(Content_Length)), String(contentLength));
    } else if (_contentLength != CONTENT_LENGTH_UNKNOWN) {
        sendHeader(String(FPSTR(Content_Length)), String(_contentLength));
    } else if (_contentLength == CONTENT_LENGTH_UNKNOWN && _currentVersion) { //HTTP/1.1 or above client
        //let's do chunked
        _chunked = true;
        sendHeader(String(F("Accept-Ranges")), String(F("none")));
        sendHeader(String(F("Transfer-Encoding")), String(F("chunked")));
    }
    if (_corsEnabled) {
        sendHeader(String(FPSTR("Access-Control-Allow-Origin")), String("*"));
        sendHeader(String(FPSTR("Access-Control-Allow-Methods")), String("*"));
        sendHeader(String(FPSTR("Access-Control-Allow-Headers")), String("*"));
    }
    sendHeader(String(F("Connection")), String(F("close")));

    response += _responseHeaders;
    response += "\r\n";
    _responseHeaders = "";
}

void HTTPServer::send(int code, const char* content_type, const String& content) {
    String header;
    // Can we assume the following?
    //if(code == 200 && content.length() == 0 && _contentLength == CONTENT_LENGTH_NOT_SET)
    //  _contentLength = CONTENT_LENGTH_UNKNOWN;
    if (content.length() == 0) {
        log_w("content length is zero");
    }
    _prepareHeader(header, code, content_type, content.length());
    _currentClientWrite(header.c_str(), header.length());
    if (content.length()) {
        sendContent(content);
    }
}

void HTTPServer::send(int code, char* content_type, const String& content) {
    send(code, (const char*)content_type, content);
}

void HTTPServer::send(int code, const String& content_type, const String& content) {
    send(code, (const char*)content_type.c_str(), content);
}

void HTTPServer::send(int code, const char* content_type, const char* content) {
    send(code, content_type, content, content ? strlen(content) : 0);
}

void HTTPServer::send(int code, const char* content_type, const char* content, size_t contentLength) {
    String header;
    _prepareHeader(header, code, content_type, contentLength);
    _currentClientWrite(header.c_str(), header.length());
    if (contentLength) {
        sendContent(content, contentLength);
    }
}

void HTTPServer::send_P(int code, PGM_P content_type, PGM_P content) {
    size_t contentLength = 0;

    if (content != nullptr) {
        contentLength = strlen_P(content);
    }

    String header;
    _prepareHeader(header, code, content_type, contentLength);
    _currentClientWrite(header.c_str(), header.length());
    sendContent_P(content);
}

void HTTPServer::send_P(int code, PGM_P content_type, PGM_P content, size_t contentLength) {
    String header;
    _prepareHeader(header, code, content_type, contentLength);
    sendContent(header);
    sendContent_P(content, contentLength);
}

void HTTPServer::sendContent(const String& content) {
    sendContent(content.c_str(), content.length());
}

void HTTPServer::sendContent(const char* content, size_t contentLength) {
    const char * footer = "\r\n";
    if (_chunked) {
        char chunkSize[11];
        sprintf(chunkSize, "%x%s", (unsigned int)contentLength, footer);
        _currentClientWrite(chunkSize, strlen(chunkSize));
    }
    _currentClientWrite(content, contentLength);
    if (_chunked) {
        _currentClient->write(footer, 2);
        if (contentLength == 0) {
            _chunked = false;
        }
    }
}

void HTTPServer::sendContent_P(PGM_P content) {
    sendContent_P(content, strlen_P(content));
}

void HTTPServer::sendContent_P(PGM_P content, size_t size) {
    const char * footer = "\r\n";
    if (_chunked) {
        char chunkSize[11];
        sprintf(chunkSize, "%x%s", (unsigned int)size, footer);
        _currentClientWrite(chunkSize, strlen(chunkSize));
    }
    _currentClientWrite_P(content, size);
    if (_chunked) {
        _currentClient->write(footer, 2);
        if (size == 0) {
            _chunked = false;
        }
    }
}


void HTTPServer::_streamFileCore(const size_t fileSize, const String & fileName, const String & contentType, const int code) {
    using namespace mime;
    setContentLength(fileSize);
    if (fileName.endsWith(String(FPSTR(mimeTable[gz].endsWith))) &&
            contentType != String(FPSTR(mimeTable[gz].mimeType)) &&
            contentType != String(FPSTR(mimeTable[none].mimeType))) {
        sendHeader(F("Content-Encoding"), F("gzip"));
    }
    send(code, contentType, "");
}

String HTTPServer::pathArg(unsigned int i) {
    if (_currentHandler != nullptr) {
        return _currentHandler->pathArg(i);
    }
    return "";
}

String HTTPServer::arg(String name) {
    for (int j = 0; j < _postArgsLen; ++j) {
        if (_postArgs[j].key == name) {
            return _postArgs[j].value;
        }
    }
    for (int i = 0; i < _currentArgCount; ++i) {
        if (_currentArgs[i].key == name) {
            return _currentArgs[i].value;
        }
    }
    return "";
}

String HTTPServer::arg(int i) {
    if (i < _currentArgCount) {
        return _currentArgs[i].value;
    }
    return "";
}

String HTTPServer::argName(int i) {
    if (i < _currentArgCount) {
        return _currentArgs[i].key;
    }
    return "";
}

int HTTPServer::args() {
    return _currentArgCount;
}

bool HTTPServer::hasArg(String  name) {
    for (int j = 0; j < _postArgsLen; ++j) {
        if (_postArgs[j].key == name) {
            return true;
        }
    }
    for (int i = 0; i < _currentArgCount; ++i) {
        if (_currentArgs[i].key == name) {
            return true;
        }
    }
    return false;
}


String HTTPServer::header(String name) {
    for (int i = 0; i < _headerKeysCount; ++i) {
        if (_currentHeaders[i].key.equalsIgnoreCase(name)) {
            return _currentHeaders[i].value;
        }
    }
    return "";
}

void HTTPServer::collectHeaders(const char* headerKeys[], const size_t headerKeysCount) {
    _headerKeysCount = headerKeysCount + 1;
    if (_currentHeaders) {
        delete[]_currentHeaders;
    }
    _currentHeaders = new RequestArgument[_headerKeysCount];
    _currentHeaders[0].key = FPSTR(AUTHORIZATION_HEADER);
    for (int i = 1; i < _headerKeysCount; i++) {
        _currentHeaders[i].key = headerKeys[i - 1];
    }
}

String HTTPServer::header(int i) {
    if (i < _headerKeysCount) {
        return _currentHeaders[i].value;
    }
    return "";
}

String HTTPServer::headerName(int i) {
    if (i < _headerKeysCount) {
        return _currentHeaders[i].key;
    }
    return "";
}

int HTTPServer::headers() {
    return _headerKeysCount;
}

bool HTTPServer::hasHeader(String name) {
    for (int i = 0; i < _headerKeysCount; ++i) {
        if ((_currentHeaders[i].key.equalsIgnoreCase(name)) && (_currentHeaders[i].value.length() > 0)) {
            return true;
        }
    }
    return false;
}

String HTTPServer::hostHeader() {
    return _hostHeader;
}

void HTTPServer::onFileUpload(THandlerFunction fn) {
    _fileUploadHandler = fn;
}

void HTTPServer::onNotFound(THandlerFunction fn) {
    _notFoundHandler = fn;
}

void HTTPServer::_handleRequest() {
    bool handled = false;
    if (!_currentHandler) {
        log_e("request handler not found");
    } else {
        handled = _currentHandler->handle(*this, _currentMethod, _currentUri);
        if (!handled) {
            log_e("request handler failed to handle request");
        }
    }
    if (!handled && _notFoundHandler) {
        _notFoundHandler();
        handled = true;
    }
    if (!handled) {
        using namespace mime;
        send(404, String(FPSTR(mimeTable[html].mimeType)), String(F("Not found: ")) + _currentUri);
        handled = true;
    }
    if (handled) {
        _finalizeResponse();
    }
    _currentUri = "";
}


void HTTPServer::_finalizeResponse() {
    if (_chunked) {
        sendContent("");
    }
}

String HTTPServer::_responseCodeToString(int code) {
    switch (code) {
    case 100: return F("Continue");
    case 101: return F("Switching Protocols");
    case 200: return F("OK");
    case 201: return F("Created");
    case 202: return F("Accepted");
    case 203: return F("Non-Authoritative Information");
    case 204: return F("No Content");
    case 205: return F("Reset Content");
    case 206: return F("Partial Content");
    case 300: return F("Multiple Choices");
    case 301: return F("Moved Permanently");
    case 302: return F("Found");
    case 303: return F("See Other");
    case 304: return F("Not Modified");
    case 305: return F("Use Proxy");
    case 307: return F("Temporary Redirect");
    case 400: return F("Bad Request");
    case 401: return F("Unauthorized");
    case 402: return F("Payment Required");
    case 403: return F("Forbidden");
    case 404: return F("Not Found");
    case 405: return F("Method Not Allowed");
    case 406: return F("Not Acceptable");
    case 407: return F("Proxy Authentication Required");
    case 408: return F("Request Time-out");
    case 409: return F("Conflict");
    case 410: return F("Gone");
    case 411: return F("Length Required");
    case 412: return F("Precondition Failed");
    case 413: return F("Request Entity Too Large");
    case 414: return F("Request-URI Too Large");
    case 415: return F("Unsupported Media Type");
    case 416: return F("Requested range not satisfiable");
    case 417: return F("Expectation Failed");
    case 500: return F("Internal Server Error");
    case 501: return F("Not Implemented");
    case 502: return F("Bad Gateway");
    case 503: return F("Service Unavailable");
    case 504: return F("Gateway Time-out");
    case 505: return F("HTTP Version not supported");
    default:  return F("");
    }
}
