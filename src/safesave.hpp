/**
 * @file
 * 
 * Atomically save to a file.
 *
 * @date 2017-08-20
 * @author Patxi Lazur
 */

#ifndef HPP_2a699ab6_859d_11e7_bed9_6bed937a2ff9
#define HPP_2a699ab6_859d_11e7_bed9_6bed937a2ff9
#include "temp_file.hpp"
#include "fileutils.hpp"
#include <sstream>
#include <string>
#include <system_error>



namespace patxitron { namespace util {

struct safesave_error: public ::std::system_error
{
    safesave_error(int error_code, ::std::string const& what):
        ::std::system_error(error_code, ::std::generic_category(), what) {}
};



/**
 * Writes atomically the given data to a file.
 * 
 * @param data Data to write, must be of a streamable type.
 * @param filename File name and path to be writen. If empty, a temporary one
 *                 in /tmp is created.
 * @return Name of the writen file if successful, empty string otherwise.
 */
template<typename T, bool THROW=false>
inline ::std::string safesave(T const& data
                             ,::std::string const& filename=::std::string())
{
    ::std::string name(filename);
    if (name.empty()) name = "/tmp/tempfileXXXXXX";
    ::std::vector<char> result(name.begin(), name.end());
    temp_file<THROW> tmp(name);
    if (tmp) {
        ::std::stringstream sstream;
        sstream << data;
        if (write(tmp, sstream.str())) {
            if (!filename.empty()) {
                if (!tmp.rename(filename)) return ::std::string();
            }
            return tmp.name();
        }
    }
    return ::std::string();
}

}} // namespace patxitron::util

#endif // HPP_2a699ab6_859d_11e7_bed9_6bed937a2ff9
