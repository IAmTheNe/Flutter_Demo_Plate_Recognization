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
        drawContours(input1, contours, -1, Scalar(0, 255, 0), 3);

        sort(contours.begin(), contours.end(), [](const vector<Point>& c1, const vector<Point>& c2) {
            return cv::contourArea(c1) > cv::contourArea(c2);
        });
        vector<vector<Point>> cnts(contours.begin(), contours.begin() + 30);

        Mat input2 = input.clone();
        drawContours(input2, cnts, -1, Scalar(0, 255, 0), 3);
        
       
        int i = 7;
        Point2f screenCnt[4];
        for (size_t idx = 0; idx < cnts.size(); idx++) {
            double perimeter = arcLength(cnts[idx], true);
            double epsilon = 0.018 * perimeter;
            vector<Point> approx;
            approxPolyDP(cnts[idx], approx, epsilon, true);

            if (approx.size() == 4) {
                for (int j = 0; j < 4; j++) {
                    screenCnt[j] = approx[j];
                }

                Rect b = boundingRect(cnts[idx]);
                Mat new_img = input(b);
                imwrite(outputImagePath, new_img);

                i++;
                break;
            }
        }

       drawContours(input, vector<vector<Point>>{vector<Point>(screenCnt, screenCnt + 4)}, -1, Scalar(0, 255, 0), 3);

     }
}

