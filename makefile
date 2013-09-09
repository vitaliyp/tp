prefix=/usr/local
tp :
	cc tp.c -o tp

.PHONY : install
install : tp
	install tp $(prefix)/bin

.PHONY : deinstall
deinstall :
	rm $(prefix)/bin/tp

.PHONY : clean
clean :
	rm -r tp

.DEFAULT : tp
