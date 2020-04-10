#!/usr/bin/python

import xml.sax
# from xml.sax import handler, make_parser, ContentHandler, \
# SAXException, SAXReaderNotAvailable, SAXParseException

import string
import sys
import re

include = []
exclude = []
subclassed = []
widget_id = None
booty = False

from xmlutil import XML_Handler, List_XML_Handler, Dict_XML_Handler

class Glade_DTD_Handler(xml.sax.ContentHandler):
    def notationDecl(self, name, public_id, system_id):
        print 'notation decl'

    def unparsedEntityDecl(self, name, public_id, system_id, ndata):
        print 'unparsed entity decl'

class Glade_XML_Import(xml.sax.ContentHandler):
    def __init__(self, glade_file, widget_id, handler_class):
        self.data    = None
        self.path    = []
        self.parent  = Glade_XML_Import.Root_XML_Handler(self)
        self.handler = None
        self.widgets   = []

        self.glade_file = glade_file
        self.widget_id = widget_id
        self.handler_class = handler_class

    def get_cxx_constructor(self):
        return self.widgets

    def set_widget_list(self, list):
        self.widgets   = list

    def get_widget_list(self):
        return self.widgets
    
    def setDocumentLocator(self, locator):
        self._locator = locator

    def startDocument(self):
        pass

    def endDocument(self):
        pass

    def startPrefixMapping(self, prefix, uri):
        pass

    def endPrefixMapping(self, prefix):
        pass

    def startElement(self, name, attrs):
        self.path.append(name)
        #end print string.join(self.path, '.') + ' start'

        if self.handler is not None:
            self.parent = self.handler
        
        if Glade_XML_Import.handlers.has_key(name):
            self.handler = Glade_XML_Import.handlers[name](self.parent, attrs)
        else:
            self.handler = XML_Handler(self.parent, attrs)

        self.handler.start(self.path)

    def endElement(self, name):
        # print string.join(self.path, '.') + ' end'

        self.data = self.handler.end()
        try:
            self.handler.get_parent().add_sibling(name, self.data)
        except AttributeError:
            pass

        self.handler = self.handler.get_parent()
        self.path.pop()

    def startElementNS(self, name, qname, attrs):
        print "startElementNS"

    def endElementNS(self, name, qname):
        print "endElementNS"

    def characters(self, content):
        self.handler.characters(content)

    def ignorableWhitespace(self, whitespace):
        print "ignorableWhitespace"
    
    def processingInstruction(self, target, data):
        print "processingInstruction"

    def skippedEntity(self, name):
        print "skippedEntity"

    # Nested classes
    class Root_XML_Handler(XML_Handler):
        def __init__(self, parent):
            XML_Handler.__init__(self, parent, None)
            
        def add_sibling(self, name, object):
            def dummy(a,b):
                name1 = a[2]
                name2 = b[2]
                return cmp(name1, name2)
            object.sort(dummy)
            self.get_parent().set_widget_list(object)

    class Child_XML_Handler(List_XML_Handler):
        def __init__(self, parent, attrs):
            List_XML_Handler.__init__(self, parent, attrs)

        def add_sibling(self, name, object):
            if name == 'widget':
                List_XML_Handler.add_sibling(self, name, object)

    class Widget_XML_Handler(XML_Handler):
        def __init__(self, parent, attrs):
            XML_Handler.__init__(self, parent, attrs)

            self.siblings = []

        def start(self, path):
            pass
                
        def end(self):
            cls = self.attrs['class']
            id  = self.attrs['id']
            emit = False
            signals = []
            widgets = []

            global booty

            if booty:
                # print self.siblings
                return self.siblings

            if id.find("-"):
                # argh
                id = id.replace("-", "_")

            if id[0].isdigit():
                raise RuntimeError("Bad identifier %s" % id)
                
            if self.interesting_name(cls, id) or id in include:
                # print 'Interesting: ', id
                emit = True
            else:
                # print 'Interesting... not: ', id
                pass
            for sibbe in self.siblings:
                name, tmp = sibbe
                if name == 'signal':
                    attrs, data = tmp
                    try:
                        sig_handler = attrs['handler']
                        sig_name = attrs['name']
                        # print 'Signal:', cls, id, sig_name, sig_handler
                        if not emit:
                            # print 'Interesting after all: ', id
                            emit = True

                        if sig_handler.find("-"):
                            # argh
                            sig_handler = sig_handler.replace("-", "_")
                            
                        signals.append((sig_name, sig_handler))
                    except KeyError:
                        pass
                elif name == 'child' and not booty:
                    if len(tmp) > 0:
                        widgets = tmp + widgets
                        
            if id <> widget_id and id in subclassed:
                def dummy(arg):
                    a,b,c,_ = arg
                    return (a,b,c,[])
                widgets = [dummy(eek) for eek in widgets]

            if emit:
                widgets.append(('widget', cls, id, signals))
            if id == widget_id:
                # print '// Booty:', id, widgets
                booty = widgets
                raise StopIteration("Booty found")
            return widgets
                
        def add_sibling(self, name, object):
            self.siblings.append((name, object))

        def interesting_name(self, cls, id):
            if cls.startswith('Gtk'):
                cls = cls[3:].lower()

                # NOTE: Temporary
                if cls in ['entry', 'table', 'window', 'dialog', 'spinbutton']:
                    return True
               
                if id.startswith(cls) and len(id) > len(cls):
                    end = id[len(cls):]
                    if end.isdigit():
                        return False
            return True
            
    handlers = { 'child': Child_XML_Handler,
                 'glade-interface': List_XML_Handler,
                 'packing': List_XML_Handler,
                 'placeholder': XML_Handler,
                 'property': XML_Handler,
                 'signal': XML_Handler,
                 'widget': Widget_XML_Handler }

    def get_cxx_headers(self, args):
        unique = {}
        print '// -*- C++ -*-'
        print '# include <libglademm.h>'
        print '# include <sigc++/signal.h>'

        forward = []
        main_widget = None
        for widget in self.widgets:
            _, cls, id, signals = widget

            if cls.startswith('Gtk') and id not in args['exclude']:
                unique[cls[3:].lower()] = True

            if id == self.widget_id:
                main_widget = cls[3:]
            elif id in args['subclassed']:
                forward.append('    class %s;' % (id,))

        keys = unique.keys()
        keys.sort()
        for k in keys:
            # disaster waiting to happen
            if (k[0] == 'h' or k[0] == 'v') \
               and not k.startswith("handle") :
                print '# include <gtkmm/%s.h>' % k[1:]
            else:
                print '# include <gtkmm/%s.h>' % k
        # print '\n'.join(forward)

    def get_cxx_members(self, args):
        # print '// ', self.widgets
        cobject = None
        lines = []
        for widget in self.widgets:
            _, cls, id, signals = widget

            if cls.startswith('Gtk') and id not in args['exclude']:
                if id == self.widget_id:
                    cobject = cls
                    # print '    Gtk::%s * _win;' % cls[3:]
                elif id in args['subclassed']:
                    cls = args['subclassed'][id]
                    lines.append('    %s * %s;' % (cls, id))
                else:
                    lines.append('    Gtk::%s * %s;' % (cls[3:], id))

        print '// -*- C++ -*-'
        print 'public:'
        print '\n'.join(lines)
        glade_ptr = "const Glib::RefPtr<Gnome::Glade::Xml>"
        glade_xml = "const Glib::RefPtr<Gnome::Glade::Xml> & xml"
        glade_fptr = 'const Glib::ustring & glade_file'
        glade_fstr = 'const Glib::ustring & glade_file = "%s"' \
                     % self.glade_file

        if cobject is None:
            raise RuntimeError("Expected to find a main widget")
        
        print 'public:'
        print '    %s(%s * obj, %s);' % (handler_class, cobject, glade_xml)
        print '    static void create_widget(%s *& win);' % (handler_class,)

        print 'protected:'
        print '    virtual void init_widgets(%s);' % (glade_xml,)
        print '    virtual void init_signals(void);'

    def get_cxx_constructor(self, args):
        print 'Glade::Xml> %s::init_widgets(const Glib::ustring & glade_file)' \
              % self.handler_class
        print '{'
        print '    Glib::RefPtr<Gnome::Glade::Xml> xml = Gnome::Glade::Xml::create(glade_file);' 
        print ''
        sub_init = []
        for widget in self.widgets:
            _, cls, id, signals = widget
            if cls.startswith('Gtk'):
                if not (id in exclude or id in subclassed):
                    class_name = 'Gtk::' + cls[3:]
                    if id <> self.widget_id:
                        print '    xml->get_widget("%s", this->%s);' \
                              % (id, id)
                elif id in subclassed:
                    sub_init.append((cls, id))
        print ''
        print '    this->init_signals();'
        if len(sub_init) > 0:
            tmp = ['    xml->get_widget_derived("%s", %s);' % (id, id) \
                   for _, id in sub_init]
            print '\n'.join(tmp)
        print '    return xml;'
        print '}'

    def get_cxx_constructor2(self, args):
        print 'void %s::init_widgets(const Glib::RefPtr<Gnome::Glade::Xml> & xml)' \
              % self.handler_class
        print '{'
        sub_init = []
        for widget in self.widgets:
            _, cls, id, signals = widget

            if cls.startswith('Gtk'):
                if not (id in exclude or id in subclassed):
                    class_name = 'Gtk::' + cls[3:]
                    if id <> self.widget_id:
                        print '    xml->get_widget<%s>("%s", this->%s);' \
                              % (class_name, id, id)
                elif id in subclassed:
                    sub_init.append((cls, id))                    
        if len(sub_init) > 0:
            tmp = ['    xml->get_widget_derived("%s", %s);' % (id, id) \
                   for _, id in sub_init]
            print '\n'.join(tmp)
        print '}'
        print ''

    def get_cxx_signals(self, args):
        print 'void %s::init_signals(void)' % self.handler_class
        print '{'
        
        for widget in self.widgets:
            _, cls, id, signals = widget

            if not (id in args['subclassed'] or id in args['exclude']):
                for signal in signals:
                    sig_name, sig_handler = signal
                    if id == self.widget_id:
                        id = 'this'
                    print '    %s->signal_%s().connect(' % (id, sig_name)
                    print '      sigc::mem_fun(*this, &%s::%s), false);' \
                          % (handler_class, sig_handler)
        print '}'
        print ''

def usage():
    print "python glade-helper.py [options] glade-file widget-id handler-class"
    print " options:"
    print " --help"
    print " --headers"
    print " --members"
    print " --init"
    return 1
   
def do_work(args):
    import getopt

    global exclude
    global include
    global subclassed
    global widget_id
    global handler_class

    opts, args = getopt.getopt(args, [], ["help", "exclude=", "include=",
                                          "headers", "init", "members",
                                          "subclassed=" ])
    opts = dict(opts)
    if "--help" in opts:
        usage()
        return 1
    try:
        glade_file, widget_id, handler_class = args
    except:
        usage()
        return 1

    gxi = Glade_XML_Import(glade_file, widget_id, handler_class)
    parser = xml.sax.make_parser()
    
    parser.returns_unicode = 1
    parser.setContentHandler(gxi)
    parser.setDTDHandler(Glade_DTD_Handler())

    lines = open(glade_file).readlines()
    if lines[1].startswith('<!DOCTYPE'):
        lines = lines[0:1] + lines[2:]

    from StringIO import StringIO
    stringio = StringIO(''.join(lines))

    try:
        subclassed = opts["--subclassed"]
        def foo(x):
            (a, b) = x.strip().split(':')
            return (a,b)
        subclassed = dict([foo(s) for s in subclassed.split(',')])
    except KeyError:
        subclassed = []

    try:
        exclude = opts["--exclude"]
        exclude = [s.strip() for s in exclude.split(',')]
        # print '// exclude', exclude
    except KeyError:
        exclude = []

    try:
        include = opts["--include"]
        include = [s.strip() for s in include.split(',')]
        # print '// include', include
    except KeyError:
        include = []

    try:
        parser.parse(stringio)
    except StopIteration:
        # print '// Got booty'
        gxi.set_widget_list(booty)

    args = { 'subclassed': subclassed, 'include': include, 'exclude': exclude }
    if "--headers" in opts:
        gxi.get_cxx_headers(args)
    elif "--members" in opts:
        gxi.get_cxx_members(args)
    elif "--init" in opts:
        #gxi.get_cxx_constructor(args)
        gxi.get_cxx_constructor2(args)
        gxi.get_cxx_signals(args)
    else:
        usage()
        return 1

    return 0

do_work(sys.argv[1:])
    
    
