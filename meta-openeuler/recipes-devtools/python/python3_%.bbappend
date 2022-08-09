PV = "3.9.9"

FILESEXTRAPATHS_append := "${THISDIR}/files/:"

SRC_URI[sha256sum] = "06828c04a573c073a4e51c4292a27c1be4ae26621c3edc7cf9318418ce3b6d27"

SRC_URI_remove += " \
           file://0001-Makefile-fix-Issue36464-parallel-build-race-problem.patch \
"

SRC_URI =+ " \
    file://00001-rpath.patch \
    file://00111-no-static-lib.patch \
    file://00251-change-user-install-location.patch \
    file://backport-Add--with-wheel-pkg-dir-configure-option.patch \
    file://backport-bpo-46811-Make-test-suite-support-Expat-2.4.5.patch \
    file://backport-bpo-20369-concurrent.futures.wait-now-deduplicates-f.patch \
    file://Make-mailcap-refuse-to-match-unsafe-filenam.patch \
    file://add-the-sm3-method-for-obtaining-the-salt-value.patch \
"