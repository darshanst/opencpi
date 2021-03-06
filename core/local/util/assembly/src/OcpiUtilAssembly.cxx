/*
 *  Copyright (c) Mercury Federal Systems, Inc., Arlington VA., 2009-2011
 *
 *    Mercury Federal Systems, Incorporated
 *    1901 South Bell Street
 *    Suite 402
 *    Arlington, Virginia 22202
 *    United States of America
 *    Telephone 703-413-0781
 *    FAX 703-413-0784
 *
 *  This file is part of OpenCPI (www.opencpi.org).
 *     ____                   __________   ____
 *    / __ \____  ___  ____  / ____/ __ \ /  _/ ____  _________ _
 *   / / / / __ \/ _ \/ __ \/ /   / /_/ / / /  / __ \/ ___/ __ `/
 *  / /_/ / /_/ /  __/ / / / /___/ ____/_/ / _/ /_/ / /  / /_/ /
 *  \____/ .___/\___/_/ /_/\____/_/    /___/(_)____/_/   \__, /
 *      /_/                                             /____/
 *
 *  OpenCPI is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  OpenCPI is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with OpenCPI.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "OcpiUtilExceptionApi.h"
#include "OcpiUtilEzxml.h"
#include "OcpiUtilMisc.h"
#include "OcpiUtilAssembly.h"

namespace OCPI {
  namespace Util {
    namespace OA = OCPI::API;
    namespace OE = OCPI::Util::EzXml;

    Assembly::Assembly(const char *file, const char **extraTopAttrs, const char **extraInstAttrs)
      : m_xml(ezxml_parse_file(file)), m_copy(NULL), m_xmlOnly(false), m_isImpl(false) {
      if (!m_xml)
	throw Error("Can't open or xml-parse assembly xml file: \"%s\"", file);
      const char *err = parse(NULL, extraTopAttrs, extraInstAttrs);
      if (err)
	throw Error("Error parsing assembly xml file (\"%s\") due to: %s", file, err);
    }
    Assembly::Assembly(const std::string &string, const char **extraTopAttrs, const char **extraInstAttrs)
      : m_xmlOnly(false), m_isImpl(false) {
      m_copy = new char[string.size() + 1];
      strcpy(m_copy, string.c_str());
      if (!(m_xml = ezxml_parse_str(m_copy, string.size())))
	throw Error("Can't xml-parse assembly xml string");
      const char *err = parse(NULL, extraTopAttrs, extraInstAttrs);
      if (err)
	throw Error("Error parsing assembly xml string due to: %s", err);
    }
    // FIXME:  we infer that this is an impl assy from this constructor.  Make it explicit?
    Assembly::Assembly(const ezxml_t top, const char *defaultName,
		       const char **extraTopAttrs, const char **extraInstAttrs)
      : m_xml(top), m_copy(NULL), m_xmlOnly(true), m_isImpl(true) {
      const char *err = parse(defaultName, extraTopAttrs, extraInstAttrs);
      if (err)
	throw Error("Error parsing assembly xml string due to: %s", err);
    }
    Assembly::~Assembly() {
      if (m_xml && !m_xmlOnly)
	ezxml_free(m_xml);
      delete [] m_copy;
    }

    unsigned Assembly::s_count = 0;

    const char *Assembly::
    parse(const char *defaultName, const char **extraTopAttrs, const char **extraInstAttrs) {
      // This is where common initialization is done except m_xml and m_copy
      m_doneInstance = -1;
      m_cMapPolicy = RoundRobin;
      m_processors = 0;
      ezxml_t ax = m_xml;
      const char *err;
      static const char *baseAttrs[] = { "name", "package", NULL};
      bool maxProcs = false, minProcs = false, roundRobin = false;
      // FIXME: move app-specific parsing up into library assy
      if ((err = OE::checkAttrsVV(ax, baseAttrs, extraTopAttrs, NULL)) ||
	  (err = OE::checkElements(ax, "instance", "connection", "policy", "property", NULL)) ||
	  (err = OE::getNumber(ax, "maxprocessors", &m_processors, &maxProcs)) ||
	  (err = OE::getNumber(ax, "minprocessors", &m_processors, &minProcs)) ||
	  (err = OE::getBoolean(ax, "roundrobin", &roundRobin)))
	return err;
      if (maxProcs)
	m_cMapPolicy = MaxProcessors;
      else if (minProcs)
	m_cMapPolicy = MinProcessors;
      else if (roundRobin)
	m_cMapPolicy = RoundRobin;
      else
	m_cMapPolicy = RoundRobin;
      ezxml_t px = ezxml_cchild(ax, "policy");
      if (px) {
	const char * tmp = ezxml_attr(px, "mapping" );
	if ( tmp ) {
	  if (!strcasecmp(tmp, "maxprocessors"))
	    m_cMapPolicy = MaxProcessors;
	  else if (!strcasecmp(tmp, "minprocessors"))
	    m_cMapPolicy = MinProcessors;
	  else if (!strcasecmp(tmp, "roundrobin"))
	    m_cMapPolicy = RoundRobin;
	  else
	    return esprintf("Invalid policy mapping option: %s", tmp);
	}
	tmp  = ezxml_attr(px, "processors");	
	if (tmp) {
	  m_processors = atoi(tmp);
	}
      }
	
      OE::getNameWithDefault(ax, m_name, defaultName ? defaultName : "unnamed%u", s_count);
      OE::getOptionalString(ax, m_package, "package");
      if (m_package.empty())
	m_package = "local";
      m_instances.resize(OE::countChildren(ax, "Instance"));
      Instance *i = &m_instances[0];
      unsigned n = 0;
      for (ezxml_t ix = ezxml_cchild(ax, "Instance"); ix; ix = ezxml_next(ix), i++, n++)
	if ((err = i->parse(ix, *this, n, extraInstAttrs)))
	  return err;
      const char *done = ezxml_cattr(ax, "done");
      if (done) {
	if ((err = getInstance(done, n)))
	  return err;
	m_doneInstance = (int)n;
      }
      m_mappedProperties.resize(OE::countChildren(ax, "property"));
      MappedProperty *p = &m_mappedProperties[0];
      for (ezxml_t px = ezxml_cchild(ax, "property"); px; px = ezxml_next(px), p++)
	if ((err = p->parse(px, *this)))
	  return err;
      n = 0;
      for (ezxml_t cx = ezxml_cchild(ax, "Connection"); cx; cx = ezxml_next(cx), n++) {
	Connection tmp;
	m_connections.push_back(tmp);
	if ((err = m_connections.back().parse(cx, *this, n)))
	  return err;
      }
      i = &m_instances[0];
      for (ezxml_t ix = ezxml_cchild(ax, "Instance"); ix; ix = ezxml_next(ix), i++)
	if ((err = i->parseConnection(ix, *this)))
	  return err;
      return NULL;
    }

    const char * Assembly::
    getInstance(const char *name, unsigned &n) {
      for (n = 0; n < m_instances.size(); n++)
	if (m_instances[n].m_name == name)
	  return NULL;
      return esprintf("No instance named \"%s\" found", name);
    }

    const char *Assembly::
    addConnection(const char *name, Connection *&c) {
      for (ConnectionIter ci = m_connections.begin(); ci != m_connections.end(); ci++)
	if (!strcasecmp((*ci).m_name.c_str(), name))
	  return esprintf("Duplicate connection named '%s' in assembly", name);
      Connection tmp;
      m_connections.push_back(tmp);
      c = &m_connections.back();
      c->m_name = name;
      return NULL;
    }
    const char *Assembly::
    addPortConnection(unsigned from, const char *fromPort, unsigned to, const char *toPort) {
      std::string name = m_instances[from].m_name + "." + (fromPort ? fromPort : "output");
      Connection *c;
      const char *err = addConnection(name.c_str(), c);
      if (!err) {
	Port &toP = c->addPort(*this, to, toPort, true, false, true); // implicitly input
	Port &fromP = c->addPort(*this, from, fromPort, false, false, true); // implicitly output
	toP.m_connectedPort = &fromP;
	fromP.m_connectedPort = &toP;
      }
      return err;
    }
    const char *Assembly::
    addExternalConnection(unsigned instance, const char *port) {
      Connection *c;
      const char *err = addConnection(port, c);
      if (!err) {
	c->addPort(*this, instance, port, false, false, false); // we don't know the direction
	c->addExternal(port);
      }
      return err;
    }
    const char *Assembly::Property::
    setValue(ezxml_t px) {
      const char *cp, *err = NULL;
      const char *df = ezxml_cattr(px, "dumpFile");
      m_hasValue = false;
      if ((cp = ezxml_cattr(px, "value"))) {
	m_hasValue = true;
	m_value = cp;
      } else if ((cp = ezxml_cattr(px, "valueFile"))) {
	m_hasValue = true;
	err = file2String(m_value, cp, ',');
      } else if (!df)
	return "Missing value or valueFile or dumpFile attribute for instance property";
      if (!err && df)
	m_dumpFile = df;
      return err;
    }

    const char *Assembly::MappedProperty::
    parse(ezxml_t px, Assembly &a) {
      const char *err;
      std::string instance;

      if ((err = OE::checkAttrs(px, "name", "value", "valuefile", "dumpFile", "instance", "property", NULL)) ||
	  (err = OE::getRequiredString(px, m_name, "name", "property")) ||
	  (err = OE::getRequiredString(px, instance, "instance", "property")) ||
	  (err = a.getInstance(instance.c_str(), m_instance)))
	return err;
      MappedProperty *p = &a.m_mappedProperties[0];
      for (unsigned n = a.m_mappedProperties.size(); n && p < this; n--, p++)
	if (p->m_name == m_name)
	  return esprintf("Duplicate mapped property: %s", m_name.c_str());
      const char *cp = ezxml_cattr(px, "property");
      m_instPropName = cp ? cp : m_name.c_str();
      if (ezxml_cattr(px, "value") || ezxml_cattr(px, "valueFile") || ezxml_cattr(px, "dumpFile"))
	a.m_instances[m_instance].addProperty(m_instPropName.c_str(), px);
      return NULL;
    }

    const char *Assembly::Property::
    parse(ezxml_t px) {
      const char *err;

      if ((err = OE::checkAttrs(px, "name", "value", "valuefile", "dumpFile", NULL)) ||
	  (err = OE::getRequiredString(px, m_name, "name", "property")))
	return err;
      return setValue(px);
    }

    // connect, then optionally, which local port (from) and which dest port (to).
    // external=port, connect=instance, then to or from?
    const char *Assembly::Instance::
    parseConnection(ezxml_t ix, Assembly &a) {
      const char
	*err = NULL,
	*c = ezxml_cattr(ix, "connect"),
	*e = ezxml_cattr(ix, "external");
      if (c) {
	unsigned n;
	if ((err = a.getInstance(c, n)))
	  return err;
	err = a.addPortConnection(m_ordinal, ezxml_cattr(ix, "from"), n, ezxml_cattr(ix, "to"));
      }
      if (e)
	err = a.addExternalConnection(m_ordinal, e);
      return err;
    }

    const char *Assembly::Instance::
    addProperty(const char *name, ezxml_t px) {
      Property *p = &m_properties[0];
      unsigned n;
      for (n = m_properties.size(); n ; n--, p++)
	if (p->m_name == name)
	  break;
      if (!n) {
	m_properties.resize(m_properties.size() + 1);
	p = &m_properties.back();
      }
      p->setValue(px);
      return NULL;
    }

    const char *Assembly::Instance::
    parse(ezxml_t ix, Assembly &a, unsigned ordinal, const char **extraInstAttrs) {
      m_ordinal = ordinal;
      const char *err;
      static const char *instAttrs[] = { "component", "Worker", "Name", "connect", "to", "from",
					 "external", "selection", NULL};
      if ((err = OE::checkAttrsVV(ix, instAttrs, extraInstAttrs, NULL)))
	return err;
      std::string component;
      const char *dot = 0;
      if (a.isImpl()) {
	if (ezxml_cattr(ix, "component"))
	  err = "'component' attributes invalid in this implementaiton assembly";
	else
	  OE::getOptionalString(ix, m_implName, "worker"); // FIXME: optional due to container xml...
      } else {
	OE::getOptionalString(ix, m_implName, "worker");
	if (!(err = OE::getRequiredString(ix, component, "component", "instance"))) {
	  if ((dot = strrchr(component.c_str(), '.')))
	    dot++;
	  else {
	    m_specName = a.m_package;
	    m_specName += ".";
	    dot = component.c_str();
	  }
	  m_specName += component;
	}
      }

      if (err ||
	  (err = OE::checkElements(ix, "property", NULL)))
	return err;
      OE::getOptionalString(ix, m_name, "name");
      OE::getOptionalString(ix, m_selection, "selection");
      // default is component%d unless there is only one, in which case it is "component".
      if (m_name.empty()) {
	unsigned me = 0, n = 0;
	for (ezxml_t x = ezxml_cchild(a.xml(), "instance"); x; x = ezxml_next(x)) {
	  const char
	    *c = ezxml_cattr(x, "component"),
	    *w = ezxml_cattr(x, "worker");
	  if (component.size() && c && !strcasecmp(dot, c) ||
	      m_implName.size() && w && !strcasecmp(m_implName.c_str(), w)) {
	    if (x == ix)
	      me = n;
	    n++;
	  }
	}
	const char *myBase = component.size() ? dot : m_implName.c_str();
	if (n > 1)
	  formatString(m_name, "%s%u", myBase, me);
	else
	  m_name = myBase;
      }
      ocpiDebug("Component: %s name: %s impl: %s spec: %s",
		component.c_str(), m_name.c_str(), m_implName.c_str(), m_specName.c_str());
      m_properties.resize(OE::countChildren(ix, "property"));
      Property *p = &m_properties[0];
      for (ezxml_t px = ezxml_cchild(ix, "property"); px; px = ezxml_next(px), p++)
	if ((err = p->parse(px)))
	  return err;
      return m_parameters.parse(ix, "name", "component", "worker", "selection", "connect",
				"external", "from", "to", NULL);
    }

    const char *Assembly::Connection::
    parse(ezxml_t cx, Assembly &a, unsigned &n) {
      const char *err;
      if ((err = OE::checkElements(cx, "port", "external", NULL)) ||
	  (err = OE::checkAttrs(cx, "name", "transport", "external", NULL)))
	return err;
      
      OE::getNameWithDefault(cx, m_name, "conn%u", n);
      if ((err = m_parameters.parse(cx, "name", "external", NULL)))
	return err;

      const char *ext = ezxml_cattr(cx, "external");
      if (ext) {
	External tmp;
	m_externals.push_back(tmp);
	// default the external's name from the connection's name
	if ((err = m_externals.back().init(m_name.c_str(), ext)))
	  return err;
      }
      unsigned nExt = 0; // for name ordinals when unnamed
      for (ezxml_t x = ezxml_cchild(cx, "external"); x; x = ezxml_next(x), nExt++) {
	External tmp;
	m_externals.push_back(tmp);
	if ((err = m_externals.back().parse(x, nExt, m_parameters)))
	  return err;
      }
      if (OE::countChildren(cx, "port") < 1)
	return "no ports found under connection";
      Port *other = NULL;
      for (ezxml_t x = ezxml_cchild(cx, "port"); x; x = ezxml_next(x)) {
	Port tmp;
	m_ports.push_back(tmp);
	Port &p = m_ports.back();
	if ((err = p.parse(x, a, m_parameters)))
	  return err;
	if (other) {
	  ocpiAssert(!p.m_connectedPort && !other->m_connectedPort);
	  p.m_connectedPort = other;
	  other->m_connectedPort = &p;
	} else
	  other = &p;
      }
      return NULL;
    }

    Assembly::Port & Assembly::Connection::
    addPort(Assembly &a, unsigned instance, const char *port, bool isInput, bool bidi, bool known) {
      Port tmp;
      m_ports.push_back(tmp);
      Port &p = m_ports.back();
      p.init(a, port, instance, isInput, bidi, known);
      return p;
    }

    void Assembly::Port::
    init(Assembly &a, const char *name, unsigned instance, bool isInput, bool bidir, bool isKnown) {
      if (name)
	m_name = name;
      m_provider = isInput;
      m_bidirectional = bidir;
      m_knownRole = isKnown;
      m_instance = instance;
      m_provider = isInput;
      m_connectedPort = NULL;
      a.m_instances[instance].m_ports.push_back(this);
    }

    void Assembly::Connection::
    addExternal(const char *port) {
      m_externals.resize(m_externals.size() + 1);
      External &e = m_externals.back();
      e.m_name = port;
      e.m_provider = false; // provisional
      e.m_bidirectional = false;
      e.m_knownRole = false;
    }
    const char *Assembly::Port::
    parse(ezxml_t x, Assembly &a, const PValue *pvl) {
      const char *err;
      std::string iName, name;
      unsigned instance;
      if ((err = OE::checkElements(x, NULL)) ||
	  (err = OE::getRequiredString(x, name, "name", "port")) ||
	  (err = OE::getRequiredString(x, iName, "instance", "port")) ||
	  (err = a.getInstance(iName.c_str(), instance)))
	return err;
      // We don't know the role at all at this point
      init(a, name.c_str(), instance, false, false, false);
      return m_parameters.parse(pvl, x, "name", "instance", NULL);
    }

    const char *Assembly::External::
    init(const char *name, const char *r) {
      if (name)
	m_name = name;
      m_knownRole = false;
      m_bidirectional = false;
      m_provider = false;
      if (r) {
	if (!strcasecmp(r, "provider") || !strcasecmp(r, "input") || !strcasecmp(r, "consumer")) {
	  m_provider = true;
	  m_knownRole = true;
	} else if (!strcasecmp(r, "user") || !strcasecmp(r, "output") || !strcasecmp(r, "producer")) {
	  m_provider = false;
	  m_knownRole = true;
	} else if (!strcasecmp(r, "bidirectional")) {
	  m_bidirectional = true;
	  m_knownRole = true;
	} else if (*r)
	  return esprintf("Invalid external role: %s", r);
      }
      return NULL;
    }
    const char *Assembly::External::
    parse(ezxml_t x, unsigned &n, const PValue *pvl) {
      OE::getNameWithDefault(x, m_name, "ext%u", n);
      OE::getOptionalString(x, m_url, "url");
      std::string role;
      OE::getOptionalString(x, role, "role");
      // Parse all attributes except the explicit ones here.
      const char *err = init(NULL, role.c_str());
      return err ? err : m_parameters.parse(pvl, x, "name", "url", "provider", NULL);
    }
  }
}

