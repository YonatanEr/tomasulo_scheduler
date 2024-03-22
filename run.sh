gcc *.c -Wextra -Wall -Wreturn-type -o sol

rm example_210124_win/my_tracecdb.txt
./sol example_210124_win/cfg.txt example_210124_win/memin.txt example_210124_win/my_regout.txt example_210124_win/my_traceinst.txt example_210124_win/my_tracecdb.txt > example_210124_win/output.txt

rm test_1/my_tracecdb.txt
./sol test_1/cfg.txt test_1/inst_file_meming.txt test_1/my_regout.txt test_1/my_traceinst.txt test_1/my_tracecdb.txt > test_1/output.txt

rm test_2/my_tracecdb.txt
./sol test_2/cfg.txt test_2/inst_file_meming.txt test_2/my_regout.txt test_2/my_traceinst.txt test_2/my_tracecdb.txt > test_2/output.txt

rm test_3/my_tracecdb.txt
./sol test_3/cfg.txt test_3/inst_file_meming.txt test_3/my_regout.txt test_3/my_traceinst.txt test_3/my_tracecdb.txt > test_3/output.txt

rm test_5/my_tracecdb.txt
./sol test_5/cfg.txt test_5/inst_file_meming.txt test_5/my_regout.txt test_5/my_traceinst.txt test_5/my_tracecdb.txt > test_5/output.txt