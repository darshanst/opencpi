# Common definition for altera tools
ifndef OCPI_ALTERA_TOOLS_DIR
$(error The variable OCPI_ALTERA_TOOLS_DIR must be defined.)
endif
# The trick here is to filter the output and capture the exit code.
# Note THIS REQUIRES BASH not just POSIX SH due to pipefail option
DoAltera=set -o pipefail; export LM_LICENSE_FILE=$(OCPI_ALTERA_LICENSE_FILE); $(OCPI_ALTERA_TOOLS_DIR)/quartus/bin/$1 2>&1 | sed 's/^.\[0m.\[0;32m//'; set +o pipefail
#; echo HELLO X=\$$?Y)
ifneq (,)
? to establish partitions that you will import into, can you just use qxp as source or do you need a bb module?
need to clearly specify netlist type for partitions?
top down netlist type preservationn may be different than bottom up export type
"design partitions" window may clarify things about the state of partitions
default partition name (in UI) from hierarchy and module names
partition names can have :, |, _
doc 015 is incremental compilation chapter
doc 017 is best practices
?? If you just post_synth results do you get proper optimizations?
is QXP_FILE considered a source file?  why isn't this sufficient to avoid partitions?
TRY THIS AGAIN - no partitions in the upper level design with qxp inputs?
could I be being screwed by lack of settings update?
maybe make a copy of the original, and then let the tools update it anyway
netlist type setting to "empty" - is this automatically determined from a bb file?
This is an explicit indicator that something is bb (like platform needing container)
qxp input files can then be forced to "empty" so that only the bb info is used,
merging re-enables optimizations
There are options for clocks to be global or regional which can help.
Any black box declaration in the source code?
it might make sense to use to statistics and advisor tools to check practices (like unconnected stuff etc.)."incremental compilation advisor"
Look for "messages" on the bb warning we are getting.
netlist type settings should probably avoid any defaults and be explicit.
parameters are checked when resynthesis is necessary
high fanout signals can be set to global in a partition (shouldn't matter wth post_synth)
virtual pins when upper design doesn't use output?
"master_makefile" command for scripts etc...perhaps use this to see what it does?
qxp report by opening the qxp in quartus.
"If you do not designate the .qxp instance as a partition, the software reuses just the post-synthesis compilation results from the .qxp, removes unconnected ports and unused logic just like a regular source file, and then performs placement and routing."
"The import process adds more control than using the .qxp as a source file, and is useful only in the following circumstances:"
"If you are using command-line executables, run quartus_cdb with the --incremental_compilation_export option."
"If you want to use just the post-synthesis information, you can choose whether you want the file to be a partition in the top-level design"

endif
