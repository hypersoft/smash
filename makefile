
# This is the build control panel

M185C = src/mozilla/js-1.8.5/js/src/Makefile
SMASH-BUILDER = build-scripts/smash.mk

all: smash

$(M185C):
	@make -sf $(SMASH-BUILDER) configuration

debug:
	@touch .debugging
	@echo debug mode is ready for clean and configuration

release:
	@$(RM) .debugging
	@echo release mode is ready for clean and configuration

install:
	@make -sf $(SMASH-BUILDER) $@

uninstall:
	@make -sf $(SMASH-BUILDER) $@

smash: $(M185C) 
	@make -sf $(SMASH-BUILDER) $@

clean:
	@make -sf $(SMASH-BUILDER) $@

configuration:
	@make -sf $(SMASH-BUILDER) $@

.PHONY: all clean configuration install uninstall debug smash
