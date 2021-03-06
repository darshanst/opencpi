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

#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/mman.h>
#include <string>
#include "fasttime.h"
#include "OcpiUtilMisc.h"
#include "KernelDriver.h"
#include "HdlOCCP.h"
#include "PciDriver.h"
// This should be in OCPIOS.
// This is the linux version (kernel 2.6+).
namespace OCPI {
  namespace HDL {
    namespace PCI {
      namespace OS = OCPI::OS;
      namespace OU = OCPI::Util;
#define PCIDIR "/sys/bus/pci/devices"

      class Device
	: public OCPI::HDL::Device {
	void *m_bar0, *m_bar1;
	uint32_t m_bar0size, m_bar1size;
	int m_fd;
	friend class Driver;
	Device(std::string &name, int fd, ocpi_pci_t &pci, void *bar0, void *bar1)
	  : OCPI::HDL::Device(name, "ocpi-pci-pio"), m_bar0(bar0), m_bar1(bar1),
	    m_bar0size(pci.size0), m_bar1size(pci.size1), m_fd(fd) {
	  uint64_t endpointPaddr, controlOffset, bufferOffset;
	  uint32_t holeOffset, holeEnd;
	  if (pci.bar0 < pci.bar1) {
	    endpointPaddr = pci.bar0;
	    m_endpointSize = pci.bar1 + pci.size1 - endpointPaddr;
	    controlOffset = 0;
	    bufferOffset = pci.bar1 - pci.bar0;
	    holeOffset = pci.size0;
	    holeEnd = pci.bar1 - pci.bar0;
	  } else {
	    endpointPaddr = pci.bar1;
	    m_endpointSize = pci.bar0 + pci.size0 - endpointPaddr;
	    controlOffset = pci.bar0 - pci.bar1;
	    bufferOffset = 0;
	    holeOffset = pci.size1;
	    holeEnd = pci.bar0 - pci.bar1;
	  }
	  const char *cp = name.c_str();
	  if (!strncasecmp("PCI:", cp, 4))
	    cp += 4;
	  unsigned bus = atoi(cp); // does anyone use non-zero PCI domains?
	  OU::formatString(m_endpointSpecific,
			   "ocpi-pci-pio:%u.0x%" PRIx64 ".0x%" PRIx32 ".0x%" PRIx32,
			   bus, endpointPaddr, holeOffset, holeEnd);
	  cAccess().setAccess((uint8_t*)bar0, NULL, controlOffset);
	  dAccess().setAccess((uint8_t*)bar1, NULL, bufferOffset);
	}
	~Device() {
	  if (m_bar0)
	    munmap(m_bar0, m_bar0size);
	  if (m_bar1)
	    munmap(m_bar1, m_bar1size);
	  if (m_fd >= 0)
	    ::close(m_fd);
	}
	static int compu32(const void *a, const void *b) { return *(int32_t*)a - *(int32_t*)b; }
    // Set up the FPGAs clock, assuming it has no GPS.
    // This does two things:
    // 1. calculate the delay when the CPU reads the 64 bit time register.
    //   Ie. the time interval between the actual sampling of the 64 bit clock ON the FPGA and when
    //   the "load" instruction actually returns that value.
    // 2. set the FPGA time value to something close to our system time.
    static inline uint64_t ticks2ns(uint64_t ticks) {
      return (ticks * 1000000000ull + (1ull << 31)) / (1ull << 32);
    }
    static inline int64_t dticks2ns(int64_t ticks) {
      return (ticks * 1000000000ll + (1ll << 31))/ (1ll << 32);
    }
    static inline uint64_t ns2ticks(uint32_t sec, uint32_t nsec) {
      return ((uint64_t)sec << 32ull) + ((nsec + 500000000ull) * (1ull<<32)) /1000000000ull;
    }
    static inline uint64_t now() {
#if 1
      static int x;
      if (!x) {
	fasttime_init();
	x = 1;
      }
      struct timespec tv;
      fasttime_gettime(&tv);
      return ns2ticks(tv.tv_sec, tv.tv_nsec);
#else
#ifdef __APPLE__
      struct timeval tv;
      gettimeofday(&tv, NULL);
      return ns2ticks(tv.tv_sec, tv.tv_usec * 1000);
#else
      struct timespec ts;
      clock_gettime (CLOCK_REALTIME, &ts);
      return ns2ticks(ts.tv_sec, ts.tv_nsec);
#endif
#endif
    }
	void initTime() {
	  unsigned n;
	  uint32_t delta[100];
	  uint32_t sum = 0;
  
	  // Take a hundred samples of round trip time, and sort
	  for (n = 0; n < 100; n++) {
	    // Read the FPGA's time, and set its delta register
	    m_cAccess.set64Register(admin.timeDelta, OccpSpace,
				    m_cAccess.get64Register(admin.time, OccpSpace));
	    // occp->admin.timeDelta = occp->admin.time;
	    // Read the (incorrect endian) delta register
	    delta[n] = (int32_t)swap32(m_cAccess.get64Register(admin.timeDelta, OccpSpace));
	  }
	  qsort(delta, 100, sizeof(uint32_t), compu32);
  
	  // Ignore the slowest 10%
	  for (n = 0; n < 90; n++)
	    sum += delta[n];
	  sum = (sum + 45) / 90;
	  m_timeCorrection = sum;
	  // we have average delay
	  ocpiInfo("For %s: round trip times (ns) deltas: min %"
		   PRIu64 " max %" PRIu64 " avg of best 90: %" PRIu64 "\n",
		   name().c_str(), ticks2ns(delta[0]), ticks2ns(delta[99]), ticks2ns(sum));
	  sum /= 2; // assume half a round trip for writes
	  uint64_t nw1 = now();
	  uint64_t nw1a = nw1 + sum;
	  uint64_t nw1as = swap32(nw1a);
	  m_cAccess.set64Register(admin.time, OccpSpace, nw1as);
	  // set32Register(admin.scratch20, OccpSpace, nw1as>>32);
	  // set32Register(admin.scratch24, OccpSpace, (nw1as&0xffffffff));
	  //uint64_t nw1b = 0; // get64Register(admin.time, OccpSpace);
	  //      uint64_t nw1bs = swap32(nw1b);
	  uint64_t nw2 = 0; // now();
	  uint64_t nw2s = swap32(nw2);
	  m_cAccess.set64Register(admin.timeDelta, OccpSpace, nw1as);
	  uint64_t nw1b = m_cAccess.get64Register(admin.time, OccpSpace);
	  uint64_t nw1bs = swap32(nw1b);
	  int64_t dt = m_cAccess.get64Register(admin.timeDelta, OccpSpace);
	  ocpiDebug("Now delta is: %" PRIi64 "ns "
		    "(dt 0x%"PRIx64" dtsw 0x%"PRIx64" nw1 0x%"PRIx64" nw1a 0x%"PRIx64 " nw1as 0x%"PRIx64
		    " nw1b 0x%"PRIx64" nw1bs 0x%"PRIx64" nw2 0x%"PRIx64" nw2s 0x%"PRIx64" t 0x%lx)",
		    dticks2ns(swap32(dt)), dt, swap32(dt), nw1, nw1a, nw1as, 
		    nw1b, nw1bs, nw2, nw2s, time(0));
#ifndef __APPLE__
	  {
	    struct timespec ts;
	    clock_getres(CLOCK_REALTIME, &ts);
	    ocpiInfo("res: %ld", ts.tv_nsec);
	  }
#endif
	}
	// Load a bitstream via jtag
	void load(const char *fileName) {
	  // FIXME: there should be a utility to run a script in this way
	  char *command, *base = getenv("OCPI_CDK_DIR");
	  if (!base)
	    throw "OCPI_CDK_DIR environment variable not set";
	  asprintf(&command,
		   "%s/scripts/loadBitStreamOnPlatform \"%s\" \"%s\" \"%s\" \"%s\" \"%s\" \"%s\"",
		   base, fileName, name().c_str(), m_platform.c_str(), m_part.c_str(),
		   m_esn.c_str(), m_position.c_str());
	  ocpiInfo("Executing command to load bit stream for device %s: \"%s\"\n",
		   fileName, command);
	  int rc = system(command);
	  const char *err = 0;
	  switch (rc) {
	  case 127:
	    err = "Couldn't execute bitstream loading command.  Bad OCPI_CDK_DIR environment variable?";
	    break;
	  case -1:
	    err = OU::esprintf("Unknown system error (errno %d) while executing bitstream loading command",
			       errno);
	    break;
	  case 0:
	    ocpiInfo("Successfully loaded bitstream file: \"%s\" on HDL device \"%s\"\n",
		     fileName, name().c_str());
	    break;
	  default:
	    err = OU::esprintf("Bitstream loading error (%d) loading \"%s\" on HDL device \"%s\"",
			       rc, fileName, name().c_str());
	  }
	  if (err)
	    throw OU::Error(err);
	}
      };

      Driver::
      Driver()
	: m_pciMemFd(::open(OCPI_DRIVER_MEM, O_RDWR | O_SYNC)),
	  m_useDriver(m_pciMemFd >= 0) {
      }
      Driver::
      ~Driver() {
	if (m_pciMemFd >= 0) {
	  ::close(m_pciMemFd);
	  m_pciMemFd = -1;
	}
      }

      static const char *
      getPciValue(const char *dev, const char *value, char *buf, unsigned len)
      {
	int n, fd;
	// get bars
	n = snprintf(buf, len, "%s/%s/%s", PCIDIR, dev, value);
	if (n <= 0 || (unsigned)n >= len)
	  return "buffer violation";
	fd = ::open(buf, O_RDONLY);
	if (fd < 0)
	  return "can't open descriptor file for PCI device";
	n = read(fd, buf, len - 1); // leave room for the null char
	close(fd);
	if (n <= 0)
	  return "can't read descriptor file for PCI device";
	buf[n] = 0;
	return 0;
      }
      static const char*
      getPciNumber(const char *dev, const char *value, char *buf, unsigned len, unsigned long *np)
      {
	const char *err = getPciValue(dev, value, buf, len);
	if (err)
	  return err;
	errno = 0;
	unsigned long long ull = strtoull(buf, NULL, 0);
	if (ull == ULLONG_MAX && errno == ERANGE)
	  return "unexpected attribute value for PCI Device";
	*np = ull;
	return 0;
      }
      // See if this looks like an appropriate PCI entry
      static bool
      probe(const char *name, unsigned theVendor, unsigned theDevice, unsigned theClass,
	    unsigned theSubClass, Bar *bars, unsigned &nbars, std::string &error) {
	unsigned long domain, bus, deviceN, function, vendor, device, classword;
	const char *err = 0;
	char buf[512], rbuf[512];
	if (sscanf(name, "%lx:%lx:%lx.%ld", &domain, &bus, &deviceN, &function) != 4)
	  err = "bad PCI /sys device name";
	else if ((err = getPciValue(name, "resource", rbuf, sizeof(rbuf))) ||
		 (err = getPciNumber(name, "vendor", buf, sizeof(buf), &vendor)) ||
		 (err = getPciNumber(name, "class", buf, sizeof(buf), &classword)) ||
		 (err = getPciNumber(name, "device", buf, sizeof(buf), &device)) ||
		 (err = getPciValue(name, "config", buf, sizeof(buf))))
	  err = "PCI device attributes not accessible";
	else {
	  unsigned pciClass = classword >> 16, pciSubClass = (classword >> 8) & 0xff;
#if 0
	  printf("dom %ld bus %ld devN %ld func %ld vendor 0x%lx "
		 "device 0x%lx class 0x%x subclass 0x%x\n",
		 domain, bus, deviceN, function, vendor, device, pciClass, pciSubClass);
#endif
	  if (vendor == theVendor && device == theDevice &&
	      pciClass == theClass && pciSubClass == theSubClass) {
	    // device words match, look at BARs in the "resource" file (rbuf)
	    char *r = rbuf;
	    Bar *bar = bars;
	    for (unsigned i = 0; !err && i < nbars; i++) {
	      unsigned long long bottom, top, flags;
	      if (!r || sscanf(r, "%llx %llx %llx\n", &bottom, &top, &flags) != 3) {
		err = "bad resource/bar file contents";
		break;
	      }
	      r = strchr(r, '\n');
	      if (r)
		r++;
	      if (bottom && bottom != top) {
		// Got a bar
		bar->size = top - bottom + 1;
		bar->io = flags & 1;
		bar->address = bottom;
		if (bar->io) {
		  bar->prefetch = false;
		  bar->addressSize = 32;
		} else {
		  bar->prefetch = (flags & 8) != 0;
		  if ((flags & 0x6) == 0)
		    bar->addressSize = 32;
		  else if ((flags & 0x6) == 4)
		    bar->addressSize = 64;
		  else
		    err = "Invalid address space indication";
		}
#if 0
		printf("   BAR %d: 0x%llx to 0x%llx (%lu%s %db %s %s)\n",
		       i, bottom, top,
		       bar->size >= 1024 ? bar->size/1024 : bar->size,
		       bar->size >= 1024 ? "K" : "",
		       bar->addressSize, bar->io ? "io":"mem",
		       bar->prefetch ? "pref" : "npf");
#endif
		bar++;
	      } // end of good bar
	    } // end of bar loop
	    if (!err) {
	      nbars = bar - bars;
	      return true;
	    }
	  }
	}
	if (err)
	  error = err;
	return false;
      }
      unsigned Driver::
      search(const OU::PValue */*params*/, const char **exclude, bool /*discoveryOnly*/,
	     std::string &error) {
	unsigned count = 0;
	const char *dir = m_useDriver ? OCPI_DRIVER_PCI : PCIDIR;
	DIR *pcid = opendir(dir);
	if (!pcid) {
#ifdef OCPI_OS_darwin
	    return 0;
#else
	  if (m_useDriver)
	    return 0;
	  OU::formatString(error, "can't open the %s directory for PCI search", dir);
#endif
	} else {
	  for (struct dirent *ent; error.empty() && (ent = readdir(pcid)) != NULL;)
	    if (ent->d_name[0] != '.') {
	      // Opening implies canonicalizing the name, which is needed for excludes
	      OCPI::HDL::Device *dev;
	      if ((dev = open(ent->d_name, error))) {
		if (exclude)
		  for (const char **ap = exclude; *ap; ap++)
		    if (!strcmp(*ap, dev->name().c_str()))
		      goto skipit; // continue(2);
		if (!found(*dev, error))
		  count++;
	      }
	    skipit:
	      ;
	    }
	  closedir(pcid); // FIXME: try/catch?
	}
	return count;
      }
      
      OCPI::HDL::Device *Driver::
      open(const char *pciName, std::string &error) {
	const char *cp;
	for (cp = pciName; *cp && isdigit(*cp); cp++)
	  ;
	std::string name("PCI:");
	if (*cp)
	  name += pciName;
	else
	  OU::formatStringAdd(name, "0000:%02d:00.0", atoi(pciName));

	void *bar0 = 0, *bar1 = 0;
	ocpi_pci_t pci;
	int fd = -1;
	if (m_useDriver) {
	  std::string devName(OCPI_DRIVER_PCI);
	  devName += '/';
	  devName += name.c_str() + 4;
	  fd = ::open(devName.c_str(), O_RDWR | O_SYNC);
	  if (fd < 0)
	    OU::formatString(error, "Can't open device %s", devName.c_str());
	  else if (ioctl(fd, OCPI_CMD_PCI, &pci))
	    OU::formatString(error, "Can't get pci info for device %s", devName.c_str());
	  else if ((bar0 = mmap(NULL, pci.size0, PROT_READ|PROT_WRITE, MAP_SHARED,
				fd, 0)) == (void*)-1)
	    OU::formatString(error, "Can't mmap %s for bar0", devName.c_str());
	  else if ((bar1 = mmap(NULL, pci.size1, PROT_READ|PROT_WRITE, MAP_SHARED,
				fd, pci.size0)) == (void*)-1)
	    OU::formatString(error, "can't mmap %s for bar1", devName.c_str());
	  // So fd, bar0, bar1, and pci are good here if error.empty()
	} else {
	  Bar bars[6];
	  unsigned nbars = 6;
	  if (probe(name.c_str()+4, OCPI_HDL_PCI_VENDOR_ID, OCPI_HDL_PCI_DEVICE_ID, OCPI_HDL_PCI_CLASS,
		    OCPI_HDL_PCI_SUBCLASS, bars, nbars, error))
	    if (nbars != 2 || bars[0].io || bars[0].prefetch || bars[1].io || bars[1].prefetch ||
		bars[0].addressSize != 32 || bars[0].size != sizeof(OccpSpace))
	      error = "Found PCI device w/ good vendor/device/class, but bars are wrong; skipping it; use lspci";
	    else {
	      pci.bar0 = bars[0].address;
	      pci.bar1 = bars[1].address;
	      pci.size0 = bars[0].size;
	      pci.size1 = bars[1].size;
	      if (m_pciMemFd < 0 && (m_pciMemFd = ::open("/dev/mem", O_RDWR|O_SYNC)) < 0)
		error = "Can't open /dev/mem, forgot sudo?";
	      else if ((bar0 = mmap(NULL, sizeof(OccpSpace), PROT_READ|PROT_WRITE, MAP_SHARED,
				    m_pciMemFd, bars[0].address)) == (void*)-1)
		error = "can't mmap /dev/mem for bar0";
	      else if ((bar1 = mmap(NULL, bars[1].size, PROT_READ|PROT_WRITE, MAP_SHARED,
				    m_pciMemFd, bars[1].address)) == (void*)-1)
		error = "can't mmap /dev/mem for bar1";
	    }
	  else
	    return NULL; // not really an error
	}
	if (error.empty())
	  return new Device(name, fd, pci, bar0, bar1);
	if (bar0)
	  munmap(bar0, pci.size0);
	if (bar1)
	  munmap(bar1, pci.size1);
	if (fd >= 0) {
	  close(fd);
	  fd = -1;
	}
	ocpiBad("When searching for PCI device '%s': %s", pciName, error.c_str());
	return NULL;
      }
      void *Driver::
      map(uint32_t size, uint64_t &phys, std::string &error) {
	// FIXME: mutex
	if (m_pciMemFd == -1) { // This can only happen if there is no driver to open
	  m_pciMemFd = ::open("/dev/mem", O_RDWR | O_SYNC);
	  if (m_pciMemFd < 0) {
	    OU::formatString(error, "Can't open memory /dev/mem for DMA memory.  Forgot sudo or missing driver");
	    return NULL;
	  }
	}
	ocpi_request_t request;
	request.needed = size;
	if (m_useDriver) {
	  if (ioctl(m_pciMemFd, OCPI_CMD_REQUEST, &request)) {
	    OU::formatString(error, "Can't allocate memory size %u for DMA memory", size);
	    return NULL;
	  }
	} else {
	  static uint64_t base = 0, top;
	  static uint64_t pagesize = getpagesize();
	  
	  if (!base) {
	    const char *dma = getenv("OCPI_DMA_MEMORY");
	    if (!dma) {
	      error = "No OCPI_DMA_MEMORY environment setup";
	      return NULL;
	    }
	    unsigned sizeM;
	    if (sscanf(dma, "%uM$0x%llx", &sizeM, (unsigned long long *) &base) != 2) {
	      error = "Bad format for OCPI_DMA_MEMORY environment setup";
	      return NULL;
	    }
	    ocpiDebug("DMA Memory:  %uM at 0x%llx\n", sizeM, (unsigned long long)base);
	    top = base + sizeM * 1024llu * 1024llu;
	    if (base & (pagesize-1)) {
	      base += pagesize - 1;
	      base &= ~(pagesize - 1);
	      top &= ~(pagesize - 1);
	      ocpiDebug("DMA Memory is NOT page aligned.  Now %llu at 0x%llx\n",
			(unsigned long long)(top - base), (unsigned long long)base);
	    }
	  }
	  request.actual = (size + pagesize - 1) & ~(pagesize - 1);
	  if (top - base < request.actual) {
	    OU::formatString(error, "Insufficient memory for request of %u", size);
	    return NULL;
	  }
	  request.address = base;
	  base += request.actual;
	}
	void *vaddr =  mmap(NULL, request.actual, PROT_READ|PROT_WRITE, MAP_SHARED,
			    m_pciMemFd, request.address);
	if (vaddr == (void *)-1) {
	  error = "DMA mmap failure"; // FIXME: a leak of physical memory in this case
	  return NULL;
	}
	phys = request.address;
	return vaddr;
      }
    } // namespace PCI
  } // namespace HDL
} // namespace OCPI
