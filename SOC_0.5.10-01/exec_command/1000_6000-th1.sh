export OMP_NUM_THREADS=1
time ../soc-lm sf=../test-data/t-1000_6000.sv cf=../test-data/t-1000_6000.D200.cv loop=100 rate=0.95 lf=../test-data/t-1000_6000.D200.cv.st.log
