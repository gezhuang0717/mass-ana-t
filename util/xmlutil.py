from types import *

class XML_Handler:
    def __init__(self, parent, attrs):
        self.__parent = parent
        self.data = None
        self.name = None
        if attrs is not None:
            self.attrs = {}
            for key in attrs.keys():
                self.attrs[key] = attrs[key]
        else:
            self.attrs = None

    def characters(self, data):
        self.data = data
            
    def start(self, path):
        self.path = path
        self.tag = path[-1]
            
    def end(self):
        return (self.attrs, self.data)
                
    def get_parent(self):
        return self.__parent

class List_XML_Handler(XML_Handler):
    def __init__(self, parent, attrs):
        XML_Handler.__init__(self, parent, attrs)
        self.siblings = []

    def add_sibling(self, name, object):
        if isinstance(object, ListType):
            self.siblings += object
        else:
            self.siblings.append(object)

    def end(self):
        return self.siblings
    
class Dict_XML_Handler(XML_Handler):
    def __init__(self, parent, attrs):
        XML_Handler.__init__(self, parent, attrs)
        self.siblings = {}

    def add_sibling(self, name, object):
        if not self.siblings.has_key(name):
            self.siblings[name] = object
        else:
            raise 'duplicate value: %s' % (name.encode())

    def end(self):
        return self.siblings
