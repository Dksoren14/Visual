#include "utils.h"
#include "manipulator.h"

#include <filesystem>

using namespace std;
using namespace cv;



int radius = 15;

cv::Point nonBackround_point;

int main() {
    
    // Load configuration file 
    std::string configPath = CONFIG_PATH;
    Config cfg = loadConfig(configPath);
    // Load image
    string filename = "jesper2.jpeg";
    cv::Mat image = fetch_image(filename);
    std::cout << cfg.median_filter.kernel_size << std::endl;

    // Process image

    cv::Mat saturatedImage = saturation(image, 2);


    cv::Mat brightness_contrast_image = brightnees_contrast(saturatedImage, cfg.brightness_contrast.contrast, cfg.brightness_contrast.brightness);
    cv::Mat medianFiltered = median_filter(image, cfg.median_filter.kernel_size);
    //cv::Mat bilateralImage = bilateralFilter(image, cfg.bilateral_filter.d, cfg.bilateral_filter.sigmaColor, cfg.bilateral_filter.sigmaSpace);
    //cv::Mat denoisedImage = denoise_algorithm(bilateralImage, cfg.denoise.h, cfg.denoise.hColor, cfg.denoise.templateWindowSize, cfg.denoise.searchWindowSize);
    cv::Mat edges = canny_edge_detection(brightness_contrast_image, cfg.canny_parameters.threshold.low_threshold, cfg.canny_parameters.threshold.max_threshold);
    //
    cv::Mat closedImage = closing_morphology(edges, cfg.blob_detection.connectivity);
    cv::Mat openingImage = opening_morphology(closedImage, 2);
    BlobData blobs = blob_detection(openingImage, 4);
    cout << "Total labels (including background): " << blobs.numLabels << endl;
    // Draw circles around detected blobs (excluding background aka label 0)
    //for(int i = 1; i < blobs.numLabels; ++i) {
    //    cv::Point2d centroid(
    //        blobs.centroids.at<double>(i, 0),
    //        blobs.centroids.at<double>(i, 1)
    //    );
    //    cv::Point center(static_cast<int>(centroid.x), static_cast<int>(centroid.y));
    //    image = draw_circles(image, center, radius, i);
    //}
  
    cout << "Number of blobs detected: " << blobs.numLabels -1 << endl;
    cv::Mat labeledImage = draw_circles(image, nonBackround_point, radius, 1);
    cv::Mat RedEnhanced = BGR_channel_changer(labeledImage, 2, 2.0);
    //cv::imshow("Bilateral Filtered Image", bilateralImage); 
    //cv::imshow("Denoised Image", denoisedImage);
    //cv::imshow("Brightness & Contrast Adjusted Image", brightness_contrast_image);
    //cv::imshow("Median Filtered Image", medianFiltered);
    //cv::imshow("Edges", labeledImage);
    //cv::imshow("Labeled Blobs", labeledImage);
    //cv::imshow("Saturation Adjusted Image", saturatedImage);
    cv::imshow("Final Labeled Image", RedEnhanced);
    cv::waitKey(0);


    //!! Tuning test
    //tuning(image, 3);
    return 0;

}


