#include <opencv2/opencv.hpp>
#include <opencv2/stitching.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;
using namespace cv;

int main(int argc, char* argv[]) 
{
    // Check if the directory is provided as a command-line argument
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <directory_path>" << endl;
        return -1;
    }

    // Read images from the specified directory
    string directory_path = argv[1];
    string file_mask = directory_path + std::string("/*.jpg");
    vector<Mat> images;
    vector<String> image_paths;

    // Use the glob function to get all files in the directory matching a specific pattern (e.g., "*.jpg")
    glob(directory_path + "/*.jpg", image_paths);

    cout  << "Directory path: " << directory_path << std::endl;

    // Print images
    for (const auto& image_path : image_paths) {
        cout << "Image path: " << image_path << '\n';
    }

    // Read images
    for (const auto& image_path : image_paths) {
        Mat img = imread(image_path);
        if (img.empty()) {
            cerr << "Error reading image: " << image_path << '\n';
            return -1;
        }
        images.push_back(img);
    }

    // Check if there are enough images for stitching
    if (images.size() < 2) {
        cerr << "Insufficient number of images for stitching." << endl;
        return -1;
    }
    cout << "Number of images to stitch: " << images.size() << '\n';

    cout << "Stitch images\n";
    Mat result;
    cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create();
    stitcher->setPanoConfidenceThresh(1);  // Set confidence threshold to 1 for more output
    stitcher->setWaveCorrection(true);      // Enable wave correction

    Stitcher::Status status = stitcher->stitch(images, result);

    cout << "Check if stitching was successful\n";
    if (status != Stitcher::OK) {
        static std::unordered_map<Stitcher::Status, std::string> error_messages = {
            {Stitcher::OK, "OK"},
            {Stitcher::ERR_NEED_MORE_IMGS, "Not enough images for stitching"},
            {Stitcher::ERR_HOMOGRAPHY_EST_FAIL, "Homography estimation failed"},
            {Stitcher::ERR_CAMERA_PARAMS_ADJUST_FAIL, "Camera parameter adjustment failed"}
            // Add more mappings as needed
        };

        cerr << "Stitching failed: ";
        auto it = error_messages.find(status);
        if (it != error_messages.end()) {
            cerr << it->second;
        } 
        else {
            cerr << "Unknown error";
        }

        cerr << endl;
        return -1;
    }

    // Display the stitched image
    imshow("Stitched Image", result);
    waitKey(0);

    return 0;
}
