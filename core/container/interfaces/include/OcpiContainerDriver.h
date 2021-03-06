
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
/*
 * Abstact:
 *   This file defines classes and templates for the container manager and containers.
 *   It is based on the generic ManagerManager->Manager->Driver->Device stack defined in
 *   OcpiDriverManager.h
 *   Where "Device" is a "Container" object that implements an OpenCPI container.
 *   There is one singleton Container::Manager to manage all container drivers.
 *   Each container driver implements a certain type of container, acting as a factory
 *   for those containers (devices).
 *   The inclusion of the separate "ContainerInterface" file is temporary...FIXME.
 *
 * Revision History: 
 * 
 *    Author: Jim Kulp
 *    Date: 2/2011
 *    Revision Detail: Created
 *
 */

#ifndef OCPI_CONTAINER_DRIVER_H
#define OCPI_CONTAINER_DRIVER_H
#include "OcpiParentChild.h"
#include "OcpiDriverManager.h"
#include "OcpiContainerApi.h"
#include "OcpiTransportGlobal.h"
#include "OcpiUtilImplementation.h"
//#include "OcpiContainerInterface.h"
//#include "OcpiContainerApplication.h"
//#include "OcpiContainerArtifact.h"
//#include "OcpiWorker.h"
//#include "OcpiContainerPort.h"
namespace OCPI {
  namespace Container {
    using OCPI::Util::Child;
    using OCPI::Util::Parent;
    class Driver;
    extern const char *container;
    // This class is what is used when looking for containers
    // It is called back on containers that are suitable.
    // It returns true if the search should stop.
    class Container;
    class Callback {
    protected:
      virtual ~Callback(){};
    public:
      // if true is returned, stop looking further.
      virtual bool foundContainer(Container &i) = 0;
    };
    // The concrete class that manages container drivers
    class Manager : public OCPI::API::ContainerManager,
		    public OCPI::Driver::ManagerBase<Manager, Driver, container> {
      unsigned cleanupPosition();
      OCPI::DataTransport::TransportGlobal &getTransportGlobalInternal(const OCPI::Util::PValue *params);
    public:
      Manager();
      ~Manager();
      OCPI::API::Container *find(const char *model, const char *which,
				 const OCPI::API::PValue *props);
      bool findContainersX(Callback &cb, OCPI::Util::Implementation &i, const char *name); 
      inline static bool findContainers(Callback &cb, OCPI::Util::Implementation &i, const char *name) {
	return getSingleton().findContainersX(cb, i, name);
      }
      void shutdown();
      // convenience
      static inline OCPI::DataTransport::TransportGlobal &
      getTransportGlobal(const OCPI::Util::PValue *params = NULL) {
	return getSingleton().getTransportGlobalInternal(params);
      }
    private:
      // Globals dependant on polling
      OCPI::DataTransport::TransportGlobal *m_tpg_events, *m_tpg_no_events;
    };

    // A base class inherited by all container drivers for common behavior
    class Driver : public OCPI::Driver::DriverType<Manager,Driver> {
    protected:
      Driver(const char *);
    public:
      virtual Container *firstContainer() const = 0;
      virtual Container *findContainer(const char *which) = 0;
      virtual Container *probeContainer(const char *which, std::string &error,
					const OCPI::API::PValue *props = 0) = 0;
      // Any methods specific to container drivers (beyond what drivers do)
      // NONE at this time.
    };
    // A template class directly inherited by derived container drivers, with the
    // requirement that the name of the container driver be passed to the template.
    template <class ConcreteDriver, class ConcreteCont, const char *&name>
    class DriverBase :
      public OCPI::Driver::DriverBase
      <Manager, Driver, ConcreteDriver, ConcreteCont, name>
    {
      Container *firstContainer() const { return Parent<ConcreteCont>::firstChild(); }
      virtual Container *findContainer(const char *which) {
	if (!which)
	  return firstContainer();
	return Parent<ConcreteCont>::findChildByName(which);
      }
    };
    // The convenience class for driver registration/static instantiation
    template<class Driver>
    class RegisterContainerDriver : OCPI::Driver::Registration<Driver> {
    };
  }
}
#endif
