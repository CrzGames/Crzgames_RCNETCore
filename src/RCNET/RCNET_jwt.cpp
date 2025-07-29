#include "RCNET/RCNET_jwt.h"

// JWT-C++ Library
#include <jwt-cpp/jwt.h>
#include <picojson/picojson.h>

// Base64 Library
#include "base64/base64.hpp"

// Standard C Libraries
#include <stdlib.h>
#include <string.h>
#include <string>
#include <variant>
#include <algorithm>

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

// Nettoie une chaîne PEM pour ne conserver que la partie Base64
static std::string clean_pem_to_base64(const std::string& pem) 
{
    std::string result = pem;

    // Supprimer les en-têtes et pieds de page PEM
    const std::string begin_marker = "-----BEGIN PUBLIC KEY-----";
    const std::string end_marker = "-----END PUBLIC KEY-----";
    size_t begin_pos = result.find(begin_marker);
    if (begin_pos != std::string::npos) {
        result.erase(begin_pos, begin_marker.length());
    }
    size_t end_pos = result.find(end_marker);
    if (end_pos != std::string::npos) {
        result.erase(end_pos, end_marker.length());
    }

    // Supprimer les retours à la ligne et espaces
    result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
    result.erase(std::remove(result.begin(), result.end(), '\r'), result.end());
    result.erase(std::remove(result.begin(), result.end(), ' '), result.end());

    return result;
}

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
    if (!input) return nullptr;
    
    try {
        // Nettoyer la chaîne PEM pour obtenir uniquement la partie Base64
        std::string cleaned_input = clean_pem_to_base64(input);
        if (cleaned_input.empty()) 
        {
            rcnet_logger_log(RCNET_LOG_ERROR, "Erreur : la chaîne Base64 nettoyée est vide.");
            return nullptr;
        }

        // Décoder la chaîne Base64 nettoyée
        std::string decoded = base64::from_base64(cleaned_input);
        return strdup(decoded.c_str());
    } catch (const std::exception& e) {
        rcnet_logger_log(RCNET_LOG_ERROR, "Exception lors du décodage Base64 : %s", e.what());
        return nullptr;
    }
}