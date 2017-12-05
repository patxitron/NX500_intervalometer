/**
 * @file
 * 
 * 
 *
 * @date 2017-09-28
 * @author Patxi Lazur
 */

#ifndef HPP_2f8657d0_a453_11e7_9b1b_082e5f016151
#define HPP_2f8657d0_a453_11e7_9b1b_082e5f016151

namespace patxitron { namespace util {

/**
 * This function does nothing and generates no code.
 * 
 * This function can be used to avoid compiler warnings about ignored return
 * values.
 */
template<typename T>
inline void ignore_result(T /* unused result */) {}

}} // namespace patxitron::util

#endif // HPP_2f8657d0_a453_11e7_9b1b_082e5f016151
