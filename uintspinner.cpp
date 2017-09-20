#include "uintspinner.hpp"
#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstring>
#include "patxitron_util.hpp"

using namespace std;

namespace {

static uint16_t spinners_value(::std::vector<::std::unique_ptr<patxitron::ui::Spinner>> const& spinners)
{
    uint16_t result = 0;
    uint16_t factor = 1;
    for (auto const& v: spinners) {
        result += v->value() * factor;
        factor *= 10;
    }
    return result;
}

} // anonymous

namespace patxitron { namespace ui {

UintSpinner::UintSpinner(int x, int y, uint16_t minval, uint16_t maxval
                        ,char const* label, uint16_t val)
        :Fl_Group(x, y, Spinner::WIDTH * static_cast<uint16_t>(log10(maxval) + 1), Spinner::HEIGHT + 28)
        ,value_(val < minval ? minval : (val > maxval ? maxval : val))
        ,max_(maxval)
        ,min_(minval)
        ,order_(static_cast<uint16_t>(log10(max_)) + 1)
        ,label_(new Fl_Output(x, y + 2, Spinner::WIDTH * static_cast<uint16_t>(log10(max_) + 1), 26))
{
    label_->box(FL_BORDER_BOX);
    label_->color(FL_BLACK);
    label_->labelcolor(FL_DARK_RED);
    label_->selection_color (FL_DARK_RED);
    label_->textsize(24);
    label_->textcolor(FL_DARK_RED);
    label_->value(label);
    strncpy(fmt_, "%00hu", 6);
    fmt_[2] = static_cast<char>(order_) + '0';
    char buffer[6];
    snprintf(buffer, 6, fmt_, val);
    for (ssize_t i = order_ - 1; i >= 0; i--) {
        Spinner* spinner = new Spinner(x + i * Spinner::WIDTH, y + 28, buffer[i] - '0');
        spinners_.emplace_back(spinner);
        spinners_.back()->callback(&UintSpinner::spinner_cb, this);
    }
    end();
}



uint16_t UintSpinner::value(uint16_t val)
{
    if (val > max_) val = max_;
    if (val < min_) val = min_;
    value_ = val;
    char buffer[6];
    snprintf(buffer, 6, fmt_, value_);
    for (size_t i = 0; i < order_; i++) {
        spinners_[i]->value(buffer[order_ - i - 1] - '0');
    }
    do_callback();
    return value_;
}



void UintSpinner::min(uint16_t val)
{
    min_ = val;
    if (value_ < min_) util::ignore_result(value(min_));
}



uint16_t UintSpinner::increment()
{
    if (value_ < max_) return value(value_ + 1);
    else return value_;
}



uint16_t UintSpinner::decrement()
{
    if (value_ > min_) return value(value_ - 1);
    else return value_;
}



void UintSpinner::spinnercb()
{
    util::ignore_result(value(spinners_value(spinners_)));
}



void UintSpinner::spinner_cb(Fl_Widget* cb, void* p)
{
    reinterpret_cast<UintSpinner*>(p)->spinnercb();
}

}} // patxitron::ui
