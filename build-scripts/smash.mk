RM := rm -rf

smash_installation_path = /bin/smash
smash_resource_path = /usr/local/lib/smash

root_nopped = "make will not perform building of target: $@; as the root user."
target_operation_complete = echo 'Finished building: $@'
source_operation_complete = echo 'Finished building: $<'
building_target = echo 'Building target: $@'
compiling_source = echo 'Building source: $<'

operation_cancelled = will not install smash because
account_has_no_authority = your user account does not have the filesystem rights

$(shell test -e .debugging)
ifeq ($(.SHELLSTATUS), 0)
   build_path = build/debug
   DEBUG_FLAGS = -g3
else
   build_path = build/release
   DEBUG_FLAGS = -O3
endif

bin2inc = build-scripts/bin2inc
bin2inc_source_files = $(shell scripts/get-files src -name '*.b2i')

# All Target
all: build-support smash

build-support: bin2inc

bin2inc: $(bin2inc)

$(bin2inc): src/hypersoft/bin2inc/bin2inc.c
	gcc $< -o $@

%.b2i: ${bin2inc}

%.b2i: %.b2i.sh.in
	@cat $< | bash > $@

%.c: %.b2i
	@mkdir -p `dirname $@`;
	@$(bin2inc) `basename $<` < $< > $@

smash: $(build_path)/mozjs/lib/libjs_static.a $(build_path)/StringBuffer/libStringBuffer.a $(build_path)/bin/smash

$(build_path)/smash/smash.o: src/hypersoft/smash/smash.cpp
	@if test $$UID == 0; then echo $(root_nopped)  >&2; false; fi;
	@$(building_source)
	@mkdir -vp $(build_path)/smash;
	@echo 'Invoking: GCC C++ Compiler'
	g++ $(DEBUG_FLAGS) -DSMASH_RESOURCE_PATH=$(smash_resource_path) -DSMASH_MAIN_LOADER=$(smash_resource_path)/smash.jsh -I"src/hypersoft/StringBuffer" -I"$(build_path)/mozjs/include" -Wall -c -fmessage-length=0 -MMD -MP -MF"$(build_path)/smash/smash.d" -MT"$(build_path)/smash/smash.d" -o "$@" "$<"
	@$(source_operation_complete)
	@echo ' '

$(build_path)/StringBuffer/StringBuffer.o: src/hypersoft/StringBuffer/StringBuffer.c
	@if test $$UID == 0; then echo $(root_nopped)  >&2; false; fi;
	@$(building_source)
	@mkdir -vp $(build_path)/StringBuffer;
	@echo 'Invoking: GCC C Compiler'
	gcc $(DEBUG_FLAGS) -Wall -c -fmessage-length=0 -MMD -MP -MF"$(build_path)/StringBuffer/StringBuffer.d" -MT"$(build_path)/StringBuffer/StringBuffer.d" -o "$@" "$<"
	@$(source_operation_complete)
	@echo ' '

$(build_path)/StringBuffer/libStringBuffer.a: $(build_path)/StringBuffer/StringBuffer.o
	@if test $$UID == 0; then echo $(root_nopped)  >&2; false; fi;
	@$(building_target)
	@echo 'Invoking: GCC Archiver'
	ar -r  "$@" "$<"
	@$(target_operation_complete)
	@echo ' '

$(build_path)/mozjs/lib/libjs_static.a:
	@if test $$UID == 0; then echo $(root_nopped)  >&2; false; fi;
	@mkdir -vp $(build_path)/mozjs;
	@build-scripts/monkey.sh 

$(build_path)/bin/smash: $(build_path)/smash/smash.o $(build_path)/StringBuffer/libStringBuffer.a $(build_path)/mozjs/lib/libjs_static.a
	@if test $$UID == 0; then echo $(root_nopped)  >&2; false; fi;
	@$(building_target)
	@mkdir -vp $(build_path)/bin;
	@echo 'Invoking: GCC C++ Linker'
	g++ $(DEBUG_FLAGS) -o $@ $^ -lreadline -lpthread `pkg-config --libs nspr`
	@$(target_operation_complete)
	@echo ' '

# Other Targets
clean:
	-@$(RM) $(build_path)
	-@build-scripts/monkey.sh $@
	-@echo ' '

install:
	@if test '!' -e $(build_path)/bin/smash; then { echo "you must call \`\$ make smash'; before calling $@"; false; }; fi;
	@cp -v $(build_path)/bin/smash $(smash_installation_path);
	@mkdir -vp $(smash_resource_path);
	@echo "installing newest smash resources at: $(smash_resource_path) ...";
	@cp -vR -u src/hypersoft/smash/scripts/* $(smash_resource_path)
	@echo ''
	
uninstall:
	@rm -vRd $(smash_resource_path)
	@rm -v $(smash_installation_path)

configuration:
	@if test $$UID == 0; then echo $(root_nopped)  >&2; false; fi;
	@build-scripts/monkey.sh $@
