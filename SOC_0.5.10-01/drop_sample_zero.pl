#!/usr/bin/perl

while(<>){
	push(@org_lines,$_);
}

@org_dims_AND_nums = split(/\s+/,$org_lines[0]);
$org_dims = @org_dims_AND_nums[0];
$org_nums = @org_dims_AND_nums[1];
$org_second_line = $org_lines[1];
@org_second_line = split(/\s+/,$org_second_line);
$columns_second_line = @org_second_line;

if($columns_second_line == $org_dims+1){
	$j = 0;
	shift(@org_lines);
	foreach(@org_lines){
		chomp;
		@columns = split(/\s+/,$_);
		$head = shift(@columns);
		$err = 0;
		foreach(@columns){
			$err = $err + ($_ ** 2);
		}
		if($err != 0){
			$arr[$j] = join(' ',$head,@columns,"\n");
			$j++;
		}
	}
}elsif($columns_second_line == $org_dims){
	$j = 0;
	shift(@org_lines);
	foreach(@org_lines){
		$err = 0;
		chomp;
		@columns = split(/\s+/,$_);
		foreach(@columns){
			$err = $err + ($_ ** 2);
		}
		if($zero != 0){
			$arr[$j] = join(' ',@columns,"\n");
			$j++;
		}
	}

}
print "$org_dims $j\n";
print @arr;
