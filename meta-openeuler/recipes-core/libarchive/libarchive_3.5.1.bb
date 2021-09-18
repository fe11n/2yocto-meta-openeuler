DESCRIPTION = "Yet Another JSON Library - A Portable JSON parsing and serialization library in ANSI C"
LICENSE = "MIT"

LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/GPL-2.0-only;md5=801f80980d171dd6425610833a22dbe6"

SRC_URI = "file://libarchive/libarchive-3.5.1.tar.gz   \
	   file://libarchive/libarchive-uninitialized-value.patch \ 
	"

FILESPATH_prepend += "${LOCAL_FILES}/${BPN}:"
DL_DIR = "${LOCAL_FILES}"
S = "${WORKDIR}/${BPN}-${PV}"

inherit cmake

INHIBIT_PACKAGE_DEBUG_SPLIT = "1"

FILES_${PN} += "${libdir}/libarchive.so* "

do_install_append() {
	[[ "${libdir}" != "/usr/lib" ]] || return 0
	if test -d ${D}/usr/lib; then
		mv ${D}/usr/lib ${D}/${libdir}
	fi

        ${STRIP} ${D}/${libdir}/*.so*
}
