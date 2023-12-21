#pragma once
#include <boost/program_options.hpp>
#include <string>


class CommandLineParams 
{
public:
    /**
     * Constructor creates command-line options description
     */
    CommandLineParams();

    /**
     * Read command-line parameters
     * @param argc
     * @param argv
     */
    void read_params(int argc, char* argv[]);

    /**
     * Set boolean params
     * @param vm variables map
     * @param flag boolean flag
     * @param str description
     */
    void set_flag(const boost::program_options::variables_map& vm, bool& flag, const char* str);

    /**
     * Getter function for command-line options description
     * @return command-line options description
     */
    boost::program_options::options_description& options_descript() { return cmd_options_description; }

    /**
     * Validate the stitching method
     */
    void validate_method();

    /**
     * Validate the confidence threshold
     */
    void validate_confidence();

    // Getter functions for flags
    bool is_help() const {
        return _help;
    }

    // Getter functions for flags
    bool is_version() const {
        return _version;
    }

    // Getter functions for parameters
    const std::string& get_directory_path() const {
        return _directory_path;
    }

    // Getter functions for parameters
    const std::string& get_image_extension() const {
        return _image_extension;
    }

    // Getter functions for parameters
    const std::string& get_stitching_method() const {
        return _stitching_method;
    }

    // Getter functions for parameters
    const std::string& get_output_file_path() const {
        return _output_file_path;
    }

    // Getter functions for parameters
    double get_confidence_threshold() const {
        return _confidence_threshold;
    }

    // Getter functions for flags
    bool get_debug_mode() const {
        return _debug_mode;
    }

    // Getter functions for flags
    bool get_d3() const {
        return _d3;
    }

private:
    bool _help = false;
    bool _version = false;
    bool _debug_mode = false;
    bool _d3 = false;

    // Parameters
    std::string _directory_path;
    std::string _image_extension;
    std::string _stitching_method;
    std::string _output_file_path;
    double _confidence_threshold;

    boost::program_options::options_description cmd_options_description;
};
