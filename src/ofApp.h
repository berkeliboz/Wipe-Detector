#pragma once

#include <iostream>
#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "ofxGui.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		void createSTI(cv::Mat matFramesWidht, cv::Mat matFramesHeight);
		
		void createSTIbyHist();

		void createSTIButFunc();
		void createSTIHistButFunc();

		vector<vector<float>> makeHistogram(cv::Mat matrix);

		cv::Point2d getChromacity(cv::Mat, int, int);

		float getIPixel(vector <vector <float>> hist1, vector <vector <float>> hist2);

		ofVideoPlayer video;
		ofFileDialogResult fileGrabber;
	
		bool firstTime;
	
		int videoWidth;
		int videoHeight;
		int totalVidFrames;
		bool isProcessingWidth;
		
		bool createSTIBut;
		bool createSTIHistBut;

		int bins;

		ofxPanel gui;

		ofxButton createSTIButton;
		ofxButton createSTIHistButton;

		ofxFloatSlider processingSpeedSlider;

		

		ofImage resultImageWidth;
		

		ofImage resultImageWidthSTI;

		

		cv::Mat matFramesHeight;
		
		cv::Mat matFramesWidht;
		

		cv::Mat currentFrameMat;
		cv::Mat nextFrameMat;
		cv::Mat previousFrameMat;

		float iPixelVal = 0;
		
		cv::Mat stiByDeltaHistMat;
		
		
	
		vector<vector<float>> stibyDeltaMat;
};


