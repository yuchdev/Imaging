#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-pass-by-value"
#include <image_stitching/image_stitcher.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/stitching.hpp>

#if 0
#include <Log.h>
using namespace leica_logger;
#else
#define LogWarning(x) std::cout 
#endif

using namespace image_stitching;

using std::string;
using std::vector;
using std::map;


// static
map<cv::Stitcher::Status, string> ImageStitcher::error_messages = {
        {cv::Stitcher::OK, "OK"},
        {cv::Stitcher::ERR_NEED_MORE_IMGS, "Not enough images for stitching"},
        {cv::Stitcher::ERR_HOMOGRAPHY_EST_FAIL, "Homography estimation failed"},
        {cv::Stitcher::ERR_CAMERA_PARAMS_ADJUST_FAIL, "Camera parameter adjustment failed"}
};


ImageStitcher::ImageStitcher(const string& directory_path, const string& stitching_method) :
        _directory_path(directory_path)
{
    // Set the stitching method
    cv::Stitcher::Mode stitching_mode = cv::Stitcher::PANORAMA; // Default to PANORAMA
    if (!stitching_method.empty()) {
        // Check for different stitching modes
        if (stitching_method == "panorama") {
            stitching_mode = cv::Stitcher::PANORAMA;
        }
        else if (stitching_method == "scans") {
            stitching_mode = cv::Stitcher::SCANS;
        }
    }
    _stitcher = cv::Stitcher::create(stitching_mode);

}

std::string ImageStitcher::getStitchingErrorMessage(cv::Stitcher::Status status)
{
    auto it = error_messages.find(status);
    return (it != error_messages.end()) ? it->second : "Unknown error";
}

void ImageStitcher::setDirectoryPath(const string& directory_path)
{
    _directory_path = directory_path;
}

void ImageStitcher::setImageExtension(const std::string& image_extension)
{
    _image_extension = image_extension;
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

void ImageStitcher::setD3Optimization(bool enable) 
{
    _d3 = enable;
}

bool ImageStitcher::stitchImages()
{
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

        if (_d3) {

            // Divide each image into three chunks
            int chunkWidth = img.cols / 3;
            for (int i = 0; i < 3; ++i) {
                cv::Rect rect(i * chunkWidth, 0, chunkWidth, img.rows);
                images.push_back(img(rect).clone());
            }
        } 
        else {
            // Use the whole image
            images.push_back(img);
        }
    }

    // Check if there are enough images for stitching
    if (images.size() < 2) {
        LogWarning() << "Insufficient number of images for stitching";
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
        LogWarning() << "Stitching failed; status=" << status << "; " << getStitchingErrorMessage(status);
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
            LogWarning() << "Error saving stitched image to: " << _output_file_path;
            return false;
        }
    }

    return true;
}

#pragma clang diagnostic pop
