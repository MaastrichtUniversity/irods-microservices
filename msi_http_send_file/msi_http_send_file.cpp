// =-=-=-=-=-=-=-
#include "irods_error.hpp"
#include "rsDataObjClose.hpp"
#include "rsDataObjOpen.hpp"
#include "rsDataObjRead.hpp"
#include "irods_ms_plugin.hpp"

// =-=-=-=-=-=-=-
// STL Includes
#include <string>
#include <iostream>

// =-=-=-=-=-=-=-
// cURL Includes
#include <curl/curl.h>
#include <curl/easy.h>

typedef struct readData_t {
  char path[MAX_NAME_LEN];
  int desc;
  rsComm_t *rsComm;
} readData_t;

class irodsCurl {
    private:
        // iRODS server handle
        rsComm_t *rsComm;

        // cURL handle
        CURL *curl;


    public:
        irodsCurl( rsComm_t *comm ) {
            rsComm = comm;

            curl = curl_easy_init();
            if ( !curl ) {
                rodsLog( LOG_ERROR, "irods_http_send_file: %s", curl_easy_strerror( CURLE_FAILED_INIT ) );
            }
        }

        ~irodsCurl() {
            if ( curl ) {
                curl_easy_cleanup( curl );
            }
        }

    int post( char *url, char* objectPath ) {
        CURLcode res = CURLE_OK;
        char errbuf[CURL_ERROR_SIZE];
        readData_t readData;
        openedDataObjInp_t openedSource;

        /* Create readData object */
        memset(&readData, 0, sizeof(readData_t));
        memset(&openedSource, 0, sizeof(openedDataObjInp_t));
        snprintf(readData.path, MAX_NAME_LEN, "%s", objectPath);
        readData.desc = 0;
        readData.rsComm = rsComm;

        // Set up easy handler
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_PROTOCOLS,  CURLPROTO_HTTP | CURLPROTO_HTTPS);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(curl, CURLOPT_READDATA, &readData);
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, &irodsCurl::my_read_obj);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-iRODS");
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
        // Possibly this timeout is too conservative
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

        // CURL call
        res = curl_easy_perform(curl);

        if ( res != 0 ) {
            rodsLog( LOG_ERROR, "irods_http_send_file: curl error (%i): %s", res, errbuf );
            return SYS_INTERNAL_NULL_INPUT_ERR;
        } else {
             long http_code = 0;
             curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);

             if (http_code != 200) {
                 rodsLog( LOG_ERROR, "irods_http_send_file: HTTP returned error code %d", http_code );
                 return SYS_INTERNAL_NULL_INPUT_ERR;
             }
        }

        if (readData.desc) {
             openedSource.l1descInx = readData.desc;
             int status = rsDataObjClose(rsComm, &openedSource);
             if (status < 0) {
                 rodsLog(LOG_ERROR, "irods_http_send_file: rsDataObjClose failed for %s, status = %d",readData.path, status);
             }
         }

        return 0;
    }

    static size_t my_read_obj(void *buffer, size_t size, size_t nmemb, void* userp) {
        struct readData_t *readData = (struct readData_t *) userp;

        dataObjInp_t file;
        openedDataObjInp_t openedFile;
        bytesBuf_t bytesBuf;

        size_t bytesRead;

        // Make sure we have something to read from
        if (!readData) {
            rodsLog(LOG_ERROR, "irods_http_send_file: readData is NULL");
            return CURL_READFUNC_ABORT;
        }

        // Zero fill input structs
        memset(&file, 0, sizeof (dataObjInp_t));
        memset(&openedFile, 0, sizeof (openedDataObjInp_t));
        memset(&bytesBuf, 0, sizeof (bytesBuf_t));

        // If this is the first call we need to create our data object before reading from it
        if (!readData->desc) {
            strncpy(file.objPath, readData->path, MAX_NAME_LEN);

            // readData->desc = 5;
            readData->desc = rsDataObjOpen(readData->rsComm, &file);

            // TODO: <= 2 instead of < 0? Look up rsDataObjOpen return codes
            if (readData->desc < 0) {
                rodsLog(LOG_ERROR, "irods_http_send_file: Cannot read iRODS object %s Status =  %d",readData->path, readData->desc);
                return CURL_READFUNC_ABORT;
            }
        }

        // Setup buffer for rsDataObjRead
        bytesBuf.len = (int)(size * nmemb);
        bytesBuf.buf = buffer;

        // Setup input struct for rsDataObjRead
        openedFile.l1descInx = readData->desc;
        openedFile.len = bytesBuf.len;

        // bytesRead = 0;
        //TODO: At Paul: during compilation warning: implicit conversion changes signedness: 'int' to 'size_t' (aka 'unsigned long')
        bytesRead = rsDataObjRead(readData->rsComm, &openedFile, &bytesBuf);
        //TODO: At Paul: during compilation warning: comparison of unsigned expression < 0 is always false
        if (bytesRead < 0) {
            rodsLog(LOG_ERROR, "irods_http_send_file: Problem reading iRODS object. Status =  %d", bytesRead);
            return CURL_READFUNC_ABORT;
        }

        return (bytesRead);
    }

}; 	// class irodsCurl


extern "C" {

// =-=-=-=-=-=-=-
// 1. Write a standard issue microservice
    int irods_http_send_file( msParam_t* url, msParam_t* src_obj, ruleExecInfo_t* rei ) {
        dataObjInp_t dataObjInp, *myDataObjInp;
        //TODO: At Paul: during compilation "warning: unused variable 'myUrl'"
        char *myUrl;

        // Sanity checks
        if ( !rei || !rei->rsComm ) {
            rodsLog( LOG_ERROR, "irods_http_send_file Input rei or rsComm is NULL." );
            return ( SYS_INTERNAL_NULL_INPUT_ERR );
        }

        // Get path of destination object
        rei->status = parseMspForDataObjInp( src_obj, &dataObjInp, &myDataObjInp, 0 );
        if ( rei->status < 0 ) {
            rodsLog( LOG_ERROR, "irods_http_send_file: Input object error. status = %d", rei->status );
            return ( rei->status );
        }

        // Create irodsCurl instance
        irodsCurl myCurl( rei->rsComm );

        // Call irodsCurl::post
        rei->status = myCurl.post( parseMspForStr( url ), myDataObjInp->objPath );

        // Done
        return rei->status;

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
                ruleExecInfo_t*>("irods_http_send_file",
                                 std::function<int(
                                         msParam_t*,
                                         msParam_t*,
                                         ruleExecInfo_t*)>(irods_http_send_file));
        // =-=-=-=-=-=-=-
        // 5. return the newly created microservice plugin
        return msvc;
    }

}   // extern "C"
