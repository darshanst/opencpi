
/*
 *  Copyright (c) Mercury Federal Systems, Inc., Arlington VA., 2009-2010
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


#include <memory>
#include <unistd.h>
#include <stdarg.h>
#include <strings.h>
#include "OcpiUtilEzxml.h"
#include "OcpiUtilMisc.h"
#include "OcpiUtilDataTypes.h"
#include "OcpiUtilValue.h"
#include "OcpiPValue.h"

namespace OE = OCPI::Util::EzXml;
namespace OCPI {
  namespace API {
    PVULong PVEnd(0,0);

    unsigned PValue::length() const {
      unsigned n = 0;
      if (this)
	for (const PValue *p = this; p->name; p++, n++)
	  ;
      return n;
    }
  }
  namespace Util {
    // This list provides names and types and defaults
    PValue allPVParams[] = {
      PVString("transport"),
      PVString("xferrole"),
      PVString("DLLEntryPoint"),
      PVString("monitorIPAddress"),
      PVString("protocol"),
      PVString("endpoint"),
      PVString("Device"),
      PVBool("ownthread"),
      PVBool("polled"),
      PVULong("bufferCount"),
      PVULong("bufferSize"),
      PVUChar("index"),
      PVString("interconnect"),
      PVString("adapter"),
      PVString("configure"),
      PVString("io"),
      PVEnd
    };

    PVULong PVEnd(0,0);
    static const PValue *
    find(const PValue* p, const char* name) {
      if (p)
	for (; p->name; p++)
	  if (!strcasecmp(p->name, name))
	    return p;
      return NULL;
    }
 
    //#undef OCPI_DATA_TYPE_S
#define OCPI_DATA_TYPE(sca, corba, letter, bits, run, pretty, store)	\
    bool 							        \
    find##pretty(const PValue* p, const char* name, run &value) { \
      const PValue *fp = find(p, name);					\
      if (fp)								\
        if (fp->type == OCPI::API::OCPI_##pretty) {	                \
          value = fp->v##pretty;					\
          return true;							\
	} else								\
	  throw ApiError("Property \"", name, "\" is not a ", #pretty, NULL); \
      return false;							\
    }
#if 0
#define OCPI_DATA_TYPE_S(sca, corba, letter, bits, run, pretty, store)	\
    bool 							        \
    find##pretty(const PValue* p, const char* name, run &value) {	\
      const PValue *fp = find(p, name);					\
      if (fp)								\
        if (fp->type == OCPI::API::OCPI_##pretty) {	                \
          value = fp->v##pretty;					\
          return true;							\
	} else								\
	  throw ApiError("Parameter \"", name, "\" is not a ", #pretty, NULL); \
      return false;							\
    }
#endif

    OCPI_PROPERTY_DATA_TYPES
#undef OCPI_DATA_TYPE
    //#undef OCPI_DATA_TYPE_S
    //#define OCPI_DATA_TYPE_S OCPI_DATA_TYPE

    bool 
    findAssign(const PValue *p, const char *name, const char *var, const char *&val) {
      if (p)
	for (; p->name; p++)
	  if (!strcasecmp(p->name, name))
	    if (p->type == OCPI::API::OCPI_String) {
	      unsigned len = p->vString[0] == '=' ? 0 : strlen(var);
	      if (len == 0 ||
		  !strncasecmp(var, p->vString, len) && p->vString[len] == '=') {
		val = p->vString + len + 1;
		return true;
	      }
	    } else
	      throw ApiError("Parameter \"", name, "\" is not a string", NULL);
      return false;
    }

    bool 
    findAssignNext(const PValue *p, const char *name, const char *var,
		   const char *&val, unsigned &next) {
      if (p)
	for (unsigned n = 0; p->name; p++, n++)
	  if (n >= next && !strcasecmp(p->name, name))
	    if (p->type == OCPI::API::OCPI_String) {
	      if (!var) {
		val = p->vString;
		next = n + 1;
		return true;
	      } else {
		unsigned len = p->vString[0] == '=' ? 0 : strlen(var);
		if (len == 0 ||
		    !strncasecmp(var, p->vString, len) && p->vString[len] == '=') {
		  val = p->vString + len + 1;
		  next = n + 1;
		  return true;
		}
	      }
	    } else
	      throw ApiError("Parameter \"", name, "\" is not a string", NULL);
      return false;
    }

    PValueList::PValueList() : m_list(NULL) {}
    PValueList::~PValueList() { delete [] m_list; }
    const char *PValueList::parse(ezxml_t x, ...) {
      va_list ap;
      va_start(ap, x);
      const char *err = vParse(NULL, x, ap);
      va_end(ap);
      return err;
    }
    const char *PValueList::parse(const PValue * pvl, ezxml_t x, ...) {
      va_list ap;
      va_start(ap, x);
      const char *err = vParse(pvl, x, ap);
      va_end(ap);
      return err;
    }
    const char *PValueList::vParse(const PValue *pvl, ezxml_t x, va_list ap) {
      unsigned nPvl = pvl ? pvl->length() : 0;
      unsigned nXml = OE::countAttributes(x);
      unsigned n = nPvl + nXml;
      if (!n) {
	m_list = NULL;
	return NULL;
      }
      PValue *p = m_list = new PValue[n + 1];
      for (unsigned n = 0; n < nPvl; n++)
	*p++ = pvl[n];
      const char *name, *value;
      EZXML_FOR_ALL_ATTRIBUTES(x, name, value) {
	const char *attr;
	bool found = false;
	va_list dest;
	va_copy(dest, ap);
	while ((attr = va_arg(dest, const char *)))
	  if (!strcasecmp(name, attr)) {
	    found = true;
	    break;
	  }
	va_end(dest);
	if (!found) {
	  const PValue *allP = find(allPVParams, name);
	  if (!allP)
	    return esprintf("parameter named \"%s\" not defined, misspelled?", name);
	  p->name = allP->name;
	  p->type = allP->type;
	  ValueType type(allP->type);
	  Value val(type);
	  const char *err = val.parse(value);
	  if (err)
	    return err;
	  if (p->type == OCPI::API::OCPI_String) {
	    p->vString = strdup(val.m_String);
	    p->owned = true;
	  } else
	    p->vULongLong = val.m_ULongLong;
	  p++;
	}
      }
      p->name = NULL;
      return NULL;
    }
  }
}
