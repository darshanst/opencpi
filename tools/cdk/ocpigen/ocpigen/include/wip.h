
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

#ifndef WIP_H
#define WIP_H
#include <stdint.h>
#include <string.h>
#include <vector>
#include <list>
#include "OcpiPValue.h"
#include "OcpiUtilProperty.h"
#include "OcpiUtilProtocol.h"
#include "OcpiUtilValue.h"
#include "OcpiUtilEzxml.h"
#include "OcpiUtilAssembly.h"
#include "OcpiMetadataWorker.h"
#include "ezxml.h"
#include "cdkutils.h"

namespace OM=OCPI::Metadata;
namespace OE=OCPI::Util::EzXml;
namespace OU=OCPI::Util;
namespace OA=OCPI::API;

#define myCalloc(t, n) ((t *)calloc(sizeof(t), (n)))
inline void *myCrealloc_(void *p, size_t s, size_t o, size_t add) {
  void *np = realloc(p, s*(o + add));
  if (np)
    memset((char *)(np) + o*s, 0, add*s);
  return np;
}
// Add to end, with zeroing
#define myCrealloc(t, p, o, additional) \
  ((t *)myCrealloc_(p, sizeof(t), (o), (additional)))

inline unsigned long roundup(unsigned long n, unsigned long grain) {
  return (n + grain - 1) & ~(grain - 1);
}

enum ControlOp {
#define CONTROL_OP(x, c, t, s1, s2, s3)  ControlOp##c,
  OCPI_CONTROL_OPS
#undef CONTROL_OP
  NoOp
};

class Port;

// We derive a class to implement xi:include parsing, file names, etc.
class Protocol : public OU::Protocol {
public:
  Protocol(Port &port);
  Port &m_port;
  const char *parse(const char *file, ezxml_t prot = NULL);
  const char *parseOperation(ezxml_t op);
};

struct WDI {
  //  unsigned dataValueWidth;
  //  unsigned dataValueGranularity;
  // bool diverseDataSizes;
  // unsigned maxMessageValues;
  bool continuous;
  bool isProducer;
  bool isBidirectional;
  // bool variableMessageLength;
  // bool zeroLengthMessages;
  bool isOptional;
  unsigned minBufferCount;
  unsigned nOpcodes;
  unsigned bufferSize;
};
// OCP_SIGNAL_I(name,
// OCP_SIGNAL_IV(name,
// OCP_SIGNAL_O(
// OCP_SIGNAL_OV(
#define OCP_SIGNALS \
  OCP_SIGNAL_MS(Clk) \
  OCP_SIGNAL_MVR(MAddr, 0) \
  OCP_SIGNAL_MVR(MAddrSpace, 1) \
  OCP_SIGNAL_MVR(MBurstLength, 0) \
  OCP_SIGNAL_MSR(MBurstSingleReq) \
  OCP_SIGNAL_MVR(MByteEn, 0) \
  OCP_SIGNAL_MTR(MCmd, 3) \
  OCP_SIGNAL_MV(MData, 0) \
  OCP_SIGNAL_MV(MDataByteEn, 0) \
  OCP_SIGNAL_MV(MDataInfo, 0) \
  OCP_SIGNAL_MS(MDataLast) \
  OCP_SIGNAL_MS(MDataValid) \
  OCP_SIGNAL_MV(MFlag, 0) \
  OCP_SIGNAL_MSR(MBurstPrecise) \
  OCP_SIGNAL_MVR(MReqInfo, 0)		\
  OCP_SIGNAL_MSR(MReqLast) \
  OCP_SIGNAL_MS(MReset_n) \
  OCP_SIGNAL_MS(MRespAccept) \
  OCP_SIGNAL_SS(SCmdAccept) \
  OCP_SIGNAL_SV(SData, 0) \
  OCP_SIGNAL_SS(SDataAccept) \
  OCP_SIGNAL_SV(SDataInfo, 0) \
  OCP_SIGNAL_SV(SDataThreadBusy, 1) \
  OCP_SIGNAL_SV(SFlag, 0) \
  OCP_SIGNAL_ST(SResp, 2) \
  OCP_SIGNAL_SS(SRespLast) \
  OCP_SIGNAL_SS(SReset_n) \
  OCP_SIGNAL_SV(SThreadBusy, 1) \
  /**/
struct OcpSignalDesc {
  const char *name;
  bool vector;
  bool master;
  unsigned width;
  bool type;
  bool request;
};
// A bit redundant from the above, but for adhoc signals
struct Signal {
  const char *name;
  enum Direction { IN, OUT, INOUT } direction;
  unsigned width;
};
#define OCP_SIGNAL_MT(n,w) OCP_SIGNAL_MV(n,w)
#define OCP_SIGNAL_ST(n,w) OCP_SIGNAL_SV(n,w)
#define OCP_SIGNAL_MS(n) OCP_SIGNAL(n)
#define OCP_SIGNAL_MV(n, w) OCP_SIGNAL(n)
#define OCP_SIGNAL_MSR(n) OCP_SIGNAL(n)
#define OCP_SIGNAL_MVR(n, w) OCP_SIGNAL(n)
#define OCP_SIGNAL_MTR(n, w) OCP_SIGNAL(n)
#define OCP_SIGNAL_SS(n) OCP_SIGNAL(n)
#define OCP_SIGNAL_SV(n, w) OCP_SIGNAL(n)
#define OCP_SIGNAL(n) OCP_##n,
enum OcpSignalEnum {
OCP_SIGNALS
N_OCP_SIGNALS
};
extern OcpSignalDesc ocpSignals[N_OCP_SIGNALS+1];
#undef OCP_SIGNAL
struct OcpSignal {
  uint8_t *value;
  unsigned width;
  const char *signal;
};
union OcpSignals {
  OcpSignal signals[N_OCP_SIGNALS];
  struct {
#define OCP_SIGNAL(n) OcpSignal n;
OCP_SIGNALS
#undef OCP_SIGNAL_MS
#undef OCP_SIGNAL_MV
#undef OCP_SIGNAL_SS
#undef OCP_SIGNAL_SV
#undef OCP_SIGNAL_MSR
#undef OCP_SIGNAL_MVR
#undef OCP_SIGNAL_MTR
#undef OCP_SIGNAL
  };
};

// Generic control attributes

struct WCI {
  char *name;
  bool resetWhileSuspended;
  uint32_t timeout;
};

struct WTI {
  unsigned secondsWidth, fractionWidth;
  bool allowUnavailable;
};
struct WSI {
  WDI wdi;
  bool abortable;
  bool earlyRequest;
  bool regRequest; // request is registered
};
struct WMI {
  WDI wdi;
  bool talkBack;
  uint32_t mflagWidth;// kludge for shep - FIXME
};
struct WMemI {
  bool writeDataFlowControl, readDataFlowControl, isSlave;
  uint64_t memoryWords;
  uint32_t maxBurstLength;
};
enum WIPType{
  NoPort,
  WCIPort,
  WSIPort,
  WMIPort,
  WDIPort, // used temporarily
  WMemIPort,
  WTIPort,
  NWIPTypes
};
struct Clock {
  const char *name;
  const char *signal;
  Port *port;
  bool assembly; // This clock is at the assembly level
};
class Worker;
struct Connection;
union Profiles {
    WDI wdi;
    WSI wsi;
    WMI wmi;
    WCI wci;
    WMemI wmemi;
    WTI wti;
};
class Port {
public:
  const char *name;
  std::string fullNameIn, fullNameOut; // used during HDL generation
  std::string typeNameIn, typeNameOut; // used during HDL generation
  Worker *worker;
  unsigned count;
  bool isExternal;          // external port of an assembly (not part of worker)
  bool isData;		    // data plane port, model independent
  const char *pattern;      // pattern if it overrides global pattern
  WIPType type;
  unsigned dataWidth;
  unsigned byteWidth; // derived
  bool impreciseBurst;// used in multiple types, but not all
  bool preciseBurst;  // used in multiple types, but not all
  Clock *clock;
  Port *clockPort; // used temporarily
  bool myClock;
  OcpSignals ocp;
  uint8_t *values;
  bool master;
  Protocol *protocol; // a pointer to make copying easier
  union Profiles {
    WDI wdi;
    WSI wsi;
    WMI wmi;
    WCI wci;
    WMemI wmemi;
    WTI wti;
  } u;
  Port();
  // Are master signals inputs at this port?
  inline bool masterIn() {
  return
    type == WCIPort ? 1 :
    type == WTIPort ? 1 :
    type == WMemIPort ? (u.wmemi.isSlave ? 1 : 0) :
    type == WMIPort ? 0 :
    type == WSIPort ? (u.wdi.isProducer ? 0 : 1) :
    false;
  }
};

class LocalMemory {
  public:
    LocalMemory ( )
      : name ( 0 ),
        sizeOfLocalMemory ( 0 )
    {
      // Empty
    }
    const char* name;
    unsigned int sizeOfLocalMemory;
};

typedef std::list<OU::Property *> Properties;
typedef Properties::const_iterator PropertiesIter;
class Control {
 public:
  Control();
  uint64_t sizeOfConfigSpace;
  bool writables;
  bool readables;
  bool sub32Bits;
  bool volatiles;
  unsigned nRunProperties; // all but non-readable parameters.
  uint32_t controlOps; // bit mask
  Properties properties;
  unsigned offset;// temporary while properties are being parsed.
  unsigned ordinal; // ditto
};

enum Endian {
  NoEndian,
  Neutral,
  Big,
  Little,
  Static,
  Dynamic
};

enum Language {
  NoLanguage,
  Verilog,
  VHDL
};


struct InstancePort;
struct Connection {
  OCPI::Util::Assembly::Connection *connection; // connection in the underlying generic assembly
  const char *name;   // signal
  InstancePort *ports;
  //  unsigned nConsumers, nProducers, nBidirectionals,
  unsigned nPorts;
  unsigned nExternals;
  Clock *clock;
  const char *masterName, *slaveName;
  InstancePort *external; // external assembly port
};
struct InstanceProperty {
  OU::Property *property;
  OU::Value value;
};
struct Instance {
  OCPI::Util::Assembly::Instance *instance; // instance in the underlying generic assembly
  const char *name;
  const char *wName;
  Worker *worker;
  Clock **clocks;      // mapping of instance's clocks to assembly clocks
  InstancePort *ports;
  uint32_t index;      // index within container
  enum {
    Application, Interconnect, IO, Adapter
  } iType;
  const char *attach;  // external node port this worker is attached to for io or interconnect
  unsigned nValues;    // number of property values
  InstanceProperty *properties;
  bool hasConfig;      // hack for adapter configuration FIXME make normal properties
  uint32_t config;
};
struct OcpAdapt {
  const char *expr;
  const char *comment;
  const char *signal;
  OcpSignalEnum other;
};
struct InstancePort {
  Instance *instance;
  Connection *connection;
  InstancePort *nextConn;
  Port *port;  // The actual port of the instance's or assembly's worker
  OU::Assembly::External *external;
  const char *name;
  unsigned ordinal; // ordinal for external array ports (e.g. WCI)
  Port *externalPort;
  // Information for making the connection, perhaps tieoff etc.
  OcpAdapt ocp[N_OCP_SIGNALS];
};
typedef std::list<Worker *> Workers;
typedef Workers::const_iterator WorkersIter;
class Assembly {
 public:
  Assembly();
  bool isContainer;
  Worker *outside;
  Workers workers;
  unsigned nInstances;
  Instance *instances;
  unsigned nConnections;
  Connection *connections;
  OU::Assembly *assembly;
};

enum Model {
  NoModel,
  HdlModel,
  RccModel,
  OclModel
};
class Worker {
 public:
  Worker();
  Model model;
  const char *modelString;
  bool isDevice;
  bool noControl; // no control port on this one.
  const char *file, *specFile;
  const char *implName;
  const char *specName;
  const char *fileName;
  bool isThreaded;
  Control ctl;
  std::vector<Port*> ports;
  std::vector<LocalMemory*> localMemories;
  unsigned nClocks;
  Clock *clocks;
  Endian endian;
  const char 
    *pattern,                     // pattern for signal names within ports
    *portPattern;                 // pattern for port names
  const char *staticPattern;      // pattern for rcc static methods
  bool isAssembly;
  int defaultDataWidth;           // initialized to -1 to allow zero
  unsigned nInstances;
  Language language;
  Assembly assembly;
  unsigned nSignals;
  Signal *signals;
};

#if 0
// Are master signals inputs at this port?
static inline bool masterIn(Port *p) {
  return
    p->type == WCIPort ? 1 :
    p->type == WTIPort ? 1 :
    p->type == WMemIPort ? (p->u.wmemi.isSlave ? 1 : 0) :
    p->type == WMIPort ? 0 :
    p->type == WSIPort ? (p->u.wdi.isProducer ? 0 : 1) :
    false;
}
#endif

#define SKEL "_skel"
#define IMPL "_impl"
#define DEFS "_defs"
#define ASSY "_assy"
#define VHD ".vhd"
#define VER ".v"
#define VERH ".vh"
#define BOOL(b) ((b) ? "true" : "false")
extern const char
  *container, *platform, *device, *load,
  *openOutput(const char *name, const char *outDir, const char *prefix,
	      const char *suffix, const char *ext, const char *other, FILE *&f),
  *propertyTypes[],
  *controlOperations[],
  *parseHdlAssy(ezxml_t xml, Worker *aw),
  *parseRccAssy(ezxml_t xml, const char *file, Worker *aw),
  *parseOclAssy(ezxml_t xml, const char *file, Worker *aw),
  *parseFile(const char *file, const char *parent, const char *element,
	     ezxml_t *xp, const char **xfile, bool optional = false),
  *pattern(Worker *w, Port *p, int n0, unsigned n1, bool in, bool master,
	   std::string &, bool port = false),
  *parseWorker(const char *file, const char *parent, Worker *),
  *deriveOCP(Worker *w),
  *emitDefsHDL(Worker*, const char *, bool wrap = false),
  *emitImplHDL(Worker*, const char *, const char *),
  *emitImplRCC(Worker*, const char *, const char *),
  *emitImplOCL(Worker*, const char *, const char *),
  *emitSkelHDL(Worker*, const char *),
  *emitSkelRCC(Worker*, const char *),
  *emitSkelOCL(Worker*, const char *),
  *emitBsvHDL(Worker*, const char *),
  *emitArtHDL(Worker *, const char *root, const char *wksFile),
  *emitArtRCC(Worker *, const char *root),
  *emitArtOCL(Worker *, const char *root),
  *emitWorkersHDL(Worker*, const char *, const char *file),
  *emitAssyHDL(Worker*, const char *);

extern void
  addInclude(const char *),
  addDep(const char *dep, bool child),
  emitWorker(FILE *f, Worker *w),
  cleanWIP(Worker *w);

extern uint64_t ceilLog2(uint64_t n);

#endif
