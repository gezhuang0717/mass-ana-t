import xml.etree.ElementTree as ET
tree = ET.parse('../lakritsi.xml')
root = tree.getroot()

for objekti in root.findall('object'):
	if objekti.get('id') == "postProcessor":
		print objekti.attrib
	else:
		root.remove(objekti)

tree.write('hahaa.xml')
