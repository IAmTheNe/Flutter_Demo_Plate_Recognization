#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

// #include "LicenseDetector.h"

using namespace cv;
using namespace std;

// static LicenseDetector* detector = nullptr;

extern "C" {
    // Attributes to prevent 'unused' function from being removed and to make it visible
    __attribute__((visibility("default"))) __attribute__((used))
    const char* version() {
        return CV_VERSION;
    }

    // __attribute__((visibility("default"))) __attribute__((used))
    // void initDetector(uint8_t* markerPngBytes) {
    //     if (detector != null) {
    //         delete detector;
    //         detector = nullptr;
    //     }

    //     vector<uint8_t> buffer(markerPngBytes, markerPngBytes);
    //     Mat marker = imdecode(buffer, IMREAD_COLOR);

    //     detector = new LicenseDetector(marker);
    // }

    // __attribute__((visibility("default"))) __attribute__((used))
    // void destroyDetector() {
    //     if (detector != null) {
    //         delete detector;
	// 		detector = nullptr;
    //     }
    // }

     __attribute__((visibility("default"))) __attribute__((used))
     void process_image(char* inputImagePath, char* outputImagePath) {
        Mat input = imread(inputImagePath, IMREAD_GRAYSCALE);
        Mat filter, grey_image, edged;

        vector<vector<Point>> contours;
        
        cvtColor(input, grey_image, COLOR_GRAY2BGR);
        bilateralFilter(grey_image, filter ,11, 17, 17);

        Canny(filter, edged, 30, 200);

        findContours(edged.clone(), contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

        Mat input1 = input.clone();
        drawContours(input1, contours, -1, cv::Scalar(0, 255, 0), 3);

        imwrite(outputImagePath, input1);
     }
}

