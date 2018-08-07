// =-=-=-=-=-=-=-
#include "irods_error.hpp"
#include "irods_ms_plugin.hpp"

#include "rsPhyPathReg.hpp"

// =-=-=-=-=-=-=-
// STL Includes
#include <string>
#include <iostream>


extern "C" {

double get_plugin_interface_version() {
    return 1.0;
}
// =-=-=-=-=-=-=-
// 1. Write a standard issue microservice
    int irods_unmount( msParam_t* in, ruleExecInfo_t* rei ) {
        dataObjInp_t *destObjInp;

        // Sanity checks
        if ( !rei || !rei->rsComm ) {
            rodsLog( LOG_ERROR, "irods_unmount: Input rei or rsComm is NULL." );
            return ( SYS_INTERNAL_NULL_INPUT_ERR );
        }

        destObjInp = ( dataObjInp_t* ) malloc( sizeof( dataObjInp_t ) );
        memset( destObjInp, 0, sizeof( dataObjInp_t ) );
        strncpy( destObjInp->objPath, parseMspForStr( in ), MAX_NAME_LEN );

        int status = unmountFileDir( rei->rsComm, destObjInp );

        free( destObjInp );

        rodsLog( LOG_ERROR, "rods_unmount status: %i", status);

        // Done
        return 0;

    }



// =-=-=-=-=-=-=-
// 2.  Create the plugin factory function which will return a microservice
//     table entry
        irods::ms_table_entry* plugin_factory() {
        // =-=-=-=-=-=-=-
        // 3.  allocate a microservice plugin which takes the number of function
        //     params as a parameter to the constructor
        irods::ms_table_entry* msvc = new irods::ms_table_entry(1);

        // =-=-=-=-=-=-=-
        // 4. add the microservice function as an operation to the plugin
        //    the first param is the name / key of the operation, the second
        //    is the name of the function which will be the microservice
        msvc->add_operation<
                msParam_t*,
                ruleExecInfo_t*>("irods_unmount",
                                 std::function<int(
                                         msParam_t*,
                                         ruleExecInfo_t*)>(irods_unmount));

        // =-=-=-=-=-=-=-
        // 5. return the newly created microservice plugin
        return msvc;
    }



}	// extern "C"
