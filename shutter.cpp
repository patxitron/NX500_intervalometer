#include "shutter.hpp"
#include <FL/Fl.H>
#include <ctime>
#include <iostream>

using namespace std;

namespace patxitron { namespace nx {

double const Shutter::PERIOD = .01;
chrono::milliseconds const Shutter::FOCUS_TIME(100ms);



Shutter::Shutter(callback_t* callback, void* data)
        :exposure_(0)
        ,last_(chrono::steady_clock::now())
        ,callback_(callback)
        ,data_(data)
        ,state_(IDLE)
        ,xdo_(xdo_new(":0"))
        ,camera_app_window_(CURRENTWINDOW)
{
    if (!xdo_) throw nx_error("Can not allocate xdo_t structure.");
    Fl::add_timeout(PERIOD, reinterpret_cast<Fl_Timeout_Handler>(&Shutter::tmrcb), this);
    target_focus_window();
}



Shutter::~Shutter()
{
    if (xdo_) {
        if (state_ != IDLE || state_ != FINISHED) {
            if (state_ == EXPOSING) {
                xdo_send_keysequence_window_up(xdo_, camera_app_window_, "Super_R", 0);
                usleep(10000);
            }
            xdo_send_keysequence_window_up(xdo_, camera_app_window_, "Super_L", 0);
        }
        xdo_free(xdo_);
    }
}



void Shutter::callback(callback_t* callback, void* data)
{
    callback_ = callback;
    data_ = data;
}



void Shutter::start(uint16_t deciseconds)
{
    cout << "Shutter::start(" << deciseconds << ")" << endl;
    lock_guard<recursive_mutex> lock(mutex_);
    if (state_ == IDLE) {
        exposure_ = deciseconds * 100;
        state_ = PRE_FOCUS;
        if (0 != xdo_send_keysequence_window_down(xdo_, camera_app_window_, "Super_L", 0)) {
            throw nx_error("Can not half-press camera shutter.");
        }
        last_ = chrono::steady_clock::now();
    }
}



void Shutter::cancel()
{
    lock_guard<recursive_mutex> lock(mutex_);
    if (state_ != IDLE) {
        if (state_ == EXPOSING) {
            if (0 != xdo_send_keysequence_window_up(xdo_, camera_app_window_, "Super_R", 0)) {
                throw nx_error("Can not half-release camera shutter.");
            }
            state_ = POST_FOCUS;
            last_ = chrono::steady_clock::now();
        } else {
            if (0 != xdo_send_keysequence_window_up(xdo_, camera_app_window_, "Super_L", 0)) {
                throw nx_error("Can not full-release camera shutter.");
            }
            state_ = FINISHED;
        }
    } else {
        do_callback();
    }
}



void Shutter::target_focus_window()
{
    if (0 != xdo_get_focused_window(xdo_, &camera_app_window_)) {
        throw nx_error("Can not get NX camera app window.");
    }
}



void Shutter::target_window(Window const& w)
{
    camera_app_window_ = w;
}



void Shutter::do_callback()
{
    if (callback_) (*callback_)(this, data_);
}



void Shutter::iterate()
{
    lock_guard<recursive_mutex> lock(mutex_);
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(
        chrono::steady_clock::now() - last_
    );
    switch(state_) {
    case PRE_FOCUS:
        if (elapsed > FOCUS_TIME) {
            if (0 != xdo_send_keysequence_window_down(xdo_, camera_app_window_, "Super_R", 0)) {
                throw nx_error("Can not full-press camera shutter.");
            }
            state_ = EXPOSING;
            last_ = chrono::steady_clock::now();
        }
        break;
    case EXPOSING:
        if (elapsed.count() > exposure_) {
            if (0 != xdo_send_keysequence_window_up(xdo_, camera_app_window_, "Super_R", 0)) {
                throw nx_error("Can not half-release camera shutter.");
            }
            state_ = POST_FOCUS;
            last_ = chrono::steady_clock::now();
        }
        break;
    case POST_FOCUS:
        if (elapsed > FOCUS_TIME) {
            if (0 != xdo_send_keysequence_window_up(xdo_, camera_app_window_, "Super_L", 0)) {
                throw nx_error("Can not full-release camera shutter.");
            }
            state_ = FINISHED;
        }
        break;
    case FINISHED:
        do_callback();
        state_ = IDLE;
        break;
    default:
        break;
    }
}



void Shutter::tmrcb(Shutter* shttr)
{
    shttr->iterate();
    Fl::repeat_timeout(PERIOD, reinterpret_cast<Fl_Timeout_Handler>(&Shutter::tmrcb), shttr);
}

}} // patxitron::nx
