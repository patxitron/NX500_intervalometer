/**
 * @file
 * 
 * 
 */
#ifndef UINTSPINNER_HPP
#define UINTSPINNER_HPP

#include <FL/Fl_Group.H>
#include <vector>
#include <memory>
#include <stdint.h>
#include "spinner.hpp"

namespace patxitron {

class UintSpinner: public Fl_Group
{
public:
    UintSpinner(int x, int y, uint16_t minval, uint16_t maxval
               ,uint16_t val=0);

    uint16_t value() const {return value_;}
    uint16_t value(uint16_t val);
    uint16_t increment();
    uint16_t decrement();

private:
    uint16_t value_;
    uint16_t const max_;
    uint16_t const min_;
    uint16_t const order_;
    char fmt_[6];
    ::std::vector<::std::unique_ptr<Spinner>> spinners_;

    void spinnercb();
    static void spinner_cb(Fl_Widget* cb, void* p);
};

} // patxitron

#endif // UINTSPINNER_HPP
