#include "shutter.hpp"
#include <FL/Fl.H>
#include <ctime>
#include <cstdio>
#include <unistd.h>

using namespace std;

namespace {

} // Anonymous



namespace patxitron { namespace nx {

double const Shutter::PERIOD = .1;
chrono::milliseconds const Shutter::FOCUS_TIME(100ms);

Shutter::Shutter(int x, int y)
        :Fl_Group(x, y, WIDTH, HEIGHT)
        ,status_(IDLE)
        ,begin_delay_(0ms)
        ,interval_(0ms)
        ,exposure_time_(0ms)
        ,exposure_count_(0)
        ,exposure_done_(0)
        ,last_time_(chrono::steady_clock::now())
        ,xdo_(xdo_new(":0"))
        ,status_output_(new Fl_Output(x + 2, y + 2, WIDTH - 4, HEIGHT - 4))
        ,camera_app_window_(CURRENTWINDOW)
{
    if (!xdo_) throw nx_error("Can not allocate xdo_t structure.");
    if (0 != xdo_get_focused_window(xdo_, &camera_app_window_)) {
        throw nx_error("Can not get NX camera app window.");
    }
    status_output_->textsize(HEIGHT - 8);
    status_output_->box(FL_BORDER_BOX);
    status_output_->color(FL_BLACK);
    status_output_->labelcolor(FL_RED);
    status_output_->selection_color (FL_RED);
    status_output_->textcolor(FL_RED);
    status_output_->value("Idle.");
    Fl::add_timeout(PERIOD, reinterpret_cast<Fl_Timeout_Handler>(&Shutter::tmrcb), this);
    end();
}



Shutter::~Shutter()
{
    if (xdo_) {
        xdo_send_keysequence_window_up(xdo_, camera_app_window_, "Super_R", 0);
        usleep(10000);
        xdo_send_keysequence_window_up(xdo_, camera_app_window_, "Super_L", 0);
        xdo_free(xdo_);
    }
}



void Shutter::start(::std::chrono::seconds const& delay
                   ,::std::chrono::seconds const& interval
                   ,::std::chrono::seconds const& exposure
                   ,uint16_t count)
{
    if (status_ == IDLE) {
        exposure_count_ = count;
        exposure_done_ = 0;
        begin_delay_ = delay;
        exposure_time_ = exposure;
        interval_ = interval >= exposure + 2s ? interval : exposure + 2s;
        status_ = WAITING_TO_BEGIN;
    }
}



void Shutter::cancel()
{
    if (status_ != IDLE && status_ != FINISHED) {
        exposure_count_ = exposure_done_;
        if (status_ == EXPOSING) {
            if (0 != xdo_send_keysequence_window_up(xdo_, camera_app_window_, "Super_R", 0)) {
                throw nx_error("Can not half-release camera shutter.");
            }
            last_time_ = chrono::steady_clock::now();
            status_ = POST_EXPOSING;
        }
        status_output_->value("Cancelled.");
        status_output_->redraw();
    }
}



void Shutter::timer_callback()
{
    char buffer[64];
    buffer[63] = 0;
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - last_time_);
    switch(status_) {
    case WAITING_TO_BEGIN:
        if (elapsed > begin_delay_) {
            if (0 != xdo_send_keysequence_window_down(xdo_, camera_app_window_, "Super_L", 0)) {
                throw nx_error("Can not half-press camera shutter.");
            }
            last_time_ = chrono::steady_clock::now();
            status_ = PRE_EXPOSING;
        }
        snprintf(
            buffer
           ,63
           ,"Starting in %ds"
           ,chrono::duration_cast<chrono::seconds>(begin_delay_ - elapsed).count()
        );
        status_output_->value(buffer);
        status_output_->redraw();
        break;
    case WAITING_NEXT_SHOOT:
        if (elapsed > (interval_ - exposure_time_ - 2 * FOCUS_TIME)) {
            if (0 != xdo_send_keysequence_window_down(xdo_, camera_app_window_, "Super_L", 0)) {
                throw nx_error("Can not half-press camera shutter.");
            }
            last_time_ = chrono::steady_clock::now();
            status_ = PRE_EXPOSING;
        }
        snprintf(
           buffer
           ,63
           ,"Next exposure in in %ds, %hu exposures left"
           ,chrono::duration_cast<chrono::seconds>((interval_ - exposure_time_ - 2 * FOCUS_TIME) - elapsed).count()
           ,exposure_count_ - exposure_done_
        );
        status_output_->value(buffer);
        status_output_->redraw();
        break;
    case PRE_EXPOSING:
        if (elapsed > FOCUS_TIME) {
            if (0 != xdo_send_keysequence_window_down(xdo_, camera_app_window_, "Super_R", 0)) {
                throw nx_error("Can not full-press camera shutter.");
            }
            last_time_ = chrono::steady_clock::now();
            status_ = EXPOSING;
        }
        break;
    case EXPOSING:
        if (elapsed > exposure_time_) {
            if (0 != xdo_send_keysequence_window_up(xdo_, camera_app_window_, "Super_R", 0)) {
                throw nx_error("Can not half-release camera shutter.");
            }
            last_time_ = chrono::steady_clock::now();
            status_ = POST_EXPOSING;
        }
        snprintf(
           buffer
           ,63
           ,"Exposing %ds, %d to go, %hu exposures left"
           ,chrono::duration_cast<chrono::seconds>(elapsed).count()
           ,chrono::duration_cast<chrono::seconds>(exposure_time_ - elapsed).count()
           ,exposure_count_ - exposure_done_
        );
        status_output_->value(buffer);
        status_output_->redraw();
        break;
    case POST_EXPOSING:
        if (elapsed > FOCUS_TIME) {
            if (0 != xdo_send_keysequence_window_up(xdo_, camera_app_window_, "Super_L", 0)) {
                throw nx_error("Can not full-release camera shutter.");
            }
            last_time_ = chrono::steady_clock::now();
            if (exposure_count_ > exposure_done_) {
                exposure_done_ += 1;
                status_ = WAITING_NEXT_SHOOT;
            } else {
                status_ = FINISHED;
            }
        }
        break;
    case FINISHED:
        status_ = IDLE;
        status_output_->value("Finished.");
        status_output_->redraw();
        do_callback();
    default:
        break;
    }
}



void Shutter::tmrcb(Shutter* shttr)
{
    shttr->timer_callback();
    Fl::repeat_timeout(PERIOD, reinterpret_cast<Fl_Timeout_Handler>(&Shutter::tmrcb), shttr);
}

}} // patxitron::nx
