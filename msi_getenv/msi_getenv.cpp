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

        fillStrInMsParam(result, std::getenv(in_env_var));
    }

    irods::ms_table_entry* plugin_factory() {
        irods::ms_table_entry* msvc = new irods::ms_table_entry(2);

        msvc->add_operation("msi_getenv_impl", "msi_getenv");

        return msvc;
    }

} // extern "C"
