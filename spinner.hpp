/**
 * @file
 * 
 * 
 */
#ifndef SPINNER_HPP
#define SPINNER_HPP

#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <memory>
#include <cstdlib>
#include <stdint.h>

namespace patxitron {

class Spinner: public Fl_Group
{
public:
    static unsigned int const WIDTH = 36;
    static unsigned int const HEIGHT = 100;

    Spinner(int x, int y, size_t val=0);

    size_t value() const {return static_cast<size_t>(value_->index(0) - '0');}
    void value(size_t val);
    size_t increment();
    size_t decrement();

private:
    ::std::unique_ptr<Fl_Button> button_up_;
    ::std::unique_ptr<Fl_Button> button_down_;
    ::std::unique_ptr<Fl_Output> value_;

    static void upcbk(Fl_Widget*, Spinner* spnr);
    static void downcbk(Fl_Widget*, Spinner* spnr);
};

} // patxitron

#endif // SPINNER_HPP
