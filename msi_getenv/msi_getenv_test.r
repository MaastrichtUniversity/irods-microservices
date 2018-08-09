# Simple rule to test microservice
#
# Call with
#
# irule -F msi_getenv_test.r "*var='MIRTH_METADATA_CHANNEL'"

irule_dummy() {
    writeLine("stdout", "Testing msi_getenv ...");
    IRULE_msi_getenv_test(*var, *result);
    writeLine("stdout", "The value for environment var '*var' is: *result");
}

IRULE_msi_getenv_test(*var, *result) {

    msi_getenv(*var, *result);

}

INPUT *var=""
OUTPUT ruleExecOut