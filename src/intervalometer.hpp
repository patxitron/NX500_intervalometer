/**
 * @file
 * 
 * 
 */
#ifndef HPP_9fcd8480_9b86_11e7_9598_379b686877e8
#define HPP_9fcd8480_9b86_11e7_9598_379b686877e8

#include <memory>
#include <mutex>
#include <chrono>
#include <stdexcept>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Output.H>
#include "uintspinner.hpp"
#include "shutter.hpp"

namespace patxitron {

struct Intervalometer_error: public ::std::runtime_error
{
    Intervalometer_error(::std::string const& what): ::std::runtime_error(what) {}
};



class Intervalometer: public Fl_Group
{
public:
    typedef enum {
        IDLE,
        WAITING_START,
        EXPOSING,
        WAITING_INTERVAL,
        CANCELLING,
        FINISHED
    } state_t;

    static double const PERIOD;
    static unsigned int const WIDTH = 720;
    static unsigned int const HEIGHT = 390;

    Intervalometer();

    void cancel();

private:
    ::std::unique_ptr<ui::UintSpinner> exposure_;
    ::std::unique_ptr<ui::UintSpinner> count_;
    ::std::unique_ptr<ui::UintSpinner> delay_;
    ::std::unique_ptr<ui::UintSpinner> interval_;
    ::std::unique_ptr<Fl_Button> start_;
    ::std::unique_ptr<Fl_Output> status_;
    ::std::chrono::time_point<::std::chrono::steady_clock> last_;
    uint16_t counter_;
    state_t state_;
    nx::Shutter shutter_;
    mutable ::std::recursive_mutex mutex_;
   

    void iterate();
    void update_status();
    static void exposure_cb(Fl_Widget*, Intervalometer* self);
    static void start_cb(Fl_Widget*, Intervalometer* self);
    static void tmrcb(Intervalometer* self);
    static void shutter_cb(nx::Shutter*, Intervalometer* self);
};

} // patxitron

#endif // HPP_9fcd8480_9b86_11e7_9598_379b686877e8
