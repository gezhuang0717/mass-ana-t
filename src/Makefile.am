GIT_VERSION=@GIT_DESCRIBE@
COMPILE_DATE=$(shell date +%F)

LAKRITSI_XML_IDS = \
	fit_parameters \
	inputting \
	ok_cancel_dialog_2 \
	postProcessor \
	ramseyer \
	tickerer \
	window1

BUILT_SOURCES = $(addprefix $(top_srcdir)/src/, $(addsuffix _headers.inc,$(LAKRITSI_XML_IDS)))
BUILT_SOURCES += $(addprefix $(top_srcdir)/src/, $(addsuffix _headers_cpp.inc,$(LAKRITSI_XML_IDS)))
BUILT_SOURCES += $(addprefix $(top_srcdir)/src/, $(addsuffix _members.inc,$(LAKRITSI_XML_IDS)))
BUILT_SOURCES += $(addprefix $(top_srcdir)/src/, $(addsuffix _init.inc,$(LAKRITSI_XML_IDS)))

GTKBUILDER_HELPER=python $(top_srcdir)/util/gtkbuilder-helper.py --output-suffix=./
lakritsi-xml-stamp: $(top_srcdir)/lakritsi.xml
	$(GTKBUILDER_HELPER) --subclassed=fit_parameters:Fit_Parameters,inputting:Inputting,tickerer:Tickerer --include=hbox33\
		$(top_srcdir)/lakritsi.xml window1 Window1
	$(GTKBUILDER_HELPER) $(top_srcdir)/lakritsi.xml postProcessor Post_Processor
	$(GTKBUILDER_HELPER) $(top_srcdir)/lakritsi.xml ramseyer Ramseyer
	$(GTKBUILDER_HELPER) --include=calib_vbox \
		$(top_srcdir)/lakritsi.xml tickerer Tickerer
	$(GTKBUILDER_HELPER) $(top_srcdir)/lakritsi.xml fit_parameters Fit_Parameters
	$(GTKBUILDER_HELPER) $(top_srcdir)/lakritsi.xml inputting Inputting
	$(GTKBUILDER_HELPER) --exclude=dialog_vbox6,okbutton2,dialog_action_area6,cancelbutton1 \
		$(top_srcdir)/lakritsi.xml ok_cancel_dialog_2 ok_cancel_dialog_2
	touch $@

$(BUILT_SOURCES): lakritsi-xml-stamp

EXTRA_DIST = gtkbuilder-gen.sh \
	fields_ISOLTRAP.txt \
	fields_JYFLTRAP.txt \
	fields.txt \
	ref_masses.txt \
	hajorajat.txt \
	nubtab03.asc \
	nubase.mas12 \
	lakritsi.ini \
	ramseyer.ini

noinst_HEADERS = \
	alarm_cb.hh \
	ame_stuff.hh \
	collection.hh \
	compat.h \
	Consumer_Task.h \
	curl_stuff.hh \
	datap.hh \
	datastructures.hh \
	eva_loader.hh \
	Event_Data.h \
	filechooserdialog1.hh \
	fit_parameters.hh \
	fitting1.hh \
	guiutil.hh \
	inputting.hh \
	JYFLTRAP_Consumer.h \
	loctm.hh \
	logoes.h \
	Mass_Scanning_Consumer.h \
	mass_scan_settings.hh \
	mjonot.hh \
	mjonotpp.hh \
	nrutil.hh \
	ok_cancel_dialog_2.hh \
	online_histogrammer.hh \
	ORB_Task.h \
	postProcessor.hh \
	ramseyer.hh \
	tickerer.hh \
	unit.hh \
	util.hh \
	window1.hh \
	window2.hh \
	plotstructures.hh

lakritsidir = $(pkgdatadir)
lakritsi_DATA = \
	fields_ISOLTRAP.txt \
	fields_JYFLTRAP.txt \
	fields.txt \
	ref_masses.txt \
	hajorajat.txt \
	nubtab03.asc \
	nubase.mas12 \
	lakritsi.ini \
	ramseyer.ini
#	nu_c_calibration_trap1.txt\
#	nu_c_calibration_trap2.txt\
#	nu_+_calibration_trap2.txt\
#	tof_rfq_to_trap1_calibration.txt\
#	tof_trap1_to_trap2_calibration.txt


if WINDOWS
 ACE_ROOT =/home/tomero/Projects/corbatshow/win32/ACE_wrappers
 LAKRITSI_DATA_DIR = "c:/Program Files/Lakritsi"
else
 ACE_ROOT = /home/tomero/Projects/corbatshow/ACE_wrappers
 LAKRITSI_DATA_DIR = @PACKAGE_DATA_DIR@/lakritsi
endif

AM_CFLAGS =
AM_CPPFLAGS = \
	-DLAKRITSI_DATA_DIR=\"$(LAKRITSI_DATA_DIR)\" \
	$(LIBCURL_CFLAGS) \
	$(OPENSSL_CFLAGS) \
	$(GTKMM_CFLAGS) \
	@BOOST_CPPFLAGS@ \
	-DGIT_VERSION=\"$(GIT_VERSION)\" \
	-DCOMPILE_DATE=\"$(COMPILE_DATE)\"

INCLUDES = -I$(top_srcdir)/extras -I$(top_srcdir)/extras/json_spirit/include

if WINDOWS
AM_CFLAGS += -mthreads 
if ONLINE
AM_CFLAGS += -D_MT -D_REENTRANT -DACE_HAS_AIO_CALLS
endif
endif

AM_CXXFLAGS = -std=c++11 -U__STRICT_ANSI__

lakritsi_LDFLAGS =
if WINDOWS
AM_CXXFLAGS += -DUNICODE -D_UNICODE

# Without terminal window (this is option -mwindows)
lakritsi_LDFLAGS += -mwindows
## with a terminal window
#lakritsi_LDFLAGS += -mconsole
endif

lakritsi_LDFLAGS += @BOOST_SYSTEM_LDFLAGS@ @BOOST_DATE_TIME_LDFLAGS@ @BOOST_FILESYSTEM_LDFLAGS@
lakritsi_LDADD = \
	$(LIBCURL_LIBS) \
	$(GTKMM_LIBS) \
	$(OPENSSL_LIBS)

# lakritsi_LDADD += ../extras/json_spirit/src/lib_json_spirit.la
lakritsi_LDADD += @BOOST_SYSTEM_LIBS@ @BOOST_DATE_TIME_LIBS@ @BOOST_FILESYSTEM_LIBS@

if ONLINE
 lakritsi_LDADD += $(GLIBSTUFF_LIBS) $(TAO_LIBS)
 AM_CFLAGS += $(GLIBSTUFF_CFLAGS) $(TAO_CFLAGS)

 IDLS = ../idl/JYFLTRAP.idl
 CORBA_SRC = $(IDLS:.idl=C.cpp)
 CORBA_HDR = $(IDLS:.idl=C.h) $(IDLS:.idl=C.inl)

 BUILT_SOURCES += $(CORBA_SRC) $(CORBA_HDR)
 noinst_HEADERS += JYFLTRAPC.h
endif

bin_PROGRAMS = lakritsi coma ramseyer crap

base_SOURCES = \
	ame_stuff.cc \
	loctm.cc \
	util.cc \
	guiutil.cc \
	window2.cc \
	window2_extra.cc \
	plotstructures.cc \
	lakritsi_xml.c

lakritsi_SOURCES = $(base_SOURCES) \
	ok_cancel_dialog_2.cc \
	mjonot.c \
	mjonotpp.cc \
	collection.cc \
	nrutil.cc \
	unit.cc \
	postProcessor.cc \
	fit_parameters.cc \
	mass_scan_settings.cc \
	ramseyer.cc \
	tickerer.cc \
	curl_stuff.cc \
	eva_loader.cc \
	fitting1.cc \
	inputting.cc \
	loading_and_saving_settings.cc \
	gnuplotter.cc \
	signal_to_noise.cc \
	window1.cc \
	lakritsi.cc

if ONLINE
lakritsi_SOURCES += \
 JYFLTRAP_Consumer.cpp\
 Mass_Scanning_Consumer.cpp\
 JYFLTRAPC.cpp\
 ORB_Task.cpp\
 Event_Data.cpp\
 alarm_cb.cc

base_SOURCES += online_histogrammer.cc
endif

coma_SOURCES = $(base_SOURCES) \
	postProcessor.cc \
	coma.cc

coma_LDADD = $(lakritsi_LDADD)
coma_LDFLAGS = $(lakritsi_LDFLAGS)

ramseyer_SOURCES = $(base_SOURCES) \
	fitting1.cc \
	nrutil.cc \
	ramseyer.cc \
	ramsey_cleaner.cc

ramseyer_LDADD = $(lakritsi_LDADD)
ramseyer_LDFLAGS = $(lakritsi_LDFLAGS)

crap_SOURCES = \
	classes.cpp \
	stillflying.cpp \
	crap.cpp \
	util.cc

crap_LDADD = $(lakritsi_LDADD)
crap_LDFLAGS = $(lakritsi_LDFLAGS)

CLEANFILES = lakritsi_xml.c lakritsi-xml-stamp

lakritsi-xml: lakritsi-xml-stamp

lakritsi_xml.c: ${top_srcdir}/lakritsi.xml
	python ${top_srcdir}/util/embed.py lakritsi_xml ${top_srcdir}/lakritsi.xml > $@

lakritsi_xml.o: lakritsi_xml.c

clean-local:
	-rm -f *.inc

$(CORBA_SRC) $(CORBA_HDR): idl_stamp
idl_stamp: $(IDLS)
ACE_GPERF=`which ace_gperf`
	if test "x${ACE_GPERF}" = "x"; then \
	echo "ace_gperf not found in the PATH"; \
	exit 1; \
	fi
	for idl in $(IDLS); do \
	  @TAO_IDL@ -I@TAO_IDL_PATH@ -in ${srcdir}/$$idl; \
	done
	touch idl_stamp

DISTCLEANFILES = $(CORBA_SRC) $(CORBA_HDR) idl_stamp

.PHONY: lakritsi-xml
