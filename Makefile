
name			= dmf

builddir 		= build
releasedir		= $(builddir)/release
debugdir 		= $(builddir)/debug
artifactsdir	= $(builddir)/artifacts
releaseexedir	= bin
includedirs		= include
srcdir			= src
testsdir		= tests
exesdir			= exes
ALLDIRS			= 	\
	$(builddir) 	\
	$(releasedir) 	\
	$(debugdir) 	\
	$(artifactsdir) \
	$(releaseexedir)\
	$(includedirs) 	\
	$(srcdir) 		\
	$(exesdir)

CC				= clang
MMDFLAGS		= -MMD -MP
CFLAGS			= $(addprefix -I, $(includedirs)) -std=c23
CHECKFLAGS		= $(CFLAGS) -DDMF_CHECK -Wall -Wextra -fsyntax-only
DEBUGFLAGS		= $(CFLAGS) -DDMF_BUILD -DDEBUG -g -O0 -fno-omit-frame-pointer -fsanitize=address,undefined $(MMDFLAGS)
RELEASEFLAGS	= $(CFLAGS) -DDMF_BUILD -O3 $(MMDFLAGS)
TESTFLAGS		= $(CFLAGS) -DDMF_BUILD -DDMF_TEST -DDEBUG -g -O0 -fno-omit-frame-pointer -fsanitize=address,undefined -Wall -Wextra

HEADERS			= $(wildcard include/*.h) $(wildcard include/dmf/*.h)

LIB 			= $(builddir)/$(name).a
SRC				= $(wildcard $(srcdir)/*.c)
OBJ				= $(patsubst $(srcdir)/%.c, $(builddir)/%.o, $(SRC))
TESTS			= $(wildcard $(testsdir)/*.c)
TESTEXES		= $(patsubst $(testsdir)/%.c, $(artifactsdir)/%, $(TESTS))
CEXES			= $(wildcard $(exesdir)/*.c)
EXES			= $(patsubst $(exesdir)/%.c, $(releaseexedir)/%, $(CEXES))

RELEASE_LIB 	= $(releasedir)/$(name).a
DEBUG_LIB		= $(debugdir)/$(name).a

RELEASE_OBJ 	= $(patsubst $(srcdir)/%.c, $(releasedir)/%.o, $(SRC))
DEBUG_OBJ		= $(patsubst $(srcdir)/%.c, $(debugdir)/%.o, $(SRC))

REL_WATCH		= $(RELEASE_OBJ) $(SRC)

BUILD_ARTIFACTS	= $(shell find build -name "*.o" -o -name "*.a")

all: release compile_commands.json

-include $(DEBUG_OBJ:.o=.d)
-include $(RELEASE_OBJ:.o=.d)

compile_commands.json: Makefile
	@bear -- make check

# .PHONY: bear
bear: compile_commands.json

ensuredirs:
	@mkdir -p $(ALLDIRS)

$(releasedir)/%.o: $(srcdir)/%.c $(HEADERS) | $(releasedir)
	@$(CC) $(RELEASEFLAGS) -c $< -o $@ || failure "Build Failed! \"$@\""

$(RELEASE_LIB): $(RELEASE_OBJ)
	@ar rcs $@ $^

$(debugdir)/%.o: $(srcdir)/%.c $(HEADERS) | $(debugdir)
	@$(CC) $(DEBUGFLAGS) -c $< -o $@ || failure "Build Failed! \"$@\""

$(DEBUG_LIB): $(DEBUG_OBJ)
	@ar rcs $@ $^

$(artifactsdir)/%: $(testsdir)/%.c $(SRC) $(HEADERS) $(DEBUG_LIB) $(DEBUG_OBJ) | $(testsdir)
	@$(CC) $(TESTFLAGS) $< $(DEBUG_LIB) -o $@

$(releaseexedir)/%: $(exesdir)/%.c $(SRC) $(HEADERS) $(RELEASE_LIB) $(RELEASE_OBJ) | $(exesdir)
	@$(CC) $(RELEASEFLAGS) $< $(RELEASE_LIB) -o $@ || failure "Build failed for executables."

release: ensuredirs $(RELEASE_LIB)

debug: ensuredirs $(DEBUG_LIB)

check:
	@$(CC) $(CHECKFLAGS) $(SRC) $(TESTS) && success "All checks passed!"

test: ensuredirs $(TESTEXES)
	@for t in $(TESTEXES); do $$t; done

bin: ensuredirs $(EXES)

clean:
	@rm -rf build/* && success "The project has been cleansed and blessed by a high priestess." || failure "Dunno what happened. (clean)"