#!/usr/bin/perl
use: Fcntl;

$clout = $ARGV[0];
$namelist = $ARGV[1];

$count = 0;
%name = ();
open(NAME,$namelist);
while(<NAME>){
	chomp;
	$name{$count} = $_;
	$count++;
	#seek(MEM, 0, SEEK_SET);
}
close(NAME);

open(MEM,$clout);
while(<MEM>){
	chomp;
	$_ =~ s/^{.*?,//;
	$_ =~ s/}.*$//;
	#print "$_\n";
	@arr = ();
	@arr = split(/,/,$_);
	#print @arr;
	foreach(@arr){
		print "$_ ";
		print "$name{$_}\n";
	}
	print ";;\n";
}
close(MEM);
