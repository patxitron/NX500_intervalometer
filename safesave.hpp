/**
 * @file
 * 
 * Atomically save to a file.
 *
 * @date 2017-08-20
 * @uthor Francisco J. Lazur
 */

#ifndef H_2a699ab6_859d_11e7_bed9_6bed937a2ff9
#define H_2a699ab6_859d_11e7_bed9_6bed937a2ff9
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <unistd.h>



namespace patxitron {

/**
 * Writes atomically the given data to a file.
 * 
 * @param data Data to write, must be of a streamable type.
 * @param filename File name and path to be writen. If empty, a temporary one
 *                 in /tmp is created.
 * @return filename of the writen file if successful, empty string otherwise.
 */
template<typename T>
::std::string safeSave(T const& data
                      ,::std::string const& filename=::std::string())
{
    ::std::vector<char> result(filename.begin(), filename.end());
    if (result.size() == 0) {
        char tmpfilename[] = "/tmp/tempfileXXXXXX";
        result.insert(
             result.end()
            ,tmpfilename
            ,tmpfilename + strlen(tmpfilename) + 1
        );
    } else {
        char tmpfilename[] = "XXXXXX";
        result.insert(
             result.end()
            ,tmpfilename
            ,tmpfilename + strlen(tmpfilename) + 1
        );
    }
    int fd = mkstemp(result.data());
    if (fd > 0) {
        ::std::stringstream sstream;
        sstream << data;
        ::std::string to_write(sstream.str());
        size_t bytes_so_far = 0;
        while (bytes_so_far < to_write.size()) {
            ssize_t bytes_written = write(
                 fd
                ,to_write.c_str() + bytes_so_far
                ,to_write.size() - bytes_so_far
            );
            if (bytes_written > 0) {
                bytes_so_far += bytes_written;
            } else {
                close(fd);
                unlink(result.data());
                return ::std::string();
            }
        }
        close(fd);
        if (filename.empty()) {
            return ::std::string(result.begin(), result.end() - 1);
        } else {
            if (rename(result.data(), filename.c_str()) == 0) {
                return filename;
            } else {
                unlink(result.data());
            }
        }
    }
    return ::std::string();
}

} // namespace patxitron

#endif // H_2a699ab6_859d_11e7_bed9_6bed937a2ff9
