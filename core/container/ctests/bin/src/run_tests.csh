#!/bin/csh -f

# #####
#
#  Copyright (c) Mercury Federal Systems, Inc., Arlington VA., 2009-2010
#
#    Mercury Federal Systems, Incorporated
#    1901 South Bell Street
#    Suite 402
#    Arlington, Virginia 22202
#    United States of America
#    Telephone 703-413-0781
#    FAX 703-413-0784
#
#  This file is part of OpenCPI (www.opencpi.org).
#     ____                   __________   ____
#    / __ \____  ___  ____  / ____/ __ \ /  _/ ____  _________ _
#   / / / / __ \/ _ \/ __ \/ /   / /_/ / / /  / __ \/ ___/ __ `/
#  / /_/ / /_/ /  __/ / / / /___/ ____/_/ / _/ /_/ / /  / /_/ /
#  \____/ .___/\___/_/ /_/\____/_/    /___/(_)____/_/   \__, /
#      /_/                                             /____/
#
#  OpenCPI is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published
#  by the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  OpenCPI is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with OpenCPI.  If not, see <http://www.gnu.org/licenses/>.
#
########################################################################### #

# quick hack to run all tests
# run this in the binary executables directory by doing: ../bin/src/run_tests.csh
setenv OCPI_RCC_TARGET $OCPI_BUILD_HOST
setenv OCPI_SMB_SIZE 3000000
setenv OCPI_LIBRARY_PATH ../../../../components/lib/rcc
if $OCPI_RUNTIME_HOST == darwin-x86_64 then
  setenv OCPI_RCC_SUFFIX dylib
#  setenv DYLD_LIBRARY_PATH ../../../../lib/$OCPI_RUNTIME_HOST-bin
else
  setenv OCPI_RCC_SUFFIX so
  setenv LD_LIBRARY_PATH ../../../../lib/$OCPI_RUNTIME_HOST-bin
endif
if $#argv == 1 then
  sh -c "./$argv[1] 2> /dev/null | grep Test:"
else
foreach i ( `ls -d test* | grep -v '_main' | grep -v '\.'`)
  echo Running $i...
  sh -c "./$i 2> /dev/null | grep Test:"
end
endif

