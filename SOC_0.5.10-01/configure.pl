#!/usr/bin/perl

###################################
## configure.pl                  ##
## last modified 2020.06.15 JST  ##
## configuration program for SOC ##
## to generate Makefile          ##
## by K.AMANO                    ##
###################################

$OS_TYPE = `echo \$OSTYPE`; chomp($OS_TYPE);
$WHO = `whoami`; chomp($WHO);
$OMP = 1;	#for OpenMP
$OMP_SPEC = '';
$CMP = "cc";
$CMP_TYPE = "gcc";
$CC_W_OPT = '-w2';
$CC_OP_OPT = '-O2';
$CC_LINK_OPT = '-lm';
$BODY_FILE = "makefile.body";
$INST = "../bin";
$BRK = 1;	#Mathematica format

$current_dir = `pwd`;
chomp($current_dir);
if(-d "../../include"){
	$additional_inc = "../../include";
}elsif(-d "/home/pub/include"){
	$additional_inc = "/home/pub/include";
}elsif(-d "include"){
	$additional_inc = $current_dir."/include";
}
$additional_inc = "-I".$additional_inc;

if($OS_TYPE =~ /linux/){
	$ICCPATH = `which icc`;
	$FCCPATH = `which fcc`;
	$FCCxPATH = `which fccpx`;
	if($ICCPATH =~ /icc/){
		$CMP = "icc";
	}elsif($FCCPATH =~ /fcc$/){
		$CMP = "fcc";
	}elsif($FCCxPATH =~ /fccpx/){
		$CMP = "fccpx";
	}else{
		$CMP = "gcc";
		$CMP_TYPE = "gcc";
	}
	if($CMP == "gcc"){
		$OMP_SPEC = "-fopenmp";
	}else{
		$OMP_SPEC = "-openmp";
	}
}elsif($OS_TYPE =~ /irix/){
	$CMP = "cc";
	$CC_OP_OPT = "-O";
	$CC_LINK_OPT = "-lm";
	$OMP_SPEC = "-mp";
}elsif($OS_TYPE =~ /hpux/){
	$CMP = "cc";
	$CC_OP_OPT = "-O3";
	$CC_LINK_OPT = "-lm";
	$OMP_SPEC = "+Oopenmp";
}elsif($OS_TYPE =~ /solaris/){
	$CMP = "suncc";
	$CC_W_OPT = '';
	$CC_OP_OPT = '';
	$OMP_SPEC = "-xopenmp";
}elsif($OS_TYPE =~ /darwin/){
	$CMP = "/usr/local/Cellar/gcc/12.2.0/bin/gcc-12";
	$OMP_SPEC = "-fopenmp";
}else{
	$CMP = "gcc";
}

&set_args(@ARGV);
$INST =~ s/^--INSTALL=//;
if($INST eq ''){
	print("type install dir.\n");
	exit;
}
print("who? : $WHO\n");
print("system : $OS_TYPE\n");
print("OpenMP : $OMP\n");
print("BRK_TYPE : $BRK\n");
print("compiler : $CMP\n");
print("includes : $additional_inc\n");

if(-e "$BODY_FILE"){
	system("rm $BODY_FILE");
	system("echo rm $BODY_FILE");
}
system("echo create $BODY_FILE");
open(IN,">$BODY_FILE");
select(IN);
print <<"EOF";

## This file is automaticaly created and appended to head ##
all: soc-init soc-lm soc-dtable soc-split soc-dsort soc-csort soc-smalldsort soc-smallestdsort fasta2matrix

soc-init: soc-init.c soc.h files.h soc.c
	\$(CC) soc-init.c -o soc-init \$(OPT)

soc-lm: soc-lm.c soc.h files.h soc.c learning-loop-monitor.c
	\$(CC) soc-lm.c -o soc-lm \$(OPT)

soc-dtable: soc-dtable.c soc.h files.h soc.c
	\$(CC) soc-dtable.c -o soc-dtable \$(OPT)

soc-split: soc-split.h soc-split.c soc.c
	\$(CC) soc-split.c -o soc-split \$(OPT)

soc-dsort: soc-dsort.c soc.h files.h soc.c
	\$(CC) soc-dsort.c -o soc-dsort \$(OPT)

soc-csort: soc-csort.c soc.h files.h soc.c
	\$(CC) soc-csort.c -o soc-csort \$(OPT)

soc-smalldsort: soc-smalldsort.c soc.h files.h soc.c
	\$(CC) soc-smalldsort.c -o soc-smalldsort \$(OPT)

soc-smallestdsort: soc-smallestdsort.c soc.h files.h soc.c
	\$(CC) soc-smallestdsort.c -o soc-smallestdsort \$(OPT)

fasta2matrix: fasta2matrix.c
	\$(CC) fasta2matrix.c -o fasta2matrix \$(OPT)

cc:
	cc soc-init.c -o soc-init \$(CCOPT) \$(INCOPT)
	cc soc-lm.c -o soc-lm \$(CCOPT) \$(INCOPT)
	cc soc-dtable.c -o soc-dtable \$(CCOPT) \$(INCOPT)
	cc soc-split.c -o soc-split \$(CCWOPT)
	cc soc-dsort.c -o soc-dsort \$(CCOPT) \$(INCOPT)
	cc soc-csort.c -o soc-csort \$(CCOPT) \$(INCOPT)
	cc soc-smalldsort.c -o soc-smalldsort \$(CCOPT) \$(INCOPT)
	cc soc-smallestdsort.c -o soc-smallestdsort \$(CCOPT) \$(INCOPT)
	cc fasta2matrix.c -o fasta2matrix \$(CCOPT) \$(INCOPT)

cc-omp:
	cc soc-init.c -o soc-init \$(CCOPT) \$(OMPOPT) \$(INCOPT)
	cc soc-lm.c -o soc-lm \$(CCOPT) \$(OMPOPT) \$(INCOPT)
	cc soc-dtable.c -o soc-dtable \$(CCOPT) \$(OMPOPT) \$(INCOPT)
	cc soc-split.c -o soc-split \$(CCWOPT)
	cc soc-dsort.c -o soc-dsort \$(CCOPT) \$(OMPOPT) \$(INCOPT)
	cc soc-csort.c -o soc-csort \$(CCOPT) \$(OMPOPT) \$(INCOPT)
	cc soc-smalldsort.c -o soc-smalldsort \$(CCOPT) \$(OMPOPT) \$(INCOPT)
	cc soc-smallestdsort.c -o soc-smallestdsort \$(CCOPT) \$(OMPOPT) \$(INCOPT)
	cc fasta2matrix.c -o fasta2matrix \$(CCOPT) \$(OMPOPT) \$(INCOPT)

icc:
	icc soc-init.c -o soc-init \$(ICCOPT) \$(INCOPT)
	icc soc-lm.c -o soc-lm \$(ICCOPT) \$(INCOPT)
	icc soc-dtable.c -o soc-dtable \$(ICCOPT) \$(INCOPT)
	icc soc-split.c -o soc-split \$(ICCOPT)
	icc soc-dsort.c -o soc-dsort \$(ICCOPT) \$(INCOPT)
	icc soc-csort.c -o soc-csort \$(ICCOPT) \$(INCOPT)
	icc soc-smalldsort.c -o soc-smalldsort \$(ICCOPT) \$(INCOPT)
	icc soc-smallestdsort.c -o soc-smallestdsort \$(ICCOPT) \$(INCOPT)
	icc fasta2matrix.c -o fasta2matrix \$(ICCOPT) \$(INCOPT)

ice:
	ice soc-init.c -o soc-init \$(ICCOPT) \$(INCOPT)
	ice soc-lm.c -o soc-lm \$(ICCOPT) \$(INCOPT)
	ice soc-dtable.c -o soc-dtable \$(ICCOPT) \$(INCOPT)
	ice soc-split.c -o soc-split \$(ICCOPT)
	ice soc-dsort.c -o soc-dsort \$(ICCOPT) \$(INCOPT)
	ice soc-csort.c -o soc-csort \$(ICCOPT) \$(INCOPT)
	ice soc-smalldsort.c -o soc-smalldsort \$(ICCOPT) \$(INCOPT)
	ice soc-smallestdsort.c -o soc-smallestdsort \$(ICCOPT) \$(INCOPT)
	ice fasta2matrix.c -o fasta2matrix \$(ICCOPT) \$(INCOPT)

icc-omp:
	icc soc-init.c -o soc-init \$(ICCOPT) \$(OMPOPT) \$(INCOPT)
	icc soc-lm.c -o soc-lm \$(ICCOPT) \$(OMPOPT) \$(INCOPT)
	icc soc-dtable.c -o soc-dtable \$(ICCOPT) \$(OMPOPT) \$(INCOPT)
	icc soc-split.c -o soc-split \$(ICCOPT)
	icc soc-dsort.c -o soc-dsort \$(ICCOPT) \$(OMPOPT) \$(INCOPT)
	icc soc-csort.c -o soc-csort \$(ICCOPT) \$(OMPOPT) \$(INCOPT)
	icc soc-smalldsort.c -o soc-smalldsort \$(ICCOPT) \$(OMPOPT) \$(INCOPT)
	icc soc-smallestdsort.c -o soc-smallestdsort \$(ICCOPT) \$(OMPOPT) \$(INCOPT)
	icc fasta2matrix.c -o fasta2matrix \$(ICCOPT) \$(OMPOPT) \$(INCOPT)

icc10:
	icc10 soc-init.c -o soc-init \$(ICCOPT) \$(INCOPT)
	icc10 soc-lm.c -o soc-lm \$(ICCOPT) \$(INCOPT)
	icc10 soc-dtable.c -o soc-dtable \$(ICCOPT) \$(INCOPT)
	icc10 soc-split.c -o soc-split \$(ICCOPT)
	icc10 soc-dsort.c -o soc-dsort \$(ICCOPT) \$(INCOPT)
	icc10 soc-csort.c -o soc-csort \$(ICCOPT) \$(INCOPT)
	icc10 soc-smalldsort.c -o soc-smalldsort \$(ICCOPT) \$(INCOPT)
	icc10 soc-smallestdsort.c -o soc-smallestdsort \$(ICCOPT) \$(INCOPT)
	icc10 fasta2matrix.c -o fasta2matrix \$(ICCOPT) \$(INCOPT)

icc9-omp:
	icc9 soc-init.c -o soc-init \$(ICCOPT) \$(OMPOPT) \$(INCOPT)
	icc9 soc-lm.c -o soc-lm \$(ICCOPT) \$(OMPOPT) \$(INCOPT)
	icc9 soc-dtable.c -o soc-dtable \$(ICCOPT) \$(OMPOPT) \$(INCOPT)
	icc9 soc-split.c -o soc-split \$(ICCOPT)
	icc9 soc-dsort.c -o soc-dsort \$(ICCOPT) \$(OMPOPT) \$(INCOPT)
	icc9 soc-csort.c -o soc-csort \$(ICCOPT) \$(OMPOPT) \$(INCOPT)
	icc9 soc-smalldsort.c -o soc-smalldsort \$(ICCOPT) \$(OMPOPT) \$(INCOPT)
	icc9 soc-smallestdsort.c -o soc-smallestdsort \$(ICCOPT) \$(OMPOPT) \$(INCOPT)
	icc9 fasta2matrix.c -o fasta2matrix \$(ICCOPT) \$(OMPOPT) \$(INCOPT)

pgcc-omp:
	pgcc soc-init.c -o soc-init \$(INCOPT)
	pgcc soc-lm.c -o soc-lm -mp \$(INCOPT)
	pgcc soc-dtable.c -o soc-dtable \$(INCOPT)
	pgcc soc-split.c -o soc-split
	pgcc soc-dsort.c -o soc-dsort \$(INCOPT)
	pgcc soc-csort.c -o soc-csort \$(INCOPT)
	pgcc soc-smalldsort.c -o soc-smalldsort \$(INCOPT)
	pgcc soc-smallestdsort.c -o soc-smallestdsort \$(INCOPT)
	pgcc fasta2matrix.c -o fasta2matrix -mp \$(INCOPT)

fcc:
	fcc soc-init.c -o soc-init \$(GCCOPT) \$(INCOPT)
	fcc soc-lm.c -o soc-lm \$(GCCOPT) \$(INCOPT)
	fcc soc-dtable.c -o soc-dtable \$(GCCOPT) \$(INCOPT)
	fcc soc-split.c -o soc-split \$(GCCWOPT)
	fcc soc-dsort.c -o soc-dsort \$(GCCOPT) \$(INCOPT)
	fcc soc-csort.c -o soc-csort \$(GCCOPT) \$(INCOPT)
	fcc soc-smalldsort.c -o soc-smalldsort \$(GCCOPT) \$(INCOPT)
	fcc soc-smallestdsort.c -o soc-smallestdsort \$(GCCOPT) \$(INCOPT)
	fcc fasta2matrix.c -o fasta2matrix \$(GCCOPT) \$(INCOPT)

fccpx:
	fccpx soc-init.c -o soc-init \$(GCCOPT) \$(INCOPT)
	fccpx soc-lm.c -o soc-lm \$(GCCOPT) \$(INCOPT)
	fccpx soc-dtable.c -o soc-dtable \$(GCCOPT) \$(INCOPT)
	fccpx soc-split.c -o soc-split \$(GCCWOPT)
	fccpx soc-dsort.c -o soc-dsort \$(GCCOPT) \$(INCOPT)
	fccpx soc-csort.c -o soc-csort \$(GCCOPT) \$(INCOPT)
	fccpx soc-smalldsort.c -o soc-smalldsort \$(GCCOPT) \$(INCOPT)
	fccpx soc-smallestdsort.c -o soc-smallestdsort \$(GCCOPT) \$(INCOPT)
	fccpx fasta2matrix.c -o fasta2matrix \$(GCCOPT) \$(INCOPT)

gcc:
	gcc soc-init.c -o soc-init \$(GCCOPT) \$(INCOPT)
	gcc soc-lm.c -o soc-lm \$(GCCOPT) \$(INCOPT)
	gcc soc-dtable.c -o soc-dtable \$(GCCOPT) \$(INCOPT)
	gcc soc-split.c -o soc-split \$(GCCWOPT)
	gcc soc-dsort.c -o soc-dsort \$(GCCOPT) \$(INCOPT)
	gcc soc-csort.c -o soc-csort \$(GCCOPT) \$(INCOPT)
	gcc soc-smalldsort.c -o soc-smalldsort \$(GCCOPT) \$(INCOPT)
	gcc soc-smallestdsort.c -o soc-smallestdsort \$(GCCOPT) \$(INCOPT)
	gcc fasta2matrix.c -o fasta2matrix \$(GCCOPT) \$(INCOPT)

gcc44:
	gcc44 soc-init.c -o soc-init \$(GCCOPT) \$(INCOPT)
	gcc44 soc-lm.c -o soc-lm \$(GCCOPT) \$(INCOPT)
	gcc44 soc-dtable.c -o soc-dtable \$(GCCOPT) \$(INCOPT)
	gcc44 soc-split.c -o soc-split \$(GCCWOPT)
	gcc44 soc-dsort.c -o soc-dsort \$(GCCOPT) \$(INCOPT)
	gcc44 soc-csort.c -o soc-csort \$(GCCOPT) \$(INCOPT)
	gcc44 soc-smalldsort.c -o soc-smalldsort \$(GCCOPT) \$(INCOPT)
	gcc44 soc-smallestdsort.c -o soc-smallestdsort \$(GCCOPT) \$(INCOPT)
	gcc44 fasta2matrix.c -o fasta2matrix \$(GCCOPT) \$(INCOPT)

clear:
	rm soc-init soc-lm soc-dtable soc-split soc-dsort soc-csort soc-smalldsort soc-smallestdsort fasta2matrix

install:
	strip ./soc-lm
	strip ./soc-init
	strip ./soc-dtable
	strip ./soc-split
	strip ./soc-dsort
	strip ./soc-csort
	strip ./soc-smalldsort
	strip ./soc-smallestdsort
	strip ./fasta2matrix
	cp ./soc-lm \$(INST_DIR) 
	cp ./soc-init \$(INST_DIR)
	cp ./soc-dtable \$(INST_DIR)
	cp ./soc-split \$(INST_DIR)
	cp ./soc-dsort \$(INST_DIR)
	cp ./soc-csort \$(INST_DIR)
	cp ./soc-smalldsort \$(INST_DIR)
	cp ./soc-smallestdsort \$(INST_DIR)
	cp ./fasta2matrix \$(INST_DIR)
	cp ./refine-mat.pl \$(INST_DIR)
	cp ./mksummary_from_soc_out.pl \$(INST_DIR)
	cp ./add_cluster_origin.pl \$(INST_DIR)
	cp ./drop_sample_zero.pl \$(INST_DIR)
	cp ./drop_unidentifiedFASTA.pl \$(INST_DIR)
EOF
close(IN);
select(STDOUT);
system("echo ... done");

if(-e "makefile.head"){
	system("rm makefile.head");
	system("echo rm makefile.head");
}

system("echo create makefile.head");
system("echo '## This file is automaticaly created ##' > makefile.head");
system("echo 'INCOPT = $additional_inc' >> makefile.head");
system("echo 'INST_DIR = $INST' >> makefile.head");
system("echo 'CCWOPT = $CC_W_OPT' >> makefile.head");
system("echo 'CCOPOPT = $CC_OP_OPT' >> makefile.head");
system("echo 'CCLINKOPT = $CC_LINK_OPT' >> makefile.head");
system("echo 'ICCWOPT = -w1' >> makefile.head");
system("echo 'ICCOPOPT = -O2' >> makefile.head");
system("echo 'ICCLINKOPT = -lm' >> makefile.head");
system("echo 'GCCWOPT = -Wall' >> makefile.head");
system("echo 'GCCOPOPT = -O3' >> makefile.head");
system("echo 'GCCLINKOPT = -lm' >> makefile.head");
system("echo 'CCOPT = \$(CCWOPT) \$(CCOPOPT) \$(CCLINKOPT)' >> makefile.head");
system("echo 'GCCOPT = \$(GCCWOPT) \$(GCCOPOPT) \$(GCCLINKOPT)' >> makefile.head");
system("echo 'ICCOPT = \$(ICCWOPT)' >> makefile.head");
system("echo 'OMPOPT = $OMP_SPEC' >> makefile.head");
system("echo ... done");

if(-e "soc.h"){
	system("echo rm soc.h");
	system("rm soc.h");
}

system("echo create soc.h");
system("echo '/* This file is automaticaly created */' > soc.h");

if($OS_TYPE =~ /linux/){
	system("echo '#define SYS_LINUX' > soc.h");
}elsif($OS_TYPE =~ /irix/){
	system("echo '#define SYS_IRIX' > soc.h");
}elsif($OS_TYPE =~ /hpux/){
	system("echo '#define SYS_HPUX' > soc.h");
}elsif($OS_TYPE =~ /solaris/){
	system("echo '#define SYS_SORALIS' > soc.h");
}elsif($OS_TYPE =~ /darwin/){
	system("echo '#define SYS_DARWIN' > soc.h");
}else{
	system("echo '#define SYS_OTHER' > soc.h");
}

if($OMP == 1){
	system("echo '#define OPEN_MP' >> soc.h");
}

if($BRK == 1){
	system("echo '#define BRK_PRINT 1' >> soc.h");
}elsif($BRK == 2){
	system("echo '#define BRK_PRINT 2' >> soc.h");
}

system("echo ... done");

if(-e "makefile.head"){
	system("echo 'append to makefile.head'");
	if($CMP_TYPE eq "gcc"){
		system("echo 'CC = gcc' >> makefile.head");
		if($OMP == 0){
			system("echo 'OPT =  \$(GCCOPT) \$(INCOPT)' >> makefile.head");
		}elsif($OMP == 1){
			system("echo 'OPT =  \$(GCCOPT) \$(OMPOPT) \$(INCOPT)' >> makefile.head");
		}
	}elsif($CMP eq "icc"){
		system("echo 'CC = icc' >> makefile.head");
		if($OMP == 0){
			system("echo 'OPT =  \$(ICCOPT) \$(INCOPT)' >> makefile.head");
		}elsif($OMP == 1){
			system("echo 'OPT =  \$(ICCOPT) \$(OMPOPT) \$(INCOPT)' >> makefile.head");
		}
	}elsif($CMP eq "fcc"){
		system("echo 'CC = fcc' >> makefile.head");
		if($OMP == 0){
			system("echo 'OPT =  \$(ICCOPT) \$(ICCLINKOPT) \$(INCOPT)' >> makefile.head");
		}elsif($OMP == 1){
			system("echo 'OPT =  \$(ICCOPT) \$(ICCLINKOPT) \$(OMPOPT) \$(INCOPT)' >> makefile.head");
		}
	}elsif($CMP eq "fccpx"){
		system("echo 'CC = fccpx' >> makefile.head");
		if($OMP == 0){
			system("echo 'OPT =  \$(ICCOPT) \$(ICCLINKOPT) \$(INCOPT)' >> makefile.head");
		}elsif($OMP == 1){
			system("echo 'OPT =  \$(ICCOPT) \$(ICCLINKOPT) \$(OMPOPT) \$(INCOPT)' >> makefile.head");
		}
	}elsif($CMP eq "cc"){
		system("echo 'CC = cc' >> makefile.head");
		if($OMP == 0){
			system("echo 'OPT =  \$(CCOPT) \$(INCOPT)' >> makefile.head");
		}elsif($OMP == 1){
			system("echo 'OPT =  \$(CCOPT) \$(OMPOPT) \$(INCOPT)' >> makefile.head");
		}
	}elsif($CMP eq "suncc"){
		system("echo 'CC = /opt/SUNWspro/bin/cc' >> makefile.head");
		if($OMP == 0){
			system("echo 'OPT =  \$(CCOPT) \$(INCOPT)' >> makefile.head");
		}elsif($OMP == 1){
			system("echo 'OPT =  \$(CCOPT) \$(OMPOPT) \$(INCOPT)' >> makefile.head");
		}
	}
	system("echo ... done");
}
if(-e "Makefile"){
	system("echo rm Makefile");
	system("rm Makefile");
}
system("echo create Makefile");
system("cat makefile.head makefile.body > Makefile");
system("echo ... done");
system("touch soc-init.c soc-lm.c soc-dtable.c soc-split.c soc-dsort.c soc-csort.c soc-smalldsort.c soc-smallestdsort.c fasta2matrix.c");

sub set_args{
	$n_args = @_;
	$r = $n_args;
	while($r > 0){
		for($i=0;$i<$n_args;$i++){
			if($_[$i] eq '--with-OpenMP'){
				$OMP = 1;
				$r--;
			}elsif($_[$i] =~ /^--INSTALL=/){
				$INST = $_[$i];
				$r--;
			}elsif($_[$i] eq '--BRKTYPE=Mathematica'){
				$BRK = 1;
				$r--;
			}elsif($_[$i] eq '--BRKTYPE=Lisp'){
				$BRK = 2;
				$r--;
			}elsif($_[$i] eq '--CC=cc'){
				$CMP = "cc";
				$r--;
			}elsif($_[$i] eq '--CC=gcc'){
				$CMP = "gcc";
				$r--;
			}elsif($_[$i] eq '--CC=icc'){
				$CMP = "icc";
				$r--;
			}elsif($_[$i] eq '--CC=suncc'){
				$CMP = "suncc";
				$r--;
			}else{
				print("$_[$i] : unknown option.\n");
				exit;
			}
		}
	}
}
