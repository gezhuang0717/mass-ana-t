#include "stillflying.h"
#include "util.hh"

#include <iostream>



int global_stop=0;

MainWindow::MainWindow():
	m_Table(12, 4, false),
	m_Box1(false, 10),
	m_Box2(false, 10),
	m_Alignment(0.5,0.5,1.0,0.0),
//  m_Box4(false, 10),
	contnum_adjust(1,1,5,1,2,0),
	charge_adjust(1,1,5,1,2,0),
	cluster_adjust(1,1,500,1,5,0),
	freq_adjust1(1,1,50000000,0.01,100,0),
	time_adjust1(1,1,1000,0.1,10,0),
	freq_adjust2(1,1,50000000,0.01,100,0),
	time_adjust2(1,1,1000,0.1,10,0),
	freq_adjust3(1,1,50000,1,100,0),
	contnum_button(contnum_adjust),
	charge_button(charge_adjust),
	freq1_button(freq_adjust1),
	freq2_button(freq_adjust2),
	freq3_button(freq_adjust3),
	time1_button(time_adjust1),
	time2_button(time_adjust2),
	csize_button(cluster_adjust),
	m_Frame_ref("References"),
	m_Label_trap1("Trap1"),
	m_Label_trap2("Trap2"),
	m_Label_wall1("Wall1"),
	m_Label_wall2("Wall2"),
	m_Label_minus("f-"),
	m_Label_charge("Charge state"),
	m_Label_cont("Contamination"),
	m_Label_contnum("#atoms"),
	m_Label_contini("contamination contains"),
	m_Label_calib("Calibrate this"),
	m_Label_cluster1("Clusters chez Viki"),
	m_Label_cluster2("x C12"),
	trap_RadioButton1("trap1"),
	trap_RadioButton2("trap2"),
	stable_Button("stable only"),
	file1_Button("output to file"),
	file2_Button("output to file"),
	save_button("Re_member!",true),
	crap_button(" _Crap! ",true), 
	hco_button(" _HCO ",true),
	clear_button(" C_lear ",true),
	attack_button("_Attack!",true),
	cluster_button("Carbonize!"),
	aclear_button(" Clea_r ",true), 
	stop_button("_Stoooop!",true), 
	m_Button_Close("_Escape",true),
	testi(1)
{
	set_title("Crap!");
	std::string build_string = std::string(GIT_VERSION) + ", compiled " + std::string(COMPILE_DATE);
	set_title("Crap " + build_string);
	set_border_width(2);
	set_size_request(700, 400);

	Gtk::RadioButton::Group group = trap_RadioButton1.get_group();
	trap_RadioButton2.set_group(group);

// 	add(m_Box0);
  add(m_HPaned);

	m_Label_trap1.set_alignment(Gtk::ALIGN_LEFT, Gtk::ALIGN_TOP);
	m_Label_trap2.set_alignment(Gtk::ALIGN_LEFT, Gtk::ALIGN_TOP);
//  m_Label_trap2.set_alignment(Gtk::ALIGN_LEFT, Gtk::ALIGN_TOP);
	m_Label_wall1.set_alignment(Gtk::ALIGN_LEFT, Gtk::ALIGN_TOP);
	m_Label_wall2.set_alignment(Gtk::ALIGN_LEFT, Gtk::ALIGN_TOP);
	m_Label_cont.set_alignment(Gtk::ALIGN_LEFT, Gtk::ALIGN_TOP);
	m_Label_charge.set_alignment(Gtk::ALIGN_LEFT, Gtk::ALIGN_TOP);
	m_Label_calib.set_alignment(Gtk::ALIGN_LEFT, Gtk::ALIGN_TOP);

	freq1_button.set_digits(2);
	freq2_button.set_digits(2);
	time1_button.set_digits(2);
	time2_button.set_digits(2);

	//freq1_Entry.set_text("833622");
	freq1_button.set_alignment(1);
	//freq1_Entry.set_max_length(10);
	freq1_button.set_width_chars(12);
	//freq1_Entry.select_region(0, freq1_Entry.get_text_length());
	//freq2_Entry.set_text("1222230.08");
	freq2_button.set_alignment(1);
	// freq2_Entry.set_max_length(10);
	freq2_button.set_width_chars(12);
	freq3_button.set_alignment(1);
	freq2_button.set_width_chars(12);
	//freq2_Entry.select_region(0, freq2_Entry.get_text_length());
	// elem1_Entry.set_text("129xe");
	elem1_Entry.set_max_length(7);
	elem1_Entry.set_width_chars(7);
	elem1_Entry.select_region(0, elem1_Entry.get_text_length());
	//elem2_Entry.set_text("88rb");
	elem2_Entry.set_max_length(7);
	elem2_Entry.set_width_chars(7);
	elem2_Entry.select_region(0, elem2_Entry.get_text_length());
	// time1_Entry.set_text("170");
	time1_button.set_alignment(1);
	time1_button.set_max_length(5);
	time1_button.set_width_chars(5);
	//time1_Entry.select_region(0, time1_Entry.get_text_length());
	//time2_Entry.set_text("72");
	time2_button.set_alignment(1);
	time2_button.set_max_length(5);
	time2_button.set_width_chars(5);
	//time2_Entry.select_region(0, time2_Entry.get_text_length());
	//timeelem_Entry.set_text("129xe");
	timeelem1_Entry.set_max_length(7);
	timeelem1_Entry.set_width_chars(7);
	timeelem1_Entry.select_region(0, timeelem1_Entry.get_text_length());
	timeelem2_Entry.set_max_length(7);
	timeelem2_Entry.set_width_chars(7);
	timeelem2_Entry.select_region(0, timeelem2_Entry.get_text_length());
	contfreq_Entry.set_text("1222230.1");
	contfreq_Entry.set_alignment(1);
	//contfreq_Entry.set_max_length(10);
	contfreq_Entry.set_width_chars(12);
	contfreq_Entry.select_region(0, contfreq_Entry.get_text_length());
	conterr_Entry.set_text("20");
	conterr_Entry.set_alignment(1);
	conterr_Entry.set_max_length(5);
	conterr_Entry.set_width_chars(5);
	conterr_Entry.select_region(0, conterr_Entry.get_text_length());
	calibmol_Entry.set_text("81krm18o");
	calibmol_Entry.set_width_chars(12);
	calibmol_Entry.select_region(0, calibmol_Entry.get_text_length());
	contini_Entry.set_text("");
	contini_Entry.set_width_chars(12);
	contini_Entry.select_region(0, contini_Entry.get_text_length());
	contnum_button.set_width_chars(1);
	charge_button.set_width_chars(1);
	contnum_button.set_value(1);
	charge_button.set_value(1);

	read_default();

	c_ScrolledWindow.add(c_TreeView);
	c_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	a_ScrolledWindow.add(a_TreeView);
	a_ScrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

// 	m_Box0.pack_start(v_Box,Gtk::PACK_SHRINK);
// 	m_Box0.pack_start(v_Separator1,Gtk::PACK_SHRINK);
// 	m_Box0.pack_start(v_Separator2,Gtk::PACK_SHRINK);
// 	m_Box0.pack_start(c_ScrolledWindow);
  
  m_HPaned.pack1(v_Box, false, false);
  m_HPaned.pack2(c_ScrolledWindow, true, false);

	v_Box.pack_start(m_Table,Gtk::PACK_SHRINK);
	v_Box.pack_start(m_Separator4,Gtk::PACK_SHRINK);
	v_Box.pack_start(a_ScrolledWindow);
	v_Box.pack_start(m_Alignment,Gtk::PACK_SHRINK);
	m_Alignment.add(m_ProgressBar);

	m_Box3.pack_start(m_Label_contnum,Gtk::PACK_SHRINK);
	m_Box3.pack_start(contnum_button,Gtk::PACK_SHRINK);
	m_Box4.pack_start(trap_RadioButton1,Gtk::PACK_SHRINK);
	m_Box4.pack_start(trap_RadioButton2,Gtk::PACK_SHRINK);
//  m_Box1.pack_start(time1_button,Gtk::PACK_SHRINK);
//  m_Box1.pack_start(time2_button,Gtk::PACK_SHRINK);
	m_Box2.pack_start(file2_Button,Gtk::PACK_SHRINK);
	m_Box2.pack_start(aclear_button,Gtk::PACK_SHRINK);
	m_Box5.pack_start(csize_button,Gtk::PACK_SHRINK);
	m_Box5.pack_start(m_Label_cluster2,Gtk::PACK_SHRINK);

	m_Table.attach(m_Label_trap1,0,1,0,1,Gtk::SHRINK);
	m_Table.attach(elem1_Entry,1,2,0,1,Gtk::SHRINK);
	m_Table.attach(freq1_button,2,3,0,1,Gtk::SHRINK);
	m_Table.attach(m_Button_Close,3,4,0,1,Gtk::SHRINK);

	m_Table.attach(m_Label_trap2,0,1,1,2,Gtk::SHRINK);
	m_Table.attach(elem2_Entry,1,2,1,2,Gtk::SHRINK);
	m_Table.attach(freq2_button,2,3,1,2,Gtk::SHRINK);
	m_Table.attach(save_button,3,4,1,2,Gtk::SHRINK);

//  m_Table.attach(m_Label_trap2,0,1,2,3,Gtk::SHRINK);
	m_Table.attach(m_Label_minus,1,2,2,3,Gtk::SHRINK);
	m_Table.attach(freq3_button,2,3,2,3,Gtk::SHRINK);

	m_Table.attach(m_Label_wall1,0,1,3,4,Gtk::SHRINK);
	m_Table.attach(timeelem1_Entry,1,2,3,4,Gtk::SHRINK);
	m_Table.attach(time1_button,2,3,3,4,Gtk::SHRINK);

	m_Table.attach(m_Label_wall2,0,1,4,5,Gtk::SHRINK);
	m_Table.attach(timeelem2_Entry,1,2,4,5,Gtk::SHRINK);
	m_Table.attach(time2_button,2,3,4,5,Gtk::SHRINK);

	m_Table.set_row_spacing(4,5); // was 3,5
	m_Table.attach(m_Separator,0,4,5,6);

	m_Table.attach(m_Label_charge,0,1,6,7,Gtk::SHRINK);
	m_Table.attach(charge_button,1,2,6,7,Gtk::SHRINK);

	m_Table.set_row_spacing(6,5); // was 3,5
	m_Table.attach(m_Separator2,0,4,7,8);

	m_Table.attach(m_Label_cont,0,1,8,9,Gtk::SHRINK);
	m_Table.attach(contfreq_Entry,1,2,8,9,Gtk::SHRINK);
	m_Table.attach(conterr_Entry,2,3,8,9,Gtk::SHRINK);
	m_Table.attach(clear_button,3,4,8,9,Gtk::SHRINK);

	m_Table.attach(m_Box4,1,2,9,10,Gtk::SHRINK);
	m_Table.attach(m_Box3,2,3,9,10,Gtk::SHRINK);
	m_Table.attach(stop_button,3,4,9,10,Gtk::SHRINK);

	m_Table.attach(stable_Button,1,2,10,11,Gtk::SHRINK);
	m_Table.attach(file1_Button,2,3,10,11,Gtk::SHRINK);
	m_Table.attach(hco_button,3,4,10,11,Gtk::SHRINK);


	m_Table.attach(m_Label_contini,1,2,11,12,Gtk::SHRINK);
	m_Table.attach(contini_Entry,2,3,11,12,Gtk::SHRINK);
	m_Table.attach(crap_button,3,4,11,12,Gtk::SHRINK);

	m_Table.set_row_spacing(11,5); //was 8,5?
	m_Table.attach(m_Separator3,0,4,12,13);

	m_Table.attach(m_Label_calib,0,1,13,14,Gtk::SHRINK);
	m_Table.attach(calibmol_Entry,1,2,13,14,Gtk::SHRINK);
	m_Table.attach(m_Box2,2,3,13,14,Gtk::SHRINK);
	m_Table.attach(attack_button,3,4,13,14,Gtk::SHRINK);
 
	m_Table.attach(m_Label_cluster1,1,2,14,15,Gtk::SHRINK);
	m_Table.attach(m_Box5,2,3,14,15,Gtk::SHRINK);
	m_Table.attach(cluster_button,3,4,14,15,Gtk::SHRINK);

	trap_RadioButton1.set_active();
	stable_Button.set_active();

	calibmol_Entry.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::attack_button_clicked));

	trap_RadioButton1.signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &MainWindow::radio_button_clicked),1));
	trap_RadioButton2.signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &MainWindow::radio_button_clicked),2));

	file1_Button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::file1_button_clicked));
	file2_Button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::file2_button_clicked));

	crap_button.signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &MainWindow::crap_button_clicked),0));
	hco_button.signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &MainWindow::crap_button_clicked),1));
	clear_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::clear_button_clicked));
	attack_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::attack_button_clicked));
	cluster_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::cluster_button_clicked));
	aclear_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::aclear_button_clicked));
	stop_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::stop_button_clicked));
	save_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::write_default));

	m_Button_Close.set_flags(Gtk::CAN_DEFAULT);
	m_Button_Close.grab_default();
	m_Button_Close.signal_clicked().connect( sigc::mem_fun(*this, &MainWindow::on_button_close) );

	show_all_children();
}

MainWindow::~MainWindow()
{
}

void MainWindow::crap_button_clicked(int i)
{

	//  std::cout << "calculate frequency" << std::endl;
	int stable = (stable_Button.get_active())? 1:0 ;
	int fout = (file1_Button.get_active())? 1:0;
	if (testi == 1){
		crap(elem1_Entry.get_text(),freq1_button.get_value(),atof(contfreq_Entry.get_text().c_str()),atof(conterr_Entry.get_text().c_str()),contnum_button.get_value_as_int(),charge_button.get_value_as_int(), stable, contini_Entry.get_text(),charge_button.get_value_as_int(), fout,i);
	}
	if (testi == 2){
		crap(elem2_Entry.get_text(),freq2_button.get_value(),atof(contfreq_Entry.get_text().c_str()),atof(conterr_Entry.get_text().c_str()),contnum_button.get_value_as_int(),charge_button.get_value_as_int(), stable, contini_Entry.get_text(),charge_button.get_value_as_int(),fout,i);
	}
}

void MainWindow::on_button_close()
{
	std::cout << "Have a nice day!" << std::endl;
	hide();
}

void MainWindow::radio_button_clicked(int i)
{
	if (i==1) testi=1;
	if (i==2) testi=2;
}

void MainWindow::attack_button_clicked()
{
	//  std::cout << "calibrate" << std::endl;
	int fout = (file2_Button.get_active())? 1:0;
	attack(elem1_Entry.get_text(),elem2_Entry.get_text(),timeelem1_Entry.get_text(),timeelem2_Entry.get_text(),freq1_button.get_value(),freq2_button.get_value(),time1_button.get_value(),time2_button.get_value(),calibmol_Entry.get_text(),charge_button.get_value_as_int(),fout);
  
}

void MainWindow::cluster_button_clicked()
{
	//  std::cout << "calibrate" << std::endl;
	//int fout = (file2_Button.get_active())? 1:0;
	cluster(elem1_Entry.get_text(),elem2_Entry.get_text(),timeelem1_Entry.get_text(),timeelem2_Entry.get_text(),freq1_button.get_value(),freq2_button.get_value(),time1_button.get_value(),time2_button.get_value(),charge_button.get_value_as_int(),csize_button.get_value_as_int());

}

void MainWindow::clear_button_clicked()
{
	c_TreeView.c_refTreeModel->clear();
}

void MainWindow::aclear_button_clicked()
{
	a_TreeView.a_refTreeModel->clear();
}

void MainWindow::stop_button_clicked()
{
	global_stop=1;
}

void MainWindow::file1_button_clicked()
{
	int fout = (file1_Button.get_active())? 1:0;
	if (fout==1)
    {
		Gtk::FileChooserDialog dialog("Write crappy file!", Gtk::FILE_CHOOSER_ACTION_SAVE);
		dialog.set_transient_for(*this);
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);
		Gtk::FileFilter filter_text;
		filter_text.set_name("Text files");
		filter_text.add_mime_type("text/plain");
		dialog.add_filter(filter_text);
		Gtk::FileFilter filter_any;
		filter_any.set_name("Any files");
		filter_any.add_pattern("*");
		dialog.add_filter(filter_any);

		int result = dialog.run();
		//Handle the response:
		switch(result)
		{
		case(Gtk::RESPONSE_OK):
			{
				file1 = dialog.get_filename();     
				break;
			}
		case(Gtk::RESPONSE_CANCEL):
			{
				break;
			}
		default:
		{
			break;
		}
		}
    }
}

void MainWindow::file2_button_clicked()
{
	int fout = (file2_Button.get_active())? 1:0;
	if (fout==1)
    {
		Gtk::FileChooserDialog dialog("Write crappy file!", Gtk::FILE_CHOOSER_ACTION_SAVE);
		dialog.set_transient_for(*this);
		dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		dialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);
		Gtk::FileFilter filter_text;
		filter_text.set_name("Text files");
		filter_text.add_mime_type("text/plain");
		dialog.add_filter(filter_text);
		Gtk::FileFilter filter_any;
		filter_any.set_name("Any files");
		filter_any.add_pattern("*");
		dialog.add_filter(filter_any);

		int result = dialog.run();
		switch(result)
		{
		case(Gtk::RESPONSE_OK):
			{
				file2 = dialog.get_filename();     
				break;
			}
		case(Gtk::RESPONSE_CANCEL):
			{
				break;
			}
		default:
		{
			break;
		}
		}
    }
}

void MainWindow::read_default()
{
	ifstream xfile;
	xfile.open("default.dat");
	if (!xfile)
	{
	  cout << "could not read \"default.dat\"\n"; /* exit(0); */
	  //Instead of silently dying, set some default values
	  elem1_Entry.set_text("85rb");
	  elem2_Entry.set_text("85rb");
	  timeelem1_Entry.set_text("85rb");
	  timeelem2_Entry.set_text("85rb");
	  freq1_button.set_value(6.6);
	  freq2_button.set_value(66.6);
	  freq3_button.set_value(666.6);
	  time1_button.set_value(6.6);
	  time2_button.set_value(6.7);
	}
	else
	{
	  while(!xfile.eof()){
		  string row;
		  getline(xfile,row);
		  if (row[0]=='?') continue;
		  int s = row.find(':');
		  int t = row.find('?');
		  string substr1(row,0,s);
		  string substr2(row,s+1,t-s-1); 

		  if (substr1=="trap1_isotope") elem1_Entry.set_text(substr2);
		  if (substr1=="trap2_isotope") elem2_Entry.set_text(substr2);
		  if (substr1=="timing1_isotope") timeelem1_Entry.set_text(substr2);
		  if (substr1=="timing2_isotope") timeelem2_Entry.set_text(substr2);
		  if (substr1=="trap1_frequency") freq1_button.set_value(atof(substr2.c_str()));
		  if (substr1=="trap2_frequency") freq2_button.set_value(atof(substr2.c_str()));
		  if (substr1=="trap3_frequency") freq3_button.set_value(atof(substr2.c_str()));
		  if (substr1=="time1") time1_button.set_value(atof(substr2.c_str()));
		  if (substr1=="time2") time2_button.set_value(atof(substr2.c_str()));
	  }
	  xfile.close();
	}
}

void MainWindow::write_default()
{
	char buffer[64];
	ofstream xfile;
	xfile.open("default.dat");
	if (!xfile){cout << "could not write default\n"; exit(0);}
	xfile << "trap1_isotope:" << elem1_Entry.get_text() << "?" << endl; 
	xfile << "trap2_isotope:" << elem2_Entry.get_text() << "?" << endl; 
	xfile << "timing1_isotope:" << timeelem1_Entry.get_text() << "?" << endl;
	xfile << "timing2_isotope:" << timeelem2_Entry.get_text() << "?" << endl;
	sprintf(buffer,"%.2f",freq1_button.get_value());
	xfile << "trap1_frequency:" << buffer << "?" << endl; 
	sprintf(buffer,"%.2f",freq2_button.get_value());
	xfile << "trap2_frequency:" << buffer << "?" << endl; 
	sprintf(buffer,"%f",freq3_button.get_value());
	xfile << "trap3_frequency:" << buffer << "?" << endl;
	xfile << "time1:" << time1_button.get_value() << "?" << endl;
	xfile << "time2:" << time2_button.get_value() << "?" << endl; 
	xfile.close();
}

int MainWindow::attack(string iso1,string iso2, string iso3, string iso4, double f1, double f2, double t1, double t2, string c, int charge, int fout )
{
	//  cout << "using calibration file\n";
	CCalibrate ccalib(iso1,iso2, iso3, iso4, f1, f2, t1, t2);
	CMolecule moly;
	Gtk::TreeModel::Row row;
	moly.unmake_mol(c);
	
	for (int i=0;i<moly.num();i++)
    {
		// CIsotope *iso = &moly.get_ref(i);
		if (moly.get_ref(i).get_mass()==0)
		{
			row = *(a_TreeView.a_refTreeModel->append());
			row[a_TreeView.a_Columns.m_col_mol] =  moly.get_ref(i).get_name()+ " !?";
			row[a_TreeView.a_Columns.m_col_freq_T1_f_c] =  "Wow! ";
			row[a_TreeView.a_Columns.m_col_freq_T2_f_c] =  "Very exotic...";

			return(0);
		}

		moly = moly + moly.get_ref(i);
    }
        cout.precision(13);
	cout << moly.get_mass() << "\t" << moly.get_name() << "\t" << ccalib.get_trap1().get_mass()<< endl ;
	double TOF_RFQ_to_T1 = sqrt(moly.get_mass()/(ccalib.get_timing1().get_mass()*charge))*ccalib.get_time1();
	double TOF_T1_to_T2 = sqrt(moly.get_mass()/(ccalib.get_timing2().get_mass()*charge))*ccalib.get_time2();
// 	char TOF_RFQ_to_T1_str[64],TOF_T1_to_T2_str[64],tempc3[64],tempc4[64],tempc5[64] ;
  
  double T1_f_c = moly.get_freq(ccalib.get_trap1(),charge);
  double T2_f_c = moly.get_freq(ccalib.get_trap2(),charge);
  double T2_f_minus = freq3_button.get_value();
  double T2_f_plus = T2_f_c - T2_f_minus;
  double T2_f_axial = sqrt( pow( T2_f_c, 2.0) - pow( T2_f_minus, 2.0) - pow(T2_f_plus, 2.0) );

	cout << "frequencies: ";
	cout << setprecision(12) << moly.get_freq(ccalib.get_trap1(),charge) << "\t" << moly.get_freq(ccalib.get_trap2(),charge) << "\n";
	cout << "timings: cooler to trap: " << setprecision(4) << TOF_RFQ_to_T1 <<  "\tmiddle wall: " << TOF_T1_to_T2 << "\n";
	row = *(a_TreeView.a_refTreeModel->append());
	row[a_TreeView.a_Columns.m_col_mol] =  c;
	row[a_TreeView.a_Columns.m_col_freq_T1_f_c] =  as_1f(T1_f_c);
	row[a_TreeView.a_Columns.m_col_freq_T2_f_c] =  as_2f(T2_f_c);
	row[a_TreeView.a_Columns.m_col_freq_T2_f_plus] = as_1f(T2_f_plus);
  row[a_TreeView.a_Columns.m_col_freq_T2_f_axial] = as_0f(T2_f_axial);
	row[a_TreeView.a_Columns.m_col_time1] =  as_1f(TOF_RFQ_to_T1);
	row[a_TreeView.a_Columns.m_col_time2] =  as_1f(TOF_T1_to_T2);
  
  row[a_TreeView.a_Columns.m_col_freq_T2_T_axial] =  as_0f(1./T2_f_axial*1.0e6);
  row[a_TreeView.a_Columns.m_col_freq_T2_T_plus] =  as_1f(1./T2_f_plus*1.0e6);
  row[a_TreeView.a_Columns.m_col_freq_T2_T_minus] =  as_0f(1./T2_f_minus*1.0e6);

	if (fout==1)
    {
		ofstream xfile;
		xfile.open(file2.c_str(),ios::app);
		xfile << c << "\t" <<  setprecision(12) << T1_f_c << "\t" << T2_f_c << "\t" << setprecision(4) << TOF_RFQ_to_T1 <<  "\t" << TOF_T1_to_T2 << "\n";
		xfile.close();
    }

	return(0);
}

int MainWindow::cluster(string iso1,string iso2, string iso3, string iso4, double f1, double f2, double t1, double t2, int charge, int size )
{
	//  cout << "using calibration file\n";
	CCalibrate ccalib(iso1,iso2, iso3, iso4, f1, f2, t1, t2);
	CMolecule moly;
	Gtk::TreeModel::Row row;
	moly.cluster(size);
	//  cout << isotope.get_mass() << "\t" <<isotope.get_ME() << "\t"<< isotope.get_A() << "\t" << ccalib.get_trap1().get_mass()<< endl ;
	double temp1 = sqrt(moly.get_mass()/(ccalib.get_timing1().get_mass())*charge)*ccalib.get_time1();
	double temp2 = sqrt(moly.get_mass()/(ccalib.get_timing2().get_mass())*charge)*ccalib.get_time2();
	char tempc1[64],tempc2[64],tempc3[64],tempc4[64],tempc5[64] ;
	sprintf(tempc1,"%.3f",temp1);
	sprintf(tempc2,"%.3f",temp2);
	sprintf(tempc3,"%.3f", moly.get_freq(ccalib.get_trap1(),charge) );
	sprintf(tempc4,"%.3f", moly.get_freq(ccalib.get_trap2(),charge) );
	double temp5 = atof(tempc4) - freq3_button.get_value();
	sprintf(tempc5,"%.3f",temp5);
//  cout << temp5 << "\t" << tempc5 << endl;
	string temps1 = tempc1;
	string temps2 = tempc2;
	string temps3 = tempc3;
	string temps4 = tempc4;
	string temps5 = tempc5;
	cout << "frequencies: ";
	cout << setprecision(12) << moly.get_freq(ccalib.get_trap1(),charge) << "\t" << moly.get_freq(ccalib.get_trap2(),charge) << "\n";
	cout << "timings: cooler to trap: " << setprecision(4) << temp1 <<  "\tmiddle wall: " << temp2 << "\n";
	row = *(a_TreeView.a_refTreeModel->append());
	row[a_TreeView.a_Columns.m_col_mol] =  moly.get_name();
	row[a_TreeView.a_Columns.m_col_freq_T1_f_c] =  temps3;
	row[a_TreeView.a_Columns.m_col_freq_T2_f_c] =  temps4;
	row[a_TreeView.a_Columns.m_col_freq_T2_f_plus] =  temps5;
  row[a_TreeView.a_Columns.m_col_freq_T2_f_axial] =  "to come";
  
	row[a_TreeView.a_Columns.m_col_time1] =  temps1;
	row[a_TreeView.a_Columns.m_col_time2] =  temps2;

	return(0);
}

int MainWindow::crap(string el, double f, double cont, double er, int num, int chqrge, int stable, string im, int charge, int fout, int hco)
{
	CRef cref(el,f);
	//cout << cref.get_name() << "\t" << cref.get_freq() <<  "\t" << cref.get_A() <<  "\t" << cref.get_ME() <<"\t" << cref.get_mass() << "\n";
	CCont ccont(cont,er,num,charge);
	if (hco==1)   ccont.set_count(100);
	ccont.make_masses(cref);
	//cout << ccont.get_A_limit() << "\t" << ccont.get_lower() << "\t" << ccont.get_higher() << "\n";
	CMolecule ini_moly;
	ini_moly.set_null();

	ini_moly.unmake_mol(im);
	for (int i=0;i<ini_moly.num();i++)
    {
		// CIsotope *iso = &ini_moly.get_ref(i);
		// ini_moly = ini_moly + *iso;
		ini_moly = ini_moly + ini_moly.get_ref(i);
    }
	int ini_num;
	if (hco==0) ini_num=ccont.get_count();
	else if (hco==1) ini_num=1;
	CTable atable(stable);
	if (hco==1){
		CIsotope hydro("h",1,7289),carb("c",12,0),oxy("o",16,-4737);
		vector<CIsotope> hco_iso;
		hco_iso.push_back(hydro);
		hco_iso.push_back(carb);
		hco_iso.push_back(oxy);
		atable.set_iso(hco_iso,3);
	}
	else {atable.ini_isotopes(ccont.get_A_limit());}
	vector<CTable> ctable;
	for (int i=0;i<ccont.get_count();i++) ctable.push_back(atable);
	int c,i_mol;
	Gtk::TreeModel::Row row;
	i_mol=0;
	for (int atoms=ini_num;atoms<=ccont.get_count();atoms++)
	{
		int end_test=0;
		for (int i=0;i<atoms;i++) {ctable.at(i).reset();}
		CMolecule temp_moly = ini_moly;
//		cout << "line: " << ctable.at(0).get_line() << "  limit: "ctable.at(0).get_limit() << "  ratio:" << ctable.at(0).get_line()/ctable.at(0).get_limit();
		while (ctable.at(0).get_line()<ctable.at(0).get_limit())  
		{
			m_ProgressBar.set_fraction(1.0*ctable.at(0).get_line()/(1.0*ctable.at(0).get_limit()));
			while (Gtk::Main::events_pending()) Gtk::Main::iteration();
			CMolecule molecule;
			for (int i=i_mol;i<atoms;i++)
			{
				if (global_stop==1)
				{
					global_stop=0;
					cout << "Patience, young Skywalker" << endl;
					m_ProgressBar.set_fraction(0.0);
					return(0);
				}
				molecule.set_mass(temp_moly.get_mass());
				molecule.set_name(temp_moly.get_name());
				molecule = molecule + ctable.at(i).get_isotope();
				c = ccont.compare1(molecule,atoms+ini_moly.num(),i+1+ini_moly.num());
				if (c==0)
				{
					temp_moly.set_mass(molecule.get_mass());
					temp_moly.set_name(molecule.get_name());
					i_mol=i+1;
//					cout << "temp_moly: " << temp_moly.get_name()<< endl;
				}
				if ((c==10 && (atoms-i-1)<0.0001) && (((i>0.0001) && ctable.at(i).get_line()<ctable.at(i-1).get_line()) || (i<0.0001)))
				{
					step(ctable,atoms);
					break;
				}
				else if (c==10 && (i>0.0001) && ctable.at(i).get_line()==ctable.at(i-1).get_line())
				{
					step(ctable,atoms);
					temp_moly.set_mass(ini_moly.get_mass());
					temp_moly.set_name(ini_moly.get_name());
					i_mol=0;
					break;
				}
				else if (c==10)
				{
					step_reset(ctable,i,atoms);
					break;
				}
				if (c==1 && i<0.0001 ){end_test=1; break;}
				else if (c==1)
				{
					step_check(ctable,i-1,atoms);
					temp_moly.set_mass(ini_moly.get_mass());
					temp_moly.set_name(ini_moly.get_name());
					i_mol=0;
					break;
				}
			}
			if (end_test==1) break;
			if (c==10) continue;
			if (c==1) continue;
			c = ccont.compare2(molecule);
			if (c==1) {
				if (hco==1)
				{
					int hy=0;
					int ca=0;
					int ox=0;
					for (int t=0;t<atoms;t++)
					{
						if (ctable.at(t).get_line()==0) hy++;
						if (ctable.at(t).get_line()==1) ca++;
						if (ctable.at(t).get_line()==2) ox++;
					}
					char chy[32],cca[32],cox[32];
//					string shy,sca,sox;
					sprintf(chy,"%d",hy);
					sprintf(cca,"%d",ca);
					sprintf(cox,"%d",ox);
					string name=ini_moly.get_name()+"O"+cox+"-C"+cca+"-H"+chy;
					molecule.set_name(name);
				}
				cout << molecule.get_name() << "\t"<< setprecision(12) << molecule.get_freq(cref,charge) << "\n";
				row = *(c_TreeView.c_refTreeModel->append());
				row[c_TreeView.c_Columns.m_col_mol] =  molecule.get_name();
				row[c_TreeView.c_Columns.m_col_freq] = molecule.get_freq(cref,charge);
				if (fout==1) {   
					ofstream xfile;
					xfile.open(file1.c_str(),ios::app);
					xfile << molecule.get_name() << "\t"<< setprecision(12) << molecule.get_freq(cref,charge) << "\n";
					xfile.close();   
				}     
			}
			step(ctable,atoms);
			temp_moly.set_mass(ini_moly.get_mass());
			temp_moly.set_name(ini_moly.get_name());
			i_mol=0;
		}
		m_ProgressBar.set_fraction(0.0);
	}
	ctable.clear();
	cout << "...that's all there is\n";
	return(0);
}

void MainWindow::step_reset(vector<CTable>& table,int i, int count)
{
//	cout << "step_reset" << endl;
	table.at(i).inc_line();
	for (int k=i+1;k<count;k++) table.at(k).reset();
}

void MainWindow::step(vector<CTable>& table, int count)
{ 
//	cout << "step" << endl;
	int check = count-1;
	for (int k=count-1;k>0;k--)
	{
		if (table.at(k).get_line()<table.at(k-1).get_line()) break;
		check--;
	}
	table.at(check).inc_line();
	for (int j=check+1;j<count;j++) table.at(j).reset();
}

void MainWindow::step_check(vector<CTable>& table, int h, int count)
{ 
//	cout << "step_check" << endl;
	int check = h;
	for (int k=h;k>0;k--)
	{
		if (table.at(k).get_line()<table.at(k-1).get_line()) break;
		check--;
	}
	table.at(check).inc_line();
	for (int j=check+1;j<count;j++) table.at(j).reset();

}

CrapWindow::CrapWindow()
{
	c_refTreeModel = Gtk::ListStore::create(c_Columns);
	set_model(c_refTreeModel);
	enable_model_drag_source();
	enable_model_drag_dest();

	append_column_editable("Contamination", c_Columns.m_col_mol); //This number will be shown with the default numeric formatting.
	append_column_editable("Frequency", c_Columns.m_col_freq);
	for(guint i = 0; i < 2; i++)
	{
		Gtk::TreeView::Column* pColumn = get_column(i);
		pColumn->set_reorderable();
	}
	{
		Gtk::Menu::MenuList& menulist = m_Menu_Popup.items();

		menulist.push_back( Gtk::Menu_Helpers::MenuElem("_Delete", sigc::mem_fun(*this, &CrapWindow::on_menu_file_popup_generic) ) );
	}
	m_Menu_Popup.accelerate(*this);
}

CrapWindow::~CrapWindow()
{
}

bool CrapWindow::on_button_press_event(GdkEventButton* event)
{
	bool return_value = TreeView::on_button_press_event(event);
	if( (event->type == GDK_BUTTON_PRESS) && (event->button == 3) )
	{
		m_Menu_Popup.popup(event->button, event->time);
	}
	return return_value;
}

void CrapWindow::on_menu_file_popup_generic()
{
	Glib::RefPtr<Gtk::TreeView::Selection> refSelection = get_selection();
	//refSelection->set_mode(Gtk::SELECTION_MULTIPLE);
	Gtk::TreeModel::iterator iter = refSelection->get_selected();
	// refSelection->selected_foreach_iter(sigc::mem_fun(*this, &CrapWindow::selected_row_callback) );
	if(iter)
	{
		c_refTreeModel->erase(iter);
	}

}

void CrapWindow::selected_row_callback(const Gtk::TreeModel::iterator& iter)
{
	if(iter)
	{
		c_refTreeModel->erase(iter);
	}

}

AttackWindow::AttackWindow()
{
	a_refTreeModel = Gtk::ListStore::create(a_Columns);
	set_model(a_refTreeModel);
	enable_model_drag_source();
	enable_model_drag_dest();
  set_grid_lines(Gtk::TREE_VIEW_GRID_LINES_BOTH);
  
	append_column_editable("Name", a_Columns.m_col_mol); //This number will be shown with the default numeric formatting.
	append_column_editable("T1 fc", a_Columns.m_col_freq_T1_f_c);
	append_column_editable("T2 fc", a_Columns.m_col_freq_T2_f_c);
	append_column_editable("T2 f+", a_Columns.m_col_freq_T2_f_plus);
  append_column_editable("T2 fz", a_Columns.m_col_freq_T2_f_axial);
	append_column_editable("RFQ-T1", a_Columns.m_col_time1);
	append_column_editable("T1-T2", a_Columns.m_col_time2);
  append_column_editable("T2 T+", a_Columns.m_col_freq_T2_T_plus);
  append_column_editable("T2 T-", a_Columns.m_col_freq_T2_T_minus);
  append_column_editable("T2 Tz", a_Columns.m_col_freq_T2_T_axial);
	for(guint i = 0; i < 2; i++)
	{
		Gtk::TreeView::Column* pColumn = get_column(i);
		pColumn->set_reorderable();
	}
	{
		Gtk::Menu::MenuList& menulist = m_Menu_Popup.items();

		menulist.push_back( Gtk::Menu_Helpers::MenuElem("_Delete",
														sigc::mem_fun(*this, &AttackWindow::on_menu_file_popup_generic) ) );
		/*    menulist.push_back( Gtk::Menu_Helpers::MenuElem("_Process",
			  sigc::mem_fun(*this, &AttackWindow::on_menu_file_popup_generic) ) );
			  menulist.push_back( Gtk::Menu_Helpers::MenuElem("_Remove",
			  sigc::mem_fun(*this, &AttackWindow::on_menu_file_popup_generic) ) );
		*/
	}
	m_Menu_Popup.accelerate(*this);  
}

AttackWindow::~AttackWindow()
{
}

bool AttackWindow::on_button_press_event(GdkEventButton* event)
{
	//Call base class, to allow normal handling,
	//such as allowing the row to be selected by the right-click:
	bool return_value = TreeView::on_button_press_event(event);

	//Then do our custom stuff:
	if( (event->type == GDK_BUTTON_PRESS) && (event->button == 3) )
	{
		m_Menu_Popup.popup(event->button, event->time);
	}

	return return_value;
}

void AttackWindow::on_menu_file_popup_generic()
{
	Glib::RefPtr<Gtk::TreeView::Selection> refSelection = get_selection();
	//refSelection->set_mode(Gtk::SELECTION_MULTIPLE);
	Gtk::TreeModel::iterator iter = refSelection->get_selected();
	// refSelection->selected_foreach_iter(sigc::mem_fun(*this, &AttackWindow::selected_row_callback) );
   
	if(iter)
	{
		a_refTreeModel->erase(iter);
	}
   
}

void AttackWindow::selected_row_callback(const Gtk::TreeModel::iterator& iter)
{
	// TreeModel::Row row = *iter;
	/*
	  if(refSelection)
	  {
	*/ 
	if(iter)
	{
		a_refTreeModel->erase(iter);
	}

} 

