DESCRIPTION = "Yet Another JSON Library - A Portable JSON parsing and serialization library in ANSI C"
LICENSE = "MIT"

LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/GPL-2.0-only;md5=801f80980d171dd6425610833a22dbe6"

SRC_URI = "file://libwebsockets/libwebsockets-4.0.20.tar.gz"

FILESPATH_prepend += "${LOCAL_FILES}/${BPN}:"
DL_DIR = "${LOCAL_FILES}"
S = "${WORKDIR}/${BPN}-${PV}"

inherit cmake

DEPENDS = "zlib openssl"

EXTRA_OECMAKE = "-D LWS_WITH_HTTP2=ON -D LWS_IPV6=ON -D LWS_WITH_ZIP_FOPS=ON -D LWS_WITH_SOCKS5=ON -D LWS_WITH_RANGES=ON -D LWS_WITH_ACME=ON \
		-D LWS_WITH_LIBUV=OFF -D LWS_WITH_LIBEV=OFF -D LWS_WITH_LIBEVENT=OFF -D LWS_WITH_FTS=ON -D LWS_WITH_THREADPOOL=ON -D LWS_UNIX_SOCK=ON \
		-D LWS_WITH_HTTP_PROXY=ON -D LWS_WITH_DISKCACHE=ON -D LWS_WITH_LWSAC=ON -D LWS_LINK_TESTAPPS_DYNAMIC=ON -D LWS_WITHOUT_BUILTIN_GETIFADDRS=ON -D LWS_USE_BUNDLED_ZLIB=OFF  \
		-D LWS_WITHOUT_BUILTIN_SHA1=ON \
    		-D LWS_WITH_STATIC=OFF \
    		-D LWS_WITHOUT_CLIENT=OFF \
    		-D LWS_WITHOUT_SERVER=OFF \
    		-D LWS_WITHOUT_TESTAPPS=OFF \
    		-D LWS_WITHOUT_TEST_SERVER=ON \
    		-D LWS_WITHOUT_TEST_SERVER_EXTPOLL=ON \
    		-D LWS_WITHOUT_TEST_PING=ON \
    		-D LWS_WITHOUT_TEST_CLIENT=ON \
		"

INHIBIT_PACKAGE_DEBUG_SPLIT = "1"

FILES_${PN} += "${libdir}/*"

do_install_append() {
	rm -rf ${D}/usr/share
	[[ "${libdir}" != "/usr/lib" ]] || return 0
	if test -d ${D}/usr/lib; then
		mv ${D}/usr/lib ${D}/${libdir}
	fi
        ${STRIP} ${D}/${libdir}/*.so*
}