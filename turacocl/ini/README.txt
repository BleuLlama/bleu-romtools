----------------------------------------
        MAME Graphics Driver Dumper Hack

                         Jerry Carpenter
                turacocl@umlautllama.com

	                      2003-03-17

----------------------------------------
                       Driver Generation

    The file in this directory, "fronthlp.c" is from the MAME
    0.65.1 source code.  It is from the src/unix/ directory in
    the source tree.  Replace this file with the one there to
    add the "-dgd" command line option to your MAME build.

    This will create a directory called 'turaco' which contains
    subdirectories whose names are based on the .c file that
    the driver was in.  This is what I'm calling the "grouping
    name".  In each of these directories, there are many .ini
    files for the Turaco drivers.

    It should be noted that there are more fields in this
    version of the drivers over the original Turaco drivers,
    although these should function properly in the older
    versions of Turaco.  If it does not work, let me know.


----------------------------------------
                             Limitations

    FRAC

	The driver generator does not currently support any files
	with Fractional offsets.  I'm not even entirely sure what
	it means.  Regardless, MCR games like "TRON" which worked
	just fine with the old drivers no longer work.


    COLORS

	The driver generator does not currently generate palettes,
	so if you have old palettes from the old Turaco drivers,
	copy them over.  This is due to the fact that you need the
	color roms in place in order to generate these.  Perhaps a
	future version will generate proper palettes, but for now,
	just use old drivers, or make your own palettes.


----------------------------------------
$Id: README.txt,v 1.2 2003/03/31 17:19:55 jerry Exp $
