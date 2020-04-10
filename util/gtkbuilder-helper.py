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
            def dummy(a, b):
                name1 = a[2]
                name2 = b[2]
                return cmp(name1, name2)
            object.sort(dummy)
            self.get_parent().set_widget_list(object)

    class Child_XML_Handler(List_XML_Handler):
        def __init__(self, parent, attrs):
            List_XML_Handler.__init__(self, parent, attrs)

        def add_sibling(self, name, object):
            if name == 'object':
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
                widgets.append(('object', cls, id, signals))
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
                if cls in ['entry', 'table', 'window', 'dialog', 'spinbutton',
                           'toggleaction', 'uimanager', 'radioaction']:
                    return True
               
                if id.startswith(cls) and len(id) > len(cls):
                    end = id[len(cls):]
                    if end.isdigit():
                        return False
            return True
            
    handlers = { 'child': Child_XML_Handler,
                 'interface': List_XML_Handler,
                 'packing': List_XML_Handler,
                 'placeholder': XML_Handler,
                 'property': XML_Handler,
                 'signal': XML_Handler,
                 'object': Widget_XML_Handler }

    def get_cxx_headers_h(self, fp, args):
        unique = {}
        fp.write('// -*- C++ -*-\n')
        fp.write('# include <gtkmm/builder.h>\n')
        fp.write('# include <sigc++/signal.h>\n')

        forward = []
        main_widget = None
        for widget in self.widgets:
            _, cls, id, signals = widget

            if cls.startswith('Gtk') and id not in args['exclude']:
                unique[cls[3:]] = True

            if id == self.widget_id:
                main_widget = cls[3:]
            elif id in args['subclassed']:
                forward.append('    class %s;' % (id,))

        if (main_widget[0] == 'H' or main_widget[0] == 'V') \
               and not main_widget.startswith("handle"):
            fp.write('# include <gtkmm/%s.h>\n' % main_widget[1:].lower())
        else:
            fp.write('# include <gtkmm/%s.h>\n' % main_widget.lower())

        keys = unique.keys()
        keys.sort()
        fp.write('namespace Gtk {\n')
        for k in keys:
            # disaster waiting to happen
            if (k[0] == 'h' or k[0] == 'v') \
               and not k.startswith("handle") :
                fp.write('    class %s;\n' % k[1:])
            else:
                fp.write('    class %s;\n' % k)
        fp.write('};\n')
        # print '\n'.join(forward)

    def get_cxx_headers_cpp(self, fp, args):
        unique = {}
        fp.write('// -*- C++ -*-\n')
        fp.write('# include <gtkmm/builder.h>\n')
        fp.write('# include <sigc++/signal.h>\n')

        forward = []
        main_widget = None
        for widget in self.widgets:
            _, cls, id, signals = widget

            if cls.startswith('Gtk') and id not in args['exclude']:
                unique[cls[3:].lower()] = True

            if id == self.widget_id:
                main_widget = cls[3:]
            elif id in args['subclassed']:
                forward.append('    class %s;\n' % (id,))

        keys = unique.keys()
        keys.sort()
        for k in keys:
            # disaster waiting to happen
            if (k[0] == 'h' or k[0] == 'v') \
               and not k.startswith("handle") :
                fp.write('# include <gtkmm/%s.h>\n' % k[1:])
            else:
                fp.write('# include <gtkmm/%s.h>\n' % k)
        # fp.write('\n'.join(forward))

    def get_cxx_members(self, fp, args):
        # fp.write('// ', self.widgets)
        cobject = None
        lines = []
        for widget in self.widgets:
            _, cls, id, signals = widget

            if cls.startswith('Gtk') and id not in args['exclude']:
                if id == self.widget_id:
                    cobject = cls
                    # fp.write('    Gtk::%s * _win;\n' % cls[3:])
                elif id in args['subclassed']:
                    cls = args['subclassed'][id]
                    lines.append('    %s * %s;' % (cls, id))
                else:
                    lines.append('    Gtk::%s * %s;' % (cls[3:], id))

        fp.write('// -*- C++ -*-\n')
        fp.write('public:\n')
        fp.write('\n'.join(lines))
        builder_ptr = "const Glib::RefPtr<Gtk::Builder>"
        builder_xml = "const Glib::RefPtr<Gtk::Builder> & builder"
        builder_fptr = 'const Glib::ustring & glade_file'
        builder_fstr = 'const Glib::ustring & glade_file = "%s"' \
                     % self.glade_file

        if cobject is None:
            raise RuntimeError("Expected to find a main widget")
        
        fp.write('public:\n')
        fp.write('    %s(%s * obj, %s);\n' % (handler_class, cobject, builder_xml))
        fp.write('    static void create_widget(%s *& win, %s);\n' \
              % (handler_class, "const Glib::RefPtr<Gtk::Builder> & builder"))

        fp.write('protected:\n')
        fp.write('    virtual void init_widgets(%s);' % (builder_xml,))
        fp.write('    virtual void init_signals(void);')

    def get_cxx_constructor(self, fp, args):
        fp.write('Gtk::Builder> %s::init_widgets(const Glib::ustring & glade_file)' \
              % self.handler_class)
        fp.write('{\n')
        fp.write('    Glib::RefPtr<Gtk::Builder> xml = Gtk::Builder::create_from_file(glade_file);\n')
        fp.write('\n')
        sub_init = []
        for widget in self.widgets:
            _, cls, id, signals = widget
            if cls.startswith('Gtk'):
                if not (id in exclude or id in subclassed):
                    class_name = 'Gtk::' + cls[3:]
                    if id <> self.widget_id:
                        fp.write('    builder->get_widget("%s", this->%s);\n' \
                              % (id, id))
                elif id in subclassed:
                    sub_init.append((cls, id))
        fp.write('\n')
        fp.write('    this->init_signals();\n')
        if len(sub_init) > 0:
            tmp = ['    builder->get_widget_derived("%s", %s);' % (id, id) \
                   for _, id in sub_init]
            fp.write('\n'.join(tmp))
        fp.write('    return builder;\n')
        fp.write('}\n')

    def get_cxx_constructor2(self, fp, args):
        fp.write('void %s::init_widgets(const Glib::RefPtr<Gtk::Builder> & builder)\n' % self.handler_class)
        fp.write('{\n')

        sub_init = []
        for widget in self.widgets:
            _, cls, id, signals = widget

            if cls.startswith('Gtk'):
                if not (id in exclude or id in subclassed):
                    class_name = 'Gtk::' + cls[3:]
                    if id <> self.widget_id:
                        fp.write('    builder->get_widget<%s>("%s", this->%s);\n' % (class_name, id, id))
                elif id in subclassed:
                    sub_init.append((cls, id))                    
        if len(sub_init) > 0:
            tmp = ['    builder->get_widget_derived("%s", %s);' % (id, id) \
                   for _, id in sub_init]
            fp.write('\n'.join(tmp))
        fp.write('}\n')
        fp.write('\n')

    def get_cxx_signals(self, fp, args):
        fp.write('void %s::init_signals(void)\n' % self.handler_class)
        fp.write('{\n')
        
        for widget in self.widgets:
            _, cls, id, signals = widget

            if not (id in args['subclassed'] or id in args['exclude']):
                for signal in signals:
                    sig_name, sig_handler = signal
                    sig_name = sig_name.replace('-', '_')
                    if id == self.widget_id:
                        id = 'this'
                    fp.write('    %s->signal_%s().connect(\n' % (id, sig_name))
                    fp.write('      sigc::mem_fun(*this, &%s::%s), false);\n' \
                          % (handler_class, sig_handler))
        fp.write('}\n')
        fp.write('\n')

def usage():
    print "python gtkbuilder-helper.py [options] glade-file widget-id handler-class"
    print " options:"
    print " --help"
    print " --output-suffix"
    return 1
   
def do_work(args):
    import getopt

    global exclude
    global include
    global subclassed
    global widget_id
    global handler_class

    opts, args = getopt.getopt(args, [], ["help", "exclude=", "include=",
                                          "output-suffix=", "subclassed=" ])
    opts = dict(opts)
    if "--help" in opts or "--output-suffix" not in opts:
        usage()
        return 1

    try:
        glade_file, widget_id, handler_class = args
    except Exception, ex:
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
        # fp.write('// exclude', exclude
    except KeyError:
        exclude = []

    try:
        include = opts["--include"]
        include = [s.strip() for s in include.split(',')]
        # fp.write('// include', include
    except KeyError:
        include = []

    try:
        parser.parse(stringio)
    except StopIteration:
        # fp.write('// Got booty'
        gxi.set_widget_list(booty)

    args = { 'subclassed': subclassed, 'include': include, 'exclude': exclude }

    suffix = opts['--output-suffix']
    wid = widget_id
    with open('%s_headers.inc' % (suffix + wid,), 'w') as fp:
        gxi.get_cxx_headers_h(fp, args)

    with open('%s_headers_cpp.inc' % (suffix + wid,), 'w') as fp:
        gxi.get_cxx_headers_cpp(fp, args)

    with open('%s_members.inc' % (suffix + wid,), 'w') as fp:
        gxi.get_cxx_members(fp, args)

    with open('%s_init.inc' % (suffix + wid,), 'w') as fp:
        fp.write('#include "%s_headers_cpp.inc"\n' % wid)
        # gxi.get_cxx_constructor(fp, args)
        gxi.get_cxx_constructor2(fp, args)
        gxi.get_cxx_signals(fp, args)

    return 0

do_work(sys.argv[1:])
    
    
