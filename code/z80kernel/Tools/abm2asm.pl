#!/bin/perl
#
#	Reads in an .abm ascii/absurd bitmap and exports a BLG RLE image
#
# 	2007 Scott Lawrence
#	yorgle@gmail.com
#	http://umlautllama.com

$sourcefile = $ARGV[0];
$destfile = $ARGV[1];

printf "  Reading in %s, outputting to %s\n", $sourcefile, $destfile;

# make sure the source file exists
if( !-e $sourcefile ) {
	printf "ERROR: %s nonexistant!\n", $sourcefile;
	exit( -1 )
}

# make sure the destintation file doesn't exist
if( -e $destfile ) {
	printf "WARNING: %s exists.  Overwriting!\n", $destfile;
}

# trim down the filename to its base
$dfn = (split "/", $destfile)[-1];
$dfn =~ s/.asm//g;

# open the output file
open OF, ">$destfile";


# dump out a header to the file...
printf OF<<EOB;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  $destfile
;;
;;     Map data ASM file generated from $sourcefile
;;
;;  NOTE: THIS IS COMPUTER GENERATED.  DO NOT EDIT IT.
;;        IF YOU DO EDIT IT... WELL... JUST DON'T.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.module $dfn

$dfn:
; This file contains the maps as defined in the source file.
EOB

# open up the source file to parse it in...
open IF, "$sourcefile";

foreach $line( <IF> )
{
	chomp $line;

	if( "=" eq substr $line, 0, 1 )
	{
		($junk, $ch, $val, $cval, $name) = split " ", $line;
		printf OF "\t%10s  = %4d ; %s\n", $name, $val, $ch;
		printf OF "\t%10sC = %4d ; %s\n", $name, $cval, $ch;
		$xref{ $ch } = $val;
		$cols{ $ch } = $cval;
	}

	if( "::" eq substr $line, 0, 2 )
	{
		($identifier, $cidentifier, $rleh) =
					split " ", (substr $line, 2);
		$idx = 0; # array index
	}

	if( ": " eq substr $line, 0, 2 )
	{
		# while passing over it, split each line into an array
		# store that array aside

		for( $x=0 ; $x<28 ; $x++ )
		{
			$v = substr $line, 2+$x, 1;
			if( $xref{$v} eq "" ) {
				printf "ERROR: %s not defined!\n", $v;
			}
			$chars[$idx][$x] = $xref{ $v };
			$colors[$idx][$x] = $cols{ $v };
		}

		$idx++;
	}

	# 0x01 - end of line - skip 16 bytes on the screen
	# 0x02 - end of image
	# 0x03 - 0x0f - repeat next byte this number of times
	# 0x00 - data
	# 0x10 - 0xff - data

sub dmult
{
	my ($the_value, $multiples, $ret);
	$the_value = shift;
	$multiples = shift;
	$ret = "";

	while( $multiples > 15 )
	{
		$multiples -= 15;
		$ret .= dmult( $the_value, 15 );
	}

	if( $multiples < 3 ) {
		for( $n=0 ; $n<$multiples ; $n++ )
		{
			$ret .= sprintf "%s, ", $the_value;
		}
	} else {
		$ret .= sprintf "%d, %s, ", $multiples, $the_value;
	}
	return $ret;
}


	if( ":END" eq $line )
	{
		printf OF "\n;;;;;;;;;;;;;;;;;;;;\n";
		printf OF "%s::\n", $identifier;
		push @chlist, $identifier;
		for( $x=27 ; $x>=0; $x-- )
		{
			printf OF "\t.byte  ";
			$prev = -1;
			$reps = -1;
			for( $y=0 ; $y<$rleh ; $y++ )
			{
				$ch = $chars[$y][$x];

				if( $ch == $prev ) { $reps++ };

				if( $ch != $prev )
				{
					if( $prev != -1 ) {
						printf OF "%s", dmult( $prev, $reps );
					}
					$prev = $ch;
					$reps = 1;
				}
			}
			if( $reps >= 1 ) # leftovers
			{
				printf OF "%s", dmult( $prev, $reps );
			}
			if( $rleh == 16 )
			{
				printf OF "0x01";
			}
			printf OF "\n";
		}
		print OF "\t.byte  0x02 \t; end of image chars\n"; 


		printf OF "\n%s::\n", $cidentifier;
		push @colist, $cidentifier;
		for( $x=27 ; $x>=0; $x-- )
		{
			printf OF "\t.byte  ";
			$prev = -1;
			$reps = -1;
			for( $y=0 ; $y<$rleh ; $y++ )
			{
				$ch = $colors[$y][$x];

				if( $ch == $prev ) { $reps++ };

				if( $ch != $prev )
				{
					if( $prev != -1 ) {
						printf OF "%s", dmult( $prev, $reps );
					}
					$prev = $ch;
					$reps = 1;
				}
			}
			if( $reps >= 1 ) # leftovers
			{
				printf OF "%s", dmult( $prev, $reps );
			}
			if( $rleh == 16 )
			{
				printf OF "0x01";
			}
			printf OF "\n";
		}
		print OF "\t.byte  0x02 \t; end of image colors\n"; 


		if( $idx != $rleh ) {
			print "ERROR: $identifier contains $idx lines!\n";
			print "       It should have $rleh lines.\n";
		}
	}
}


# now dump out the lists
printf OF "\n";
printf OF "; the list of character maps\n";
printf OF "CH_%s::\n", $dfn;
foreach $c (@chlist) { printf OF "\t.word %s\n", $c; }
printf OF "\n";

printf OF "; the list of color maps\n";
printf OF "CO_%s::\n", $dfn;
foreach $c (@colist) { printf OF "\t.word %s\n", $c; }

printf OF "\n";
printf OF "; the count of items in the list\n";
printf OF "CN_%s = (.-CO_%s)/2\n", $dfn, $dfn;

printf OF "\n\n%s_SZ = .-%s\n", $dfn, $dfn; # for size calculations...

close IF;

close OF;
