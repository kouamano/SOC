#!/usr/bin/perl

$/='/';
#$/='~';
my @grand_sum = ();
my @sd = ();
my $num_smpl = 0;
my @mean = ();

while(<>){
	chomp();
	push(@arr,$_);
}
$arr[0] =~ s/(^<[1234567890]+>\n)//;
print $1;

$arr_last = pop(@arr);
@line_last = split('\n',$arr_last);

foreach (@arr) {
	my @sum;
	$_ =~ s/^\n+//;
	$_ =~ s/\n+/\n/;
	@lines = split('\n',$_);
	$size = shift(@lines);
	$size =~ s/\s+/ /;
	@size = split(' ',$size);
	print "$size[0] $size[1]\n";
	for($i=0;$i<$size[0];$i++){
		$sum[$i] = 0;
	}
	foreach (@lines) {
		@oligo_nuc_count = split(' ',$_);
		$ID = shift(@oligo_nuc_count);
		$num_cols = @oligo_nuc_count;
		for($i=0;$i<$num_cols;$i++){
			$sum[$i] += $oligo_nuc_count[$i];
			$grand_sum[$i] += $oligo_nuc_count[$i];
		}
		$num_smpl++;
	}
	print "SUM";
	for($i=0;$i<$num_cols;$i++){
		print " $sum[$i]";
	}
	print "\nMEAN";
	for($i=0;$i<$num_cols;$i++){
		$ave = $sum[$i] / $size[1];
		print " $ave";
	}
	#print "\n/\n";
	print "\n~\n";
}
print "$num_cols $num_smpl\n";
print "SUM";
for($i=0;$i<$num_cols;$i++){
	print " $grand_sum[$i]";
}
print "\nMEAN";
for($i=0;$i<$num_cols;$i++){
	$mean[$i] = $grand_sum[$i] / $num_smpl;
	print " $mean[$i]";
}
foreach (@arr) {
	$_ =~ s/^\n+//;
	$_ =~ s/\n+/\n/;
	@lines = split('\n',$_);
	$size = shift(@lines);
	$size =~ s/\s+/ /;
	@size = split(' ',$size);
	foreach (@lines) {
		@oligo_nuc_count = split(' ',$_);
		$ID = shift(@oligo_nuc_count);
		$num_cols = @oligo_nuc_count;
		for($i=0;$i<$num_cols;$i++){
			$sd[$i] +=  ($oligo_nuc_count[$i] - $mean[$i])**2;
		}
	}
}
print "\nSD";
for($i=0;$i<$num_cols;$i++){
	$sd = $sd[$i] / ($num_smpl - 1);
	$sd = sqrt($sd);
	print " $sd";
}
print "\n";
