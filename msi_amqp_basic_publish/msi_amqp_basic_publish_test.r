# Basic existence checker of resources
#
# Call with
#
# irule -F msi_amqp_basic_publish_test.r
#

test() {
    *message = '{"project": "P000000001", "collection": "C000000012"}'
    # hostname, port, user, password, exchange, routing_key, message
    msi_amqp_basic_publish("rabbitmq.dh.local", 5672, "user", "password", "datahub.events_tx", "projectCollection.metadata.validated", *message );
}


INPUT null
OUTPUT ruleExecOut