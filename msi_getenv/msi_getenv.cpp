// =-=-=-=-=-=-=-
#include "apiHeaderAll.hpp"
#include "msParam.hpp"
#include "reGlobalsExtern.hpp"
#include "irods_ms_plugin.hpp"
#include "reFuncDefs.hpp"

// =-=-=-=-=-=-=-
// STL/boost Includes
#include <string>

extern "C" {
    // =-=-=-=-=-=-=-
    int msi_getenv_impl(msParam_t* env_var, msParam_t* result, ruleExecInfo_t* rei) {

        const char *in_env_var = parseMspForStr( env_var );

        if( ! in_env_var ) {
            rodsLog(LOG_ERROR,"msi_getenv - invalid env_var");
            return SYS_INVALID_INPUT_PARAM;
        }

        fillStrInMsParam(result, std::getenv(in_env_var));

        return 0;
    }

    irods::ms_table_entry* plugin_factory() {
        irods::ms_table_entry* msvc = new irods::ms_table_entry(2);

        msvc->add_operation("msi_getenv_impl", "msi_getenv");

        return msvc;
    }

} // extern "C"
