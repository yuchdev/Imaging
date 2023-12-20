#include "ImageStitcher.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/stitching.hpp>

using std::string;
using std::vector;
using std::map;
using namespace image_stitching;

// static
map<cv::Stitcher::Status, string> ImageStitcher::error_messages = {
        {cv::Stitcher::OK, "OK"},
        {cv::Stitcher::ERR_NEED_MORE_IMGS, "Not enough images for stitching"},
        {cv::Stitcher::ERR_HOMOGRAPHY_EST_FAIL, "Homography estimation failed"},
        {cv::Stitcher::ERR_CAMERA_PARAMS_ADJUST_FAIL, "Camera parameter adjustment failed"}
};


void ImageStitcher::setDirectoryPath(const string& directory_path) :
        _stitcher(cv::Stitcher::create()),
        _directory_path(directory_path)
{
}

void ImageStitcher::setImageExtension(const std::string& image_extension)
{
    _image_extension = image_extension;
}

void ImageStitcher::setStitchingMethod(const std::string& stitching_method)
{
    _stitching_method = stitching_method;
}

void ImageStitcher::setConfidenceThreshold(double confidence_threshold)
{
    _confidence_threshold = confidence_threshold;
}

void ImageStitcher::setOutputFilePath(const std::string& output_file_path)
{
    _output_file_path = output_file_path;
}

void ImageStitcher::setDebugMode(bool debug_mode)
{
    _debug_mode = debug_mode;
}

bool ImageStitcher::stitchImages() {

    // Read images from the specified directory
    std::vector<cv::Mat> images;
    std::vector<cv::String> imagePaths;
    cv::glob(_directory_path + "/*." + _image_extension, imagePaths);

    // Read images
    for (const auto& imagePath : imagePaths) {
        cv::Mat img = cv::imread(imagePath);
        if (img.empty()) {
            std::cerr << "Error reading image: " << imagePath << std::endl;
            return false;
        }
        images.push_back(img);
    }

    // Check if there are enough images for stitching
    if (images.size() < 2) {
        std::cerr << "Insufficient number of images for stitching." << std::endl;
        return false;
    }

    // Configure the Stitcher
    cv::Mat result;
    _stitcher->setPanoConfidenceThresh(_confidence_threshold);
    // Set other Stitcher parameters based on the class settings

    // Perform image stitching
    cv::Stitcher::Status status = _stitcher->stitch(images, result);

    // Check if stitching was successful
    if (status != cv::Stitcher::OK) {
        std::cerr << "Stitching failed: " << status << std::endl;
        return false;
    }

    // Display the stitched image (optional)
    if (_debug_mode) {
        cv::imshow("Stitched Image", result);
        cv::waitKey(0);
    }

    // Save the stitched image
    if (!_output_file_path.empty()) {
        if (!cv::imwrite(_output_file_path, result)) {
            std::cerr << "Error saving stitched image to: " << _output_file_path << std::endl;
            return false;
        }
    }

    return true;
}
