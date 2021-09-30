cmake .. 
make 
cp libtest.so ./../test/
python ./../test/test.py
ps2pdf ./../test/test.ps ./../test/test.pdf