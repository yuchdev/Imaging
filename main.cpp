#include <opencv2/opencv.hpp>
#include <opencv2/stitching.hpp>
#include <iostream>
#include <vector>
#include <string>

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
    string directoryPath = argv[1];
    vector<Mat> images;
    vector<String> imagePaths;

    // Use the glob function to get all files in the directory matching a specific pattern (e.g., "*.jpg")
    glob(directoryPath + "/*.jpg", imagePaths);

    std::cout  << "Directory path: " << directoryPath << std::endl;

    // Print images
    for (const auto& imagePath : imagePaths) {
        std::cout << "Image path: " << imagePath << std::endl;
    }

    // Read images
    for (const auto& imagePath : imagePaths) {
        Mat img = imread(imagePath);
        if (img.empty()) {
            cerr << "Error reading image: " << imagePath << endl;
            return -1;
        }
        images.push_back(img);
    }

    // Check if there are enough images for stitching
    if (images.size() < 2) {
        cerr << "Insufficient number of images for stitching." << endl;
        return -1;
    }

    // Stitch images
    Mat result;
    cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create();
    Stitcher::Status status = stitcher->stitch(images, result);

    // Check if stitching was successful
    if (status != Stitcher::OK) {
        cerr << "Stitching failed: " << status << endl;
        return -1;
    }

    // Display the stitched image
    imshow("Stitched Image", result);
    waitKey(0);

    return 0;
}
