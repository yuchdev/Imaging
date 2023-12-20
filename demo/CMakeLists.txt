project(stitching_demo)

# Find Boost components
find_package(Boost REQUIRED COMPONENTS program_options)

# Add the stitching_demo executable
add_executable(stitching_demo
        stitching_demo.cpp
        stitching_cli.cpp
        stitching_cli.h
)

# Link libraries
target_link_libraries(stitching_demo PRIVATE 
    Boost::program_options
    image_stitching
)

# Compiler features
target_compile_features(stitching_demo PRIVATE cxx_std_11)
