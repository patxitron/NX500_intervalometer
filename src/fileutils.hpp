/**
 * @file
 * 
 * 
 *
 * @date 2017-09-28
 * @author Patxi Lazur
 */

#ifndef HPP_4bb4a5b2_a767_11e7_9866_3bb99db957bd
#define HPP_4bb4a5b2_a767_11e7_9866_3bb99db957bd
#include <system_error>
#include <type_traits>
#include <iterator>
#include <iostream>
#include <cerrno>
#include <unistd.h>

namespace patxitron { namespace util {

template<typename FD_TYPE>
inline bool write_all(FD_TYPE const& fd, void const* buf, size_t count) {
    if (!fd) return false;
    size_t written_so_far = 0;
    while (count > written_so_far) {
        ssize_t bytes_written = write(
             fd
            ,reinterpret_cast<char const*>(buf) + written_so_far
            ,count - written_so_far
        );
        if (bytes_written <= 0) {
            if (errno != EINTR) {
                if (FD_TYPE::EXCEPTIONS) {
                    throw ::std::system_error(
                         errno
                        ,::std::generic_category()
                        ,"Error writting to file " + fd.name()
                    );
                }
                return false;
            }
        } else {
            written_so_far += bytes_written;
        }
    }
    return true;
}



template<>
inline bool write_all<int>(int const& fd, void const* buf, size_t count)
{
    if (fd == -1) return false;
    size_t written_so_far = 0;
    while (count > written_so_far) {
        ssize_t bytes_written = write(
             fd
            ,reinterpret_cast<char const*>(buf) + written_so_far
            ,count - written_so_far
        );
        if (bytes_written <= 0) {
            if (errno != EINTR) return false;
        } else {
            written_so_far += bytes_written;
        }
    }
    return true;
}



template<typename FD_TYPE, typename CharT, typename Traits, typename Allocator>
inline bool write(FD_TYPE const& fd
          ,::std::basic_string<CharT, Traits, Allocator> const& str)
{
    if (str.empty()) return true;
    return write_all(
         fd
        ,str.data()
        ,str.size() * sizeof(CharT)
    );
}



template<typename FD_TYPE, typename ITERATOR, typename = ::std::enable_if_t<::std::is_same<::std::random_access_iterator_tag, typename ::std::iterator_traits<ITERATOR>::iterator_category>::value>>
inline bool write(FD_TYPE const& fd, ITERATOR begin, ITERATOR end)
{
    ::std::cerr << "Writting random access iterator" << ::std::endl;
    auto distance = end - begin;
    if (distance <= 0) return true;
    return write_all(
         fd
        ,&(*begin)
        ,distance * sizeof(typename ::std::iterator_traits<ITERATOR>::value_type)
    );
}



template<typename FD_TYPE, typename ITERATOR, typename ::std::enable_if<::std::is_same<::std::forward_iterator_tag, typename ::std::iterator_traits<ITERATOR>::iterator_category>::value, int>::type = 0>
inline bool write(FD_TYPE const& fd, ITERATOR begin, ITERATOR end)
{
    ::std::cerr << "Writting forward iterator" << ::std::endl;
    while (begin != end) {
        if (!write_all(fd, &(*begin), 
                sizeof(typename ::std::iterator_traits<ITERATOR>::value_type))) {
            return false;
        }
        begin++;
    }
}

}} // namespace patxitron::util

#endif // HPP_4bb4a5b2_a767_11e7_9866_3bb99db957bd
