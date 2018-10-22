# Basic existence checker of resources
#
# Call with
#
# irule -F msi_amqp_basic_publish_test.r
#

test() {
    # hostname, port, user, password, exchange, routing_key, message
    msi_amqp_basic_publish("rabbitmq", 5672, "user", "password", "projectCollections", "projectCollection.metadata.validated", "foo!" );
}


INPUT null
OUTPUT ruleExecOut