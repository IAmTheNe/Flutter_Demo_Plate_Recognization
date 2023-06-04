#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

// #include "LicenseDetector.h"

using namespace cv;
using namespace std;

// static LicenseDetector* detector = nullptr;

extern "C"
{
    // Attributes to prevent 'unused' function from being removed and to make it visible
    __attribute__((visibility("default"))) __attribute__((used))
    const char *
    version()
    {
        return CV_VERSION;
    }

    __attribute__((visibility("default"))) __attribute__((used)) void process_image(char *inputImagePath, char *outputImagePath)
    {
        // Đọc ảnh biển số xe
        Mat image = imread(inputImagePath);

        // Chuyển đổi sang ảnh grayscale
        Mat gray;
        cvtColor(image, gray, COLOR_BGR2GRAY);

        // Tiền xử lý ảnh
        Mat processedImage;
        bilateralFilter(gray, processedImage, 11, 17, 17);

        // Phát hiện cạnh
        Mat edged;
        Canny(processedImage, edged, 30, 200);

        // Tìm contours
        vector<vector<Point>> contours;
        findContours(edged.clone(), contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

        // Sắp xếp contours theo diện tích giảm dần
        sort(contours.begin(), contours.end(), [](const vector<Point> &c1, const vector<Point> &c2)
             { return contourArea(c1) > contourArea(c2); });

        // Chọn 30 contours có diện tích lớn nhất
        vector<vector<Point>> cnts(contours.begin(), contours.begin() + min(30, static_cast<int>(contours.size())));

        // Tìm contour hình vuông
        Rect boundingRect;
        for (const auto &contour : cnts)
        {
            double perimeter = arcLength(contour, true);
            double epsilon = 0.018 * perimeter;
            vector<Point> approx;
            approxPolyDP(contour, approx, epsilon, true);

            if (approx.size() == 4)
            {
                boundingRect = cv::boundingRect(contour);
                break;
            }
        }

        // Cắt ảnh theo contour hình vuông
        Mat new_image = image(boundingRect);

        // Lưu ảnh đầu ra
        imwrite(outputImagePath, new_image);
    }
}
