#include "tickerer.hh"
#include "util.hh"


#include "tickerer_init.inc"


void
Tickerer::create_widget(Tickerer *& win,
			const Glib::RefPtr<Gtk::Builder> & builder)
{
    builder->get_widget_derived("tickerer", win);
}

Tickerer::Tickerer(GtkVBox * obj, const Glib::RefPtr<Gtk::Builder> & builder) : Gtk::VBox(obj)
{
    init_widgets(builder);
    init_signals();

	calib_vbox->pack_start(trap1_calibration,Gtk::PACK_SHRINK);
	show_all_children();
}


Calibration_tree_view::	Calibration_tree_view()
{
	columnss.add(col_element);
	columnss.add(col_A);
	columnss.add(col_q);
	columnss.add(col_mass_excess);
	columnss.add(col_calib_type);
	columnss.add(col_frequency);
	columnss.add(col_start_datum);
	columnss.add(col_end_datum);

	calib_tree = Gtk::ListStore::create(columnss);

	set_model(calib_tree);

	Gtk::TreeModel::Row row = *(calib_tree->append());
	row[col_element] = "daa";


	append_column("Element",col_element);
	append_column("A",col_A);
	append_column("q",col_q);
	append_column("Mass",col_mass_excess);
	append_column("Type",col_calib_type);
	append_column("Frequency",col_frequency);
	append_column("Valid from",col_start_datum);
	append_column("Valid to",col_end_datum);

}