CC      := gcc
PYTH    := python3
CCFLAGS := -Wall -Werror -pthread -g

B_TARGETS := test_trie_single_threaded test_trie_s_lock test_trie_rw_lock test_trie_hoh_lock
R_TARGETS := single_threaded s_lock rw_lock hoh_lock
ST_TARGETS := single_threaded
MT_TARGETS := s_lock rw_lock hoh_lock

CDIR := comparison/
IPATH := -I .

test_all: $(R_TARGETS)

test_trie_rw_lock.c: test_trie_s_lock.c
	@tail -n +2 $< > $@
	@echo Updated $@

test_trie_hoh_lock.c: test_trie_s_lock.c
	@tail -n +3 $< > $@
	@echo Updated $@

$(B_TARGETS): % : %.c
	@echo Building
	$(CC) -o $@ $^ $(CCFLAGS)

$(ST_TARGETS): % : test_trie_%
	@rm -f -r ./.testout
	@mkdir .testout
	@echo "\n-------------------------------------------"
	@echo "Program Output($<):"
	@echo "-------------------------------------------"
	@-valgrind --leak-check=full --show-leak-kinds=all --log-file="./.testout/memtest_out_$<" ./$<
	@echo "\n-------------------------------------------"
	@echo "Valgrind Output($<):"
	@echo "-------------------------------------------"
	@cat ./.testout/memtest_out_$<

$(MT_TARGETS): % : test_trie_%
	@rm -f -r ./.testout
	@mkdir .testout
	@echo "\n-------------------------------------------"
	@echo "Program Output($<):"
	@echo "-------------------------------------------"
	@-valgrind --leak-check=full --show-leak-kinds=all --log-file="./.testout/memtest_out_$<" ./$<
	@echo "\n-------------------------------------------"
	@echo "Valgrind Memcheck Output($<):"
	@echo "-------------------------------------------"
	@cat ./.testout/memtest_out_$<
	@-valgrind --tool=helgrind --log-file="./.testout/heltest_out_$<" ./$< > /dev/null
	@echo "\n-------------------------------------------"
	@echo "Valgrind Helgrind Output($<):"
	@echo "-------------------------------------------"
	@cat ./.testout/heltest_out_$<

clean: 
	rm -f $(B_TARGETS)
	rm -f -r ./.testout
	rm -f -r ./test_trie_rw_lock.c
	rm -f -r ./test_trie_hoh_lock.c

# Add your rules after this point. Don't edit the things above:

plot: comparison
	@echo Making plots
	$(PYTH) $(CDIR)plot.py

comparison: $(CDIR)comparison_s_lock $(CDIR)comparison_rw_lock $(CDIR)comparison_hoh_lock
	@echo Running comparison
	./$(CDIR)comparison_s_lock $(MTC)
	./$(CDIR)comparison_rw_lock $(MTC)
	./$(CDIR)comparison_hoh_lock $(MTC)

$(CDIR)comparison_s_lock: $(CDIR)comparison_s_lock.c trie.c
	@echo Building $@
	$(CC) -o $@ $^ $(CCFLAGS) $(IPATH)

$(CDIR)comparison_rw_lock: $(CDIR)comparison_rw_lock.c trie.c
	@echo Building $@
	$(CC) -o $@ $^ $(CCFLAGS) $(IPATH)

$(CDIR)comparison_hoh_lock: $(CDIR)comparison_hoh_lock.c trie.c
	@echo Building $@
	$(CC) -o $@ $^ $(CCFLAGS) $(IPATH)

$(CDIR)comparison_rw_lock.c: $(CDIR)comparison_s_lock.c
	@tail -n +2 $< > $@
	@echo Updated $@

$(CDIR)comparison_hoh_lock.c: $(CDIR)comparison_s_lock.c
	@tail -n +3 $< > $@
	@echo Updated $@

cleancomparison:
	@echo Cleaning...
	@rm -rvf $(CDIR)comparison_s_lock $(CDIR)comparison_rw_lock $(CDIR)comparison_hoh_lock
	@rm -rvf $(CDIR)comparison_rw_lock.c $(CDIR)comparison_hoh_lock.c
	@rm -rvf $(CDIR)*.csv
	@rm -rvf $(CDIR)*.png