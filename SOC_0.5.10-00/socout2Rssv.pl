#!/usr/bin/perl

$no = 0;

$line=<>;
$line=<>;

while(<>){
	if($_ =~ /^\//){
		$no++;
		#print $no;
		#print $_;
		$line=<>;
		#print "\n";
	}else{
		chomp;
		print $_;
		print " $no\n";
	}
}
