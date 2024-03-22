gcc *.c -Wextra -Wall -Wreturn-type -o sol

rm example_210124_win/my_tracecdb.txt
./sol example_210124_win/cfg.txt example_210124_win/memin.txt example_210124_win/my_regout.txt example_210124_win/my_traceinst.txt example_210124_win/my_tracecdb.txt > example_210124_win/output.txt

rm TO_SUBMIT_TEST_1/my_tracecdb.txt
./sol TO_SUBMIT_TEST_1/cfg.txt TO_SUBMIT_TEST_1/inst_file_meming.txt TO_SUBMIT_TEST_1/my_regout.txt TO_SUBMIT_TEST_1/my_traceinst.txt TO_SUBMIT_TEST_1/my_tracecdb.txt > TO_SUBMIT_TEST_1/output.txt

rm TO_SUBMIT_TEST_2/my_tracecdb.txt
./sol TO_SUBMIT_TEST_2/cfg.txt TO_SUBMIT_TEST_2/inst_file_meming.txt TO_SUBMIT_TEST_2/my_regout.txt TO_SUBMIT_TEST_2/my_traceinst.txt TO_SUBMIT_TEST_2/my_tracecdb.txt > TO_SUBMIT_TEST_2/output.txt
