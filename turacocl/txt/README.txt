========================================
                          Turaco CL 0.12

                             2009 Nov 10

                          Scott Lawrence
                        yorgle@gmail.com


========================================
                       Table of Contents


Content:
    1.0 Introduction
        1.1 History
        1.2 What's in a name?
    2.0 System Requirements
    3.0 Currently Supported Games
    4.0 How to Use 
    5.0 TileMaps Explained
    6.0 Environment Variables
    7.0 Command Line Options

Appendices
    A.0 Creating a New Game Driver
    B.0 Building Turaco CL
    C.0 Support
    D.0 Copyright, Credit, License



========================================
----------------------------------------
                            Introduction [1.0]

    TuracoCL allows the graphics stored in the ROMs for certain
    arcade games to be extracted, and rebuilt allowing you to modify
    or add completely new graphics to the game using external paint
    tools.

----------------------------------------
                                 History [1.1]

    Turaco CL is the latest incarnation of Turaco, which was an
    arcade graphics editor for MS-DOS.  Jerry tried to port Turaco
    over to linux, and more modern operating systems, but was unable
    to due to bugs and oddnesses found in the code.  (My bad. -J)
    So, to counteract this, and to eliminate the dependancy on using
    a graphics editor that Scott wrote, Turaco CL imports and exports
    the graphics into common image file formats to be edited in
    your favorite paint program.

    Turaco was the next generation of "AGE", the Arcade Games Editor.
    AGE was under development from November of 1998 until early
    1999.  It started out its life as a font editor that Ivan
    Mackintosh was working on.  Then it was added to by Scott, and
    Chris Moore.  Ivan created the original structure, many drivers,
    and most of the origial functionality.  Scott, who had started
    to write a pacman editor, "Stylist", earlier that spring then
    came by and added bitmap manipulations, oodles more drivers,
    map editing, text editing, and miscellaneous functionalty.
    Chris added the MAME loader, and eventually, the initial version
    of the INI loaders, and hacked MAME to create the INI driver
    files.

    But there was a problem.  Being that it was being added to, and
    added to, and, well, added to, some problems were creeping up.
    We were getting random, but reproducable crashes, the user
    interface was never great, and the user could not modify drivers
    themselves without recompiling the program.  Ivan and Scott
    decided a re-write was in order.  The re-write started about a
    month prior to the release of the final version (v0.7) of AGE.
    Scott decided to use ALLEGRO's GUI functions, and created the
    necessary GUI objects (palette display, palette chooser, bitmap
    editor, sprite palette.)

    Scott did about half of TURACO, with Ivan adding in pretty much
    all of the internal driver handling, palette internals, and
    patch internals, borrowing code from AGE where applicable.
    Chris has only had a ghostly influence, in that it was his
    effort in the INI drivers for AGE that was being re-used, along
    with the drivers created from his MAME hack, since upgraded by
    Ivan.

    Turaco CL is a complete re-write, with a distinct lack of
    depencancy on any sort of graphics library, while still using
    the original INI driver files we had created for Turaco.  This
    time, Scott is the only developer.  Chris and Ivan have had no
    involvement on this incarnation at all.


----------------------------------------
                       What's in a name? [1.2]

    Turaco initially had a slight identity crisis in that we weren't
    sure what to call it.  Scott decided (as he was the one naming
    files, and writing the makefile) to call it "AGE 2", then "New
    Age" and then briefly changed to "Helen Shapiro".  Her lawyers
    contacted the development team, and made them change it.  Scott
    then finally settled upon "TURACO", based on a very pretty bird
    in the zoo in Rochester, NY.

    Turaco CL is short for "Turaco Command Line", since this version
    is a non-graphical, command line version of the Turaco
    functionality.  In order to increase the possibility of confusion,
    the older MS-DOS program, previously known as simply "Turaco"
    is now known as "Turaco Classic".


----------------------------------------
                     System Requirements [2.0]

    Turaco CL requires any machine with a command line shell and a
    POSIX compatible layer.  Most development work on Turaco CL has
    been done under OS X as well as Solaris with the occasional
    builds on Linux and MS-DOS.

----------------------------------------
               Currently supported games [3.0]

    Turaco CL currently supports 3036 games, whose drivers can be
    found in the drivers directory.  Well, not really.  The new
    drivers have been removed from the distribution channels due
    to the fact that they suck ass.

    Tools will be provided later on to list out the supported
    romsets, but that functionality is not provided within Turaco
    CL itself.

    Oh, not all 3036 of those work properly... 
		                           ...but some of them do.

    We recommend that you use the older Turaco Drivers 1.1 pack
    with Turaco CL.

----------------------------------------
                              How to Use [4.0]

    There are basically two different things that you can do with
    Turaco CL.  You can encode ROMs with it, and you can decode
    ROMs with it.

    The way that you specify this is with the "-inf" (INput File)
    parameter.  The only other required command line option is
    either the -drv or the -ini option, which selects the name of
    the driver that we're using.  This is explained in detail in
    section [7.0].

    For example, here is a simple command line to extract the 
    bank 1 graphics from Bunny Blaster:

	turacocl -drv bunnyb -inf ROM

    You can also specify an absolute path to the driver file like so:
    (this will prevent it from searching for the driver)

	turacocl -ini drivers/bunny/bunnyb.ini -inf ROM

    This should create three files:

	bun_1.pcx 	"bunnyb" bank number 1 as a PCX image.

	bun_1c.pcx	A "checkerboard" file. (explained below)

	bun_p1.pcx	The palette key for palette 1.


    These files will be created with similar filenames unless you
    have changed them with the -dbf, -chk and -key options.  

    The palette key image is only useful for 24 bit image file
    formats like PPM files.  It is not generated, nor is it necessary
    for paletted image formats like PCX files.  It doesn't matter
    if the palette key contains colors that are not really in the
    game or in the driver... as long as it is consistant with the
    bank image, it will work fine.  This file is just so that Turaco
    CL knows how to re-encode this 24bit RGB image back into the
    paletted image for the ROM set.  Any colors in your bank that
    do not EXACTLY MATCH a color in the palette key will be set to
    color 0.

    When you edit the bank image file, you should also load in and
    save out the palette key file as well.  This consistency is
    necessary for Turaco CL to encode the graphics properly back
    into a ROM.  Turaco CL can support PPM, PNM and PBM files
    properly, so don't fret when Photo$hop saves it out with this
    new extension with a completely different file size.  It's
    okay.

    Again, if you're using a paletted image format like PCX files,
    just ignore all of this Palette Key image file stuff.

    The checkerboard file is for reference only.  I've found it
    useful to have this at 50% transparency over the bank image in
    Photo$hop so that I know where the sprite bounds are.  Future
    versions of Turaco CL will have more information in this file
    as well so that you can make a font or sprite bank from scratch.

    To take these image files and shove them back into a ROM, you can
    Turaco CL like this:  (Assuming you haven't changed any of the
    filenames.)

	turacocl -drv bunnyb -inf IMG

    If you have changed the filenames, use the appropriate command line
    option to specify them properly.

    Drivers will by default be searched for in the $(TDRIVERS)
    directory, which is explained in section [6.0].

    ROMs will initially be searched for in the local directory, unless
    specified with the -rom option or the -trom option.

    Output roms will be put in the current directory, unless you have
    changed this with the -rod option.


----------------------------------------
                      TileMaps Explained [5.0]

    One common thing that many games do is to use multiple smaller
    sprites to draw a single larger image.  For reference, this is
    done with the title scene and attract sequences for Ms. Pac-Man
    (Midway Logo), Pengo (mountain scene, Pengo logo), Dig-Dug (Dig
    Dug logo and graphics), and in Tapper (Budweiser/Root-Beer
    logo).

    The problem is that these are not always arranged in an order
    such that they are easily editable via the default exported
    bank image file.  The sprites might be out of order, repeated
    or just in an odd order in the graphics bank, making editing
    these images difficult.

    The solution for this is the TileMaps.  For this example, I
    will be referencing a jigsaw puzzle.  If you have never heard
    of a jigsaw puzzle, go to your local toy store and buy one. ;)
    Arrange all of the pieces of the jigsaw puzzle right-side-up
    on the table in front of you, in a grid fashion.  Now number
    these 0, 1, 2, and so on, starting from the upper leftmost one,
    across to the right, then the next row closest to you and so
    on.

    It might look something like this:

	0	1	2	3
	4	5	6	7
	8

    This is analogous to the bank file.  You have a grid of
    startiing pieces/sprites arranged in a known order.

    Now, complete the puzzle.  The resulting puzzle will most likely
    not be in the same order.  In fact, it might look something
    like so:

	3	8	4
	2	5	1
	0	9	6

    This resulting arrangement is analogous to the data contained
    in the "Row"s of the TileMap.  These Rows are stored in a known
    configuration in the driver, so that they can be broken out to
    the proper bank-ordering when importing and exporting.

    In order to keep this working properly, I have devised a system
    where there are "Sets" of "Tilemaps".  Each set has a name,
    and you specify it on the command line with the "-tms" command
    line option.

	    -tms setname

    When extracting the graphics out, the bank image file will be
    produced (as above), as well as an image file for each exported
    bank in the set.

    So, for example, if there were a set in the driver called
    "frogs", and it listed the tilemaps named "burrito" and "taco",
    then the following imaged files should be produced:  

	bun_1.pcx 	"bunnyb" bank number 1 as a PCX image.

	bun_1c.pcx	A "checkerboard" file. (explained below)

	burrito.pcx	The "burrito" tilemap.

	taco.pcx	The "taco" tilemap.


    A couple of important things to note however:

	When encoding these images back into ROM files, if there
	are any instances where a sprite is used more than once,
	the bottom rightmost one will be stored back into the ROM.
	For example, if the tilemap looked like this:
		0 	2	4
		1	4	3
		5	6	4

	    and you have edited it such that the three instances
	of "4" are distinct from eachother, then only the bottom
	right "4" will actually be stored back into the ROM.  That
	sprite will be copied back over the other two "4"s

	Any sprite used in a tilemap will be 'grayed out' in the
	bank file.  Since it will be overwritten by the sprite in
	the tilemap image anyway, it is pointless to have it in
	there as well.
	

    Okay, so you have a game that you want to figure out the tilemaps
    for, so that you can change the graphics in a game.  There are
    a few problems facing you at this point, which I will try to
    explain.

	1) What is necessary in the driver to implement a tilemap
	set for my game?

	    Check out the "inidriv.txt" file for an example.

	    You need to have entries in the "Tilemap" section of
	    the driver.  The "Tilemaps" item needs to contain the
	    correct number of tilemaps in the driver.  The "Sets"
	    item needs to contain the correct number of sets in
	    the driver.  Also, for each set, you need a line that
	    has the form:

		SetN= name X Y Z

	    where "N" is the set number (1, 2, 3... ), "name" is
	    the name of the set (used to select it on the command
	    line) and "X", "Y", ... are used to define which tilemaps
	    are contained in that set.

	    For each Tilemap section, you need the "FName" (used
	    for the filename for the files, the Bank number that
	    the sprite is culled from, as well as the correct Width
	    and Height.  (Height is the number of rows, Width is the
	    number of elements on each row.)

	    Then for each of the Rows, it simply is a list of which
	    sprites get used in that position on the grid.  For
	    the above example, the entry might look like this:

		[Tilemap1]
		    FName = sample
		     Name = The above sample in the document
		     Bank = 1
		    Width = 3
		   Height = 3
		     Row1 = 3 8 4
		     Row2 = 2 5 1
		     Row3 = 0 9 6

	    The Name, Description and Credit are all optional.


	2) How can I figure out which sprites are which in the game?

	    The easiest way to do this is to break out the checkerboard
	    file.  Export the bank that is used out to image files.
	    Now import the checkerboard file as the graphic file
	    using the "-dbf" option.  Next, load up the graphics
	    roms in your favorite emulator for the game (or burn
	    graphics roms and drop them into real arcade hardware)
	    and wait for the screen with the tilemap in it to come
	    up.  Then just make a note of which sprite numbers are
	    where, and put those values into the driver file.

	    Be aware that each position in the checkerboard file
	    is unique up until sprite number 2000.  After that,
	    you're on your own. ;)




----------------------------------------
                   Environment Variables [6.0]

    In order to use Turaco CL, you need to have two environment
    variables set.  These determine where Turaco CL will look for
    the game driver files, as well as where the graphics roms.

    These are the two environment variables that MUST be set:

	TDRIVERS	The path to the root of the ini drivers tree.
			ie:  /var/share/turacoCL/drivers/

	TROMS		The path to the root of the rom tree.  
			ie:  /usr/games/mame/roms/

    You'd set them like so:  (MS-DOS)
	SET TDRIVERS=C:\turacoCL\drivers
	SET TROMS=C:\games\mame\roms

    Bourne Shell:
	TDRIVERS=${HOME}/turacoCL/ini/drivers
	export TDRIVERS
	TROMS=/usr/games/mame/roms
	export TROMS

    C-Shell:
	setenv TDRIVERS ${HOME}/turacoCL/ini/drivers
	setenv TROMS    /usr/games/mame/roms

    etc.
	

----------------------------------------
                    Command Line Options [6.0]

    Syntax:

    turacocl [options ...]


    Help Options:

	-h
	    Display all supported command line options

	-v
	    Display version and build information.  Useful when
	    reporting a bug.


    Common Options: (Encode and Decode)

	-drv Drivername
			Select the driver to be searched for.

	-ini Driver.ini
			Select the absolute path to a driver to use.

	    = one of the above two is REQUIRED =

	-inf IMG
			Convert from Image to ROM (Encode)

	-inf ROM
			Convert from ROM to Image (Decode / Extract / etc.)

	    = one of the above two is REQUIRED =

	-bnk Number
			Select the bank number to use

	-pal Number
			Select the palette number to use

	-key KeyFile.pnm
			Select a color key filename to use

	-rom Directory
			Select the absolute path to the input directory
			for the ROMs

	-ff PCX
			Select paletted PCX image file format (default)
	-ff PNM
			Select 24 bit PNM image file format
	-ff PNG
			Select paletted PNG image format 
			NOTE: Export only for now.
		(use only one of these three.)

	-dmp
			Dump out the driver, as it was loaded in.


    Image to ROM Options: (Encode)

	-rod Directory
			Select the absolute path to the output directory
			for the generated ROMs 


    ROM to Image Options: (Decode)

	-dbf BankImageFile.pcx
			Override the name for the generated file

	-chk CheckerboardFile.pcx
			Override the name for the checkerboard file

	-wid Number
			Select the number of sprites wide that
			generated images will contain
	-trn
			Export the PNG images with transparency enabled
			(Color 0 will be transparent.)


    TileMap Options: (Encode/Decode)

	-tms SetName
			Work with the selected tilemap set.  If it
			was not found, a list of known tilemaps for 
			this driver will be displayed.



========================================
                              Appendices

----------------------------------------
              Creating a New Game Driver [A.0]

    The easiest way to create a new driver for Turaco CL is to copy
    an existing driver from the $(DRIVERS) directory.

    The driver's name (ie: "bunny.ini") will become the directory
    in which the roms are found.  ie:  $(DRIVERS)/bunny/

    The definitions of the structures as seen in the driver file
    can be gleaned from the source to MAME.

    If you change or create a driver, please send it to us!

    Read "inidriv.txt" for an explanation of each field in a driver
    file.


----------------------------------------
                      Building Turaco CL [B.0]

    You should be able to build Turaco CL by simply running the gnu
    make program.  On most systems, this is "gmake", but on OS X,
    it is "make".  On OS X you need to have the Apple-provided
    developers tools installed in order to build Turaco CL.

    No external libraries are needed other than the base install
    of whatever compilers are on your system.

    Current platforms tested against:
	Sun Blade 1000 running SunOS 5.8
	Macintosh PowerBook G4 running Darwin 6.4  (OS X 10.2.4)
	x86 Intel Box running Linux 2.4.20
	x86 Intel Box running PC-DOS 7.0 (MS-DOS compatible)

    The development environment bounces back and forth between the
    Blade and the G4 depending on the time of day. ;)

    Building this under MS-DOS (or PC-DOS) requires that you have
    the DJGPP compiler installed.  It might work well with other
    compilers, but they are untested.  You will need the makefile/file
    tools to build it properly.  There are also two slight modifications
    that need to be made to the Makefile.  They are documented in
    the Makefile.

    DJGPP can be acquired from http://www.delorie.com

    If you wish to compile in PNG support, be sure to enable the
    inclusion of the ImageMagick libraries in the makefile.  This
    functionality is enabled by default.


----------------------------------------
                                 Support [D.0]

    There is currently no primary support line for Turaco.  The
    Yahoo support group has been disbanded due to Yahoo Spam.

    It may still be available online here:

	http://groups.yahoo.com/group/Turaco


----------------------------------------
              Copyright, Credit, License [C.0]

    Turaco CL is written by Scott Lawrence, and is based partially on
    Turaco v1.1.3, 1999-OCT-04:

    Turaco 1.1.3 was written by:
	Scott Lawrence			yorgle@gmail.com
	Ivan Mackintosh

    With contributions by:
	Chris "Zwaxy" Moore		chris.moore@writeme.com
	Marcel "The Sil" Silvius

Legal Mumbo-Jumbo:

    No warranty is neither expressed nor implied for any damages
    which may occur during the proper or improper use of this program
    and its associated files.  Use it at your own risk!

    We make no moral or legal statement about the use of un-licensed
    rom images which are not in your ownrship.  In most cases,
    testing was performed with rom-images for games that we own.
    Many of the "supported" games have not been tested.

    Product and game names are trademarks of their respective companies.
