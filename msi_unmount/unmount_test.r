# Simple rule to test microservice
#
# Call with
#
# irule -F unmount_test.r "*coll='/nlmumc/ingest/zones/tricky-token'"
#
# Expected outcomes
# Before: `ils -L /nlmumc/ingest/zones`
#            C- /nlmumc/ingest/zones/tricky-token  mountPoint
#
# After: `ils -L /nlmumc/ingest/zones`
#            C- /nlmumc/ingest/zones/tricky-token

irule_dummy() {
    writeLine("stdout", "Testing unmount ...");
    IRULE_msi_unmount_test(*coll);
}

IRULE_msi_unmount_test(*coll) {
  msi_unmount(*coll);
}
 
INPUT *coll=""
OUTPUT null
