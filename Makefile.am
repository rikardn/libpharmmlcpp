ACLOCAL_AMFLAGS = -I m4

SUBDIRS = libpharmmlcpp pharmml2mdl pharmml2poped utils

# For binary win64 release
WIN_RELEASE_PACKAGE = ${PACKAGE_NAME}-${PACKAGE_VERSION}-windows-x64
.PHONY: winrelease
winrelease:
	mkdir -p ${WIN_RELEASE_PACKAGE}
	x86_64-w64-mingw32-strip mdl.exe
	cp mdl.exe ${WIN_RELEASE_PACKAGE}
	cp windep/lib/*.dll ${WIN_RELEASE_PACKAGE}
	cp -r pharmml_internalRelease_0_8_1 ${WIN_RELEASE_PACKAGE}
	zip -r ${WIN_RELEASE_PACKAGE}.zip ${WIN_RELEASE_PACKAGE}
	rm -rf ${WIN_RELEASE_PACKAGE}

LINUX_RELEASE_PACKAGE = ${PACKAGE_NAME}-${PACKAGE_VERSION}-linux-x64
.PHONY: linuxrelease
linuxrelease:
	mkdir -p ${LINUX_RELEASE_PACKAGE}
	strip mdl
	cp mdl ${LINUX_RELEASE_PACKAGE}
	cp -r pharmml_internalRelease_0_8_1 ${LINUX_RELEASE_PACKAGE}
	tar -zcvf ${LINUX_RELEASE_PACKAGE}.tar.gz ${LINUX_RELEASE_PACKAGE}
	rm -rf ${LINUX_RELEASE_PACKAGE}

#Fetch and compile dependencies for windows
.PHONY: windep
windep:
	rm -rf windep
	mkdir -p windep
	mkdir -p windep/include
	mkdir -p windep/lib
	wget -P windep http://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.14.tar.gz
	cd windep; tar xvfz libiconv-1.14.tar.gz
	cd windep/libiconv-1.14/;./configure --host=x86_64-w64-mingw32;make -j4
	cp windep/libiconv-1.14/include/iconv.h windep/include
	cp windep/libiconv-1.14/lib/.libs/libiconv-2.dll windep/lib
	cp windep/libiconv-1.14/lib/.libs/libiconv.dll.a windep/lib
	cp windep/libiconv-1.14/lib/libcharset.dll.a windep/lib
	cp windep/libiconv-1.14/libcharset/lib/.libs/libcharset-1.dll windep/lib
	wget -P windep http://zlib.net/zlib-1.2.11.tar.gz
	cd windep; tar xvfz zlib-1.2.11.tar.gz
	cd windep/zlib-1.2.11/win32; sed -i 's/PREFIX =/PREFIX = x86_64-w64-mingw32-/' Makefile.gcc
	cd windep/zlib-1.2.11; make -f win32/Makefile.gcc
	cp windep/zlib-1.2.11/zconf.h windep/include
	cp windep/zlib-1.2.11/zlib.h windep/include
	cp windep/zlib-1.2.11/zlib1.dll windep/lib
	cp windep/zlib-1.2.11/libz.dll.a windep/lib
	wget -P windep ftp://xmlsoft.org/libxml2/libxml2-2.9.3.tar.gz
	cd windep; tar xvfz libxml2-2.9.3.tar.gz
	cd windep/libxml2-2.9.3; ./configure --host=x86_64-w64-mingw32 --without-python --without-docbook --without-ftp --without-http --without-schematron --with-lzma=no --with-zlib=$(PWD)/windep --with-iconv=$(PWD)/windep; make -j4
#	--without-html --without-legacy --without-regexps --without-sax1 --without-schemas --without-valid --without-xpath 
	cp -r windep/libxml2-2.9.3/include/libxml windep/include
	cp windep/libxml2-2.9.3/.libs/libxml2-2.dll windep/lib
	cp windep/libxml2-2.9.3/.libs/libxml2.dll.a windep/lib
	wget -P windep ftp://xmlsoft.org/libxslt/libxslt-1.1.29.tar.gz
	cd windep; tar xvfz libxslt-1.1.29.tar.gz
	cd windep/libxslt-1.1.29; ./configure --host=x86_64-w64-mingw32 --with-libxml-src=../libxml2-2.9.3 --without-python; make -j4
	cp windep/libxslt-1.1.29/libxslt/.libs/libxslt-1.dll windep/lib
	cp windep/libxslt-1.1.29/libxslt/.libs/libxslt.dll.a windep/lib
	mkdir -p windep/include/libxslt
	cp windep/libxslt-1.1.29/libxslt/*.h windep/include/libxslt
