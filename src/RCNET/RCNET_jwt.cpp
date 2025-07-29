#include "RCNET/RCNET_jwt.h"

// JWT-C++ Library
#include <jwt-cpp/jwt.h>
#include <picojson/picojson.h>

// Standard C Libraries
#include <stdlib.h>
#include <string.h>
#include <string>
#include <variant>

/**
 * Clé publique JWT pour la vérification des tokens (RS512).
 * Cette clé est utilisée pour valider les signatures des tokens JWT.
 * Elle est chargée depuis la variable d'environnement SEATYRANTS_PUBLIC_KEY_JWT_BASE64.
 */
static std::string SEATYRANTS_PUBLIC_KEY_JWT;

/**
 * Issuer attendu pour les tokens JWT.
 * Cette valeur est utilisée pour vérifier l'émetteur du token lors de la validation.
 */
static std::string SEATYRANTS_ISSUER_JWT;

bool rcnet_jwt_clientInit(const char* public_key_pem, const char* issuer) 
{
    if (!public_key_pem || !issuer) return false;

    SEATYRANTS_PUBLIC_KEY_JWT = public_key_pem;
    SEATYRANTS_ISSUER_JWT = issuer;

    return true;
}

bool rcnet_jwt_clientVerify(const char* token) 
{
    if (!token) return false;

    try {
        auto decoded = jwt::decode(token);
        auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::rs512(SEATYRANTS_PUBLIC_KEY_JWT))
            .with_issuer(SEATYRANTS_ISSUER_JWT)
            .with_type("JWT");

        verifier.verify(decoded);
        return true;
    } catch (...) {
        return false;
    }
}

char* rcnet_jwt_clientGetClaim(const char* token, const char* claim_name) 
{
    if (!token || !claim_name) return NULL;

    try {
        auto decoded = jwt::decode(token);
        auto json = decoded.get_payload_json();

        auto it = json.find(claim_name);
        if (it == json.end()) return NULL;

        // Convert to string regardless of type
        std::string result;
        if (it->second.is<std::string>()) 
        {
            result = it->second.get<std::string>();
        } 
        else if (it->second.is<double>()) 
        {
            result = std::to_string(it->second.get<double>());
        } 
        else if (it->second.is<bool>()) 
        {
            result = it->second.get<bool>() ? "true" : "false";
        } 
        else 
        {
            return NULL;
        }

        return strdup(result.c_str());
    } catch (...) {
        return NULL;
    }
}

char* rcnet_jwt_base64Decode(const char* input) 
{
    return NULL;
}

