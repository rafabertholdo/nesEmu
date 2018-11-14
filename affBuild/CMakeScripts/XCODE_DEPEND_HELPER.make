# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.nesEmu.Debug:
/Users/rafaelgb/Developer/nesEmu/build/Debug/nesEmu:
	/bin/rm -f /Users/rafaelgb/Developer/nesEmu/build/Debug/nesEmu


PostBuild.nesEmu.Release:
/Users/rafaelgb/Developer/nesEmu/build/Release/nesEmu:
	/bin/rm -f /Users/rafaelgb/Developer/nesEmu/build/Release/nesEmu


PostBuild.nesEmu.MinSizeRel:
/Users/rafaelgb/Developer/nesEmu/build/MinSizeRel/nesEmu:
	/bin/rm -f /Users/rafaelgb/Developer/nesEmu/build/MinSizeRel/nesEmu


PostBuild.nesEmu.RelWithDebInfo:
/Users/rafaelgb/Developer/nesEmu/build/RelWithDebInfo/nesEmu:
	/bin/rm -f /Users/rafaelgb/Developer/nesEmu/build/RelWithDebInfo/nesEmu




# For each target create a dummy ruleso the target does not have to exist
