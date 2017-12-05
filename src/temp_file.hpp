#ifndef HPP_67792d4e_a754_11e7_a9be_f78be9ea07d4
#define HPP_67792d4e_a754_11e7_a9be_f78be9ea07d4
#include <system_error>
#include <string>
#include <utility>
#include <cstring>
#include <vector>
#include <cstdio>
#include <cerrno>
#ifndef _BSD_SOURCE
#   define _BSD_SOURCE
#endif
#include <cstdlib>
#include <unistd.h>
#include "ignore_result.hpp"



namespace patxitron { namespace util {

struct temp_file_error: public ::std::system_error
{
    temp_file_error(int error_code, ::std::string const& what):
        ::std::system_error(error_code, ::std::generic_category(), what) {}
};



template<bool THROW=false>
class temp_file
{
    int fd_;
    bool autoremove_;
    ::std::string name_;

public:
    constexpr static bool EXCEPTIONS = THROW;

    explicit temp_file(::std::string const& pattern, bool autoremove=true)
            :fd_(-1)
            ,autoremove_(autoremove)
    {
        ::std::vector<char> temp_filename(pattern.begin(), pattern.end());
        if (pattern.size() < 6
                || pattern.compare(pattern.size() - 6, 6, "XXXXXX") != 0) {
            char tmpfilename[] = "XXXXXX";
            temp_filename.insert(
                 temp_filename.end()
                ,tmpfilename
                ,tmpfilename + ::std::strlen(tmpfilename) + 1
            );
        } else {
            temp_filename.insert(temp_filename.end(), 0);
        }
        fd_ = mkstemp(temp_filename.data());
        if (fd_ < 0) {
            if (THROW) {
                throw temp_file_error(errno, "Error creating temporary");
            }
        } else {
            name_.insert(name_.end(), temp_filename.begin(), temp_filename.end());
        }
    }

    explicit temp_file(bool autoremove)
            :fd_(-1)
            ,autoremove_(autoremove)
    {
        char tmpfilename[] = "/tmp/tempfileXXXXXX";
        ::std::vector<char> temp_filename(
             tmpfilename
            ,tmpfilename + ::std::strlen(tmpfilename) + 1
        );
        fd_ = mkstemp(temp_filename.data());
        if (fd_ < 0) {
            if (THROW) {
                throw temp_file_error(errno, "Error creating temporary");
            }
        } else {
            name_.insert(name_.end(), temp_filename.begin(), temp_filename.end());
        }
    }

    temp_file(temp_file const&) = delete; // Non-copyable
    temp_file(temp_file&& other)
            :fd_(other.fd_)
            ,autoremove_(other.autoremove_)
            ,name_(::std::move(other.name_))
    {
        other.fd_ = -1;
    }

    ~temp_file()
    {
        if (fd_ > -1) {
            ::close(fd_);
            if (autoremove_) {
                unlink(name_.c_str());
            }
        }
    }

    temp_file& operator=(temp_file const&) = delete; // Non-copyable
    temp_file& operator=(temp_file&& other)
    {
        ignore_result(close());
        fd_ = other.fd_;
        autoremove_ = other.autoremove_;
        name_ = ::std::move(other.name_);
        other.fd_ = -1;
        return *this;
    }

    operator int() const {return fd_;}
    operator ::std::string() const {return name_;}
    operator bool() const {return fd_ != -1;}

    ::std::string const& name() const {return name_;}
    int fd() const {return fd_;}
    bool is_open() const {return fd_ != -1;}

    bool close()
    {
        if (fd_ > -1) {
            if (::close(fd_) != 0) {
                if (THROW) {
                    throw temp_file_error(errno, "Error closing '" + name_ + "'");
                }
                return false;
            }
            fd_ = -1;
            if (autoremove_) {
                if (unlink(name_.c_str()) != 0) {
                    if (THROW) {
                        throw temp_file_error(errno, "Error removing '" + name_ + "'");
                    }
                    return false;
                }
            }
        }
        return true;
    }

    bool rename(::std::string const& filename)
    {
        if (fd_ > -1) {
            if (::close(fd_) != 0) {
                if (THROW) {
                    throw temp_file_error(errno, "Error closing '" + name_ + "'");
                }
                return false;
            }
            fd_ = -1;
            if (::std::rename(name_.c_str(), filename.c_str()) != 0) {
                int error_code = errno;
                if (autoremove_) {
                    unlink(name_.c_str());
                }
                if (THROW) {
                    throw temp_file_error(
                         error_code
                        ,"Error renaming '" + name_ + "' to '" + filename + "'"
                    );
                }
                errno = error_code;
                return false;
            }
            name_ = filename;
            return true;
        }
        return false;
    }

    bool remove()
    {
        if (fd_ > -1) {
            if (::close(fd_) != 0) {
                if (THROW) {
                    throw temp_file_error(errno, "Error closing '" + name_ + "'");
                }
                return false;
            }
            fd_ = -1;
        }
        return unlink(name_.c_str()) == 0;
    }
};

}} // namespace patxitron::util

#endif // HPP_67792d4e_a754_11e7_a9be_f78be9ea07d4
