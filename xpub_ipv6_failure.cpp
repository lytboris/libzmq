#include "testutil.hpp"

#define TEST_ASSERT_EQUAL_INT(a,b) assert ((a) == (b))
#define TEST_ASSERT_NOT_NULL(a) assert ((a) != NULL)

void test_xpub_subscription_ipv6 (int ipv6, const char *socket_str)
{
    int rc;
    char buffer[2];
    void *ctx = zmq_ctx_new ();
    TEST_ASSERT_NOT_NULL (ctx);

    void *pub = zmq_socket (ctx, ZMQ_XPUB);
    TEST_ASSERT_NOT_NULL (pub);
    rc = zmq_setsockopt (pub, ZMQ_IPV6, &ipv6, sizeof (int));
    TEST_ASSERT_EQUAL_INT (0, rc);


    rc = zmq_bind (pub, socket_str);
    TEST_ASSERT_EQUAL_INT (0, rc);

    void *sub = zmq_socket (ctx, ZMQ_SUB);
    TEST_ASSERT_NOT_NULL (sub);
    rc = zmq_connect (sub, socket_str);
    TEST_ASSERT_EQUAL_INT (0, rc);

    //  Subscribe for A
    rc = zmq_setsockopt (sub, ZMQ_SUBSCRIBE, "A", 1);
    TEST_ASSERT_EQUAL_INT (0, rc);

    // Receive subscriptions from subscriber
    fprintf(stderr, "blocking read on pub bound to %s ...", socket_str);
    rc = zmq_recv (pub, buffer, 2, 0);
    fprintf(stderr, "DONE\n");
    TEST_ASSERT_EQUAL_INT (2, rc);
    assert (buffer[0] == 1);
    assert (buffer[1] == 'A');

    //  Clean up.
    rc = zmq_close (pub);
    TEST_ASSERT_EQUAL_INT (0, rc);
    rc = zmq_close (sub);
    TEST_ASSERT_EQUAL_INT (0, rc);
    rc = zmq_ctx_term (ctx);
    TEST_ASSERT_EQUAL_INT (0, rc);
}


int main (void)
{
    setup_test_environment ();

    test_xpub_subscription_ipv6(0, "tcp://127.0.0.1:7777");
    test_xpub_subscription_ipv6(1, "tcp://[::1]:7777");

    return 0;
}
