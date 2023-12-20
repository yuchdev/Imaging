#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>

namespace cv {
class Stitcher;
}

namespace image_stitching
{

/**
 * Class for image stitching
 * Accept a number of parameters to control the stitching process
 */
class ImageStitcher {
public:

    /**
     * Constructor creates an OpenCV Stitcher object
     * @param directoryPath: directory path where the input images are stored
     * @param stitchingMethod: stitching method (e.g., "panorama", "scans")
     */
    explicit ImageStitcher(const std::string& directory_path, const std::string& stitching_method);

    /**
     * Set the directory path where the input images are stored
     * @param directoryPath
     */
    void setDirectoryPath(const std::string& directory_path);

    /**
     * Set the image extension (e.g., "jpg", "png", etc.)
     * @param imageExtension
     */
    void setImageExtension(const std::string& image_extension);

    /**
     * Set the confidence threshold for the stitching process
     * The default value is 0.8
     * Increasing the value will result in fewer images being stitched together
     * @param confidenceThreshold
     */
    void setConfidenceThreshold(double confidence_threshold);

    /**
     * Set the output file path
     * @param outputFilePath
     */
    void setOutputFilePath(const std::string& output_file_path);

    /**
     * Set debug mode for additional output
     * @param debugMode
     */
    void setDebugMode(bool debug_mode);

    /**
     * Stitch images
     * @return true if successful, false otherwise
     */
    bool stitchImages();

private:

    /// OpenCV Stitcher object
    cv::Ptr<cv::Stitcher> _stitcher;

    /// Parameters for the stitching process
    std::string _directory_path;

    /// Parameters for the stitching process
    std::string _image_extension;

    /// Parameters for the stitching process
    std::string _output_file_path;

    /// Parameters for the stitching process
    double _confidence_threshold = 0.8;

    /// Parameters for the stitching process
    bool _debug_mode = false;

    /// Map for OpenCV error messages
    static std::map<cv::Stitcher::Status, std::string> error_messages;
};

} // namespace image_stitching
