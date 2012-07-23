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

#include "ofMain.h"
#include "ofxOAuth.h"

class ofxBaseFitbitApi : public ofxOAuth {
public:
    ofxBaseFitbitApi() {}
    virtual ~ofxBaseFitbitApi() {}
    
    void setup(const string& _consumerKey, const string& _consumerSecret) {
        ofxOAuth::setup("https://api.fitbit.com",_consumerKey,_consumerSecret);
        setAuthorizationURL("https://www.fitbit.com/oauth/authorize"); // custom authorization url
    }
    
    // once setup is called, authenticated calls can be made
    string testCall() {
        // if testing, make sure this date is valid for your data
        return get("/1/user/-/profile.json");
    }
    
protected:

private:
};
