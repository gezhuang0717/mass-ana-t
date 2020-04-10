#ifndef COMPAT_H
#define COMPAT_H

#include <gtkmm/filechooserdialog.h>

#if GTKMM_MAJOR_VERSION < 3
namespace Gdk {
    typedef Color RGBA;
};

inline
void
st_normal_bg_color(
    Gtk::Widget & widget,
    Gdk::Color & color) {
    widget.modify_base(Gtk::STATE_NORMAL, color);
}

inline
Gdk::Color
get_st_normal_bg_color(
    Gtk::Widget & widget) {
    return widget.get_style()->get_base(Gtk::STATE_NORMAL);
}

inline
Glib::RefPtr<Gtk::FileFilter>
get_file_filter(
    const char * name,
    const char * pattern)
{
    Glib::RefPtr<Gtk::FileFilter> filter(new Gtk::FileFilter());
    filter->set_name("*.settings");
    filter->add_pattern("*.settings");
    return filter;
}

inline
void
file_chooser_add_filter(
    Gtk::FileChooserDialog & dialog,
    Glib::RefPtr<Gtk::FileFilter> & filter)
{
    dialog.add_filter(*filter.operator->());
}

#else

inline
void
st_normal_bg_color(
    Gtk::Widget & widget,
    Gdk::RGBA & color) {
    widget.override_background_color(color, Gtk::STATE_FLAG_NORMAL);
}

inline
void
st_normal_bg_color(
    Gtk::Widget & widget,
    Gdk::Color & color) {
    Gdk::RGBA ncolor;
    ncolor.set_rgba(color.get_red_p(), color.get_green_p(), color.get_blue_p());
    widget.override_background_color(ncolor, Gtk::STATE_FLAG_NORMAL);
}

inline
Gdk::RGBA
get_st_normal_bg_color(
    Gtk::Widget & widget) {
    return widget.get_style_context()->get_background_color(Gtk::STATE_FLAG_NORMAL);
}

inline
Glib::RefPtr<Gtk::FileFilter>
get_file_filter(
    const char * name,
    const char * pattern)
{
    Glib::RefPtr<Gtk::FileFilter> filter = Gtk::FileFilter::create();
    filter->set_name("*.settings");
    filter->add_pattern("*.settings");
    return filter;
}

inline
void
file_chooser_add_filter(
    Gtk::FileChooserDialog & dialog,
    Glib::RefPtr<Gtk::FileFilter> & filter)
{
    dialog.add_filter(filter);
}
#endif

#endif
// Local Variables:
// mode: c++
// End:
