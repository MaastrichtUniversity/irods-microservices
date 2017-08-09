// =-=-=-=-=-=-=-
#include "apiHeaderAll.hpp"
#include "msParam.hpp"
#include "reGlobalsExtern.hpp"
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
        int status;
  
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
        
        status = fillStrInMsParam (outParam,tStr);

        return(status);
    }

    irods::ms_table_entry* plugin_factory() {
        irods::ms_table_entry* msvc = new irods::ms_table_entry(3);
        
        msvc->add_operation("msi_time_ts2str_impl", "msi_time_ts2str");
        
        return msvc;
    }

} // extern "C"
