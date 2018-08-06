// =-=-=-=-=-=-=-
#include "irods_error.hpp"
#include "irods_ms_plugin.hpp"

// =-=-=-=-=-=-=-
// STL/boost Includes
#include <string>

extern "C" {
    // =-=-=-=-=-=-=-
    // 1. Write a standard issue microservice
    int msi_getenv_impl(msParam_t* env_var, msParam_t* result, ruleExecInfo_t* rei) {

        const char *in_env_var = parseMspForStr( env_var );

        if( ! in_env_var ) {
            rodsLog(LOG_ERROR,"msi_getenv - invalid env_var");
            return SYS_INVALID_INPUT_PARAM;
        }

        fillStrInMsParam(result, std::getenv(in_env_var));

        return 0;
    }

    // =-=-=-=-=-=-=-
    // 2.  Create the plugin factory function which will return a microservice
    //     table entry
    irods::ms_table_entry* plugin_factory() {
        // =-=-=-=-=-=-=-
        // 3.  allocate a microservice plugin which takes the number of function
        //     params as a parameter to the constructor
        irods::ms_table_entry* msvc = new irods::ms_table_entry(2);

        // =-=-=-=-=-=-=-
        // 4. add the microservice function as an operation to the plugin
        //    the first param is the name / key of the operation, the second
        //    is the name of the function which will be the microservice
        msvc->add_operation<
                msParam_t*,
                msParam_t*,
                ruleExecInfo_t*>("msi_getenv_impl",
                                 std::function<int(
                                         msParam_t*,
                                         msParam_t*,
                                         ruleExecInfo_t*)>(msi_getenv_impl));
        // =-=-=-=-=-=-=-
        // 5. return the newly created microservice plugin
        return msvc;
    }

} // extern "C"
