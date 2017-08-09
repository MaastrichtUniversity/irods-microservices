msi_time_ts2str_test {
   *fmt = "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d";
   msi_time_ts2str( int(*ts), *fmt, *out );
   writeLine("stdout", *out);
}
INPUT *ts=$"01449136486"
OUTPUT ruleExecOut

