#include "intervalometer-ui.hpp"
#include <FL/Fl.H>
#include <iostream>
#include <cstdio>

using namespace std;

namespace patxitron {

double const IntervalometerUi::PERIOD = .25;



IntervalometerUi::IntervalometerUi()
        :Fl_Group(0, 0, WIDTH, HEIGHT)
        ,exposure_(new ui::UintSpinner(10, 10, 10, 4800, "Exposure (1/10s)", 100))
        ,count_(new ui::UintSpinner(WIDTH / 2 + 10, 10, 1, 9999, "Count", 1))
        ,delay_(new ui::UintSpinner(10, ui::UintSpinner::HEIGHT + 22, 1, 999, "Delay (s)", 2))
        ,interval_(new ui::UintSpinner(WIDTH / 2 + 10, ui::UintSpinner::HEIGHT + 22, 1, 999, "Interval (s)", 1))
        ,start_(new Fl_Button(10, delay_->y() + delay_->h() + 10, WIDTH - 20, HEIGHT - delay_->y() - delay_->h() - 10, "START"))
        ,status_(new Fl_Output(10, start_->y(), start_->w(), start_->h() / 2))
        ,last_(chrono::steady_clock::now())
        ,counter_(0)
        ,state_(IDLE)
        ,shutter_(reinterpret_cast<nx::Shutter::callback_t*>(&IntervalometerUi::shutter_cb), this)
{
    if (!exposure_ || !count_ || !delay_ || !interval_) {
        throw IntervalometerUi_error("Error allocating spinners");
    }
    start_->callback(reinterpret_cast<Fl_Callback*>(&IntervalometerUi::start_cb), this);
    exposure_->callback(reinterpret_cast<Fl_Callback*>(&IntervalometerUi::exposure_cb), this);
    start_->labelsize(start_->h() - 2);
    start_->box(FL_BORDER_FRAME);
    start_->color(FL_DARK_RED);
    start_->labelcolor(FL_DARK_RED);
    interval_->min(exposure_->value() / 10 + (exposure_->value() % 10 ? 2 : 1));
    status_->box(FL_BORDER_BOX);
    status_->color(FL_BLACK);
    status_->labelcolor(FL_DARK_RED);
    status_->selection_color (FL_DARK_RED);
    status_->textsize(status_->h() - 2);
    status_->textcolor(FL_DARK_RED);
    status_->value("Status line");
    status_->hide();
    end();
    Fl::add_timeout(PERIOD, reinterpret_cast<Fl_Timeout_Handler>(&IntervalometerUi::tmrcb), this);
}



void IntervalometerUi::cancel()
{
    lock_guard<recursive_mutex> lock(mutex_);
    if (state_ != IDLE) {
        if (state_ != CANCELLING && state_ != FINISHED) {
            state_ = CANCELLING;
            shutter_.cancel();
        }
    } else {
        do_callback();
    }
}



void IntervalometerUi::iterate()
{
    lock_guard<recursive_mutex> lock(mutex_);
    auto elapsed = chrono::duration_cast<chrono::milliseconds>(
        chrono::steady_clock::now() - last_
    );
    switch(state_) {
    case WAITING_START:
        if (elapsed.count() > delay_->value() * 1000) {
            shutter_.start(exposure_->value());
            state_ = EXPOSING;
            last_ = chrono::steady_clock::now();
        }
        update_status();
        break;
    case EXPOSING:
        update_status();
        break;
    case WAITING_INTERVAL:
        if (elapsed.count() > interval_->value() * 1000) {
            shutter_.start(exposure_->value());
            state_ = EXPOSING;
            last_ = chrono::steady_clock::now();
        }
        update_status();
        break;
    case FINISHED:
        cout << "IntervalometerUi FINISHED" << endl;
        do_callback();
        break;
    default:
        break;
    }
}



void IntervalometerUi::update_status()
{
    char buffer[64];
    buffer[63] = 0;
    if (state_ == EXPOSING) {
        ssize_t exp = shutter_.exposing();
        snprintf(
             buffer
            ,63
            ,"Exposing %lds / %hus, %hu of %hu done"
            ,exp
            ,exposure_->value() / 10
            ,counter_
            ,count_->value()
        );
    } else {
        ssize_t wt = chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - last_).count();
        if (state_ == WAITING_START) {
            snprintf(
                 buffer
                ,63
                ,"Waiting %lds / %hus, %hu of %hu done"
                ,wt
                ,delay_->value()
                ,counter_
                ,count_->value()
            );
        } else {
            snprintf(
                 buffer
                ,63
                ,"Waiting %lds / %hus, %hu of %hu done"
                ,wt
                ,interval_->value()
                ,counter_
                ,count_->value()
            );
        }
    }
    status_->value(buffer);
    status_->redraw();
}



void IntervalometerUi::exposure_cb(Fl_Widget*, IntervalometerUi* self)
{
    lock_guard<recursive_mutex> lock(self->mutex_);
    self->interval_->min(self->exposure_->value() / 10 + (self->exposure_->value() % 10 ? 2 : 1));
}



void IntervalometerUi::start_cb(Fl_Widget*, IntervalometerUi* self)
{
    cout << "IntervalometerUi STARTING" << endl;
    lock_guard<recursive_mutex> lock(self->mutex_);
    if (self->state_ == IDLE) {
        self->start_->hide();
        self->exposure_->hide();
        self->count_->hide();
        self->delay_->hide();
        self->interval_->hide();
        self->status_->show();
        self->counter_ = 0;
        self->state_ = WAITING_START;
        self->last_ = chrono::steady_clock::now();
    }
}



void IntervalometerUi::tmrcb(IntervalometerUi* self)
{
    self->iterate();
    Fl::repeat_timeout(PERIOD, reinterpret_cast<Fl_Timeout_Handler>(&IntervalometerUi::tmrcb), self);
}



void IntervalometerUi::shutter_cb(nx::Shutter*, IntervalometerUi* self)
{
    lock_guard<recursive_mutex> lock(self->mutex_);
    self->counter_ += 1;
    if (self->state_ == CANCELLING || self->counter_ >= self->count_->value()) {
        self->state_ = FINISHED;
    } else if (self->state_ == EXPOSING) {
        self->state_ = WAITING_INTERVAL;
    }
}

} // patxitron
