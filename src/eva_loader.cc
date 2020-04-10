
/* Jochen Ketter, 16. Mai 2008
 * Versuch, eine dat-Datei des MM6 auszulesen.
 *
 */

#include <iostream>
//#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>
//#include <vector>

#include <cmath> //Äquivalenter Aufrufz zu math.h?
//#include <math.h>

#include "collection.hh"

#include "eva_loader.hh"

using namespace std;

cEvaFileCollection::cEvaFileCollection()
{

}
cEvaFileCollection::~cEvaFileCollection()
{

}



cEvaFile::cEvaFile()
{
	scan_rounds = 0;

	extra_info_double_1 = 0;
	extra_info_double_2 = 0;
}
cEvaFile::~cEvaFile()
{

}

cIonDetails::cIonDetails()
{

}

cIonDetails::~cIonDetails()
{

}

bool cIonDetails::operator==(const cIonDetails& rhs)
{
	if (mass_number == rhs.mass_number &&
		element == rhs.element &&
		charge_state == rhs.charge_state)
		return true;

	return false;
}




cMCSDetails::cMCSDetails()
{
	bin_width = 0;
	MAX_MCS_channels = 0;
	MCS_recording_offset_in_bins = 0;
}
cMCSDetails::~cMCSDetails()
{

}

bool cEvaFileCollection::add_evafile(const std::string& filename)
{
	if ( !boost::filesystem::exists( filename) )
		return 0;

	evafiles.push_back(cEvaFile());
	evafiles.back().process_evafile(filename);
	//std::cout << "back charge:" << evafiles.back().charge_state << " element:" << evafiles.back().element << " time:" << evafiles.back().time_of_sweep << "|\n";
	evafiles.sort();
	return 1;
}

cIonDetails cEvaFile::get_ion_details()
{
	//std::cout << "directly! charge:" << charge_state << " element:" << element << "|\n";
	cIonDetails p = *this;
	//std::cout << "via get ! charge:" << p.charge_state << " element:" << p.element << "|\n";
	return p;

}


list< cSimilarScans > cEvaFileCollection::get_ion_species_types(void )
{
	std::list< cSimilarScans > founds;
	for (std::list<cEvaFile>::iterator i = evafiles.begin(); i != evafiles.end(); ++i)
	{
		bool existing = false;
		for (std::list <cSimilarScans>::iterator j = founds.begin(); j != founds.end(); ++j)
		{
			if ( i->operator==(*j)  )
			{
				existing = true;
				j->number_of_similar_scans++;
				j->similar_evafiles.push_back( &(*i) );
			}

		}
		if (existing == false)
		{
			founds.push_back( i->get_ion_details());
			founds.back().similar_evafiles.push_back(&(*i) );
		}

	}
	return founds;
}

cSimilarScans::cSimilarScans(const cIonDetails& ion_details)
{
	cIonDetails::operator=(ion_details);
	number_of_similar_scans = 1;
}



std::string cEvaFile::smart_parse(const std::string& line, const std::string& identifier_str, const std::string& valid_ending_characters)
/* Parses a line such as | Mass=Gd152 ,Charge= 1,Freq=  708035.64,Amp= 0.420000,Time=0.500000|
 * std::string identifier_str is like |Mass=| and valid_ending_characters like " ," which accepts
 * comma and white space ' ' as ending characters. Method returns the string in between.
 *
 * Correcteed 2012-10-24: Element can also be in order "152Gd" or "Gd152". Apparently
 * ISOLTRAP and SHIPTRAP conventions disagree.
 */
{
	// liner consists of everything after the end of identifier_str:
	std::string liner = line.substr(line.find(identifier_str)+identifier_str.size());
	// Removing any leading whitespace
	liner = liner.substr(liner.find_first_not_of(" "));

	liner = liner.substr(0,liner.find_first_of(valid_ending_characters));
	return liner;

}


bool cEvaFile::process_evafile(const std::string& filename)
{
	ifstream myinfile; //Eingabedateistream
	std::string dateiname_in = filename, zeile, einleser;
	long header_size = 0, data_start = 0, pps = 0, pps_last = 0;
	short nrMCA_channels = 0, nrScans = 0, recSize = 0, recSize_max = 0;
	double *scan_vals, time_per_channel;
//	int laenge = sizeof(long);
	//char *buffer;
	int i, j, k, pos;
	unsigned int ui;
	bool dataCompressed = false;

	myinfile.open(dateiname_in.c_str(), ios::in | ios::binary);

	if (!myinfile.is_open()) {
		cerr << "Fehler beim Oeffnen der Datei " << dateiname_in << "." << endl;
		cerr << endl;
		myinfile.clear();
		cerr << "Programmende!" << endl;
		return 0;
	}

	//myinfile.seekg (0, ios::beg);
	//myinfile.read((char*) &header_size, sizeof(long));
	//myinfile.read((char*) &data_start, sizeof(long));
	myinfile.read((char*) &header_size, 4);
	myinfile.read((char*) &data_start, 4);
	//cout.write(buffer, laenge);

	/* Test, ob Zeichen ein Kontrollzeichen ist.
	 for (i=0; i<laenge; i++){
	 cout << i << ": " << iscntrl(buffer[i]) << endl;
	 } */

	//cout << "HeaderSize: " << header_size << endl;
	//cout << "DataStart: " << data_start << endl;

	myinfile.close();

	/*
	 * Lesen aus dem Klartextteil:
	 * Suchen des Labels [MCA] und Einlesen der Kanalbreite in µs sowie der
	 * Anzahl der verwendeten Kanäle
	 */
	myinfile.open(dateiname_in.c_str(), ios::in);

	if (!myinfile.is_open()) {
		cerr << "Fehler beim Oeffnen der Datei " << dateiname_in << "." << endl;
		cerr << endl;
		myinfile.clear();
		cerr << "Programmende!" << endl;
		return 0;
	}

	//myinfile.seekg(2*sizeof(long), ios_base::beg);
	myinfile.seekg(2*4, ios_base::beg);

	//Klartextteil der Datei durchgehen.
	//Binärpart beginnt nach header_size + 2 * sizeof(long), wobei
	//die HeaderSize und DataStart zu Beginn der Datei als long-Werte dazugezählt werden.

	//while (myinfile.tellg() < header_size + 2 * sizeof(long)) {
	while (myinfile.tellg() < header_size + 2 * 4) {
		getline(myinfile, zeile);
		//cout << zeile << endl;
		//Debuggingcode zum Test, ob der richtige Teil der Datei gelesen wird.

		//cout << zeile.find("[MCA]") << endl;
		//cout << "string::npos " << string::npos << endl;

		// Eintrag finden und die beiden interessanten Werte für
		// die Binbreite (Zeit pro Kanal) und die Anzahl der MCA-Kanaäle finden.
		if (zeile == "[MCA]"){
		//if (zeile.find("[MCA]") != string::npos) {
			//Versuch, die Abfrage etwas robuster zu gestalten. Allerdings ist der
			//Klartextteil sakrosankt, da eine Änderung sofort die HeaderSize ändert.

			// cout << "[MCA] gefunden." << endl;
			getline(myinfile, zeile);
			pos = zeile.find("TimePerChannel");
			ui = pos + 14; //Position + Länge von "TimePerChannel"

			// Ende bei Komma oder Ende der Zeile
			while (zeile[ui] != ',' && ui < zeile.length()) {
				//Alles, was eine Zahl oder ein Dezimaltrenner ist, wird an den
				//String einleser angehängt.
				if (isdigit(zeile[ui]) || zeile[ui] == '.') {
					einleser.push_back(zeile[ui]);
				}
				ui++;
			}
			//Umwandeln des Strings in eine Zahl
			stringstream(einleser) >> time_per_channel;
			bin_width = time_per_channel; //TADAA
			// cout << "Einleser: " << einleser << endl;
			// cout << "Zeit pro MCA-Kanal in µs: " << time_per_channel << "|\n";

			einleser.clear();

			//Die gleiche Prozedur wie oben zur Bestimmung der MCA-Kanäle.
			//Vereinfachung: Da die Anzahl der Kanäle eine ganze Zahl ist,
			// kan auf die Beandlung eines Dezimaltrenners verzichtet werden.
			pos = zeile.find("Channels");
			ui = pos + 8; //Position
			while (zeile[ui] != ',' && ui < zeile.length()) {
					if (isdigit(zeile[ui])) {
						einleser.push_back(zeile[ui]);
					}
				ui++;
			}
			stringstream(einleser) >> nrMCA_channels;
			MAX_MCS_channels = nrMCA_channels; //TADAA
			// cout << "Einleser: " << einleser << endl;
			// cout << "Anzahl der MCA-Kanäle: " << nrMCA_channels << endl;

			//break;
		}
		if (zeile == "[Mass]")
		{
			// cout << "[Mass] gefunden." << endl;
			getline(myinfile, zeile);
			std::string isotope = smart_parse(zeile,"Mass=");
			//std::cout << "tooppi: |" << isotope << "|\n";
			std::string charge = smart_parse(zeile,"Charge=");
			//std::cout << "charge: |" << charge << "|\n";

			stringstream(charge) >> charge_state;

			//std::cout << "charge: " << charge_state << "\n";

			/* this stage, isotope = |Gd152| and charge = 1 (integer) */
			/* Could also be other way around: |152Gd| as at ISOLTRAP 2012-10-23 */
			/* We will split isotope to string::element |Gd| (make it all low cases)
			 * and int mass_numeber */
			// isotope[0] = tolower(isotope[0]);
			//std::cout << "isotope |" << isotope << "|\n";
			////////////
			int first_number = isotope.find_first_of("0123456789");
			int last_number = isotope.find_last_of("0123456789");
			int first_letter = isotope.find_first_of("abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ");
			int last_letter = isotope.find_last_of("abcdefghijklmnopqrstuvxyzABCDEFGHIJKLMNOPQRSTUVXYZ");

			element = isotope.substr(first_letter,last_letter-first_letter+1);
			element[0] = tolower(element[0]);
			std::string mass_number_str = isotope.substr(first_number,last_number-first_number+1);
			stringstream(mass_number_str) >> mass_number;
			////////////
			//OLD
			//int position = isotope.find_first_of("01234567890");
			//element = isotope.substr(0,position);
			//std::string mass = isotope.substr(position);
			//std::cout << "element: |" << element << "|mass:|" << mass << "|\n";
			//stringstream(mass) >> mass_number;
		}
		//Finding "Delay MCS"
		{
			std::string identifier_str = "Delay MCS : ";
			if (zeile.find(identifier_str) != std::string::npos )
			{
				// value should contain "40.000"
				std::string value = zeile.substr(zeile.find(identifier_str) + identifier_str.size() );
				MCS_recording_offset_in_bins = as_double(value)/bin_width;
			}
		}

		/// Custom for one particular data, should comment this out after use
		if (1)
		{
			std::string identifier_str = "Cyclotron MT : ";
			if (zeile.find(identifier_str) != std::string::npos )
			{
				// value should contain "20000.00"
				std::string value = zeile.substr(zeile.find(identifier_str) + identifier_str.size() );
				extra_info_string_1 = value;
				extra_info_double_1 = as_double(value);
				std::cout << "Extra info:|" << extra_info_double_1 << "|\n";
			}
		}
	}
	myinfile.close();


	/*
	 * Öffnen der Datei um den binären Datenteil einzulesen.
	 * Die Anzahl der Scnas wird von diesem Programmteil ermittelt.
	 * Reihenfolge
	 * Punkte pro Scan
	 * Frequenzwerte der einzelnen Scanpunkte
	 * Daten der MCA-Kanäle
	 */
	myinfile.open(dateiname_in.c_str(), ios::in | ios::binary);

	if (!myinfile.is_open()) {
		cout << "Fehler beim Oeffnen der Datei " << dateiname_in << "." << endl;
		cout << endl;
		myinfile.clear();
		cout << "Programmende!" << endl;
		return 0;
	}

	//myinfile.seekg (0, ios::beg);
	//Angabe einer Anfangsposition wohl unnötig, da die untere Funktion ohnehin
	//vom Dateianfang sucht.

	//myinfile.seekg(header_size + 2 * sizeof(long), ios_base::beg);
	myinfile.seekg(header_size + 2 * 4, ios_base::beg);

	//myinfile.read((char*) &pps, sizeof(long));
	myinfile.read((char*) &pps, 4);

	// cout << "Points per Scan: " << pps << endl;

	scan_vals = new double [pps];

	myinfile.read( (char*) scan_vals, pps*sizeof(double));

	for (i=0; i<pps; i++) {
		//cout << "blubber" << endl;
		//cout << "Hier bin ich." << endl;
		cout.precision(4);
		cout.setf(ios_base::floatfield, ios_base::fixed);
		//scan_vals[i] are scanned frequencies (doubles)
		/* cout << (i+1) << ": "  << scan_vals[i] << endl; */
		//cout << (i+1) << ": " << fixed << setprecision(3) << scan_vals[i] << endl;
	}

	cout.unsetf(ios_base::fixed);
	//cout << 1.12 << endl;

	myinfile.seekg(data_start, ios_base::beg);
	//cout << pps << endl; return 0;
	//cout << "Failbit " << myinfile.fail() << endl;

	j = 0;
	//*
	do {
		i = 0;
		while (i < pps) {
			myinfile.read( (char*) &recSize, sizeof(short));
			if (!myinfile.eof()) {
				i++;
				j++;
			} else {
				break;
			}
			if (recSize > recSize_max) {
				recSize_max = recSize;
			}
		//	cout << "RecSize: " << recSize << endl;
			myinfile.seekg(recSize, ios_base::cur);

			//cout << "Hier bin ich." << endl;
			//cout << "i: " <<  i << endl;
		}
		nrScans++;

	} while (!myinfile.eof());

	myinfile.clear();
	//Status des Eingabestreams der Datei zurücksetzen, da eine eof-Exception produziert wurde.
	//Andernfalls lassen sich keine vernünftigen Operationen mehr durchführen (zwei Stunden Fehlersuche).

	//	*/
	//cout << "Gesamtzahl der Scanpunkte: " << j <<endl;
	nrScans--;
	//Vorteil dieser Konstruktion: Es paßt auch es, wenn gar kein Scan
	//durchgeführt wurde. War der letzte Scan vollständig, wird die Schleife ein weiteres
	//Mal durchlaufen, wobei sich die Anzahl der Scnas um eins erhöht und i auf null
	//gesetzt wird. Beides wird hier korrigiert.

	pps_last = (i == 0 && nrScans > 0) ? pps : i; //Punkte im letzten Scan (eventuell vorzeitiger Abbruch)
	//Wenn mindestens ein Scan durchgeführt wurde, ist i == 0, weil nach einem vollständigen Scan
	//beendet wurde.

	// std::cout << "Anzahl der Scans: " << nrScans << endl; // JYFLTRAP slang: "rounds"
	scan_rounds = nrScans;
	// std::cout << "Scanpunkte im letzten Scan: " << pps_last << endl;
	// std::cout << "pps: " << pps << endl;
	// std::cout << "nrMCA_channels: " << nrMCA_channels << "\n";
	//short AllCounts[nrScans][pps][nrMCA_channels];

	std::vector< std::vector< std::vector<short> > > AllCounts;
	AllCounts.resize(nrScans);

	// std::cout << "here?\n";
	//time_t aTimes[nrScans][pps];
	std::vector< std::vector<time_t> > aTimes;
	aTimes.resize(nrScans);

	// std::cout << "here2?\n";
	for (i = 0; i < nrScans; i++) {
		AllCounts[i].resize(pps);
		aTimes[i].resize(pps);
		for (j = 0; j < pps; j++) {
			AllCounts[i][j].resize(nrMCA_channels);
			aTimes[i][j] = -1;

			for (k = 0; k < nrMCA_channels; k++) {
				AllCounts[i][j][k] = -1;
			}
		}
	}
	// std::cout << "here3?\n";
	short *buffer;
	buffer = new short [recSize_max];

	myinfile.seekg(data_start, ios_base::beg);
	//long testpos = data_start;

	for (i = 0; i < nrScans; i++) {

		for (j= 0; j < pps; j++) {

			// cout << myinfile.tellg() << endl;
			myinfile.read( (char*) &recSize, sizeof(short));
			//cout << myinfile.tellg() << endl;
			// cout << myinfile.gcount() << endl;
			//testpos = testpos + sizeof(short) + recSize;

			//cout << "recSize davor: " << recSize << endl;

			//recSize = (short) ((recSize - sizeof(long)) / sizeof(short));
			recSize = (short) ((recSize - 4) / sizeof(short));
			//cout << "recSize danach: " << recSize << endl;


			//myinfile.read((char*) &aTimes[i][j], sizeof(long));
			unsigned int buffer2;
			myinfile.read((char*) &buffer2, 4);
			aTimes[i][j] = buffer2;
			//cout << myinfile.gcount() << endl;
			// cout << "aTimes[" << i << "][" << j << "]: " << aTimes[i][j] << endl;

			if (recSize > 0) {
				myinfile.read((char*) buffer, recSize * sizeof(short));

				//for (k = 0; k < recSize; k++){	cout << buffer[k] << endl;	}

				dataCompressed = recSize < nrMCA_channels;

				if (dataCompressed) {
					for (k = 0; k < recSize/2; k++) {
						AllCounts[i][j][buffer[2*k]] = buffer[2*k+1];
					}
				} else {
					for (k = 0; k < recSize; k++) {
						AllCounts[i][j][k] = buffer[k];
					}
				}
			}
		}

		//myinfile.seekg(testpos, ios_base::beg);

	}

	myinfile.close();
	// std::cout << "Finished reading file.\n";

	for (i = 0; i < nrScans; i++) {
		for (j = 0; j < pps; j++) {
			//cout << aTimes[i][j] << endl;

			//cout << ctime(&aTimes[i][j]) << endl;
		}
	}

	for (i = 0; i < nrScans; i++) {  // i: round number
		for (j = 0; j < pps; j++) {  // j: frequency number, 0=smallest freq, pps-1 highest

			cUnit to_add;

			to_add.putFrequency(scan_vals[j]);
			//cout.precision(15);
			//cout << "freq: " << scan_vals[i] << "\n";

			loc_tm time(aTimes[i][j]);
			to_add.putTimeString(time.get_time_as_string());
			// cout << "time: " << to_add.getTimeString();



			for (k = 0; k < nrMCA_channels; k++) {  // k: MCS channel
				if (AllCounts[i][j][k] != -1) {
					to_add.InsertDataPoint(k,AllCounts[i][j][k],0);

					//cout << "Datapoint: ch:" << k << "| cts:" << AllCounts[i][j][k] << "|\n";
					//cout << "Scan " << (i+1) << ", Punkt " << (j+1)
					//		<< ", Kanal " << (k+1) << ": "
					//		<< AllCounts[i][j][k] << endl;
				}
			}
			to_add.set_MCS_TOF_offset_in_bins(MCS_recording_offset_in_bins);

			bunches.push_back(to_add);
		}
	}
	if (nrScans > 0 && pps > 0)
	{
		// long long scantime = aTimes[0][0]/2. + aTimes[nrScans-1][pps-1]/2.;
		// time_of_sweep = loc_tm(scantime).get_time_as_string();

		start_time_of_sweep = loc_tm(aTimes[0][0]).get_time_as_string();
		end_time_of_sweep = loc_tm(aTimes[nrScans-1][pps-1]).get_time_as_string();

		// std::cout << "Start:" << loc_tm(aTimes[0][0]).get_time_as_string() << "---stop:" << loc_tm(aTimes[nrScans-1][pps-1]).get_time_as_string() << "\n";
		// std::cout << "middle:" << time_of_sweep << "\n";
	}
	// cout << "Hier bin ich am Ende angelangt." << endl;




	delete [] scan_vals;
	delete [] buffer;
	return 0;
}

choise::choise()
{
	radiobutton = new Gtk::RadioButton;

}

// push_back method is using this so this is why this is so well implemented
choise::choise(const choise& other)
{
	radiobutton = new Gtk::RadioButton;
	//std::cout << "copy constructor actually used!\n";

	ss = other.ss;

	radiobutton->set_label(other.radiobutton->get_label());
	Gtk::RadioButtonGroup grpp = other.radiobutton->get_group();
	radiobutton->set_group(grpp);
	//std::cout << "copycons ele:" << ss->element << "|\n";
}

choise::~choise()
{
	delete radiobutton;
}

choise::choise(cSimilarScans* p, Gtk::RadioButtonGroup grp)
{
	ss = p;
	radiobutton = new Gtk::RadioButton;
	std::string leibel = p->element + asString(p->mass_number) + ", " + asString(p->charge_state) + "+, " + asString(p->number_of_similar_scans) + " files";
	radiobutton->set_label(leibel);
	radiobutton->set_group(grp);

}

void choise::operator=(const choise& other)
{
	//std::cout << "assing actually used!\n";
}


