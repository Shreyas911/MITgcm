Getting started 
---------------

git clone https://github.com/Shreyas911/MITgcm
cd MITgcm
git checkout profiling

Checking whether things work as expected: 
./testreport -tap -adm -t halfpipe_streamice

One would ideally arrive at 
Y Y Y Y 16 > 16 < 16

Some modifications for easier analysis
--------------------------------------

Adapt some of the functions to ease readability (no need to redo these things, they are now in the repo!)
* model/src/the-Model_main.F: adding some debug infos
* pkg/streamice/streamice_advect_thickness.F: commenting some prints
* pkg/streamice/streamice_timestep.F: commenting some prints
* pkg/streamice/streamice_cg_solve.F: commenting some prints
* pkg/streamice/streamice_vel_phistage.F: commenting some prints + MODIFICATION of converged loop (preventing a FP loop which wouldn't be a fixed point)
* pkg/streamice/streamice_vel_solve.F: commenting some prints
* pkg/streamice/streamice_vel_solve_openad.F: commenting some prints


Checking things again (nothing changed!)
./testreport -tap -adm -t halfpipe_streamice

Updating autodiff
-----------------

Two elements need to be done once and for all here: 
1. Remove 'NO' in the C$NOAD FP-LOOP in streamice_vel_solve.H ---> This has been done, but the tests fail
2. Update halfpipe_streamice/code_tap/STREAMICE_OPTIONS.h with to define  ALLOW_STREAMICE_OAD_FP --- WARNING: There seems to be a #undefALLOW_STREAMICE_OAD_FP towards the end of the file!

Updating ADFirstAidKit
----------------------

adStack and adProfile (both C and h file for the latter) are added, once and for all (assuming we no longer need to modify things)
You shouldn't need to change these things for now and Laurent and I (JLB) are likely the only ones modifying these files. 

cp path/to/tapenade/src/ADFirstAidKit/adStack.c tools/TAP_support/ADFirstAidKit
cp path/to/tapenade/src/ADFirstAidKit/adStack.h tools/TAP_support/ADFirstAidKit
cp path/to/tapenade/src/ADFirstAidKit/adProfile.c tools/TAP_support/ADFirstAidKit


Generating things
-----------------

This should be done only once: 

cd verification/halfpipe_streamice/build
../../../tools/genmake2 -mods ../code_tap -tap -optfile ../../../tools/build_options/linux_amd64_gfortran > traceGenmake # Adapt optfile to your own architecture
make depend > traceDepend
make -j tap_adj > traceMake

Profiling in practice
---------------------

In theory, everything could work from here on. However, we need to update the makefile a little for 
1. Handle the profiling option
2. Compile 

For this we add a few things to the Makefile. 
WARNING: If you decide to re run the genmake you will need to update these things. 

Things to do
* Add specific build target for adStack (and adProfile if debug is needed) 
adStack.o : ../../../tools/TAP_support/ADFirstAidKit/adStack.c
        # gcc -D_ADSTACKPROFILE -D_ADSTACKPREFETCH -c ../../../tools/TAP_support/ADFirstAidKit/adStack.c -o adStack.o
        gcc -D_ADSTACKPROFILE -c ../../../tools/TAP_support/ADFirstAidKit/adStack.c -o adStack.o
adProfile.o : ../../../tools/TAP_support/ADFirstAidKit/adProfile.c
        gcc -c ../../../tools/TAP_support/ADFirstAidKit/adProfile.c -o adProfile.o
        # gcc -DDEBUG_PROFILE -c ../../../tools/TAP_support/ADFirstAidKit/adProfile.c -o adProfile.o

* Add call to make sure the results of the profiling are displayed (done through a sed file), to be added after the tapenadecmd
sed -f script_print_results -i streamice_vel_solve_openad_b.f
* Add the -profile option in the tapenade command
$(TAPENADECMD) -b -profile -ext "../../../tools/TAP_support/flow_tap" -head "the_main_loop(fc)/(xx_genarr3d_dummy, xx_genarr2d_dummy, xx_gentim2d_dummy)" $(AD_FILES) streamice_cg_solve.f f95_test_mods.f90 | tee tap_log_b
