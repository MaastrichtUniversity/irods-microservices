myTestRule {
       *err = errorcode( msiput_dataobj_or_coll("/mnt/ingest/zones/worried-white-clawed/", "replRescUM01", "forceFlag=", "/nlmumc/projects/P000000001/C000000003", *real_path));
       writeLine("stdout","Error: *err");
}

INPUT null
OUTPUT ruleExecOut
