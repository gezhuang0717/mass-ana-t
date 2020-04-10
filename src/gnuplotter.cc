#include "window1.hh"

bool Window1::create_summary_plot_gnuplot_script(boost::filesystem::path &scriptfile, boost::filesystem::path &png_filename)
{
  std::string s;
  
  string scan_span = asString( (data->getMaximumFrequency() - data->getMinimumFrequency()));
  string scan_center = asString(0.5*(data->getMaximumFrequency() + data->getMinimumFrequency()));
  
  string tof_gate_low = asString((get_bin_min()+ data->get_MCS_recording_offset_in_bins() )*data->getBin_width());
  string tof_gate_high = asString((get_bin_max()+ data->get_MCS_recording_offset_in_bins() )*data->getBin_width());
  
  
  // Output each to string
  string fit_arrows_and_labels, unset_fit_arrows_and_labels;
  if ( is_class_fitted() )
  {
    // Get fitted frequencies
    int i;
    double fit_results[20];
    bool was_it_fitted[20];
    double chisq;
    data->getFitResults(showclass_spinbutton->get_value_as_int(),fit_results,i);

    fit_arrows_and_labels = "ff=" + as_3f(fit_results[0])+"; set arrow 10001 from first ff-center, graph 0 to first ff-center, graph 1 nohead lw 2 front\n";
    fit_arrows_and_labels += "set label 10001 \"f1\\n" + as_3f(fit_results[0]) + "\" at first ff-center, graph 1 offset 0.2,-.5 left front font \",12\"\n";
    
    string fitted_params = data->get_fit_results_as_text(showclass_spinbutton->get_value_as_int(),"\\n");
    fit_arrows_and_labels += "set label 23011 \""+ fitted_params + "\" at screen 0.82, screen 0.60 offset 0.3,-0.1 font \",10\" front\n";
    
    unset_fit_arrows_and_labels = "unset arrow 10001; unset label 10001; unset label 23011; ";
    if (data->is_2nd_peak_in_the_game() )
    {
      fit_arrows_and_labels += "ff=" + as_3f(fit_results[8])+"; set arrow 10002 from first ff-center, graph 0 to first ff-center, graph 1 nohead lw 2 front\n";
      fit_arrows_and_labels += "set label 10002 \"f2\\n" + as_3f(fit_results[8]) + "\" at first ff-center, graph 0 offset 0.2,2.5 left front font \",12\"\n";
      unset_fit_arrows_and_labels += "unset arrow 10002; unset label 10002; ";
    }
    if (data->is_3rd_peak_in_the_game() )
    {
      fit_arrows_and_labels += "ff=" + as_3f(fit_results[10])+"; set arrow 10003 from first ff-center, graph 0 to first ff-center, graph 1 nohead lw 2 front\n";
      fit_arrows_and_labels += "set label 10002 \"f2\\n" + as_3f(fit_results[10]) + "\" at first ff-center, graph 0 offset 0.2,4.5 left front font \",12\"\n";
      unset_fit_arrows_and_labels += "unset arrow 10002; unset label 10003; ";
    }
  }
    unset_fit_arrows_and_labels+="\n";
  
  
  s =  "#set terminal svg enh dynamic mouse size 1200,800\n";
  s =  "set terminal png enh size 1200,800 font \",16\"\n";
//   s += "set out \"most_recent.png\"\n";
  // In case of png filename and windows, we need to add double backslash
  s += "set out \"" + double_backslashify(png_filename.string()) + "\"\n";
  s += "\n";
  s += "center="+scan_center+"\n";
  s += "span="+scan_span+"\n";
  s += "mainplotwidth=0.5 #Fraction of the plot area (width) the TOF, CTS windows will take\n";
  s += "\n";
  s += "tof_gate_low ="+tof_gate_low+"\n";
  s += "tof_gate_high ="+tof_gate_high+"\n";
  s += "\n";
  s += "\n";
  s += "\n";
  s += "\n";
  s += "\n";
  s += "unset key\n";
  s += "\n";
  s += "\n";
  s += "\n";
  s += "set bmargin 0; set lmargin 0; set tmargin 0; set rmargin 0\n";
  s += "\n";
  s += "\n";
  s += "set bar 0.5\n";
  s += "\n";
  s += "set multiplot\n";
  s += "set size mainplotwidth,0.35\n";
  s += "set xtics offset 0,0.6\n";
  s += "set xlabel \"SCAN PARAMETER - \".sprintf(\"%.3f\",center) offset 0,1.2\n";
  s += "if (span < 10 ) set xtics 1;\n";
  s += "if (span < 1 )  set xtics 0.1; set format x \"%.1f\"\n";
  s += "set grid xtics\n";
  s += "set xrange [-span/2.*1.02:span/2 *1.02]\n";
  s += "set yrange [0:*]\n";
  s += "set ylabel \"COUNTS/BUNCH\" offset 3,0\n";
  s += "set ytics offset 1,0\n";
  s += "\n";
  s += "set origin 0.08,0.3 \n";
  s += "plot \'-\' u ($1-center):2:3 w yerr pt 7 lc rgb \"black\"\n";  // COUNTS PLOT
  s += "#counts_vs_scan_parameter_plot_container\n";
//   for ( Datapoint i : *counts_vs_scan_parameter_plot_container.get_data())
  for (Datapoint i: *data->get_average_counts_vs_frequency( showclass_spinbutton->get_value_as_int() ) )
  {
    s += asString(i.x_value) + " " + asString(i.y_value) + " " + asString(i.y_value_err) + "\n";
  }
  s += "e\n";
  s += "\n";
  s += "\n";
  s += "\n";
  s += fit_arrows_and_labels;
  s += "set yrange [tof_gate_low:tof_gate_high]\n";
  s += "set ylabel \"TOF ({/Symbol m}s)\" offset 2.5,0\n";
  s += "set xlabel \"\"\n";
  s += "set format x \"\"\n";
  s += "set origin 0.08,0.65 \n";
  s += "\n";
  s += "unset colorbox\n";
  s += "set tics front\n";
  s += "set grid front\n";
  s += "set palette defined ( 0 \"white\", 1 \"#3366ff\")\n";
  s += "plot \"-\" u ($1-center):2:3 w image, \\\n"; // Counts 2d-contour
  s += "\'-\' u ($1-center):2:3 with yerr pt 7 lc rgb \"black\", \\\n";  // TOF DATA
  s += " \'-\' u ($1-center):2 with lines lc rgb \"red\" lw 2\n";                // TOF FIT
  
  // This is the 2d-counts contour for TOF plot
  std::ostringstream two_dim_cts; data->print_2d_counts(two_dim_cts,showclass_spinbutton->get_value_as_int());
  s += two_dim_cts.str();
  s += "e\n";
  s += "#tof_vs_scan_parameter_plot_container\n";
  for ( Datapoint i : *tof_vs_scan_parameter_plot_container.get_data())
  {
    s += asString(i.x_value) + " " + asString(i.y_value) + " " + asString(i.y_value_err) + "\n";
  }
  s += "e\n";
  s += "#tof_vs_scan_parameter_fit_plot_container\n";
  for ( Datapoint i : *tof_vs_scan_parameter_fit_plot_container.get_data())
  {
    s += asString(i.x_value) + " " + asString(i.y_value) + " " + asString(i.y_value_err) + "\n";
  }
  s += "e\n";
  
  s += unset_fit_arrows_and_labels;
  // HISTORY
  s += "set grid back\n";
  s += "set tics back\n";

  s += "\n";
  s += "\n";
  s += "set xtics ("; // Starting making xtics
  int j (1);
  int harvennus = round_change_points.size() / 30 + 1;
  for ( int i : round_change_points)
  {
    s += "\"";
    if ( j % harvennus == 0 || j == 1) // Do not make markers to every round if there are more than 30 
      s += asString(j);
    s += "\" ";
    s += asString(i);
    s += ",";
    j++;
  }
  s.pop_back();
  s += ") font \",12\" offset 0.,0.7 left\n"; // Finished setting xtics
  s += "set grid xtics lc rgb \"grey\" lt 1\n";
  s += "set xrange [0:*]\n";
  s += "set yrange [0:*]\n";
  s += "set ylabel \"#/5 BUNCHES\"\n";
  s += "set label 1 \"ROUND #\" at graph 1,0 offset 0,-0.9 right font \",12\"\n";
  s += "set origin 0.08,0.03\n";
  s += "set size 0.91,0.2\n";
  s += "plot \'-\' u 1:2 with histeps lc rgb \"black\", \\\n";  // All history
  s += " \'-\' u 1:2 with histeps lc rgb \"red\" lw 1\n";                // History within gates
  s += "#history_histogram_plot_container\n";
  for ( Datapoint i : *history_histogram_plot_container.get_data())
  {
    s += asString(i.x_value) + " " + asString(i.y_value) + " " + asString(i.y_value_err) + "\n";
  }
  s += "e\n";
  s += "#history_histogram_with_gates_plot_container\n";
  for ( Datapoint i : *history_histogram_with_gates_plot_container.get_data())
  {
    s += asString(i.x_value) + " " + asString(i.y_value) + " " + asString(i.y_value_err) + "\n";
  }
  s += "e\n";
  s += "\n"; 
  s += "\n";
  
  // TOF HISTOGRAM
  s += "set arrow 11 from first " + asString(get_bin_min()) + ", graph 0 to first " + asString(get_bin_min()) + ", graph 1 nohead lc rgb \"blue\"\n";
  s += "set arrow 12 from first " + asString(get_bin_max()) + ", graph 0 to first " + asString(get_bin_max()) + ", graph 1 nohead lc rgb \"blue\"\n";
  s += "set label 11 \"" + asString(get_bin_min()) + "-" + asString(get_bin_max()) + "\" at graph 1, graph 1 offset -1,-1 right\n";
  s += "set format x \"%.0f\"\n";
  s += "set size 0.34,0.35\n";
  s += "set origin 0.65,0.65\n";
  s += "set ylabel \"COUNTS\"\n";
  s += "set label 1 \"TOF (bins)\" at graph 1,0 offset 0,-0.9 right font \",12\"\n";
  s += "set xtics auto\n";
  s += "plot \'-\' u 1:2 with histeps lc rgb \"black\", \\\n";  // All TOF histogram
  s += " \'-\' u 1:2 with histeps lc rgb \"red\" lw 1\n";                // TOG histogram of this class
  s += "#tof_histogram_plot_container\n";
  for ( Datapoint i : *tof_histogram_plot_container.get_data())
  {
    s += asString(i.x_value) + " " + asString(i.y_value) + " " + asString(i.y_value_err) + "\n";
  }
  s += "e\n";
  s += "#tof_histogram_of_class_container\n";
  for ( Datapoint i : *tof_histogram_of_class_container.get_data())
  {
    s += asString(i.x_value) + " " + asString(i.y_value) + " " + asString(i.y_value_err) + "\n";
  }
  s += "e\n";
  // CTS HISTOGRAM
  s += "set arrow 11 from first " + asString(get_cts_min()) + ", graph 0 to first " + asString(get_cts_min()) + ", graph 1 nohead\n";
  s += "set arrow 12 from first " + asString(get_cts_max()) + ", graph 0 to first " + asString(get_cts_max()) + ", graph 1 nohead\n";
  s += "set label 11 \"" + asString(get_cts_min()) + "-" + asString(get_cts_max()) + "\" at graph 1, graph 1 offset -1,-1 right\n";
  s += "set size 0.17,0.35\n";
  s += "set origin 0.65,0.27\n";
  s += "\n";
  s += "set ylabel \"BUNCHES\"\n";
  s += "set label 1 \"IONS/BUNCH\" at graph 1,0 offset 0,-0.9 right font \",12\"\n";
  s += "set xtics auto\n";
  s += "plot \'-\' u 1:2 with histeps lc rgb \"black\", \\\n";  // CTS histogram
  s += "#event_histogram_data\n";
  for ( Datapoint i : *event_histogram_plot_container.get_data())
  {
    s += asString(i.x_value) + " " + asString(i.y_value) + " " + asString(i.y_value_err) + "\n";
  }
  s += "e\n";
  s += "\n";
  s += "\n";
  s += "\n";
  s += "\n";
  s += "\n";
  s += "\n";
  s += "\n";
  s += "\n";
  s += "unset multiplot\n";
  s += "set out\n";
  
  boost::filesystem::ofstream ofs;
  open_file_for_writing(scriptfile,ofs);
  
  if (!ofs)
    return 1;
  
  ofs << s ;
  ofs.close();
  return 0;
//   int res = system(string("gnuplot test.blaa").c_str());
}


bool Window1::create_summary_blaa_blaa_text(boost::filesystem::path &text_file)
{
  std::string s = data->get_fit_results_as_text(showclass_spinbutton->get_value_as_int(),"\n");
  
  std::string blaa_by_user = elog_textview->get_buffer()->get_text();
  
  boost::filesystem::ofstream outfile;
  
  if (!open_file_for_overwriting_and_popup_message_if_error(text_file.string(),outfile,true))
  {
    return 1;
  }
  
  outfile << blaa_by_user << endl << endl;
  outfile << "_________________\n";
  outfile << s << endl;  
  
  outfile.close();
  
  return 0;
}

bool Window1::execute_gnuplot_script(boost::filesystem::path &scriptfile)
{
  
  int result;
  
  string cmdd = (string("\"\""+double_backslashify(gnuplot_executable_entry->get_text()) + "\" \"" + scriptfile.string()+"\"\""));
  cout << "Gnuplot command: " << cmdd << endl;
  result = system(cmdd.c_str());
  
  if (result != 0)
    return 1;
  else
    return 0;

}
// elog -h qad.igisol -p 8020 -l IGISOL2017 -a Author=Tommi -a What="JYFLTRAP Scan" -a "Experiment Number"=20170105_fission -f most_recent.png "blaarfgh"
