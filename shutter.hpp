#ifndef HPP_288a28e0_9dd6_11e7_8aa5_082e5f016151
#define HPP_288a28e0_9dd6_11e7_8aa5_082e5f016151

#include <mutex>
#include <stdexcept>
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



class Shutter
{
public:
    typedef enum {
        IDLE,
        PRE_FOCUS,
        EXPOSING,
        POST_FOCUS,
        FINISHED
    } state_t;
    typedef void(callback_t)(Shutter*,void*);

    static double const PERIOD;
    static ::std::chrono::milliseconds const FOCUS_TIME;

    explicit Shutter(callback_t* callback=nullptr, void* data=nullptr);
    Shutter(Shutter const&) = delete;
    ~Shutter();

    Shutter& operator=(Shutter const&) = delete;

    void callback(callback_t* callback, void* data=nullptr);
    state_t state() const
    {
        ::std::lock_guard<::std::recursive_mutex> lock(mutex_);
        return state_;
    }
    ssize_t exposing() const
    {
        ::std::lock_guard<::std::recursive_mutex> lock(mutex_);
        if (state_ == IDLE) return -1;
        return ::std::chrono::duration_cast<::std::chrono::seconds>(
            ::std::chrono::steady_clock::now() - last_
        ).count();
    }
    void start(uint16_t deciseconds);
    void cancel();
    void target_focus_window();
    void target_window(Window const& w);
    Window target_window() const {return camera_app_window_;}

private:
    int32_t exposure_;
    ::std::chrono::time_point<::std::chrono::steady_clock> last_;
    callback_t* callback_;
    void* data_;
    state_t state_;
    xdo_t* xdo_;
    Window camera_app_window_;
    mutable ::std::recursive_mutex mutex_;

    void do_callback();
    void iterate();
    static void tmrcb(Shutter* shttr);
};

}} // patxitron::nx

#endif // HPP_288a28e0_9dd6_11e7_8aa5_082e5f016151
