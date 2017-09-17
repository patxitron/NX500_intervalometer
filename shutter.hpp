#ifndef HPP_9fcd8480_9b86_11e7_9598_379b686877e8
#define HPP_9fcd8480_9b86_11e7_9598_379b686877e8

#include <FL/Fl_Group.H>
#include <FL/Fl_Output.H>
#include <stdexcept>
#include <memory>
#include <chrono>
#include <cstdlib>
#include <stdint.h>
extern "C" {
#include <xdo.h>
}

namespace patxitron { namespace nx {

struct nx_error: public ::std::runtime_error
{
    nx_error(::std::string const& what): ::std::runtime_error(what) {}
};



class Shutter: public Fl_Group
{
public:
    typedef enum {
        IDLE,
        WAITING_TO_BEGIN,
        WAITING_NEXT_SHOOT,
        PRE_EXPOSING,
        EXPOSING,
        POST_EXPOSING,
        FINISHED
    } status_t;

    static int const WIDTH = 700;
    static int const HEIGHT = 40;
    static double const PERIOD;
    static ::std::chrono::milliseconds const FOCUS_TIME;

    Shutter(int x, int y);
    ~Shutter();

    void start(::std::chrono::seconds const& delay
              ,::std::chrono::seconds const& interval
              ,::std::chrono::seconds const& exposure
              ,uint16_t count);

    void cancel();

private:
    status_t status_;
    ::std::chrono::milliseconds begin_delay_;
    ::std::chrono::milliseconds interval_;
    ::std::chrono::milliseconds exposure_time_;
    uint16_t exposure_count_;
    uint16_t exposure_done_;
    ::std::chrono::time_point<::std::chrono::steady_clock> last_time_;
    xdo_t* xdo_;
    ::std::unique_ptr<Fl_Output> status_output_;
    Window camera_app_window_;

    void timer_callback();
    static void tmrcb(Shutter* shttr);
};

}} // patxitron::nx

#endif // HPP_9fcd8480_9b86_11e7_9598_379b686877e8
