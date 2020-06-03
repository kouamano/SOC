#!/usr/bin/perl
sub max{
	my ($fm,$lt) = @_;
	if($fm >= $lt){
		return($fm);
	}else{
		return($lt);
	}
}
while(<>){
	chomp;
	$_ =~ s/^\s*//;
	$_ =~ s/\s*$//;
	$_ =~ s/\s+/ /g;
	push(@arr,$_);
}
$maxcount = 0;
foreach(@arr){
	$count = 0;
	$count++ while($_ =~ m/ /g);
	$count = $count + 1;
	$maxcount = max($maxcount,$count);
}
$nline = @arr;
print "$maxcount $nline\n";
foreach(@arr){
	print "$_\n";
}
