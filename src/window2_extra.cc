#include "window2.hh"

using namespace std;
//-----------------------------------------------------------------------------
bool Plottiarea::on_drawingarea1_motion_notify_event(GdkEventMotion* event) {
	double plot_x,plot_y;int plott_x,plott_y;
	get_reverse_coordinates(event->x,event->y,plot_x,plot_y);
//	std::cout << "plot x " << plot_x << ", y " << plot_y << "\n";
//	get_reverse_coordinates(event->x,event->y,plott_x,plott_y);
//	std::cout << "inttinä plot x " << plott_x << ", y " << plott_y << "\n";


	//TÄÄÄ
	//Gtk::Window::set_title(" x: "+as_2f(plot_x) + " y: " + as_2f(plot_y) + " " + window_title);
	if (is_frame_labeli_set) {
		frame_labeli->set_label(" x: "+as_2f(plot_x) + " y: " + as_2f(plot_y) + " " + plot_title);
	}
	if (is_window_labeli_set) {
		window_labeli->set_title(" x: "+as_2f(plot_x) + " y: " + as_2f(plot_y) + " " + plot_title);
	}

/*	init_pixmap();drawingarea1_paint();
	cr->save();
	cr->move_to(event->x+20,event->y);
	cr->set_source_rgb(0.0,0,0);
	cr->show_text("x: "+as_2f(plot_x));
	cr->move_to(event->x+20,event->y+15);
	cr->show_text("y: "+as_2f(plot_y));
	cr->stroke();
	cr->restore();
*/

	if (mouse_button_is_depressed) {
		init_surface(); drawingarea1_paint();
		xbars.replace_value_in_bar(plot_x,"draggi");
		drag_end_x = plot_x;
		drag_end_y = plot_y;
		queue_draw();
	}

	//drawingarea1->queue_draw();
	return true;

}
//-----------------------------------------------------------------------------
bool Plottiarea::on_drawingarea1_leave_notify_event(GdkEventCrossing* event) {
	if (is_window_labeli_set) {
		window_labeli->set_title(default_window_tittel);
	}
	if (is_frame_labeli_set) {
		frame_labeli->set_label(plot_title);
	}
	return true;
}
//-----------------------------------------------------------------------------
bool Plottiarea::get_reverse_coordinates(const gdouble screen_x, const gdouble screen_y, double & data_x, double & data_y) {
	data_x = x_min + (screen_x -margin_l)*(x_max-x_min)/(drawingsize_x*1.0);
	data_y = y_min - (y_max-y_min)*(1./drawingsize_y*(screen_y-margin_u)-1.);
	return true;
}
//-----------------------------------------------------------------------------
bool Plottiarea::get_reverse_coordinates(const gdouble screen_x, const gdouble screen_y, int & data_x, int & data_y) {
	double daatta_x,daatta_y;
	get_reverse_coordinates(screen_x,screen_y,daatta_x,daatta_y);
	data_x = static_cast<int>(daatta_x+0.5);
	data_y = static_cast<int>(daatta_y+0.5);
	return true;
}
//-----------------------------------------------------------------------------
void Plottiarea::testia() {
	cr->save();
	cr->move_to(100,100);
	cr->set_source_rgb(0.0,0,0);
	//cr->set_font_size(12);
	cr->show_text("daaa");
	cr->stroke();
	cr->restore();
	queue_draw();
}
//-----------------------------------------------------------------------------
bool Plottiarea::on_drawingarea1_button_press_event(GdkEventButton *ev)
{
	if( (ev->type == GDK_BUTTON_PRESS) && (ev->button == 3) ) //Eli oikeaa nappia painallettiin
		return right_mousebutton_pressed(ev);
	if( (ev->type == GDK_BUTTON_PRESS) && (ev->button == 1) )
		return left_mousebutton_pressed(ev);
	return 1;
}
//------------------------------------------------------------------------------
bool Plottiarea::right_mousebutton_pressed(GdkEventButton *ev) {
//	cout << "täällä\n";
	save_fitB->hide();
	save_datB->hide();
//	Gtk::Menu Popup_menu;
	if ( (fit.data.size() > 0) && (showFit == true) ) {
		save_fitB->show();
	}
	if ( (dat.data.size() > 0) && (showData == true) ) {
		save_datB->show();
	}
	
	// Add the saving option for new container-type data
	
	save_datB->show();
	
	
	
	//std::cout << " Been here done that area1 \n";
	right_click_menu->popup(ev->button, ev->time);
	return 1;
}
//-----------------------------------------------------------------------------
bool Plottiarea::left_mousebutton_pressed(GdkEventButton *ev) {
	// This is emitted only when you hear "click" from the mouse.
	// When the mouse is moved while pressed (mouse_button_is_depressed = true)
	// then on_drawingarea1_motion_notify_event() is called
//	double plot_x,plot_y;
	get_reverse_coordinates(ev->x,ev->y,drag_start_x,drag_start_y);
	BAR drag_start_xbar;
	drag_start_xbar.value=drag_start_x;
	//jaa.label="alku";
	drag_start_xbar.id="alku";
	drag_start_xbar.color= Gdk::Color("Blue");
	show_xbars(true);
	xbars.delete_bar("alku");
	xbars.add_bar(drag_start_xbar);
	
	// Make also a drag end bar. This is the creation of it; changes happen in motion_notify_event()
	drag_start_xbar.color = Gdk::Color("Red");	drag_start_xbar.id="draggi"; xbars.add_bar(drag_start_xbar);
	drag_end_x = drag_start_x;
	drag_end_y = drag_start_y;
	
	show_contents();
	// cout << "left pressed\n";
	mouse_button_is_depressed = true;
	return 1;
}
//-----------------------------------------------------------------------------
bool Plottiarea::on_drawingarea1_button_release_event(GdkEventButton *ev) {
	// double yksi = xbars.get_value_from_bar("alku");
	// double kaksi = xbars.get_value_from_bar("draggi");
	
	// At this point the values in drag_start_x, drag_start_y
	// and drag_end_x, drag_end_y should be correct
	
	// in case we have just a "click" no drag:
	if ( abs(drag_start_x - drag_end_x) < 1e-10) {
		mouse_button_is_depressed = false;
		create_point_menu();
		show_point_selection_menu();
		show_contents();
		xbars.delete_bar("alku");xbars.delete_bar("draggi");
		return 1;
	}
	create_ranges_menu();
	show_range_selection_menu();

	show_contents();
	//cout << "button released\n";
	mouse_button_is_depressed = false;
	xbars.delete_bar("alku");xbars.delete_bar("draggi");
	return 1;
}
//-----------------------------------------------------------------------------
void Plottiarea::create_ranges_menu(void) {
    left_click_menu = new Gtk::Menu();
    if (!(ranges.empty())) {
	for (vector<Glib::ustring>::iterator ii = ranges.begin(); ii != ranges.end(); ++ii) {
	    Gtk::MenuItem * item = new Gtk::MenuItem("Set as " + *ii);
	    left_click_menu->append(*item);
	    item->signal_activate().connect(sigc::bind<Glib::ustring>(sigc::mem_fun(*this, &Plottiarea::on_range_selection_finished), *ii));
	    // ii->left_value = &range_l;
	    // ii->right_value = &range_r;
	}
    }
    left_click_menu->show_all();
}
//-----------------------------------------------------------------------------
void Plottiarea::create_point_menu() {
    left_click_menu = new Gtk::Menu();
    if (!(pointmenu_items.empty())) {
		for (vector<Glib::ustring>::iterator ii = pointmenu_items.begin(); ii != pointmenu_items.end(); ++ii) {
			Gtk::MenuItem * item = new Gtk::MenuItem("Set as " + *ii);
			left_click_menu->append(*item);
			item->signal_activate().connect(sigc::bind<Glib::ustring>(sigc::mem_fun(*this, &Plottiarea::on_point_selection_finished), *ii));
			// ii->left_value = &range_l;
			// ii->right_value = &range_l;
		}
    }
    left_click_menu->show_all();
};
//-----------------------------------------------------------------------------
void Plottiarea::show_range_selection_menu(void) {
	guint32 activation_time;
	
	// set so that drag_start_x < drag_end_x
	// and drag_start_y < drag_end_y
	if (drag_end_x < drag_start_x) {
		// double tenppi = kaksi; kaksi = yksi; yksi = tenppi;
		swap(drag_end_x,drag_start_x);
	}
	if (drag_end_y < drag_start_y) {
		// double tenppi = kaksi; kaksi = yksi; yksi = tenppi;
		swap(drag_end_y,drag_start_y);
	}
	if (!(ranges.empty())) {
		left_click_menu->popup(1,activation_time); // 1 as in left mouse button
		// std::cout << "buttoni_range:" << buttoni << std::endl;
	}
}
//-----------------------------------------------------------------------------
void Plottiarea::show_point_selection_menu(void) {
	guint32 activation_time;
	if (!(pointmenu_items.empty())) {
		left_click_menu->popup(1,activation_time);  // 1 as in left mouse button
		// std::cout << "buttoni_single:" << buttoni << std::endl;
	}
}
//-----------------------------------------------------------------------------
/*void Plottiarea::add_range(Gtk::SpinButton *leftii, Gtk::SpinButton *rightii, Glib::ustring name_of_rangei) {
	RANGE duh(leftii,rightii,name_of_rangei);
	ranges.push_back(duh);
}*/
//-----------------------------------------------------------------------------
/*void Plottiarea::add_singleclickmenuitem(Gtk::SpinButton *attachthis, Glib::ustring name_of_item) {
	RANGE duh(attachthis,attachthis,name_of_item);
	pointmenu_items.push_back(duh);
}
//-----------------------------------------------------------------------------
void Plottiarea::add_singleclickmenuitem(Gtk::Entry *attachthis, Glib::ustring name_of_item) {
	RANGE duh(attachthis,attachthis,name_of_item);
	pointmenu_items.push_back(duh);
}*/
void Plottiarea::add_singleclickmenuitem(const Glib::ustring & name_of_item) {
	pointmenu_items.push_back(name_of_item);
}
//-----------------------------------------------------------------------------
void Plottiarea::remove_ranges() { //those that can be selected with mouse
	ranges.clear();
	
	// Add again the default ones
	add_default_leftclickmenuitems();
}
//-----------------------------------------------------------------------------
void Plottiarea::remove_singleclickmenuitems() { //those that can be selected with mouse
	pointmenu_items.clear();
}
//-----------------------------------------------------------------------------
bool Plottiarea::remove_range(const Glib::ustring & name_of_range_to_remove)
{
	for (std::vector<Glib::ustring>::iterator ii = ranges.begin(); ii != ranges.end(); ++ii)
	{
		if (name_of_range_to_remove == *ii)
		{
			ii->erase();
			return true;
		}
	}
	return false;
}
//-----------------------------------------------------------------------------
bool Plottiarea::remove_singleclickmenuitem(const Glib::ustring & name_of_item_to_remove)
{
	for (std::vector<Glib::ustring>::iterator ii = pointmenu_items.begin(); ii != pointmenu_items.end(); ++ii)
	{
		if (name_of_item_to_remove == *ii)
		{
			ii->erase();
			return true;
		}
	}
	return false;
}
//-----------------------------------------------------------------------------
void Plottiarea::on_range_selection_finished(const Glib::ustring & name_of_range)
{
	/*
	std::cout << "name of the range: " << name_of_range << "\n";
	std::cout << "start x: " << drag_start_x << "\n";
	std::cout << "start y: " << drag_start_y << "\n";
	std::cout << "  end x: " << drag_end_x << "\n";
	std::cout << "  end y: " << drag_end_y << "\n";
	*/

	signal_range_selected.emit(drag_start_x, drag_start_y, drag_end_x, drag_end_y,name_of_range);
	delete left_click_menu;
}	
//-----------------------------------------------------------------------------
void Plottiarea::on_point_selection_finished(const Glib::ustring & name_of_point)
{
	/*
	std::cout << "name of the point: " << name_of_point << "\n";
	std::cout << "data x: " << drag_start_x << "\n";
	std::cout << "data y: " << drag_start_y << "\n";
	*/
	
	signal_point_selected.emit(drag_start_x, drag_start_y,name_of_point);
	delete left_click_menu;
}	
//-----------------------------------------------------------------------------
void Plottiarea::add_range(const Glib::ustring & name_of_rangei)
{
	ranges.push_back(name_of_rangei);
}
//-----------------------------------------------------------------------------
void Plottiarea::add_default_leftclickmenuitems(void)
{
	add_range("Integrate");
}
//-----------------------------------------------------------------------------
void Plottiarea::on_default_leftclick_actions_process(const double & x1,const double & y1,const double & x2,const double & y2,const Glib::ustring & range_name)
{
	if (plot_datas.empty() )
		return;
	Glib::ustring result_string;
	Gtk::Dialog calculations("Result");
	if (range_name == "Integrate" )
	{
		for (std::list<PlotDataContainer *>::iterator iter = plot_datas.begin(); iter != plot_datas.end() ; ++iter)
		{
			if ( (*iter)->get_data()->empty() )
				continue;
			calculations.set_title("Integration results");
			double total(0.0);
			for ( std::vector<Datapoint>::iterator i = (*iter)->get_data()->begin() ; i != (*iter)->get_data()->end() ; ++i)
			{
				// Let's see if the data_x is within the range of selection
				if ( i->x_value >= x1 && i->x_value <= x2)
				{
					total += i->y_value;
				}
			}
			// Let's see if the total is integer:
			std::string total_str;
			if ( abs( std::floor(total) - total) < 1e-6 )
			{
				total_str = asString (static_cast<int>(total + 0.5) );
			}
			else
				total_str = as_3f(total);
			
			result_string += (*iter)->data_name + ": " + total_str;
			Gtk::HBox * hpoks = Gtk::manage(new Gtk::HBox());
			Gtk::Entry * blaa = Gtk::manage(new Gtk::Entry());
			Gtk::Label * label = Gtk::manage(new Gtk::Label(total_str));
			blaa->set_text(total_str);
			label->set_label((*iter)->data_name +":");
			
			hpoks->pack_start(*label);
			hpoks->pack_start(*blaa);
			hpoks->show_all_children();
			calculations.get_vbox()->pack_start(*hpoks);
		}
		
		calculations.add_button("Ok", GTK_RESPONSE_OK);
		calculations.set_default_response(GTK_RESPONSE_OK);
		calculations.show_all_children();
		calculations.run();
		
		
		
	}
}
//-----------------------------------------------------------------------------
void Plottiarea::on_save_new_form_data(void){
	Glib::ustring outfilename;
	
	if (plot_datas.empty() )
		return;
	for (std::list<PlotDataContainer *>::iterator iter = plot_datas.begin(); iter != plot_datas.end() ; ++iter)
	{
		if ( (*iter)->get_data()->empty() )
			continue;
		
		save_dialog->set_title("Saving now " + (*iter)->data_name);
		int result = save_dialog->run();
		save_dialog->hide();
		if (result == Gtk::RESPONSE_OK)
		{
			outfilename = save_dialog->get_filename();
			boost::filesystem::ofstream outfile;
			if (!open_file_for_appending_and_popup_message_if_error(outfilename, outfile))
				continue;
			outfile.precision(13);
			outfile << "#Dataname: " << (*iter)->data_name << "\n";
			outfile << "#x y x_err y_err\n";
			for ( std::vector<Datapoint>::iterator i = (*iter)->get_data()->begin() ; i != (*iter)->get_data()->end() ; ++i)
			{
				outfile << i->x_value << " " << i->x_value_err << " " << i->y_value << " " << i->y_value_err << "\n";
			}
			outfile.close();
		}
	}

			/*	
				
				
	int result = save_dialog->run();
	save_dialog->hide();
	if (result == Gtk::RESPONSE_OK) {
		outfilename = save_dialog->get_filename();
		save_something(dat,outfilename);
	}

	if (dat2.data.empty() )
		return;

	save_dialog->set_title("Please choose output file (data2) "+dat2.data_name);
	result = save_dialog->run();
	save_dialog->hide();
	if (result == Gtk::RESPONSE_OK) {
		outfilename = save_dialog->get_filename();
	}
	else
		return;
	save_something(dat2,outfilename);

*/
}
//------------------------------------------------------------------------------
