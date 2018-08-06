// =-=-=-=-=-=-=-
#include "irods_error.hpp"
#include "irods_ms_plugin.hpp"

// =-=-=-=-=-=-=-
// STL/boost Includes
#include <string>
#include <time.h>
#include <iostream>
#include <vector>
#include <boost/algorithm/string.hpp>

extern "C" {

    int msi_time_ts2str_impl(msParam_t* inpTimeStampParam,
                             msParam_t* inpFormatParam,
                             msParam_t* outParam,
                             ruleExecInfo_t *rei) {

        char *dateFormat;
        char tStr[TIME_LEN];
        time_t myTime;
        struct tm *mytm;
        //int status;
  
        if (rei == NULL || rei->rsComm == NULL) return (SYS_INTERNAL_NULL_INPUT_ERR);
 
        if (inpTimeStampParam == NULL || inpTimeStampParam->type == NULL ||
            strcmp(inpTimeStampParam->type, INT_MS_T) != 0) return (USER_PARAM_TYPE_ERR);
 
        if (inpFormatParam == NULL || inpFormatParam->type == NULL ||
            strcmp(inpFormatParam->type, STR_MS_T) != 0) return (USER_PARAM_TYPE_ERR);
  
        myTime = (long) parseMspForPosInt(inpTimeStampParam);
        dateFormat = (char*)inpFormatParam->inOutStruct;
  
        mytm = gmtime(&myTime);
  
        snprintf (tStr, TIME_LEN, dateFormat,
                mytm->tm_year + 1900, mytm->tm_mon + 1, mytm->tm_mday,
                mytm->tm_hour, mytm->tm_min, mytm->tm_sec);


        // TODO: Needs reviewing by Paul
        // Fix for for compilation errors with clang and iRODS 4.2.3 (RITDEV-277)
        // Error is "libmsi_time_ts2str.cpp:43:16: error: assigning to 'int' from incompatible type 'void'"
        // I made this fix according to changes in this msiGetDiffTime (see https://docs.irods.org/4.2.3/doxygen/nre_8systemMS_8cpp_source.html#l00611)
        // Note: Also commented out 'int(status)' at the top of this msi as part of this fix.

        // status = fillStrInMsParam (outParam,tStr);
        fillStrInMsParam (outParam,tStr);
        //return(status);
        return(0);

    }

    irods::ms_table_entry* plugin_factory() {
        irods::ms_table_entry* msvc = new irods::ms_table_entry(3);

        msvc->add_operation<
                msParam_t*,
                msParam_t*,
                msParam_t*,
                ruleExecInfo_t*>("msi_time_ts2str_impl",
                                 std::function<int(
                                         msParam_t*,
                                         msParam_t*,
                                         msParam_t*,
                                         ruleExecInfo_t*)>(msi_time_ts2str_impl));

        return msvc;
    }

} // extern "C"
