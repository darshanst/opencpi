
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
ifndef __HDL_MK__
__HDL_MK__=x
include $(OCPI_CDK_DIR)/include/hdl/hdl-pre.mk
# Makefile fragment for HDL primitives, cores, and workers etc.
ifndef SourceFiles
CompiledSourceFiles:= $(wildcard *.[vV]) $(wildcard *.vhd) $(wildcard *.vhdl)
#$(info csf1: $(CompiledSourceFiles) ok: $(wildcard *.v))
else
CompiledSourceFiles:= $(SourceFiles)
endif
# if no one else does anything, the default library name
# (the library into which the source files are compiled)
# will be the work library - generally when you are not exporting the library
ifndef LibName
LibName=work
else
ifndef InstallLibDir
InstallLibDir=$(OCPI_CDK_DIR)/lib/hdl/$(LibName)
$(InstallLibDir):
	$(AT)mkdir -p $@
endif
endif

# Support for oddly named targets that are not exactly parts
ifndef Parts
Parts=$(HdlTargets)
endif
Families=$(sort $(foreach t,$(Parts),$(call LibraryAccessTarget,$(t))))
ifneq ($(filter-out $(Parts),$(Families)),)
$(foreach t,$(filter-out $(Parts),$(Families)),$(OutDir)target-$(t)): | $(OutDir)
	$(AT)mkdir $@
endif
include $(OCPI_CDK_DIR)/include/hdl/xst.mk

ImportsDir=$(OutDir)imports
$(ImportsDir)::
	$(AT)echo -n

clean::
	rm -r -f $(OutDir)target-* 

cleanimports:
	rm -r -f imports
endif
