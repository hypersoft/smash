
# This is the build control panel

M185C = src/mozilla/js-1.8.5/js/src/Makefile

all: smash

$(M185C):
	@make -sf scripts/smash.mk configuration

debug:
	@touch .debugging
	@echo debug mode is ready for clean and configuration

release:
	@$(RM) .debugging
	@echo release mode is ready for clean and configuration

install:
	@make -sf scripts/smash.mk $@

uninstall:
	@make -sf scripts/smash.mk $@

smash: $(M185C) 
	@make -sf scripts/smash.mk $@

clean:
	@make -sf scripts/smash.mk $@

configuration:
	@make -sf scripts/smash.mk $@

.PHONY: all clean configuration install uninstall debug smash
