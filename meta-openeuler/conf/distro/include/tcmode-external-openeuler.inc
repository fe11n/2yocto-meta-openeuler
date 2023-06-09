require conf/distro/include/tcmode-external.inc

TCMODEOVERRIDES .= ":tcmode-external-openeuler"

# This is defined in default-providers.inc, which is parsed before the tcmode,
# so we can't use ?=
PREFERRED_PROVIDER_linux-libc-headers = "linux-libc-headers-external"
SKIP_RECIPE[linux-libc-headers] = "not building with an external toolchain"
SKIP_RECIPE[linux-libc-headers-yocto] = "not building with an external toolchain"

# Disable pseudo for these cross binaries
EXTERNAL_CROSS_NOPSEUDO = "gcc g++ cpp"

# Use tuning-defined baselib to align with multilib and external toolchain
baselib = "${@d.getVar('BASE_LIB_tune-' + (d.getVar('DEFAULTTUNE') or 'INVALID')) or d.getVar('BASELIB')}"

# Ensure that the licensing variables are available to the toolchain.
export MGLS_LICENSE_FILE
export LM_LICENSE_FILE

def openeuler_version(d):
    version_output = external_run(d, d.getVar('EXTERNAL_CC') or 'gcc', '-v')

    try:
        version = version_output.splitlines()[-1]
    except IndexError:
        bb.error('Unexpected output for {}: {}'.format(d.getVar('EXTERNAL_TOOLCHAIN_BIN') + '/' + (d.getVar('EXTERNAL_CC') or 'gcc') + ' -v', version_output))
        version = 'UNKNOWN'

    if version != 'UNKNOWN':
        # gcc version xx.yy.zz nnnnnnn, here we get xx.yy.zz
        main_version = version.split()[2]
        if main_version == 'Preview':
            main_version = version.split()[-3]
        elif '(GCC' in main_version or '(gcc' in main_version:
            match = re.search("(gcc", main_version, re.IGNORECASE)
            bb.warn(match.group())
            return match.group()
        else:
            return main_version
    return 'UNKNOWN'

OPENEULER_VERSION = "${@openeuler_version(d)}"
OPENEULER_VERSION_allarch = ""
OPENEULER_VERSION[vardepvalue] = "${OPENEULER_VERSION}"

# Add openeuler toolchain version to external recipe versions
EXTERNAL_PV_SUFFIX ?= "${@'-${OPENEULER_VERSION}' if d.getVar('OPENEULER_VERSION') != 'UNKNOWN' else ''}"
EXTERNAL_PV_SUFFIX[vardepvalue] = "${EXTERNAL_PV_SUFFIX}"
EXTERNAL_PV_SUFFIX_allarch = ""

# Make it clear that this version is the toolchain, not the CodeBench IDE
OPENEULER_TOOLCHAIN_VERSION = "${OPENEULER_VERSION}"

BUILDCFG_VARS += "${@'OPENEULER_TOOLCHAIN_VERSION' if d.getVar('OPENEULER_VERSION') != 'UNKNOWN' else ''}"

python openeuler_metadata_setup () {
    # Ensure that changes to toolchain licensing don't affect checksums
    license_vars = ['MGLS_LICENSE_FILE', 'LM_LICENSE_FILE']
    d.appendVar('BB_BASEHASH_IGNORE_VARS', ' ' + ' '.join(license_vars))
}
openeuler_metadata_setup[eventmask] = "bb.event.ConfigParsed"
addhandler openeuler_metadata_setup

python extract_env_setup_metadata_prepend() {
    from pathlib import Path
    external_toolchain = d.getVar('EXTERNAL_TOOLCHAIN')
    if not external_toolchain or external_toolchain == 'UNKNOWN':
        external_toolchains = d.getVar('EXTERNAL_TOOLCHAINS')
        if not external_toolchains or external_toolchains == 'UNKNOWN':
            toolchains_path = d.getVar('TOOLCHAINS_PATH')
            if not toolchains_path:
                codebench_path = d.getVar('CODEBENCH_PATH')
                if codebench_path:
                    bb.warn('CODEBENCH_PATH is deprecated, please define TOOLCHAINS_PATH')
                    toolchains_path = Path(codebench_path).resolve().parent / 'toolchains'
                else:
                    bb.fatal('EXTERNAL_TOOLCHAIN, EXTERNAL_TOOLCHAINS, or TOOLCHAINS_PATH must be set')
            external_toolchains = ' '.join(str(p) for p in Path(toolchains_path).glob('*/'))
            if not external_toolchains:
                bb.fatal('{}/../toolchains does not exist or is empty'.format(toolchains_path))
            d.setVar('EXTERNAL_TOOLCHAINS', external_toolchains)
}
