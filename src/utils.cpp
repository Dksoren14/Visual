#include "utils.h"


#include <filesystem>

using namespace std;
using namespace cv;
namespace fs = std::filesystem;




cv::Mat fetch_image(const std::string& filename) {
    
    fs::path imagePath = fs::current_path() / "../images" / filename;

    cv::Mat img = cv::imread(imagePath.string());

    if (img.empty()) {
        cerr << "Error: Could not load image: " << filename << endl;
    }
    
    
    return img;
}

Config loadConfig(const std::string& path) {
    Config cfg{};
    cv::FileStorage fs(path, cv::FileStorage::READ);
    if (!fs.isOpened()) {
        throw std::runtime_error("Cannot open config file: " + path);
    }


    fs["denoise"]["h"] >> cfg.denoise.h;
    fs["denoise"]["hColor"] >> cfg.denoise.hColor;
    fs["denoise"]["templateWindowSize"] >> cfg.denoise.templateWindowSize;
    fs["denoise"]["searchWindowSize"] >> cfg.denoise.searchWindowSize;

    fs["canny_parameters"]["threshold_low"] >> cfg.canny_parameters.threshold.low_threshold;
    fs["canny_parameters"]["threshold_max"] >> cfg.canny_parameters.threshold.max_threshold;

    fs["blob_detection"]["kernel_size"] >> cfg.blob_detection.kernel_size;
    fs["blob_detection"]["connectivity"] >> cfg.blob_detection.connectivity;

    fs["bilateral_filter"]["d"] >> cfg.bilateral_filter.d;
    fs["bilateral_filter"]["sigmaColor"] >> cfg.bilateral_filter.sigmaColor;
    fs["bilateral_filter"]["sigmaSpace"] >> cfg.bilateral_filter.sigmaSpace;

    fs["brightness_contrast"]["brightness"] >> cfg.brightness_contrast.brightness;
    fs["brightness_contrast"]["contrast"] >> cfg.brightness_contrast.contrast;

    fs.release();
    return cfg;
}



BlobData blob_detection(const cv::Mat& inputImage, int connectivity) {

    cv::Mat binary;
    // Threshold to ensure binary
    cv::threshold(inputImage, binary, 128, 255, cv::THRESH_BINARY);


    cv::Mat labels, stats, centroids;
    int numLabels = cv::connectedComponentsWithStats(binary, labels, stats, centroids, connectivity, CV_32S);

    return {labels, stats, centroids, numLabels};
}

cv::Mat draw_circles(const cv::Mat& inputImage, const cv::Point& centroids, int radius, int i) {
    
    cv::Mat output;
    inputImage.copyTo(output);
    cv::circle(output, centroids, radius, cv::Scalar(0, 0, 255), 4); // Red circle
    return output;

}

void tuning(const cv::Mat& inputImage, int mode) {
    if (inputImage.empty()) {
        cerr << "Error: Input image is empty!" << endl;
        return;
    }

    // Clone original to avoid modification
    Mat image;
    if (inputImage.channels() == 3)
        cvtColor(inputImage, image, COLOR_BGR2GRAY);
    else
        image = inputImage.clone();
    int d = 9;
    int sigmaColor = 33;
    int sigmaSpace = 33;
    int lowThreshold = 50;
    int highThreshold = 150;
    int medianKernelSize = 3;
    int brightness = 100; // Trackbar value from 0 to 200
    int contrast = 100;   // Trackbar value from 0 to 200
    double alpha = contrast / 100.0; // Contrast control
    int beta = brightness - 100; 
    switch(mode) {
        case 0: {
            const string windowName = "Canny + Bilateral Filter Tuning";
            namedWindow(windowName, WINDOW_AUTOSIZE);

            createTrackbar("d", windowName, &d, 20);
            createTrackbar("sigmaColor", windowName, &sigmaColor, 150);
            createTrackbar("sigmaSpace", windowName, &sigmaSpace, 150);
            createTrackbar("Canny Low", windowName, &lowThreshold, 255);
            createTrackbar("Canny High", windowName, &highThreshold, 255);

            Mat bilateral, edges, output;

            cout << "Adjust sliders. Press ESC to exit." << endl;

            while (true) {
                // --- Bilateral filter ---
                bilateralFilter(image, bilateral, d > 0 ? d : 1, sigmaColor, sigmaSpace);
            
                // --- Canny ---
                Canny(bilateral, edges, lowThreshold, highThreshold);
            
                // --- Combine for visualization ---
                cvtColor(edges, output, COLOR_GRAY2BGR);
                addWeighted(output, 0.7, inputImage, 0.3, 0, output);
            
                imshow(windowName, output);
            
                // Exit on ESC key
                int key = waitKey(30);
                if (key == 27)  // ESC
                    break;
            }
        
            destroyWindow(windowName);
            break;
        }
        case 1: {   
            const string windowName = "Canny + median Filter Tuning";
            namedWindow(windowName, WINDOW_AUTOSIZE);
            createTrackbar("Median Kernel Size", windowName, &d, 20);
            createTrackbar("Canny Low", windowName, &lowThreshold, 255);
            createTrackbar("Canny High", windowName, &highThreshold, 255);
            Mat medianFiltered, edges, output;
            cout << "Adjust sliders. Press ESC to exit." << endl;
            while (true) {
                // --- Median filter ---
                medianBlur(image, medianFiltered, d % 2 == 0 ? d + 1 : d);
                // --- Canny ---
                Canny(medianFiltered, edges, lowThreshold, highThreshold);
                // --- Combine for visualization ---
                cvtColor(edges, output, COLOR_GRAY2BGR);
                addWeighted(output, 0.7, inputImage, 0.3, 0, output);
                imshow(windowName, output);
                // Exit on ESC key
                int key = waitKey(30);
                if (key == 27)  // ESC
                    break;
            }
            destroyWindow(windowName);  
            break;
        }
        case 2: {
            const string windowName = "brightness and contrast tuning";
            namedWindow(windowName, WINDOW_AUTOSIZE);
      
            createTrackbar("Brightness", windowName, &brightness, 200);
            createTrackbar("Contrast", windowName, &contrast, 200);
            createTrackbar("Canny Low", windowName, &lowThreshold, 255);
            createTrackbar("Canny High", windowName, &highThreshold, 255);
            Mat adjusted, adjustedBGR, edges, output;
            cout << "Adjust sliders. Press ESC to exit." << endl;
            while (true) {
                // Adjust brightness and contrast
                // Recompute alpha and beta from trackbar positions
                alpha = contrast / 100.0;
                beta = brightness - 100;
                // Brightness control
                image.convertTo(adjusted, -1, alpha, beta);
                // Ensure same number of channels as inputImage for addWeighted
                if (adjusted.channels() == 1 && inputImage.channels() == 3) {
                    cvtColor(adjusted, adjustedBGR, COLOR_GRAY2BGR);
                } else {
                    adjustedBGR = adjusted;
                }
                // Combine for visualization
                addWeighted(adjustedBGR, 0.7, inputImage, 0.3, 0, output);
                Canny(adjusted, output, lowThreshold, highThreshold);
                imshow(windowName, output);
                // Exit on ESC key
                int key = waitKey(30);
                if (key == 27)  // ESC
                    break;
            }
            destroyWindow(windowName);
            break;
        } 
    }
}

cv::Mat brightnees_contrast(const cv::Mat& inputImage, double contrast, int brightness) {
    cv::Mat output;
    double alpha = contrast / 100.0;
    double beta = brightness - 100;
    inputImage.convertTo(output, -1, alpha, beta);
    return output;
}

cv::Mat erosion_morphology(const cv::Mat& inputImage, int kernel_size) {
    cv::Mat output;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,
                        cv::Size(2 * kernel_size + 1, 2 * kernel_size + 1),
                        cv::Point(kernel_size, kernel_size));
    cv::erode(inputImage, output, element);
    return output;
}

cv::Mat opening_morphology(const cv::Mat& inputImage, int kernel_size) {
    cv::Mat outputImage;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,
                        cv::Size(2 * kernel_size + 1, 2 * kernel_size + 1),
                        cv::Point(kernel_size, kernel_size));
    cv::morphologyEx(inputImage, outputImage, cv::MORPH_OPEN, element);
    return outputImage;
}