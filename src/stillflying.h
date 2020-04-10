#ifndef GTKMM_STILLFLYING_H
#define GTKMM_STILLFLYING_H

#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/window.h>
#include <gtkmm/radiobutton.h>
#include <gtkmm/box.h>
#include <gtkmm/separator.h>
#include <gtkmm/paned.h>
#include "classes.h"

class CrapWindow : public Gtk::TreeView
{
 public:
  CrapWindow();
  virtual ~CrapWindow();

 protected:
  virtual bool on_button_press_event(GdkEventButton *ev);
  virtual void on_menu_file_popup_generic();
  void selected_row_callback(const Gtk::TreeModel::iterator& iter);

  class CrapColumns : public Gtk::TreeModel::ColumnRecord
  {
  public:
    CrapColumns()
      { add(m_col_mol); add(m_col_freq);}

    Gtk::TreeModelColumn<Glib::ustring> m_col_mol;
    Gtk::TreeModelColumn<double> m_col_freq;
  };
  CrapColumns c_Columns;
  Glib::RefPtr<Gtk::ListStore> c_refTreeModel;
  Gtk::Menu m_Menu_Popup;
  friend class MainWindow;
};

class AttackWindow : public Gtk::TreeView
{
 public:
  AttackWindow();
  virtual ~AttackWindow();

 protected:
  // Override Signal handler:
  // Alternatively, use signal_button_press_event().connect_notify()
  virtual bool on_button_press_event(GdkEventButton *ev);

  //Signal handler for popup menu items:
  virtual void on_menu_file_popup_generic();
  void selected_row_callback(const Gtk::TreeModel::iterator& iter);
  class AttackColumns : public Gtk::TreeModel::ColumnRecord
  {
  public:

    AttackColumns()      
    {
      add(m_col_mol);
      add(m_col_freq_T1_f_c);
      add(m_col_freq_T2_f_c);
      add(m_col_freq_T2_f_plus);
      add(m_col_freq_T2_f_axial);
      add(m_col_freq_T2_T_minus);
      add(m_col_freq_T2_T_plus);
      add(m_col_freq_T2_T_axial);
      add(m_col_time1);
      add(m_col_time2);
      
      
    }

    Gtk::TreeModelColumn<Glib::ustring> m_col_mol;
    Gtk::TreeModelColumn<Glib::ustring> m_col_freq_T1_f_c; // T1 fc
    Gtk::TreeModelColumn<Glib::ustring> m_col_freq_T2_f_c; // T2 fc
    Gtk::TreeModelColumn<Glib::ustring> m_col_freq_T2_f_plus; // T2 f+
    Gtk::TreeModelColumn<Glib::ustring> m_col_freq_T2_f_axial; // T2 fz
    Gtk::TreeModelColumn<Glib::ustring> m_col_freq_T2_T_plus; // T2 f+ period
    Gtk::TreeModelColumn<Glib::ustring> m_col_freq_T2_T_minus; // T2 f- period
    Gtk::TreeModelColumn<Glib::ustring> m_col_freq_T2_T_axial; // T2 fz period
    Gtk::TreeModelColumn<Glib::ustring> m_col_time1; // RFQ->T1
    Gtk::TreeModelColumn<Glib::ustring> m_col_time2; // T1->T2

  };
  AttackColumns a_Columns;
  Glib::RefPtr<Gtk::ListStore> a_refTreeModel;
  Gtk::Menu m_Menu_Popup;
  friend class MainWindow;
};



class MainWindow : public Gtk::Window
{

 public:
  MainWindow();
  virtual ~MainWindow();

 protected:
  virtual void crap_button_clicked(int);
  virtual void clear_button_clicked();
  virtual void attack_button_clicked();
  virtual void cluster_button_clicked();
  virtual void aclear_button_clicked();
  virtual void stop_button_clicked();
  virtual void on_button_close();
  virtual void radio_button_clicked(int);
  virtual void file1_button_clicked();
  virtual void file2_button_clicked();
  void read_default();
  void write_default();
  int attack(string,string,string,string,double,double,double,double,string,int,int);
  int cluster(string,string,string,string,double,double,double,double,int,int);
  int crap(string, double, double, double, int, int, int, string, int, int,int);
  void step_reset(vector<CTable>&, int, int);
  void step(vector<CTable>& , int);
  void step_check(vector<CTable>&, int,int);

  Gtk::Table m_Table;
  Gtk::HBox m_Box1;  // removed m_Box0;
  Gtk::HBox m_Box4, m_Box2, m_Box3, m_Box5;
  Gtk::VBox v_Box;
  Gtk::Alignment m_Alignment;
  Gtk::Entry elem1_Entry;
  Gtk::Entry elem2_Entry;
  //  Gtk::Entry time1_Entry;
  //  Gtk::Entry time2_Entry;
  Gtk::Entry timeelem1_Entry;
  Gtk::Entry timeelem2_Entry;
  Gtk::Entry contfreq_Entry;
  Gtk::Entry conterr_Entry;
  Gtk::Adjustment contnum_adjust, charge_adjust, cluster_adjust;
  Gtk::Adjustment freq_adjust1,freq_adjust2, freq_adjust3;
  Gtk::Adjustment time_adjust1,time_adjust2;
  Gtk::SpinButton contnum_button;
  Gtk::SpinButton charge_button;
  Gtk::SpinButton freq1_button;
  Gtk::SpinButton freq2_button;
  Gtk::SpinButton freq3_button;
  Gtk::SpinButton time1_button;
  Gtk::SpinButton time2_button;
  Gtk::SpinButton csize_button;
  Gtk::Entry contini_Entry;
  Gtk::Entry calibmol_Entry;
  Gtk::Frame m_Frame_ref;
  Gtk::Label m_Label_trap1;
  Gtk::Label m_Label_trap2;
  Gtk::Label m_Label_wall1;
  Gtk::Label m_Label_wall2;
  Gtk::Label m_Label_minus;
  Gtk::Label m_Label_cont;
  Gtk::Label m_Label_charge;
  Gtk::Label m_Label_contini;
  Gtk::Label m_Label_contnum;
  Gtk::Label m_Label_calib;
  Gtk::Label m_Label_cluster1;
  Gtk::Label m_Label_cluster2;
  Gtk::RadioButton trap_RadioButton1, trap_RadioButton2;
  Gtk::CheckButton stable_Button;
  Gtk::CheckButton file1_Button;
  Gtk::CheckButton file2_Button;
  Gtk::HSeparator m_Separator;
  Gtk::HSeparator m_Separator2,m_Separator3,m_Separator4;
  Gtk::VSeparator v_Separator1, v_Separator2;
  Gtk::Button save_button;
  Gtk::Button crap_button;
  Gtk::Button hco_button;
  Gtk::Button clear_button;
  Gtk::Button attack_button;
  Gtk::Button cluster_button;
  Gtk::Button aclear_button;
  Gtk::Button stop_button;
  Gtk::Button m_Button_Close;
  Gtk::ScrolledWindow a_ScrolledWindow;
  Gtk::ScrolledWindow c_ScrolledWindow;
  Gtk::ProgressBar m_ProgressBar;
  Gtk::HPaned m_HPaned;
  
  AttackWindow a_TreeView; 
  CrapWindow c_TreeView; 
  int testi;
  string file1;
  string file2;



};

#endif // GTKMM_STILLFLYING_H

