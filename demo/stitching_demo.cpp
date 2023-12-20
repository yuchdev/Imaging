#include "stitching_cli.h"

#include <image_stitching/image_stitcher.h>
#include <boost/program_options.hpp>
#include <iostream>
#include <exception>
#include <memory>
#include <string>


namespace po = boost::program_options;
namespace is = image_stitching;

static CommandLineParams& get_params()
{
    static CommandLineParams p;
    return p;
}

int main(int argc, char* argv[])
{
    try {
        // Parse command-line parameters
        get_params().read_params(argc, argv);

        // Check if help or version flag is set
        if (get_params().is_help()) {
            std::cout << get_params().options_descript() << std::endl;
            return 0;
        }

        if (get_params().is_version()) {
            std::cout << "ImageStitched Demo Application version 1.0\n";
            return 0;
        }

        // Create an ImageStitcher object
        is::ImageStitcher stitcher(get_params().get_directory_path(), get_params().get_stitching_method());

        // Set parameters based on command-line options
        stitcher.setImageExtension(get_params().get_image_extension());
        stitcher.setConfidenceThreshold(get_params().get_confidence_threshold());
        stitcher.setOutputFilePath(get_params().get_output_file_path());
        stitcher.setDebugMode(get_params().get_debug_mode());

        // Perform image stitching
        if (!stitcher.stitchImages()) {
            std::cerr << "Image stitching failed" << std::endl;
            return -1;
        }

        std::cout << "Image stitching successful" << std::endl;
    }
    catch (const boost::program_options::error& e) {
        // Handle exceptions related to Boost Program Options
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << get_params().options_descript() << '\n';
        return -1;
    }
    catch (const std::exception& e) {
        // Handle general exceptions
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    catch (...) {
        // Handle other unexpected exceptions
        std::cerr << "An unexpected error occurred." << std::endl;
        return -1;
    }

    return 0;
}
