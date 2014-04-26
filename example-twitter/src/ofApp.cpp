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



void ofApp::setup()
{
    ofSetFrameRate(30);
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    // A simplified interface when using the ofxExampleTwitterClient class.
    // Twitter-specific configuration details can be taken care of the 
    // ofxOAuth base class.
    client.setup("0temMq9KWLx8731GcTXevA","7bI0ScK5gBN3dlGxaC9oTuVFQQXThC9prfD28nmrY");
    
    // Once authenticated, press the spacebar to try a test call.
}


void ofApp::draw()
{
    if(client.isAuthorized())
    {
        ofBackground(0,255,0);
    }
    else
    {
        ofBackground(255,0,0);
    }
}


void ofApp::keyPressed(int key)
{
     if(key == ' ')
     {
        if(client.isAuthorized())
        {
            // Get tweets.
            std::string s = client.exampleMethod();

            // Post new status
            // std::string s = client.exampleUpdateStatusMethod("Hi from OF!");

            // Post status & image
            // std::string s = client.exampleUpdateStatusWithPhotoMethod("Hi from OF!", "buses.jpg");
            ofLogNotice("testApp::keyPressed") << s;
        }
        else
        {
            ofLogWarning("testApp::keyPressed") << "Not authorized yet.";
        }
    }
    
    
}
