#!/bin/perl

$resp = `grep SZ *lst`;

@lines = split "\n", $resp;
foreach $line( @lines )
{
	next if( (index $line, "=") != 36);
	$line =~ s/\s+/ /g;
	($l, $r) = split " = ", $line;

	$l = lc $l;

	if( -1 != (index $line, " ")) {
		$l = (split " ", $l)[-1];
		$r = (split " ", $r)[0];
	}
	$l =~ s/_sz//g;
	$l =~ s/_/ /g;

	@fullist[$x] = sprintf "%s::%s", $r, $l;

	$total += hex( $r );
	$x++;
}

@fullist = reverse sort @fullist;

printf "        Section  Hex Size   Decimal    kBytes   %% of full\n";
printf "===============  ========  ==========  =======  =========\n";
		
for( $x=0 ; $x<scalar @fullist ; $x++ )
{
	($r, $l) = split "::", $fullist[$x];

	printf "%15s   0x%s   %4d bytes   %4.2fk    %4.0f%%\n",
			$l, $r, hex( $r ), hex( $r)/1024,
			hex( $r )/$total*100;
}
