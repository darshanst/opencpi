# The isim_pf platform.
HdlLibraries=bsv util ocpi
# Configurations=....
SourceFiles=fpgaTop_isim_pf.v mkSimDCP.v ../isim_pf/mkSimIO.v main.v
IsimTop=isim_pf.main
VerilogDefines= TOP=mkTB18 BSV_TIMESCALE=1ns/1ps
include $(OCPI_CDK_DIR)/include/hdl/hdl-platform.mk
