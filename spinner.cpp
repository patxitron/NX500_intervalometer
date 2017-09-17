#include "spinner.hpp"
#include <FL/Fl_Pixmap.H>
#include <iostream>
#include <cstring>
#include "util.hpp"

using namespace std;

namespace {

static char const* const up_xpm[] = {
    "32 17 4 1",
    "  c black",
    "o c #ff0000",
    "@ c #ffffff",
    "# c None",
    "################################",
    "###############oo###############",
    "##############oooo##############",
    "#############oooooo#############",
    "############oooooooo############",
    "###########oooooooooo###########",
    "##########oooooooooooo##########",
    "#########oooooooooooooo#########",
    "########oooooooooooooooo########",
    "#######oooooooooooooooooo#######",
    "######oooooooooooooooooooo######",
    "#####oooooooooooooooooooooo#####",
    "####oooooooooooooooooooooooo####",
    "###oooooooooooooooooooooooooo###",
    "##oooooooooooooooooooooooooooo##",
    "#oooooooooooooooooooooooooooooo#",
    "################################"
};

static char const* const down_xpm[] = {
    "32 17 4 1",
    "  c black",
    "o c #ff0000",
    "@ c #ffffff",
    "# c None",
    "################################",
    "#oooooooooooooooooooooooooooooo#",
    "##oooooooooooooooooooooooooooo##",
    "###oooooooooooooooooooooooooo###",
    "####oooooooooooooooooooooooo####",
    "#####oooooooooooooooooooooo#####",
    "######oooooooooooooooooooo######",
    "#######oooooooooooooooooo#######",
    "########oooooooooooooooo########",
    "#########oooooooooooooo#########",
    "##########oooooooooooo##########",
    "###########oooooooooo###########",
    "############oooooooo############",
    "#############oooooo#############",
    "##############oooo##############",
    "###############oo###############",
    "################################"
};

static Fl_Pixmap  up_arrow(up_xpm);
static Fl_Pixmap  down_arrow(down_xpm);

} // anonymous

namespace patxitron {

Spinner::Spinner(int x, int y, size_t val)
        :Fl_Group(x, y, WIDTH, HEIGHT)
        ,button_up_(new Fl_Button(x + 2, y + 1, 32, 17))
        ,button_down_(new Fl_Button(x + 2, y + 82, 32, 17))
        ,value_(new Fl_Output(x + 2, y + 18, 32, 64))
{
    char v[] = {'0', 0};
    if (val > 9) val = 9;
    v[0] = val + '0';
    button_up_->callback(reinterpret_cast<Fl_Callback*>(&Spinner::upcbk), this);
    button_down_->callback(reinterpret_cast<Fl_Callback*>(&Spinner::downcbk), this);
    button_up_->image(up_arrow);
    button_down_->image(down_arrow);
    button_up_->color(FL_BLACK);
    button_down_->color(FL_BLACK);
    button_up_->box(FL_FLAT_BOX);
    button_down_->box(FL_FLAT_BOX);
    value_->box(FL_BORDER_BOX);
    value_->color(FL_BLACK);
    value_->labelcolor(FL_RED);
    value_->selection_color (FL_RED);
    value_->textsize(46);
    value_->textcolor(FL_RED);
    value_->value(v);
    value_->redraw();
    cout << "Spinner::Spinner() initial value: '" << value_->value() << "'" << endl;
    end();
}



void Spinner::value(size_t val)
{
    char v[] = {static_cast<char>(val <= 9 ? val + '0' : '0'), 0};
    value_->value(v);
    value_->redraw();
    cout << "Spinner::value(): '" << value_->value() << "'" << endl;
}



size_t Spinner::increment()
{
    char v[] = {static_cast<char>(value_->index(0)), 0};
    v[0] += 1;
    if (v[0] > '9') v[0] = '0';
    value_->value(v);
    value_->redraw();
    cout << "Spinner::increment(): '" << value_->value() << "'" << endl;
    return static_cast<size_t>(value_->value()[0] - '0');
}



size_t Spinner::decrement()
{
    char v[] = {static_cast<char>(value_->index(0)), 0};
    v[0] -= 1;
    if (v[0] < '0') v[0] = '9';
    value_->value(v);
    value_->redraw();
    cout << "Spinner::decrement(): '" << value_->value() << "'" << endl;
    return static_cast<size_t>(value_->value()[0] - '0');
}



void Spinner::upcbk(Fl_Widget*, Spinner* spnr)
{
    cout << "Spinner::upcbk()" << endl;
    util::ignore_result(spnr->increment());
    spnr->do_callback();
}



void Spinner::downcbk(Fl_Widget*, Spinner* spnr)
{
    cout << "Spinner::downcbk()" << endl;
    util::ignore_result(spnr->decrement());
    spnr->do_callback();
}

} // patxitron
