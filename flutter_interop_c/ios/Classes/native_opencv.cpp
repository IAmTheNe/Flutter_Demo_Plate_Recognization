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
        findContours(edged, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

        // Sắp xếp contours theo diện tích giảm dần
        sort(contours.begin(), contours.end(), [](const vector<Point> &c1, const vector<Point> &c2)
             { return contourArea(c1) > contourArea(c2); });

        // Chọn 30 contours có diện tích lớn nhất
        vector<vector<Point>> cnts(contours.begin(), contours.begin() + min(30, static_cast<int>(contours.size())));

        // Tạo biến để lưu trữ contour hình vuông có diện tích lớn nhất
        vector<Point> largestSquareContour;
        double maxArea = 0.0;

        // Duyệt qua danh sách các contour
        for (const auto &contour : cnts)
        {
            // Xấp xỉ contour thành đa giác
            double perimeter = arcLength(contour, true);
            double epsilon = 0.018 * perimeter;
            vector<Point> approx;
            approxPolyDP(contour, approx, epsilon, true);

            // Kiểm tra xem đa giác xấp xỉ có 4 điểm hay không
            if (approx.size() == 4)
            {
                // Tính diện tích của contour
                double area = contourArea(contour);

                // So sánh diện tích với diện tích lớn nhất hiện tại
                if (area > maxArea)
                {
                    maxArea = area;
                    largestSquareContour = contour;
                }
            }
        }

        Mat new_image = image.clone();

        // Kiểm tra xem đã tìm được contour hình vuông có diện tích lớn nhất hay chưa
        if (!largestSquareContour.empty())
        {
            // Tạo hình chữ nhật bao quanh contour hình vuông
            Rect bRect = boundingRect(largestSquareContour);

            // Cắt ảnh theo hình chữ nhật bao quanh contour
            new_image = image(bRect);

            // Lưu ảnh đầu ra
        }

        // Lưu ảnh đầu ra
        imwrite(outputImagePath, new_image);
    }
}
