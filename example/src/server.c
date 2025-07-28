#include "server.h"

// Include necessary headers
#include <RCNET/nats.h>
#include <RCNET/logger.h>

// Standard C libraries
#include <stdbool.h>
#include <string.h>

RCNET_NATSClient client;

// Fonction de callback pour la réception des messages NATS (subscriber)
static void natsMessageHandler(natsConnection *nc, natsSubscription *sub, natsMsg *msg, void *closure)
{
    // Afficher le sujet et le contenu du message
    rcnet_logger_log(RCNET_LOG_INFO, "Reçu un message sur le sujet '%s': %.*s\n",
           natsMsg_GetSubject(msg),
           natsMsg_GetDataLength(msg),
           natsMsg_GetData(msg));

    // Détruire le message pour libérer les ressources
    natsMsg_Destroy(msg);
}


void rcnet_unload(void)
{
    // Cleanup NATS client
    rcnet_nats_cleanup(&client);

    // Log
    rcnet_logger_log(RCNET_LOG_INFO, "Server Unloaded\n");
}

void rcnet_load(void)
{
    // Initialize NATS client
    rcnet_nats_initialize(&client, "tls://staging.nats.tcp.crzcommon.com:4222", NULL, NULL, NULL, true, "UDXU4RCSJNZOIQHZNWXHXORDPRTGNJAHAHFRGZNEEJCPQTT2M7NLCNF4", "SUACSSL3UAHUDXKFSNVUZRF5UHPMWZ6BFDTJ7M6USDXIEDNPPQYYYCU3VY");

    // Check and create JetStream stream if it doesn't exist
    const char *subjects[] = { "aaaa.test", "aaaa.toto" };
    RCNET_JetStreamStreamOptions streamOptions;
    streamOptions.messageMaxAge = 30000000000; // 30 seconds in nanoseconds
    streamOptions.noAck = false;
    rcnet_nats_check_and_create_stream(&client, "mystreammmm33", subjects, 2, &streamOptions);

    // Subscribe to a subject with JetStream and without JetStream
    rcnet_nats_jetstream_subscribe(&client, "aaaa.test", natsMessageHandler, NULL, NULL);
    rcnet_nats_subscribe(&client, "subject.test2", natsMessageHandler, NULL);
 
    // Publish a message to a subject with JetStream and without JetStream
    rcnet_nats_jetstream_publish(&client, "aaaa.test", "Hello World2", strlen("Hello World2"), NULL);
    rcnet_nats_publish(&client, "subject.test2", "Hello World", strlen("Hello World"));

    // Log
    rcnet_logger_log(RCNET_LOG_INFO, "Server Loaded\n");
}

void rcnet_update(double dt)
{
    rcnet_logger_log(RCNET_LOG_INFO, "Server DeltaTime: %f\n", dt);
}