srcdir = /home/libofeng/work/php/php-5.2.6/ext/cntwd
builddir = /home/libofeng/work/php/php-5.2.6/ext/cntwd
top_srcdir = /home/libofeng/work/php/php-5.2.6/ext/cntwd
top_builddir = /home/libofeng/work/php/php-5.2.6/ext/cntwd
EGREP = grep -E
SED = /bin/sed
CONFIGURE_COMMAND = './configure' '--with-php-config=/home/s/apps/php/bin/php-config' '--enable-cntwd' '--enable-debug'
CONFIGURE_OPTIONS = '--with-php-config=/home/s/apps/php/bin/php-config' '--enable-cntwd' '--enable-debug'
SHLIB_SUFFIX_NAME = so
SHLIB_DL_SUFFIX_NAME = so
RE2C = exit 0;
AWK = gawk
shared_objects_cntwd = cntwd.lo
PHP_PECL_EXTENSION = cntwd
PHP_MODULES = $(phplibdir)/cntwd.la
all_targets = $(PHP_MODULES)
install_targets = install-modules install-headers
prefix = /home/s/apps/php-5.2.6
exec_prefix = $(prefix)
libdir = ${exec_prefix}/lib
prefix = /home/s/apps/php-5.2.6
phplibdir = /home/libofeng/work/php/php-5.2.6/ext/cntwd/modules
phpincludedir = /home/s/apps/php-5.2.6/include/php
CC = gcc
CFLAGS = -g -O2
CFLAGS_CLEAN = $(CFLAGS)
CPP = gcc -E
CPPFLAGS = -DHAVE_CONFIG_H
CXX = g++
CXXFLAGS = -g -O2
CXXFLAGS_CLEAN = $(CXXFLAGS)
EXTENSION_DIR = /home/s/apps/php-5.2.6/lib/php/extensions/no-debug-non-zts-20060613
PHP_EXECUTABLE = /home/s/apps/php-5.2.6/bin/php
EXTRA_LDFLAGS = -lstdc++
EXTRA_LIBS =
INCLUDES = -I/home/s/apps/php-5.2.6/include/php -I/home/s/apps/php-5.2.6/include/php/main -I/home/s/apps/php-5.2.6/include/php/TSRM -I/home/s/apps/php-5.2.6/include/php/Zend -I/home/s/apps/php-5.2.6/include/php/ext -I/home/s/apps/php-5.2.6/include/php/ext/date/lib
LFLAGS =
LDFLAGS =
SHARED_LIBTOOL =
LIBTOOL = $(SHELL) $(top_builddir)/libtool
SHELL = /bin/sh
INSTALL_HEADERS =
mkinstalldirs = $(top_srcdir)/build/shtool mkdir -p
INSTALL = $(top_srcdir)/build/shtool install -c
INSTALL_DATA = $(INSTALL) -m 644

DEFS = -DPHP_ATOM_INC -I$(top_builddir)/include -I$(top_builddir)/main -I$(top_srcdir)
COMMON_FLAGS = $(DEFS) $(INCLUDES) $(EXTRA_INCLUDES) $(CPPFLAGS) $(PHP_FRAMEWORKPATH)

all: $(all_targets) 
	@echo
	@echo "Build complete."
	@echo "Don't forget to run 'make test'."
	@echo
	
build-modules: $(PHP_MODULES)

libphp$(PHP_MAJOR_VERSION).la: $(PHP_GLOBAL_OBJS) $(PHP_SAPI_OBJS)
	$(LIBTOOL) --mode=link $(CC) $(CFLAGS) $(EXTRA_CFLAGS) -rpath $(phptempdir) $(EXTRA_LDFLAGS) $(LDFLAGS) $(PHP_RPATHS) $(PHP_GLOBAL_OBJS) $(PHP_SAPI_OBJS) $(EXTRA_LIBS) $(ZEND_EXTRA_LIBS) -o $@
	-@$(LIBTOOL) --silent --mode=install cp $@ $(phptempdir)/$@ >/dev/null 2>&1

libs/libphp$(PHP_MAJOR_VERSION).bundle: $(PHP_GLOBAL_OBJS) $(PHP_SAPI_OBJS)
	$(CC) $(MH_BUNDLE_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) $(PHP_GLOBAL_OBJS:.lo=.o) $(PHP_SAPI_OBJS:.lo=.o) $(PHP_FRAMEWORKS) $(EXTRA_LIBS) $(ZEND_EXTRA_LIBS) -o $@ && cp $@ libs/libphp$(PHP_MAJOR_VERSION).so

install: $(all_targets) $(install_targets)

install-sapi: $(OVERALL_TARGET)
	@echo "Installing PHP SAPI module:       $(PHP_SAPI)"
	-@$(mkinstalldirs) $(INSTALL_ROOT)$(bindir)
	-@if test ! -r $(phptempdir)/libphp$(PHP_MAJOR_VERSION).$(SHLIB_DL_SUFFIX_NAME); then \
		for i in 0.0.0 0.0 0; do \
			if test -r $(phptempdir)/libphp$(PHP_MAJOR_VERSION).$(SHLIB_DL_SUFFIX_NAME).$$i; then \
				$(LN_S) $(phptempdir)/libphp$(PHP_MAJOR_VERSION).$(SHLIB_DL_SUFFIX_NAME).$$i $(phptempdir)/libphp$(PHP_MAJOR_VERSION).$(SHLIB_DL_SUFFIX_NAME); \
				break; \
			fi; \
		done; \
	fi
	@$(INSTALL_IT)

install-modules: build-modules
	@test -d modules && \
	$(mkinstalldirs) $(INSTALL_ROOT)$(EXTENSION_DIR)
	@echo "Installing shared extensions:     $(INSTALL_ROOT)$(EXTENSION_DIR)/"
	@rm -f modules/*.la >/dev/null 2>&1
	@$(INSTALL) modules/* $(INSTALL_ROOT)$(EXTENSION_DIR)

install-headers:
	-@if test "$(INSTALL_HEADERS)"; then \
		for i in `echo $(INSTALL_HEADERS)`; do \
			i=`$(top_srcdir)/build/shtool path -d $$i`; \
			paths="$$paths $(INSTALL_ROOT)$(phpincludedir)/$$i"; \
		done; \
		$(mkinstalldirs) $$paths && \
		echo "Installing header files:          $(INSTALL_ROOT)$(phpincludedir)/" && \
		for i in `echo $(INSTALL_HEADERS)`; do \
			if test "$(PHP_PECL_EXTENSION)"; then \
				src=`echo $$i | $(SED) -e "s#ext/$(PHP_PECL_EXTENSION)/##g"`; \
			else \
				src=$$i; \
			fi; \
			if test -f "$(top_srcdir)/$$src"; then \
				$(INSTALL_DATA) $(top_srcdir)/$$src $(INSTALL_ROOT)$(phpincludedir)/$$i; \
			elif test -f "$(top_builddir)/$$src"; then \
				$(INSTALL_DATA) $(top_builddir)/$$src $(INSTALL_ROOT)$(phpincludedir)/$$i; \
			else \
				(cd $(top_srcdir)/$$src && $(INSTALL_DATA) *.h $(INSTALL_ROOT)$(phpincludedir)/$$i; \
				cd $(top_builddir)/$$src && $(INSTALL_DATA) *.h $(INSTALL_ROOT)$(phpincludedir)/$$i) 2>/dev/null || true; \
			fi \
		done; \
	fi

PHP_TEST_SETTINGS = -d 'open_basedir=' -d 'output_buffering=0' -d 'memory_limit=-1'
PHP_TEST_SHARED_EXTENSIONS =  ` \
	if test "x$(PHP_MODULES)" != "x"; then \
		for i in $(PHP_MODULES)""; do \
			. $$i; $(top_srcdir)/build/shtool echo -n -- " -d extension=$$dlname"; \
		done; \
	fi`

test: all
	-@if test ! -z "$(PHP_EXECUTABLE)" && test -x "$(PHP_EXECUTABLE)"; then \
		TEST_PHP_EXECUTABLE=$(PHP_EXECUTABLE) \
		TEST_PHP_SRCDIR=$(top_srcdir) \
		CC="$(CC)" \
			$(PHP_EXECUTABLE) $(PHP_TEST_SETTINGS) $(top_srcdir)/run-tests.php -U -d extension_dir=modules/ $(PHP_TEST_SHARED_EXTENSIONS) tests/; \
	elif test ! -z "$(SAPI_CLI_PATH)" && test -x "$(SAPI_CLI_PATH)"; then \
		INI_FILE=`$(top_builddir)/$(SAPI_CLI_PATH) -r 'echo php_ini_loaded_file();'`; \
		if test "$$INI_FILE"; then \
			$(EGREP) -v '^extension[\t\ ]*=' "$$INI_FILE" > $(top_builddir)/tmp-php.ini; \
		else \
			echo > $(top_builddir)/tmp-php.ini; \
		fi; \
		TEST_PHP_EXECUTABLE=$(top_builddir)/$(SAPI_CLI_PATH) \
		TEST_PHP_SRCDIR=$(top_srcdir) \
		CC="$(CC)" \
			$(top_builddir)/$(SAPI_CLI_PATH) $(PHP_TEST_SETTINGS) $(top_srcdir)/run-tests.php -c $(top_builddir)/tmp-php.ini -U -d extension_dir=$(top_builddir)/modules/ $(PHP_TEST_SHARED_EXTENSIONS) $(TESTS); \
	else \
		echo "ERROR: Cannot run tests without CLI sapi."; \
	fi

utest: all
	-@if test ! -z "$(SAPI_CLI_PATH)" && test -x "$(SAPI_CLI_PATH)"; then \
		INI_FILE=`$(top_builddir)/$(SAPI_CLI_PATH) -r 'echo php_ini_loaded_file();'`; \
		if test "$$INI_FILE"; then \
			$(EGREP) -v '^extension[\t\ ]*=' "$$INI_FILE" > $(top_builddir)/tmp-php.ini; \
		else \
			echo > $(top_builddir)/tmp-php.ini; \
		fi; \
		TEST_PHP_EXECUTABLE=$(top_builddir)/$(SAPI_CLI_PATH) \
		TEST_PHP_SRCDIR=$(top_srcdir) \
		CC="$(CC)" \
			$(top_builddir)/$(SAPI_CLI_PATH) $(PHP_TEST_SETTINGS) $(top_srcdir)/run-tests.php -c $(top_builddir)/tmp-php.ini -u -d extension_dir=$(top_builddir)/modules/ $(PHP_TEST_SHARED_EXTENSIONS) $(TESTS); \
	else \
		echo "ERROR: Cannot run tests without CLI sapi."; \
	fi

ntest: all
	-@if test ! -z "$(SAPI_CLI_PATH)" && test -x "$(SAPI_CLI_PATH)"; then \
		INI_FILE=`$(top_builddir)/$(SAPI_CLI_PATH) -r 'echo php_ini_loaded_file();'`; \
		if test "$$INI_FILE"; then \
			$(EGREP) -v '^extension[\t\ ]*=' "$$INI_FILE" > $(top_builddir)/tmp-php.ini; \
		else \
			echo > $(top_builddir)/tmp-php.ini; \
		fi; \
		TEST_PHP_EXECUTABLE=$(top_builddir)/$(SAPI_CLI_PATH) \
		TEST_PHP_SRCDIR=$(top_srcdir) \
		CC="$(CC)" \
			$(top_builddir)/$(SAPI_CLI_PATH) $(PHP_TEST_SETTINGS) $(top_srcdir)/run-tests.php -c $(top_builddir)/tmp-php.ini -N -d extension_dir=$(top_builddir)/modules/ $(PHP_TEST_SHARED_EXTENSIONS) $(TESTS); \
	else \
		echo "ERROR: Cannot run tests without CLI sapi."; \
	fi

clean:
	find . -name \*.gcno -o -name \*.gcda | xargs rm -f
	find . -name \*.lo -o -name \*.o | xargs rm -f
	find . -name \*.la -o -name \*.a | xargs rm -f 
	find . -name \*.so | xargs rm -f
	find . -name .libs -a -type d|xargs rm -rf
	rm -f libphp$(PHP_MAJOR_VERSION).la $(SAPI_CLI_PATH) $(OVERALL_TARGET) modules/* libs/*

distclean: clean
	rm -f config.cache config.log config.status Makefile.objects Makefile.fragments libtool main/php_config.h stamp-h php5.spec sapi/apache/libphp$(PHP_MAJOR_VERSION).module buildmk.stamp
	$(EGREP) define'.*include/php' $(top_srcdir)/configure | $(SED) 's/.*>//'|xargs rm -f
	find . -name Makefile | xargs rm -f

.PHONY: all clean install distclean test
.NOEXPORT:
cntwd.lo: /home/libofeng/work/php/php-5.2.6/ext/cntwd/cntwd.cpp
	$(LIBTOOL) --mode=compile $(CXX)  -I. -I/home/libofeng/work/php/php-5.2.6/ext/cntwd $(COMMON_FLAGS) $(CXXFLAGS_CLEAN) $(EXTRA_CXXFLAGS)  -c /home/libofeng/work/php/php-5.2.6/ext/cntwd/cntwd.cpp -o cntwd.lo 
$(phplibdir)/cntwd.la: ./cntwd.la
	$(LIBTOOL) --mode=install cp ./cntwd.la $(phplibdir)

./cntwd.la: $(shared_objects_cntwd) $(CNTWD_SHARED_DEPENDENCIES)
	$(LIBTOOL) --mode=link $(CC) $(COMMON_FLAGS) $(CFLAGS_CLEAN) $(EXTRA_CFLAGS) $(LDFLAGS) -o $@ -export-dynamic -avoid-version -prefer-pic -module -rpath $(phplibdir) $(EXTRA_LDFLAGS) $(shared_objects_cntwd) $(CNTWD_SHARED_LIBADD)

