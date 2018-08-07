# Simple rule to test microservice
#
# Call with
#
# irule -F msi_http_send_file_test.r "*url='mirthconnect:6670/?project=P000000001&collection=C000000001'" "*filePath='/nlmumc/projects/P000000001/C000000001/metadata.xml'"

irule_dummy() {
    writeLine("stdout", "Testing msi_http_send_file ...");
    IRULE_msi_http_send_file_test(*url, *filePath);
}

IRULE_msi_http_send_file_test(*url, *filePath) {

    msi_http_send_file("*url", "*filePath");

}

INPUT *url="", *filePath=""
OUTPUT ruleExecOut