/*==============================================================================
 
 Copyright (c) 2010-2013 Christopher Baker <http://christopherbaker.net>
 
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

#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(30);
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    // TODO: FILL IN YOUR CONSUMER KEY / SECRET here.
    // THIS will try to load the credentials file.  If there is no 
    // credentials file, it will initiate a web-based authorization process.
    oauth.setup("https://vimeo.com","CONSUMER_KEY","CONSUMER_SECRET");
    
    // Once authenticated, press space to try an authenticated call.
}

//--------------------------------------------------------------
void testApp::update(){}

//--------------------------------------------------------------
void testApp::draw(){
    if(oauth.isAuthorized()) {
        ofBackground(0,255,0);
    } else {
        ofBackground(255,0,0);
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
     if(key == ' ') {
        if(oauth.isAuthorized()) {
            string username = "christopherbaker"; // replace this with your own
            string s = oauth.get("/api/rest/v2?method=vimeo.activity.happenedToUser&user_id="+username);
            ofLogNotice("testApp::keyPressed") << s;
        } else {
            ofLogWarning("testApp::keyPressed") << "Not authorized yet.";
        }
    }
}
