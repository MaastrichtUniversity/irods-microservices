# Call with
#
# irule -F msi_set_avu_test.r
#

test() {
    # type, path, attribute, value, unit
    msi_set_avu("-d", "/nlmumc/home/rods/docker-compose.yml", "foo", "bar", "xyz");
}


INPUT null
OUTPUT ruleExecOut