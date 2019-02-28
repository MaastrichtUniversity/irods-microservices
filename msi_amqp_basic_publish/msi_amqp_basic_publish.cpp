//TODO: Port this microservice to iRODS 4.2.x

// =-=-=-=-=-=-=-
//#include "apiHeaderAll.hpp"
//#include "msParam.hpp"
//#include "reGlobalsExtern.hpp"
#include "irods_error.hpp"
#include "irods_ms_plugin.hpp"

// =-=-=-=-=-=-=-
// STL Includes
#include <string>
#include <iostream>

// =-=-=-=-=-=-=-
// librabbitmq includes
#include <amqp_tcp_socket.h>
#include <amqp.h>
#include <amqp_framing.h>

extern "C" {

    // =-=-=-=-=-=-=-
    // 1. Write a standard issue microservice
    int irods_amqp_basic_publish(
            msParam_t* hostnameParam,
            msParam_t* portParam,
            msParam_t* usernameParam,
            msParam_t* passwordParam,
            msParam_t* exchangeParam,
            msParam_t* routingKeyParam,
            msParam_t* messageParam,
            ruleExecInfo_t* rei
    ) {
        amqp_socket_t *socket = NULL;
        amqp_connection_state_t conn;
        amqp_rpc_reply_t reply;
        amqp_bytes_t message_bytes, exchange_bytes, routing_key_bytes;
        amqp_basic_properties_t props;
        int status;

        // Sanity checks
        if ( !rei || !rei->rsComm ) {
            rodsLog( LOG_ERROR, "amqp_basic_publish: Input rei or rsComm is NULL." );
            return ( SYS_INTERNAL_NULL_INPUT_ERR );
        }

        // hostname
        const char *hostname = parseMspForStr( hostnameParam );
        if( !hostname ) {
            rodsLog(LOG_ERROR, "amqp_basic_publish - invalid hostname");
            return SYS_INVALID_INPUT_PARAM;
        }

        // port
        if (portParam == NULL || portParam->type == NULL || strcmp(portParam->type, INT_MS_T) != 0) {
            rodsLog(LOG_ERROR, "amqp_basic_publish - invalid port");
            return (USER_PARAM_TYPE_ERR);
        }
        int port = parseMspForPosInt(portParam);

        // username
        const char *username = parseMspForStr( usernameParam );
        if( !username ) {
            rodsLog(LOG_ERROR, "amqp_basic_publish - invalid username");
            return SYS_INVALID_INPUT_PARAM;
        }

        // password
        const char *password = parseMspForStr( passwordParam );
        if( !password ) {
            rodsLog(LOG_ERROR, "amqp_basic_publish - invalid password");
            return SYS_INVALID_INPUT_PARAM;
        }

        // exchange
        const char *exchange = parseMspForStr( exchangeParam );
        if( !exchange ) {
            rodsLog(LOG_ERROR, "amqp_basic_publish - invalid exchange");
            return SYS_INVALID_INPUT_PARAM;
        }
        exchange_bytes = amqp_cstring_bytes(exchange);

        // routing_key
        const char *routingKey = parseMspForStr( routingKeyParam );
        if( !routingKey ) {
            rodsLog(LOG_ERROR, "amqp_basic_publish - invalid routing key");
            return SYS_INVALID_INPUT_PARAM;
        }
        routing_key_bytes = amqp_cstring_bytes(routingKey);

        // message
        const char *message = parseMspForStr( messageParam );
        if( !message ) {
            rodsLog(LOG_ERROR, "amqp_basic_publish - invalid message");
            return SYS_INVALID_INPUT_PARAM;
        }
        message_bytes = amqp_cstring_bytes(message);

        // Start rabbitmq sequence
        conn = amqp_new_connection();
        socket = amqp_tcp_socket_new(conn);
        status = amqp_socket_open(socket, hostname, port);

        if ( status < 0 ) {
            rodsLog( LOG_ERROR, "amqp_basic_publish: amqp_socket_open returned %d (%s)", status, amqp_error_string2(status));
            return SYS_INTERNAL_NULL_INPUT_ERR;
        }

        reply = amqp_login(conn, "/", 1, 131072, 0, AMQP_SASL_METHOD_PLAIN, username, password);

        if ( reply.reply_type != AMQP_RESPONSE_NORMAL) {
            rodsLog( LOG_ERROR, "amqp_basic_publish: amqp_login response was not normal");
            return SYS_INTERNAL_NULL_INPUT_ERR;
        }

        amqp_channel_open(conn, 1);
        reply = amqp_get_rpc_reply(conn);

        if ( reply.reply_type != AMQP_RESPONSE_NORMAL) {
            rodsLog( LOG_ERROR, "amqp_basic_publish: amqp_get_rpc_reply response was not normal");
            return SYS_INTERNAL_NULL_INPUT_ERR;
        }

        // persistent delivery mode
        props._flags = AMQP_BASIC_DELIVERY_MODE_FLAG;
        props.delivery_mode = 2;

        status = amqp_basic_publish(conn, 1, exchange_bytes, routing_key_bytes, 0, 0, &props, message_bytes);

        if ( status > 0 ) {
            rodsLog( LOG_ERROR, "amqp_basic_publish: amqp_basic_publish returned %d (%s)", status, amqp_error_string2(status));
            return SYS_INTERNAL_NULL_INPUT_ERR;
        }

        rodsLog( LOG_NOTICE, "amqp_basic_publish: Published message to rabbitmq (on %s) at exchange %s using routing_key %s.", hostname, exchange, routingKey);

        reply = amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);

        if ( reply.reply_type != AMQP_RESPONSE_NORMAL) {
            rodsLog( LOG_ERROR, "amqp_basic_publish: amqp_channel_close response was not normal");
            return SYS_INTERNAL_NULL_INPUT_ERR;
        }

        reply = amqp_connection_close(conn, AMQP_REPLY_SUCCESS);

        if ( reply.reply_type != AMQP_RESPONSE_NORMAL) {
            rodsLog( LOG_ERROR, "amqp_basic_publish: amqp_connection_close response was not normal");
            return SYS_INTERNAL_NULL_INPUT_ERR;
        }

        amqp_destroy_connection(conn);

        // Done
        return rei->status;

    }


    // =-=-=-=-=-=-=-
    // 2.  Create the plugin factory function which will return a microservice
    //     table entry
    irods::ms_table_entry*  plugin_factory() {
        // =-=-=-=-=-=-=-
        // 3.  allocate a microservice plugin which takes the number of function
        //     params as a parameter to the constructor
        // hostname, port, user, password, exchange, routing_key, message
        irods::ms_table_entry* msvc = new irods::ms_table_entry( 7 );

        // =-=-=-=-=-=-=-
        // 4. add the microservice function as an operation to the plugin
        //    the first param is the name / key of the operation, the second
        //    is the name of the function which will be the microservice
        msvc->add_operation(
                "irods_amqp_basic_publish",
                std::function<int(
                        msParam_t*,
                        msParam_t*,
                        msParam_t*,
                        msParam_t*,
                        msParam_t*,
                        msParam_t*,
                        msParam_t*,
                        ruleExecInfo_t*)>(irods_amqp_basic_publish));
        // =-=-=-=-=-=-=-
        // 5. return the newly created microservice plugin
        return msvc;
    }

}   // extern "C"
