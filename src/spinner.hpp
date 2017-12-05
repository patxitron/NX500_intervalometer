/**
 * @file
 * 
 * 
 */
#ifndef HPP_2d521f88_9b86_11e7_bec9_cfc3241b7d5e
#define HPP_2d521f88_9b86_11e7_bec9_cfc3241b7d5e

#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include <memory>
#include <cstdlib>
#include <stdint.h>

namespace patxitron { namespace ui {

class Spinner: public Fl_Group
{
public:
    static unsigned int const WIDTH = 56;
    static unsigned int const HEIGHT = 122;

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

}} // patxitron::ui

#endif // HPP_2d521f88_9b86_11e7_bec9_cfc3241b7d5e
