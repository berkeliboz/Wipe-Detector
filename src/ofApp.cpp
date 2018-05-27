#include "ofApp.h"



//--------------------------------------------------------------
void ofApp::setup() {

	createSTIButton.addListener(this, &ofApp::createSTIButFunc);
	createSTIHistButton.addListener(this, &ofApp::createSTIHistButFunc);
	

	cout << "Adjust Processing Speed according to your hardware" << endl;
	cout << "Please don't adjust Processing Speed during processing" << endl;
	cout << "This program was tested using i5-6300U and NVIDIA Maxwell 940m graphics processor" << endl;
	cout << "Thank You for using Wipe Detector by Danying Liu and Berke Boz" << endl;
	cout << endl << endl << endl << endl << endl << endl << endl << endl << endl;

	gui.setup();
	gui.add(createSTIButton.setup("Create STI"));
	gui.add(createSTIHistButton.setup("Create STI Using Histogram"));
	gui.add(processingSpeedSlider.setup("Processing Speed", 0.1, 0.01, 2));
	
	isProcessingWidth = true;
	createSTIBut = false;
	createSTIHistBut = false;
	
	fileGrabber = ofSystemLoadDialog("Load File");
	video.load(fileGrabber.fileName);
	videoHeight = video.getHeight();
	videoWidth = video.getWidth();
	totalVidFrames = video.getTotalNumFrames();
	bins = floor(1 + log2(videoHeight));
	video.play(); 
	video.setSpeed(0.5);
	firstTime = true;
	video.setVolume(0);
	stiByDeltaHistMat = cv::Mat(videoWidth,totalVidFrames*4, CV_32FC1,cv::Scalar(0));
	matFramesWidht = cv::Mat(videoHeight, totalVidFrames, CV_8UC3, cv::Scalar(0, 0, 0));



}

//--------------------------------------------------------------
void ofApp::update() {

		
		if (createSTIBut) {
			resultImageWidth.update();
			
			createSTI(matFramesWidht, matFramesHeight);
			video.setSpeed(0.5);
			video.update();
		}
		if (createSTIHistBut) {
			resultImageWidthSTI.update();
			createSTIbyHist();
			video.update();
			video.setSpeed(processingSpeedSlider);
			if (iPixelVal < 0.3)
				cout << "Transition Happened at frame: " << video.getCurrentFrame() << endl;

		}
		

}

cv::Point2d ofApp::getChromacity(cv::Mat matrix, int x, int y) {

	unsigned int b = matrix.at<cv::Vec3b>(y, x)[0];	//B
	unsigned int g = matrix.at<cv::Vec3b>(y, x)[1];	//G
	unsigned int r = matrix.at<cv::Vec3b>(y, x)[2];	//R
	unsigned int sum = (r + g + b);
	if (sum != 0) {
		double rChroma = (double)r / sum;
		double gChroma = (double)g / sum;
		return cv::Point2d(rChroma, gChroma);
	}
	else {
		return cv::Point2d(0, 0);
	}
}

//Returns Histogram for the column
// Inside Point2d, x is for R, y is for G
vector<vector<float>> ofApp::makeHistogram(cv::Mat col) {
	vector<vector<float>> histogram;
	histogram.resize(bins, vector<float>(bins,0));
	
	
	for (int i = 0; i < col.rows; i++) {
		int reds = ceil((getChromacity(col, 0, i).x) * bins) -1;
		int greens = ceil((getChromacity(col, 0, i).y) * bins) -1;
		if (reds <= -1) reds = 0;
		if (greens <= -1) greens = 0;
		histogram[reds][greens] += (double)1/col.rows;
	}
	return histogram;
}


float ofApp::getIPixel(vector <vector <float>> hist1, vector <vector <float>> hist2) {
	float sum = 0;
	for (int i = 0; i < bins; i++) {
		for (int j = 0; j < bins; j++) {
			sum += min(hist1[j][i], hist2[j][i]);
		}
	}

	return sum;
}


void ofApp::createSTIbyHist() {

		int currentFrameVal = video.getCurrentFrame();
		auto currentFrame = video.getPixels();
		currentFrameMat = ofxCv::toCv(currentFrame);


		if (!firstTime) {

			cv::Mat currentCol(currentFrameMat.cols, 1, CV_32FC1, cv::Scalar(0));

			for (int i = 0; i < currentFrameMat.cols; i++) {
				vector <vector <float>> hist1 = makeHistogram(currentFrameMat.col(i));
				vector <vector <float>> hist2 = makeHistogram(previousFrameMat.col(i));
				iPixelVal =  (float)getIPixel(hist1, hist2);
				if (iPixelVal > 0.9)
					iPixelVal = 1;
				currentCol.at<cv::Vec2f>(i, 0)[0] = iPixelVal;
			}
			currentCol.copyTo(stiByDeltaHistMat.col(currentFrameVal));
		}

		ofxCv::copy(currentFrameMat, previousFrameMat);
		ofxCv::toOf(stiByDeltaHistMat, resultImageWidthSTI);
		firstTime = false;

		if (currentFrameVal == totalVidFrames)
			video.stop();
		
}



void ofApp::createSTI(cv::Mat matFramesWidht,cv::Mat matFramesHeight) {
	if (isProcessingWidth) {

		try
		{
			int currentFrameVal = video.getCurrentFrame();
			auto currentFrame = video.getPixels();
			currentFrameMat = ofxCv::toCv(currentFrame);
			ofxCv::copy(currentFrameMat.col(videoWidth / 2), matFramesWidht.col(currentFrameVal));
			ofxCv::toOf(matFramesWidht, resultImageWidth);
			
			
		}
		catch (const std::exception&)
		{
			isProcessingWidth = false;
			video.stop();
		}

	}

	

}


void ofApp::createSTIButFunc(){ 
	createSTIBut = !createSTIBut;
	createSTIHistBut = false;
	video.setFrame(0);
	stiByDeltaHistMat = cv::Mat(videoWidth, totalVidFrames * 4, CV_32FC1, cv::Scalar(0));
	matFramesWidht = cv::Mat(videoHeight, totalVidFrames, CV_8UC3, cv::Scalar(0, 0, 0));

}
void ofApp::createSTIHistButFunc() {
	createSTIHistBut = !createSTIHistBut;
	createSTIBut = false;
	video.setFrame(0);
	stiByDeltaHistMat = cv::Mat(videoWidth, totalVidFrames * 4, CV_32FC1, cv::Scalar(0));
	matFramesWidht = cv::Mat(videoHeight, totalVidFrames, CV_8UC3, cv::Scalar(0, 0, 0));

}



//--------------------------------------------------------------
void ofApp::draw() {

	video.draw(0, 0);
	
	if (createSTIBut) {
		resultImageWidth.draw(0, videoHeight);

		
	}
	
	if(createSTIHistBut)
		resultImageWidthSTI.draw(0, videoHeight);
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {



}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

