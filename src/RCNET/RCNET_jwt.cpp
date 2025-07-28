#include <jwt-cpp/jwt.h>

jwt::jwt_object create_jwt(const std::string& secret, const std::string& issuer, const std::string& subject) {
    return jwt::create()
        .set_issuer(issuer)
        .set_subject(subject)
        .set_type("JWT")
        .sign(jwt::algorithm::hs256{secret});
}