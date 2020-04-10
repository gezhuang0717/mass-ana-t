#include <gtkmm/main.h>
#include "stillflying.h"

int main (int argc, char *argv[])
{
  Gtk::Main kit(argc, argv);
  setlocale(LC_NUMERIC,"C");

  MainWindow mainwindow;
  mainwindow.set_default_size(700,500);
  Gtk::Main::run(mainwindow); //Shows the window and returns when it is closed.
  return 0;
}

