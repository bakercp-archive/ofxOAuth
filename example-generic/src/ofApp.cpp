// =============================================================================
//
// Copyright (c) 2010-2013 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#include "ofApp.h"


//------------------------------------------------------------------------------
void ofApp::setup()
{
    ofSetFrameRate(30);
    
    // TODO: FILL IN YOUR CONSUMER KEY / SECRET here.

    // This will try to load the credentials file.  If there is no credentials
    // file, it will initiate a web-based authorization process.
    oauth.setup("https://api.twitter.com","CONSUMER_KEY","CONSUMER_SECRET");
    
    // Once authenticated, press space to try an authenticated call.
}

//------------------------------------------------------------------------------
void ofApp::draw()
{
    if(oauth.isAuthorized())
    {
        ofBackground(0,255,0);
    }
    else
    {
        ofBackground(255,0,0);
    }
}

//------------------------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    if(key == ' ')
    {
        if(oauth.isAuthorized())
        {
            std::string s = oauth.get("/1.1/statuses/retweets_of_me.json");
            std::cout << s << std::endl;
        }
        else
        {
            std::cout << "Not authorized yet." << std::endl;
        }
    }
}
