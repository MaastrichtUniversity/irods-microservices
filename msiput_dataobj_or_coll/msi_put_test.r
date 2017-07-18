myTestRule {
   #                       source coll or obj,     tgt resc,   options,      target collection
   #remote("ires","") { # Disabling the ingest zone needs to be executed on remote ires server 
       *err = errorcode( msiput_dataobj_or_coll("/mnt/ingest/zones/tense-tortoise/", "demoResc", "forceFlag=", "/nlmumc/home/rods/",*real_path));
       writeLine("stdout","*err + *real_path");
   #}
}
INPUT null
OUTPUT ruleExecOut

