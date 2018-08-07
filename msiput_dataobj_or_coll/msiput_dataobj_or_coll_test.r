# Simple rule to test microservice
#
# Call with
#
# irule -F msiput_dataobj_or_coll_test.r "*srcColl='/mnt/ingest/zones/clumsy-crabs'" "*resc='replRescUM01'" "*dstColl='/nlmumc/projects/P000000001/C000000003'"
#
# IMPORTANT NOTE: This rule has to be called on the resource server that holds the physical path defined in *srcColl

irule_dummy() {
    writeLine("stdout", "Testing msiput_dataobj_or_coll ...");
    IRULE_msiput_dataobj_or_coll_test(*srcColl, *resc, *dstColl);
}

IRULE_msiput_dataobj_or_coll_test(*srcColl, *resc, *dstColl) {
    msiput_dataobj_or_coll("*srcColl", *resc, "numThreads=10++++forceFlag=", *dstColl, *real_path)
}

INPUT *srcColl='', *resc='', *dstColl=''
OUTPUT ruleExecOut