#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat getSimilarPixelMask(const Mat& region, const Point& coord, int tolerance = 10) {
    Vec3b targetColor = region.at<Vec3b>(coord.y, coord.x);
    Mat mask(region.size(), CV_8U, Scalar(0));

    for (int y = 0; y < region.rows; y++) {
        for (int x = 0; x < region.cols; x++) {
            Vec3b color = region.at<Vec3b>(y, x);
            if (abs(color[0] - targetColor[0]) <= tolerance &&
                abs(color[1] - targetColor[1]) <= tolerance &&
                abs(color[2] - targetColor[2]) <= tolerance) {
                mask.at<uchar>(y, x) = 255;
            }
        }
    }
    return mask;
}

void drawContoursOnImage(Mat& img, const Mat& mask, Scalar color = Scalar(0, 255, 0), int thickness = 1) {
    Mat blurredMask;
    GaussianBlur(mask, blurredMask, Size(5, 5), 0);

    vector<vector<Point>> contours;
    findContours(blurredMask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    drawContours(img, contours, -1, color, thickness);
}

int main() {
    Mat img = imread("C:\\Users\\DR-LT-31\\Downloads\\image\\1-RadNo_Y12345_W12345_FCAW_Thk14mm_SFD609mm_Exp30sec (2).png");
    if (img.empty()) {
        cerr << "Error: Could not load image." << endl;
        return -1;
    }

    int x, y;
    cout << "Enter the x coordinate: ";
    cin >> x;
    cout << "Enter the y coordinate: ";
    cin >> y;

    int regionWidth, regionHeight;
    cout << "Enter the width of the region: ";
    cin >> regionWidth;
    cout << "Enter the height of the region: ";
    cin >> regionHeight;

    int halfWidth = regionWidth / 2;
    int halfHeight = regionHeight / 2;

    int topLeftX = max(x - halfWidth, 0);
    int topLeftY = max(y - halfHeight, 0);
    int bottomRightX = min(x + halfWidth, img.cols);
    int bottomRightY = min(y + halfHeight, img.rows);

    Mat region = img(Rect(topLeftX, topLeftY, bottomRightX - topLeftX, bottomRightY - topLeftY)).clone();

    int centerX = x - topLeftX;
    int centerY = y - topLeftY;

    Mat mask = getSimilarPixelMask(region, Point(centerX, centerY));

    drawContoursOnImage(region, mask);

    region.copyTo(img(Rect(topLeftX, topLeftY, region.cols, region.rows)));

    namedWindow("Image with Highlighted Contours", WINDOW_AUTOSIZE);
    imshow("Image with Highlighted Contours", img);

    imwrite("E://magicwand//highlightedimage.png", img);
    waitKey(0);

    return 0;
}
