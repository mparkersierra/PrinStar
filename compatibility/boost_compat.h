#ifndef BOOST_COMPATIBILITY_HPP
#define BOOST_COMPATIBILITY_HPP

// Universal Boost 1.88 compatibility header for BambuStudio
// Fixes all compatibility issues with newer Boost versions

#include <boost/version.hpp>

// Core filesystem includes
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

// Process includes - handle the v1 namespace change
#include <boost/process.hpp>
#if BOOST_VERSION >= 108800 // Boost 1.88.0 and later
    #include <boost/process/v1/spawn.hpp>
    #include <boost/process/v1/args.hpp>
    #include <boost/process/v1/child.hpp>
    #include <boost/process/v1/io.hpp>
    #include <boost/process/v1/pipe.hpp>
    #include <boost/process/v1/search_path.hpp>
#else
    #include <boost/process/spawn.hpp>
    #include <boost/process/args.hpp>
    #include <boost/process/child.hpp>
    #include <boost/process/io.hpp>
    #include <boost/process/pipe.hpp>
    #include <boost/process/search_path.hpp>
#endif

// Standard library includes for string_file replacement
#include <fstream>
#include <string>
#include <stdexcept>
#include <iterator>

// Create namespace aliases for backward compatibility
namespace boost {
    namespace process {
#if BOOST_VERSION >= 108800
        // Make v1 functions available in the old namespace
        using namespace boost::process::v1;
#endif
    }
}

namespace fs = boost::filesystem;

// === REPLACEMENT FUNCTIONS FOR DEPRECATED BOOST FUNCTIONALITY ===

// Replacement for the deprecated fs::load_string_file function
inline void load_string_file(const fs::path& file_path, std::string& content) {
    std::ifstream file(file_path.string().c_str(), std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + file_path.string());
    }
    
    // Read the entire file content
    file.seekg(0, std::ios::end);
    content.reserve(file.tellg());
    file.seekg(0, std::ios::beg);
    
    content.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    file.close();
}

// Replacement for the deprecated fs::save_string_file function
inline void save_string_file(const fs::path& file_path, const std::string& content) {
    std::ofstream file(file_path.string().c_str(), std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot create file: " + file_path.string());
    }
    
    file << content;
    file.close();
}

// Make the functions available in the fs namespace for compatibility
namespace boost {
    namespace filesystem {
        using ::load_string_file;
        using ::save_string_file;
    }
}

// === ADDITIONAL COMPATIBILITY FIXES ===

// Fix for any ipstream/opstream issues (common in process code)
namespace boost {
    namespace process {
        // These are usually available but let's ensure they're accessible
        using boost::process::ipstream;
        using boost::process::opstream;
    }
}

// === COMMON MACRO FIXES ===

// Some older code might use these macros
#ifndef BOOST_PROCESS_V1_COMPATIBLE
#define BOOST_PROCESS_V1_COMPATIBLE
#endif

// === PLATFORM-SPECIFIC FIXES ===
#ifdef __APPLE__
    // macOS-specific fixes if needed
    #include <boost/process/extend.hpp>
#endif

#ifdef _WIN32
    // Windows-specific fixes if needed
    #include <boost/process/windows.hpp>
#endif

// === DEBUG INFORMATION ===
#ifdef BOOST_COMPATIBILITY_DEBUG
    #pragma message("Boost Compatibility Header loaded")
    #pragma message("Boost version: " BOOST_LIB_VERSION)
#endif

#endif // BOOST_COMPATIBILITY_HPP
