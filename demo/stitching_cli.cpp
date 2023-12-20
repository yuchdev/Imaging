#include "stitching_cli.h"

namespace po = boost::program_options;

CommandLineParams::CommandLineParams() : cmd_options_description("Command-line interface has the following options")
{}

void CommandLineParams::read_params(int argc, char* argv[])
{
    cmd_options_description.add_options()
            ("help,h", "Print this help message")
            ("version,v", "Print version information")
            ("directory,d", po::value<std::string>(&_directory_path)->default_value(""), "Specify the directory path containing the input images")
            ("extension,e", po::value<std::string>(&_image_extension)->default_value("jpg"), "Specify the file extension of the images to be stitched")
            ("method,m", po::value<std::string>(&_stitching_method)->default_value("panorama"), "Choose the stitching method (e.g., panorama, cylindrical, spherical)")
            ("confidence,c", po::value<double>(&_confidence_threshold)->default_value(0.8), "Set the confidence threshold for feature matching")
            ("output,o", po::value<std::string>(&_output_file_path)->default_value(""), "Specify the path for saving the stitched image")
            ("debug", po::bool_switch(&_debug_mode)->default_value(false), "Enable extended debug output")
            ;

    // command line params processing
    po::variables_map cmd_variables_map;
    po::store(parse_command_line(argc, argv, cmd_options_description), cmd_variables_map);
    po::notify(cmd_variables_map);

    set_flag(cmd_variables_map, _help, "help");
    set_flag(cmd_variables_map, _version, "version");

    // default output file name
    if (_output_file_path.empty()) {
        _output_file_path = "stitched_image." + _image_extension;
    }

    // Additional validation
    validate_method();
    validate_confidence();
}

void CommandLineParams::validate_method()
{
    // Check if the specified stitching method is valid
    if (_stitching_method != "panorama" && _stitching_method != "scans") {
        throw std::runtime_error("Invalid stitching method. Supported methods: panorama, scans");
    }
}

void CommandLineParams::validate_confidence()
{
    // Check if confidenceThreshold is within a valid range
    const double minConfidence = 0.0;
    const double maxConfidence = 1.0;

    if (_confidence_threshold < minConfidence || _confidence_threshold > maxConfidence) {
        throw std::runtime_error("Invalid confidence threshold. Confidence must be in the range [0.0, 1.0]");
    }
}

/**@brief Set some logical param */
void CommandLineParams::set_flag(const po::variables_map& vm, bool& flag, const char* str)
{
    if (vm.count(str)) {
        flag = true;
    }
}
