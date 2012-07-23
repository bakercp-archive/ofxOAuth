/*==============================================================================
 
 Copyright (c) 2010, 2011, 2012 Christopher Baker <http://christopherbaker.net>
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 ==============================================================================*/

#pragma once

#include <set>
#include "ofMain.h"
#include "ofxOAuthVerifierCallbackInterface.h"

#include "Poco/SharedPtr.h"
#include "Poco/URI.h"

#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/NameValueCollection.h"

using namespace Poco::Net;

//--------------------------------------------------------------
class ofxOAuthAuthReqHandler : public HTTPRequestHandler {
public:
    
    ofxOAuthAuthReqHandler(ofxOAuthVerifierCallbackInterface* _callback, const string& _docRoot) {
        callback = _callback;        
        docRoot = _docRoot;
    }

    virtual ~ofxOAuthAuthReqHandler() {}
    
    void handleRequest(HTTPServerRequest& request, 
                       HTTPServerResponse& response) { 

        response.setChunkedTransferEncoding(true); 
        response.setKeepAlive(false);

        Poco::URI uri(request.getURI()); 
        
        string query = uri.getQuery();
        
        map<string, string> parsedQuery;
        
        if(parseQuery(query,parsedQuery)) {
            map<string, string>::iterator iter,iter1;
            iter = parsedQuery.find("oauth_token");
            iter1 = parsedQuery.find("oauth_verifier");
            
            if(iter != parsedQuery.end() && iter1 != parsedQuery.end()) {
                callback->setRequestTokenVerifier((*iter).second, (*iter1).second);
                ofLogVerbose("ofxOAuthAuthReqHandler: Got Request Token / Verifier = " + (*iter).second + " / " + (*iter).second + " - Fancy!");
            } else {
                ofLogError("ofxOAuthAuthReqHandler: did not have both oauth_token and oauth_verifier.");
            }
        }
        
        string path = uri.getPath();

        if(isMatch(path,"/")) { path = "/index.html";} // default index

        ofFile file(docRoot+path);
        ofBuffer outputBuffer;
        
        if(file.exists()) {
            // text types
            string ext = file.getExtension();

            if(isMatch(ext, "html") || isMatch(ext, "htm")) {
                request.setContentType("text/html");
            } else if(isMatch(ext, "json")) {
                request.setContentType("application/json");
                // image types
            } else if(isMatch(ext, "jpg") || isMatch(ext, "jpeg") || isMatch(ext, "jpe")) {
                request.setContentType("image/jpeg");
            } else if(isMatch(ext, "png")) {
                request.setContentType("image/png");
            } else if(isMatch(ext, "gif")) {
                request.setContentType("image/gif");
            } else if(isMatch(ext, "ico")) {
                request.setContentType("image/vnd.microsoft.icon");

                // everything else
            } else {
                request.setContentType("text/html");
            }
            
            // load the file into the buffer
            file >> outputBuffer;
            
        } else {
            response.setStatusAndReason(HTTPResponse::HTTP_NOT_FOUND,"Sorry, couldn't find it.");
            request.setContentType("text/html");

            ofFile file(docRoot + "/404.html");
            if(file.exists()) {
                file >> outputBuffer;
            } else {
                string s = "<html><head><title>404</title></head><body><h1>404 - not found.</h1></body>";
                outputBuffer.set(s.c_str(),s.size());
            }
        }

        std::ostream& ostr = response.send();
        ostr << outputBuffer;
        
    }
    
protected:
    
    bool parseQuery(const string& query, map<string, string>& returnParams) {
        
        if(!query.empty()) {
            // could use oauth_split_url_parameters here.
            vector<string> params = ofSplitString(query, "&", true);
            
            for(int i = 0; i < params.size(); i++) {
                vector<string> tokens = ofSplitString(params[i], "=");
                if(tokens.size() == 2) {
                    returnParams[tokens[0]] = tokens[1];
                } else {
                    ofLogWarning("ofxOAuthAuthReqHandler: Return parameter did not have 2 values: " + ofToString(params[i]) + " - skipping.");
                }
            }
        }

        return !returnParams.empty();
    }
    
    bool isMatch (std::string const& s0, std::string const& s1) {
        return Poco::icompare(s0,s1) == 0;
    }

    ofxOAuthVerifierCallbackInterface* callback;
    string docRoot;
    
};

//--------------------------------------------------------------
class ofxOAuthAuthReqHandlerFactory : public HTTPRequestHandlerFactory {
public:
    ofxOAuthAuthReqHandlerFactory(ofxOAuthVerifierCallbackInterface* _callback, const string& _docRoot) {
        callback = _callback;
        docRoot = _docRoot;
    }
    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request) {
        return new ofxOAuthAuthReqHandler(callback, docRoot); 
    }
protected:
    ofxOAuthVerifierCallbackInterface* callback;
    string docRoot;
};

//--------------------------------------------------------------
class ofxOAuthVerifierCallbackServer : public ofThread {
public:
    ofxOAuthVerifierCallbackServer(ofxOAuthVerifierCallbackInterface* _callback, 
                                   const string& _docRoot = "", 
                                   int _port = -1) {

        port = _port <= 0 ? (int)ofRandom(8000,9000) : _port; 
        docRoot = _docRoot;
        callback = _callback;
    }
   
    virtual ~ofxOAuthVerifierCallbackServer() {
        waitForThread(true); // just in case
        ofLogVerbose("ofxOAuthVerifierCallbackServer: Server destroyed.");

    }
    void start(){ startThread(true, false); }
    void stop() { stopThread(); }
    void threadedFunction(){
        ofLogVerbose("ofxOAuthVerifierCallbackServer: Server starting @ " + getURL());
        ServerSocket socket(port);
        // all of these params are an attempt to make the server shut down VERY quickly.
        HTTPServerParams* pParams = new HTTPServerParams();
        pParams->setMaxQueued(1);
        pParams->setMaxThreads(1);
        pParams->setKeepAlive(false);
        pParams->setMaxKeepAliveRequests(0);
        pParams->setKeepAliveTimeout(1);
        pParams->setServerName("ofxOAuthVerifierCallbackServer/1.0");
        Poco::ThreadPool babyPool(1,1,1,1);
        HTTPServer server(new ofxOAuthAuthReqHandlerFactory(callback, docRoot),
                          babyPool,
                          socket,
                          new HTTPServerParams());
        server.start(); // start the http server
        ofLogVerbose("ofxOAuthVerifierCallbackServer: Server successfully started @ " + getURL());
        while( isThreadRunning() != 0 ) sleep(250);
        server.stop(); // stop the http server
        socket.close();
        babyPool.joinAll();
        ofLogVerbose("ofxOAuthVerifierCallbackServer: Server successfully shut down.");
    }

    string getURL() { return "http://127.0.0.1:" + ofToString(port) + "/";}
    int    getPort() { return port; }
    void   setPort(int _port) { port = _port;}
    void   setDocRoot(const string& _docRoot) { docRoot = _docRoot;}
    
protected:
    ofxOAuthVerifierCallbackInterface* callback;
    int port;
    string docRoot;
};
