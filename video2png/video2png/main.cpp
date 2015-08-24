#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <fstream>

#include <direct.h>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
	CvCapture* capture = cvCreateFileCapture(argv[1]);
	int length = (int)cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_COUNT);
	//cout << length << endl;
	char *imageData = new char[640*480*3];

	int frameNumber = 0;

	char drive[512];
	char dir[512];
	char fileName[512];
	char ext[512];

	_splitpath(argv[1], drive, dir, fileName, ext);

	string folderName = drive;
	folderName += dir;
	folderName += fileName;

	cout << "Files are saved in: " << folderName << endl;

	mkdir(folderName.c_str());

	int w, h;

	while(1)
    {
		IplImage *tempFrame = cvQueryFrame(capture);

		if(!tempFrame)
        {
			cout << endl;
            printf("Capture Finished\n");
            break;
        }

		Mat frame(tempFrame);

		resize(frame, frame, Size(640, 480));

		cv::Size size = frame.size();

		w = size.width;
		h = size.height;

		// set filename
		int i;
		char buffer [33];

		itoa (frameNumber, buffer, 10);

		string outputFileName = folderName;
		outputFileName += "\\Frame_";
		outputFileName.append(buffer);
		string outputFileNamePNG = outputFileName + ".png";
		string outputFileNameSIF = outputFileName + ".sif";

		// save as png
		//imwrite(outputFileNamePNG, frame);

		// write using simple RGBRGBRGB... format
		for(int i = 0; i < h; i++)
		{
			for(int j = 0; j < w; j++)
			{
				imageData[3 * ((h - i - 1) * w + j) + 0] = frame.data[3 * (i * w + j) + 2];
				imageData[3 * ((h - i - 1) * w + j) + 1] = frame.data[3 * (i * w + j) + 1];
				imageData[3 * ((h - i - 1) * w + j) + 2] = frame.data[3 * (i * w + j) + 0];
			}
		}

		ofstream outputFile;
		outputFile.open(outputFileNameSIF, ios::out | ios::binary);
		outputFile.write(imageData, w * h * 3);
		outputFile.close();

		frameNumber++;

		if(frameNumber % (length / 10) == 0)
			cout << ". ";
    }

	

	// write info file
	string infoFileName = folderName;
	infoFileName += "\\info.txt";

	ofstream infoFile;
	infoFile.open(infoFileName);

	infoFile << w << endl;
	infoFile << h << endl;
	infoFile << 24 << endl;
	
	infoFile.close();

    cvReleaseCapture(&capture);
    return 0;
}