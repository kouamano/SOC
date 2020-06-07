#!/usr/bin/perl

while(<>){
	push(@org_lines,$_);
}

#print @org_lines;
@org_dims=split('\s',$org_lines[0]);
#print "org_dims[0]:$org_dims[0]:\n";
#print "org_dims[1]:$org_dims[1]:\n";
@org_first_line=split('\s',$org_lines[1]);
$org_columns=@org_first_line;
#print "$org_columns\n";
for($i=0;$i<$org_dims[0];$i++){
	push(@add_line,0);
}
$add_line = join(' ',@add_line);

if($org_dims[0] == $org_columns){
	$num_clusters = $org_dims[1] + 1;
	print "$org_dims[0] $num_clusters\n";
	shift(@org_lines);
	print @org_lines;
	print "$add_line\n";
}elsif($org_dims[0] == $org_columns-1){
	$num_clusters = $org_dims[1] + 1;
	print "$org_dims[0] $num_clusters\n";
	shift(@org_lines);
	print @org_lines;
	print "ORGIN ";
	print "$add_line\n";
}
