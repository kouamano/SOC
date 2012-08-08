#!/usr/bin/perl

$/ = '>';
$th = $ARGV[0];
$file = $ARGV[1];
$NT{'A'} = 1;
$NT{'G'} = 1;
$NT{'C'} = 1;
$NT{'T'} = 1;

open(IN,$file);
while(<IN>){
	chomp;
	push(@fasta,$_);
}
close(IN);
shift(@fasta);
foreach(@fasta){
	($head,@tail) = split("\n",$_);
	$tail = join('',@tail);
	$head =~ s/\s+$//;
	$tail =~ s/\s//g;
	$tail =~ tr/a-z/A-Z/;
	$i = &countNT($tail);
	if($i >= $th){
		print ">$head\n";
		print "$tail\n";
		#print "$i\n\n";
	}
}

#--- subroutines ---#

sub countNT {
	my @NT = split(//,$_[0]);
	my $count = 0;
	foreach $c (@NT) {
		if($NT{$c} == 1){
			$count++;
		}
	}
	return($count);
} 
