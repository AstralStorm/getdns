#ifndef _check_getdns_service_h_
#define _check_getdns_service_h_

    /*
     ***************************************************
     *                                                 *
     *  T E S T S  F O R  G E T D N S _ S E R V I C E  * 
     *                                                 *
     ***************************************************
    */

     START_TEST (getdns_service_1)
     {
      /*
       *  context = NULL
       *  expect: GETDNS_RETURN_INVALID_PARAMETER
       */
       struct getdns_context *context = NULL; 
       getdns_transaction_t transaction_id = 0;

       ASSERT_RC(getdns_service(context, "google.com", NULL, 
         NULL, &transaction_id, callbackfn), 
         GETDNS_RETURN_INVALID_PARAMETER, "Return code from getdns_service()");
     }
     END_TEST

     START_TEST (getdns_service_2)
     {
      /*
       *  name = NULL
       *  expect: GETDNS_RETURN_INVALID_PARAMETER
       */
       struct getdns_context *context = NULL; 
       struct event_base *event_base = NULL; 
       getdns_transaction_t transaction_id = 0;

       CONTEXT_CREATE(TRUE);
       EVENT_BASE_CREATE;

       ASSERT_RC(getdns_service(context, NULL, NULL, 
         NULL, &transaction_id, callbackfn), 
         GETDNS_RETURN_INVALID_PARAMETER, "Return code from getdns_service()");

       RUN_EVENT_LOOP;
       CONTEXT_DESTROY;
     }
     END_TEST

     START_TEST (getdns_service_3)
     {
      /*
       *  name = invalid domain (too many octets)
       *  expect:  GETDNS_RETURN_BAD_DOMAIN_NAME
       */
       struct getdns_context *context = NULL; 
       struct event_base *event_base = NULL;
       getdns_transaction_t transaction_id = 0;
       const char *name = "oh.my.gosh.and.for.petes.sake.are.you.fricking.crazy.man.because.this.spectacular.and.elaborately.thought.out.domain.name.of.very.significant.length.is.just.too.darn.long.because.you.know.the rfc.states.that.two.hundred.fifty.five.characters.is.the.max.com";
       
       CONTEXT_CREATE(TRUE);
       EVENT_BASE_CREATE;

       ASSERT_RC(getdns_service(context, name, NULL, 
         NULL, &transaction_id, callbackfn), 
         GETDNS_RETURN_BAD_DOMAIN_NAME, "Return code from getdns_service()");
       RUN_EVENT_LOOP;
       CONTEXT_DESTROY;

     }
     END_TEST

     START_TEST (getdns_service_4)
     {
      /*
       *  name is invalid (domain name label length > 63)
       *  expect:  GETDNS_RETURN_BAD_DOMAIN_NAME
       */
       struct getdns_context *context = NULL; 
       struct event_base *event_base = NULL;
       getdns_transaction_t transaction_id = 0;
       const char *name = "this.domain.hasalabelwhichexceedsthemaximumdnslabelsizeofsixtythreecharacters.com";
       
       CONTEXT_CREATE(TRUE);
       EVENT_BASE_CREATE;

       ASSERT_RC(getdns_service(context, name, NULL, 
         NULL, &transaction_id, callbackfn), 
         GETDNS_RETURN_BAD_DOMAIN_NAME, "Return code from getdns_service()");
       RUN_EVENT_LOOP;
       CONTEXT_DESTROY;

     }
     END_TEST

     START_TEST (getdns_service_5)
     {
      /*
       *  callbackfn = NULL
       *  expect:  GETDNS_RETURN_INVALID_PARAMETER
       */
       struct getdns_context *context = NULL; 
       struct event_base *event_base = NULL;
       getdns_transaction_t transaction_id = 0;
       
       CONTEXT_CREATE(TRUE);
       EVENT_BASE_CREATE;

       ASSERT_RC(getdns_service(context, "google.com", NULL, 
         NULL, &transaction_id, NULL), 
         GETDNS_RETURN_INVALID_PARAMETER, "Return code from getdns_service()");
       RUN_EVENT_LOOP;
       CONTEXT_DESTROY;

     }
     END_TEST

     //name is <non-existent domain name> (NXDOMAIN)
     //no extensions
     //expected: NXDOMAIN response (with SOA record)

     START_TEST (getdns_service_7)
     {
      /*
       *  name is <non-existent domain name> (NXDOMAIN)
       *  expect: NXDOMAIN response (with SOA record)
       */
       void verify_getdns_service_7(struct extracted_response *ex_response);
       struct getdns_context *context = NULL;   \
       struct event_base *event_base = NULL;    \
       getdns_transaction_t transaction_id = 0;
     
       CONTEXT_CREATE(TRUE);

       EVENT_BASE_CREATE;

       ASSERT_RC(getdns_service(context, "nitinsinghit.com", NULL, 
         verify_getdns_address_8, &transaction_id, callbackfn),
         GETDNS_RETURN_GOOD, "Return code from getdns_service()");

       RUN_EVENT_LOOP;
       CONTEXT_DESTROY;
     }
     END_TEST

     void verify_getdns_service_7(struct extracted_response *ex_response)
     {
       assert_nxdomain(ex_response);
       assert_nodata(ex_response);
       assert_soa_in_authority(ex_response);
     }



     
     Suite *
     getdns_service_suite (void)
     {
       Suite *s = suite_create ("getdns_service()");
     
       /* Negative test caseis */
       TCase *tc_neg = tcase_create("Negative");
       tcase_add_test(tc_neg, getdns_service_1);
       tcase_add_test(tc_neg, getdns_service_2);
       tcase_add_test(tc_neg, getdns_service_3);
       tcase_add_test(tc_neg, getdns_service_4);
       tcase_add_test(tc_neg, getdns_service_5);
       
       suite_add_tcase(s, tc_neg);
     
       /* Positive test cases */
       TCase *tc_pos = tcase_create("Positive");
       tcase_add_test(tc_pos, getdns_service_7);
       suite_add_tcase(s, tc_pos);
     
       return s;
     }

#endif