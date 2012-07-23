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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <oauth.h>

#include "Poco/String.h"

#include "ofMain.h"
#include "ofxXmlSettings.h"

#include "ofxOAuthVerifierCallbackServer.h"
#include "ofxOAuthVerifierCallbackInterface.h"

// OAuthMethod mirror
enum ofxOAuthMethod {
    OFX_OA_HMAC=0,
    OFX_OA_RSA,
    OFX_OA_PLAINTEXT
};

enum ofxOAuthHttpMethod {
    OFX_HTTP_GET,
    OFX_HTTP_POST
};


class ofxOAuth : public ofxOAuthVerifierCallbackInterface {
public:
    
    ofxOAuth();

    virtual ~ofxOAuth();

    void setup(const string& apiURL, 
               const string& consumerKey, 
               const string& consumerSecret);
    
    void update(ofEventArgs& args);
    
    bool isAuthorized();

    string get(const string& uri, const string& queryParams = "");
    string post(const string& uri, const string& queryParams = "");
    
    // getters and setters
    string getApiURL();
    void   setApiURL(const string& v, bool autoSetEndpoints = true);
    
    string getRequestTokenURL();
    void   setRequestTokenURL(const string& v);
    string getAccessTokenURL();
    void   setAccessTokenURL(const string& v);
    string getAuthorizationURL();
    void   setAuthorizationURL(const string& v);
    string getVerifierCallbackURL();
    void   setVerifierCallbackURL(const string& v);

    void   setApplicationDisplayName(const string& v); // xoauth_displayname
    string getApplicationDisplayName();
    
    void   setApplicationScope(const string& v); // google specific
    string getApplicationScope();
    
    // verifier callback server
    void   setEnableVerifierCallbackServer(bool v);
    bool   isVerifierCallbackServerEnabled();
    
    void   setVerifierCallbackServerDocRoot(const string& docRoot);
    string getVerifierCallbackServerDocRoot();
    
    // request token / secret
    string getRequestToken();
    void   setRequestToken(const string& v);
    string getRequestTokenSecret();
    void   setRequestTokenSecret(const string& v);
    string getRequestTokenVerifier();
    void   setRequestTokenVerifier(const string& requestToken, const string& requestTokenVerifier);
    void   setRequestTokenVerifier(const string& requestTokenVerifier);

    // request access token / secret
    string getAccessToken();
    void   setAccessToken(const string& v);
    string getAccessTokenSecret();
    void   setAccessTokenSecret(const string& v);

    string getScreenName();
    void   setScreenName(const string& v);
    
    string getEncodedUserId();
    void   setEncodedUserId(const string& v);

    string getUserId();
    void   setUserId(const string& v);
    
    string getEncodedUserPassword();
    void   setEncodedUserPassword(const string& v);

    string getUserPassword();
    void   setUserPassword(const string& v);
    
    // request consumer token / secret
    string getConsumerKey();
    void   setConsumerKey(const string& v);
    string getConsumerSecret();
    void   setConsumerSecret(const string& v);
    
    void setApiName(const string& v);
    string getApiName();

    // token verifier
    string getRealm();
    void   setRealm(const string& v);
    
    
    void setSSLCACertificateFile(const string& pathname);
    
    
    void   setCredentialsPathname(const string& credentials);
    string getCredentialsPathname();
    
    void resetErrors() {
        accessFailed = false;
        accessFailedReported = false;
    }
    
protected:
    
    void saveCredentials();
    void loadCredentials();

    ofxOAuthMethod getOAuthMethod();
    void setOAuthMethod(ofxOAuthMethod v);

    map<string, string> obtainRequestToken();
    map<string, string> obtainAccessToken();
    
    string requestUserVerification(bool launchBrowser = true);
    string requestUserVerification(string additionalAuthParams, bool launchBrowser = true);

    
    string credentialsPathname;
    
    // URLS
    string apiURL;
    
    string requestTokenURL;
    string accessTokenURL;
    string authorizationURL;
    string verifierCallbackURL;

    string applicationDisplayName;
    string applicationScope;
    
    bool callbackConfirmed;
    bool verificationRequested;
    bool accessFailed;
    bool accessFailedReported;
    
    // keys, secrets and verifiers
    
    string consumerKey;    // provided by the api for each app
    string consumerSecret; // provided by the api for each app
    
    string requestToken;         // initially acquired via obtainRequestToken()
    string requestTokenSecret;   // initially acquired via obtainRequestToken()
    string requestTokenVerifier;      // acquired via web server callback or manual entry by user

    // the following 
    string apiName;
    string accessToken;          // after requestToken, requestTokenSecret and requestVerifier, 
    string accessTokenSecret;
    string encodedUserId;

    string screenName;
    string userId;
    string encodedUserPassword;
    string userPassword;
    
    // other stuff
    string realm;
    ofxOAuthMethod oauthMethod;
    ofxOAuthHttpMethod httpMethod;
    // where is our ssl cert bundles stored?
    string SSLCACertificateFile;

    // authorization callback server
    bool enableVerifierCallbackServer;
    ofPtr<ofxOAuthVerifierCallbackServer> verifierCallbackServer;
    string verifierCallbackServerDocRoot;
private:
    
    OAuthMethod _getOAuthMethod();
    string      _getHttpMethod();

    bool firstTime;
    string old_curlopt_cainfo;
    
    string addQ(const string& url) {
        string u = url;
        if(!u.empty() && u.substr(u.size()-1,u.size()-1) != "?") u += "?"; // need that
        return u;
    }
    
    // the current broser launcher doesn't support https ... :/
    void ofxLaunchBrowser(string url){
        
        // http://support.microsoft.com/kb/224816
        
        //make sure it is a properly formatted url
        if(Poco::icompare(url.substr(0,7), "http://") != 0 &&
           Poco::icompare(url.substr(0,8), "https://") != 0) {
            ofLog(OF_LOG_WARNING, "ofLaunchBrowser: url must begin http:// or https://");
            return;
        }
        
        //----------------------------
#ifdef TARGET_WIN32
        //----------------------------
        
#if (_MSC_VER)
		// microsoft visual studio yaks about strings, wide chars, unicode, etc
		ShellExecuteA(NULL, "open", url.c_str(),
                      NULL, NULL, SW_SHOWNORMAL);
#else
		ShellExecute(NULL, "open", url.c_str(),
                     NULL, NULL, SW_SHOWNORMAL);
#endif
        
        //----------------------------
#endif
        //----------------------------
        
        //--------------------------------------
#ifdef TARGET_OSX
        //--------------------------------------
		// ok gotta be a better way then this,
		// this is what I found...
		string commandStr = "open "+url;
		system(commandStr.c_str());
        //----------------------------
#endif
        //----------------------------
        
        //--------------------------------------
#ifdef TARGET_LINUX
        //--------------------------------------
		string commandStr = "xdg-open "+url;
		int ret = system(commandStr.c_str());
		if(ret!=0) ofLog(OF_LOG_ERROR,"ofLaunchBrowser: couldn't open browser");
        //----------------------------
#endif
        //----------------------------
    }

    
};
