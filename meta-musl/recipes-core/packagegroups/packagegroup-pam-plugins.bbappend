# remove them because of the lack of ruserok_af and logwtmp functions
RDEPENDS_${PN}_remove = "\
pam-plugin-lastlog \
pam-plugin-rhosts \
"
